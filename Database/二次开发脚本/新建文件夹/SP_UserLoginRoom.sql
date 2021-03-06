USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UserLoginRoom]    Script Date: 05/17/2019 16:29:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO












-- =============================================
-- Author:		Fred Huang
-- Create date: 2008-11-19
-- Description:	用户登录房间
-- 修改：zxd
-- 日期：20100309
-- 原因：防止绕过Z服务器直接登录W服务器
-- 办法：登录W服务器时，判断是否有Z服务器登录记录，如无记录，则只有机器人允许登录到W服务器

-- 修改：zxd
-- 日期：20100806
-- 原因：判断当前玩家被GM操作过的记录
-- 办法：判断StatusByGM和StatusTime，并返回，并再由调用本存储过程者判断返回值是否为禁言状态
-- =============================================
ALTER PROCEDURE [dbo].[SP_UserLoginRoom]
(
	@UserID			INT,				--用户ID
	@PassMD5		NVARCHAR(50),		--用户密码
	@LoginIP		NVARCHAR(15),		--用户登录的IP地址
	@RoomID			INT,				--用户登录的房间ID
	@KernelNameID	INT,				--内核NAMEID
	@MatchTable		NVARCHAR(30),		--用户成绩记录表
	@IPRuleTable	NVARCHAR(30),		--房间IP限制表
	@NameRuleTable	NVARCHAR(30),		--房间用户限制表
	@LockTable		NVARCHAR(30),		--用户登录后的锁定表
	@MatchID		INT,				--如果比赛已开始，则有一个比赛ID，否则为0
	@RoomState		INT					--如果是比赛房需要判断房间是否已开赛
)	
AS
BEGIN
	SET NOCOUNT ON;	
	DECLARE	@Member			INT
	DECLARE	@GamePower		INT
	DECLARE	@MatchMember	INT
	DECLARE	@Master			INT
	DECLARE @Name			NVARCHAR(20)
	DECLARE @NickName		NVARCHAR(30)
	DECLARE @Sex			INT
	DECLARE	@LogoID			INT
	DECLARE @MasterPower	INT
	DECLARE @Fascination	INT
	DECLARE @viptime		INT
	DECLARE @DoublePointTime	INT
	DECLARE @ProtectTime	INT
	DECLARE	@Wallet			BIGINT
	DECLARE @BankMoney		BIGINT
	DECLARE	@Win			INT
	DECLARE	@Lost			INT
	DECLARE	@Cut			INT
	DECLARE	@Mid			INT
	DECLARE @Point			BIGINT
	DECLARE @LimitGame		INT
	DECLARE	@GPLimitTime	DATETIME
	--防止帐号重登录
	DECLARE	@OnLineFlag INT
	DECLARE @GameNameID INT
	--  添加用户信息字段
	DECLARE @OccuPation		NVARCHAR(50)
	DECLARE @PhoneNum		NVARCHAR(50)
	DECLARE @AdrProvince	NVARCHAR(50)
	DECLARE @AdrCity		NVARCHAR(50)
	DECLARE @AdrZone    	NVARCHAR(50)
	--进入练习场默认金币数
	DECLARE @ExercisePoint	BIGINT
	DECLARE @SignDescr	    NVARCHAR(128) --个性签名
	DECLARE @DiamondTime	INT			--邮游钻石身份
	DECLARE @StatusByGM		int			-- 被GM操作过的记录类型
	DECLARE @StatusTime		datetime	-- 被GM操作过的时效
	DECLARE @IsRobot INT	-- 是否机器人，0为普通玩家，其它值为机器人
	DECLARE @VirtualUser INT
	--登录房间时，获取玩家类型
	DECLARE @UserType  INT
	DECLARE @Salt VARCHAR(20)	
	DECLARE @ReturnVal INT
	SET @ReturnVal = 0

	--检查是否IP限制
	DECLARE	@SQL NVARCHAR(512)
	declare @HeadUrl		nvarchar(256)
	declare @Jewels			int
	declare @CutRoomID		int			--断线重连房间号
	set @HeadUrl=''
	set @CutRoomID=-1
	SET @SQL='SELECT @LGame=LimitedGame	FROM '+@IPRuleTable
	+' WHERE AccessIPChar='''+@LoginIP+''''
	+' AND RoomID=' + STR(@RoomID)
	+' AND OverTime>=GETDATE()'
	EXEC SP_EXECUTESQL @SQL,N'@LGame INT OUTPUT',@LimitGame OUTPUT

	IF @LimitGame IS NOT NULL
	BEGIN
		RETURN 1		--用户登录IP被限制了
	END

	--检查用户名是否被限制
	SET @SQL='SELECT @LGame=GameInvalidate	FROM '+@NameRuleTable
	+' WHERE UserID='+STR(@UserID)
	+' AND RoomID='+STR(@RoomID)
	+' AND OverTime>=GETDATE()'	
	EXEC SP_EXECUTESQL @SQL,N'@LGame INT OUTPUT',@LimitGame OUTPUT

	IF @LimitGame IS NOT NULL
	BEGIN
		RETURN 2		--用户帐号在此房间登录被限制了
	END
	
	--获取游戏ID
	SELECT @GameNameID = GameNameID
	FROM TGameRoomInfo with(nolock) 
	WHERE RoomID = @RoomID

	DECLARE	@BankVer INT			--银行版本
	SELECT TOP 1 @BankVer = BankVersion 
	FROM Web_Config with(nolock)

	DECLARE @oldroom int
	IF @BankVer = 1
	BEGIN
		--添加新防止账户同时登陆	
		Set @oldroom = 0
		select @oldroom=WID from  [TWLoginRecord] with(nolock) where UserID=@UserID
		IF @oldroom > 0 and @oldroom != @roomid AND DATEDIFF(MI,(SELECT UpdateTime FROM dbo.TGameRoomInfo WHERE RoomID=@oldroom),GETDATE())<=3
		BEGIN
			select roomname from  TGameRoomInfo with(nolock) WHERE RoomID = @oldroom
			RETURN 11
		END
	END

	ELSE IF  @BankVer = 2
	BEGIN
		--添加新防止账户同时登陆同游戏的不同房间
		Set @oldroom = 0
		select @oldroom=WID 
		from TWLoginRecord a with(nolock) join TGameRoomInfo b with(nolock) on a.WID=b.RoomID
		where a.UserID=@UserID
		and b.GameNameID=@GameNameID
		      
		IF @oldroom > 0 and @oldroom != @roomid AND DATEDIFF(MI,(SELECT UpdateTime FROM dbo.TGameRoomInfo WHERE RoomID=@oldroom),GETDATE())<=3
		BEGIN
			select roomname from  TGameRoomInfo WHERE RoomID = @oldroom
			RETURN 11
		END
	END

	--查询用户数据
	--先检查TUser表中是否有该用户记录
	--防止帐号重登录
	DECLARE @Pass	NVARCHAR(50)
	SELECT	@Name=UserName,
			@Pass=Pass,
			@NickName=NickName,
			@LogoID=LogoID,
			@Sex=Sex,
			@OnLineFlag=OnlineFlag,
			@IsRobot=IsRobot,
			@Salt = Salt
	FROM TUsers with(nolock)
	WHERE UserID = @UserID

	IF @Name IS NULL
	BEGIN
		RETURN 3
	END 


	-- 如果用户密码错误，返回失败
	IF @Salt IS NOT NULL AND DATALENGTH(LTRIM(@Salt)) > 0
	BEGIN
		DECLARE @MD5Temp	VARCHAR(50)
		SET @MD5Temp = @PassMD5
		SET @MD5Temp = sys.fn_VarBinToHexStr(HashBytes('MD5', @MD5Temp + @Salt))
		SET @MD5Temp = substring(@MD5Temp,charindex('x',@MD5Temp)+1,len(@MD5Temp))
		IF @MD5Temp<>@Pass
		begin
			RETURN 3	--密码不正确
		end
	END

	ELSE if @PassMD5 <> @Pass
	begin
		return 3
	end

	--防止绕过Z服务器登录W服务器
	IF @IsRobot=0
	BEGIN
		if NOT EXISTS (select ZID from [TZLoginRecord] with(nolock) where UserID = @UserID)
		begin
			return 2
		end
	END

	--防止绕过Z服务器登录W服务器
	--检查TUserInfo中是否有记录
	SELECT	@Member = a.Member,
			@MatchMember = a.MatchMember,
			@Master = a.Master,
			@GamePower = a.GamePower,
			@Fascination = a.Fascination,
			@viptime = a.viptime,
			@DoublePointTime = a.DoublePointTime,
			@ProtectTime = a.ProtectTime,
			@Wallet = a.WalletMoney,
			@BankMoney = a.BankMoney,
			@GPLimitTime = a.GPLimitTime,
			--获取玩家信息
			@OccuPation = a.OccuPation,
			@PhoneNum = a.PhoneNum,
			@AdrProvince = a.AdrProvince,
			@AdrCity = a.AdrCity,
			@AdrZone = a.AdrZone,
			@StatusByGM = isnull(a.StatusByGM, 0), 
			@StatusTime = isnull(a.StatusTime, '1970-01-01'),
			--登录房间时获取玩家类型
			@UserType = a.UserType,
			@DiamondTime = a.DiamondTime,
			--获取玩家个性签名
			@SignDescr = b.SignDescr
	FROM TUserInfo a with(nolock) LEFT JOIN Web_Users b with(nolock) on a.UserID = b.UserID
	WHERE a.UserID = @UserID

	IF @StatusByGM >= 3 and @StatusTime>GetDate()
	BEGIN
		RETURN 2	--用户被限制
	END

	IF @BankVer = 2
	BEGIN
		--读取游戏的所属的钱包，新的银行添加游戏钱包，以往的TUserInfo的总钱包已不使用
		DECLARE @UserIDFind INT
		SELECT @UserIDFind = UserID,@Wallet = WalletMoney 
		FROM TBankWallet with(nolock) 
		WHERE UserID=@UserID 
		AND GameID=@GameNameID
		IF @UserIDFind IS NULL
		BEGIN
			--第一次登陆房间
			INSERT TBankWallet(UserID,GameID,WalletMoney) VALUES(@UserID,@GameNameID,0)
			SET @Wallet = 0
		END
	END

	IF @Member IS NULL
	BEGIN
		INSERT INTO	TUserInfo(UserID)
		VALUES (@UserID)
		SET	@Member=0
		SET	@MatchMember=0
		SET	@Master=0
		SET	@GamePower=0
		SET	@Fascination=0
		SET	@viptime=0
		SET	@DoublePointTime=0
		SET	@ProtectTime=0
		SET	@Wallet=0
		SET	@BankMoney=0
		SET @GPLimitTime=GETDATE()
		SET @OccuPation='自由职业者'
		SET @PhoneNum='13800138000'
		SET @AdrProvince='广东省'
		SET @AdrCity='深圳市'
		SET @AdrZone='罗湖区'  
		SET @SignDescr = ''
	END

	IF @GPLimitTime < GETDATE()
	begin
		SET @GPLimitTime = 0
	end 
	--记录用户锁住记录
	SET	@SQL='INSERT INTO '+@LockTable
	+'(UserID,RoomID,AccessIP) VALUES ('
	+STR(@UserID)+','+STR(@RoomID)+','''+@LoginIP+''')'
	EXEC(@SQL)

	--管理员登录要记录
	IF	@Master > 0
	BEGIN
		INSERT INTO TGMRecord (GMID,beginTime,msgResult) 
		VALUES (@UserID,GETDATE(),'logon in Room ID='+cast(@RoomID as NVARCHAR))
	END

		--读取用户在游戏中的记录
	DECLARE @MasterPowerInRoom INT
	SET @SQL='SELECT @MPower=MasterPower FROM '+@MatchTable
	+' WHERE UserID='+STR(@UserID)
	+' AND NameID='+STR(@KernelNameID)
	--+' AND RoomID='+STR(@RoomID)		-- 不同的房间不同的记录
	EXEC SP_EXECUTESQL @SQL,N'@MPower INT OUTPUT',@MasterPowerInRoom OUTPUT

	IF @MasterPowerInRoom IS NULL
	BEGIN
		SET @SQL='INSERT INTO '+@MatchTable
		+'(UserID,NameID,Point,Money,'
		+'TaxCom,Win,Lost,Mid,Cut,PlayTimeCount,'
		+'OnLineTimeCount,MasterPower,GamePower)'
		+' VALUES ('+STR(@UserID)
		--+','+STR(@RoomID)
		+','+STR(@KernelNameID)
		+',0,0,0,0,0,0,0,0,0,0,0)'
		EXEC (@SQL)

		SET @MasterPowerInRoom = 0
	END

	ELSE
	begin
		SET @MasterPower = @MasterPowerInRoom
	end

	--用户的当前游戏记录
	SET @SQL ='SELECT @GWin=win,@GLost=Lost,@GCut=Cut,@GMid=Mid,@GPoint=Point'
	+' FROM '+@MatchTable
	+' WHERE UserID=' + STR(@UserID)
	+' AND NameID=' + STR(@KernelNameID)
	--+' AND RoomID='+STR(@RoomID)		-- 不同的房间不同的记录
	EXEC SP_EXECUTESQL @SQL,
	N'@GWin INT OUTPUT,@GLost INT OUTPUT,@GMid INT OUTPUT,@GCut INT OUTPUT,@GPoint bigint OUTPUT',
	@Win OUTPUT,@Lost OUTPUT,@Mid OUTPUT,@Cut OUTPUT,@Point OUTPUT

	DECLARE @BattleID INT
	SET @SQL =' SELECT @BattleID=BattleRoomID'
	+ ' FROM TGameRoomInfo'
	+ ' WHERE RoomID=' + STR(@RoomID)
	EXEC SP_EXECUTESQL @SQL ,N'@BattleID INT OUTPUT',@BattleID OUTPUT
	IF @BattleID > 0
	--是混战场房间
	BEGIN
		----检测是否是在活动时间内
		DECLARE @BeginTime_tmp DATETIME
		DECLARE @EndTime_tmp DATETIME
		DECLARE	@SQL_tmp NVARCHAR(512)
		SET @SQL_tmp ='SELECT @BeginTime_tmp=BeginTime,@EndTime_tmp=EndTime'
		+ ' FROM TBattleRoomInfo'
		+ ' WHERE BattleRoomID=' +STR(@BattleID)
		EXEC SP_EXECUTESQL @SQL_tmp, N'@BeginTime_tmp DATETIME OUTPUT,@EndTime_tmp DATETIME OUTPUT',@BeginTime_tmp OUTPUT,@EndTime_tmp OUTPUT
		--不是在混战场活动时间内
		IF GETDATE() < @BeginTime_tmp OR GETDATE()> @EndTime_tmp
		begin
			RETURN 5
		end
	END

	--用户在当前游戏房间是否是管理员
	DECLARE @GamePowerInRoom INT
	SET @SQL='SELECT @GPower=GamePower'+' FROM '+@MatchTable
	+' WHERE UserID='+STR(@UserID)
	+' AND NameID='+STR(@KernelNameID)
	--+' AND RoomID='+STR(@RoomID)-- PengJiLin, 2011-4-14, 不同的房间不同的记录
	EXEC SP_EXECUTESQL @SQL,N'@GPower INT OUTPUT',@GamePowerInRoom OUTPUT

	IF @GamePowerInRoom IS NULL
	begin
		SET @GamePowerInRoom = 0
	end
	ELSE
	begin
		SET @GamePower = @GamePower | @GamePowerInRoom
	end

		--更新数据库，用户登录次数
	SET @SQL ='UPDATE '+@MatchTable
	+' SET LogonTimes=LogonTimes+1,'
	+' LogonDate=GETDATE()'
	+' WHERE UserID='+STR(@UserID)+' AND nameid='+STR(@KernelNameID)
	--+' AND RoomID='+STR(@RoomID)	-- PengJiLin, 2011-4-14, 不同的房间不同的记录
	EXEC(@SQL)

	DECLARE @LessPoint INT
	SET @LessPoint	= 0
	--登录房间时即时获取虚拟玩家人数，而不用重新启动W服务器
	SELECT @VirtualUser = VirtualUser
	FROM TGameRoomInfo with(nolock) 
	WHERE RoomID = @RoomID
	SELECT @ExercisePoint = ExercisePoint  , @LessPoint = LessPoint
	FROM TGameRoomBase with(nolock) 
	WHERE RoomID = @RoomID

	DECLARE @ContestID INT
	DECLARE @LowChip BIGINT 
	DECLARE	@RankNum INT	--比赛排名

	SELECT @ContestID = ContestID 
	FROM TGameRoomBase with(nolock) 
	WHERE @RoomID = RoomID	
	set @RankNum = 0

	IF @ContestID <> 0 
	BEGIN 
	
		DECLARE @ContestScore bigint
		DECLARE @ContsetCount int
		DECLARE @TimeLeft Datetime
		DECLARE @ContestUser INT
		DECLARE @RemainPeople INT 

		IF @RoomState = 1 AND @MatchID <> 0
		BEGIN
			select  @ContestScore = isnull(ContestScore,0),
					@ContsetCount = isnull(ContestCount,0),
					@TimeLeft = isnull(ContestTime,0),
					@ContestUser = UserID,
					@RankNum = RankNum
			from	TContestRecord_New with(nolock)
			where	UserID = @UserID
					AND ContestID=@ContestID
					AND MatchID = @MatchID
		END
		ELSE
		BEGIN
			select @ContestScore = isnull(Chip, 0)
			from TContestInfo
			where @ContestID = ContestID	
			select @ContsetCount = 0, @TimeLeft = 0, @ContestUser = 0
		END		
		
		IF @RoomState <> 0 AND ((SELECT IsKick FROM dbo.TContestUserRecord WHERE UserID=@UserID AND ContestID=@ContestID AND MatchID = @MatchID) = 1
		OR NOT EXISTS (SELECT * FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID AND MatchID = @MatchID))
		BEGIN
			--如果用户登录的房间已经开赛且不是掉线用户，则需要用户重新登录其他的比赛房间
			RETURN 14
		END
		
		ELSE IF EXISTS (SELECT * FROM dbo.TSignUp a,dbo.TContestUserRecord b 
					WHERE a.UserID=@UserID AND b.UserID=@UserID AND a.MatchID=b.MatchID AND a.ContestID=b.ContestID AND b.IsKick=0)
				AND NOT EXISTS(SELECT * FROM dbo.TWLoginRecord WHERE UserID=@UserID AND WID=@RoomID)
		BEGIN 
			DECLARE @NetCutTime DATETIME
			SELECT @NetCutTime=b.NetCutTime,@MatchID=a.MatchID FROM dbo.TSignUp a,dbo.TContestUserRecord b 
						WHERE a.UserID=@UserID AND b.UserID=@UserID AND a.MatchID=b.MatchID AND a.ContestID=b.ContestID AND b.IsKick=0
			IF DATEDIFF(ss,@NetCutTime,GETDATE())>300
			BEGIN 
				UPDATE TContestUserRecord SET IsKick=1 WHERE UserID=@UserID AND MatchID=@MatchID
				RETURN 10
			END 
		END 
		
	END 


	SuccessLogin:
		DECLARE @Point1	int
		if  @Point < 2100000000 and  @Point > -2100000000 
		begin 
			set @Point1 = @Point
		end
		else  
		begin 
			if @Point > 2100000000 
			begin
				set @Point1 = 2100000000
			end
			else
			begin
				set @Point1 = -2100000000
			end
		end
	if exists(select * from TWeiXinRegister where UserID=@UserID)
		select @HeadUrl=HeadUrl from TWeiXinRegister where UserID=@UserID

	else
	begin
		if @LogoID>100
			select @HeadUrl=PicPosition from T_HeadPic where userid=@UserID
	end
	declare @Lotteries int
	select @Jewels=isnull(Jewels,0),@Lotteries=isnull(Lotteries,0) from Web_Users where UserID=@UserID
	 

		--if @GameNameID=33000904 --如果是捕鱼场 给用户的金币换成碎钻
		-- select @Wallet=Lotteries from Web_Users where UserID=@UserID
		SELECT  @Member AS Member, 
				@GamePower AS GamePower,
				@MatchMember AS MatchMember, 
				@Master AS Master, 
				@Name AS UserName, 
				@NickName AS NickName,
				@Sex AS Sex, 
				@LogoID AS LogoID, 
				@MasterPower AS MasterPower, 
				@Fascination AS Fascination, 
				@viptime AS viptime, 
				@DoublePointTime AS DoublePointTime, 
				@ProtectTime AS ProtectTime, 
				@Wallet AS Wallet, 
				@BankMoney AS BankMoney,
				@MasterPowerInRoom AS MasterPowerInRoom, 
				@Win AS Win, 
				@Lost AS Lost, 
				@Cut AS Cut, 
				@Mid AS Mid, 
				@Point1 AS Point,
				@OccuPation as OccuPation,@PhoneNum as  PhoneNum,
				@AdrProvince as AdrProvince, @AdrCity as AdrCity,
				@AdrZone as AdrZone,
				@VirtualUser as VirualUser,
				@UserType as UserType,
				--邮游钻石身份
				@IsRobot as IsRobot,
				@DiamondTime as DiamondTime,
				--被GM操作过记录及时效
				@StatusByGM as StatusByGM,
				@StatusTime as StatusTime,
				--被GM操作过记录及时效
				--练习房间的金币
				@ExercisePoint as ExercisePoint,
				--个性签名
				@SignDescr as SignDescr,

				@ContestID as ContestID,
				@ContestScore as ContestScore,
				@ContsetCount as ContestCount,
				@TimeLeft as LeftTime,
				@RankNum as RankNum,

				@LessPoint as LessPoint,
				@HeadUrl   as HeadUrl,
				@Jewels	   as Jewels,
				@CutRoomID as CutRoomID,
				@Lotteries as Lotteries
	RETURN @ReturnVal
END
	



--exec SP_UserLoginRoom 10012, 'd0970714757783e6cf17b26fb8e2298f', '127.0.0.1', 2, 10003300, 'Tmatchtable','TRoomIPRule','TUserNameRule','TGameLock'














