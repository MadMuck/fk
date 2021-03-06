USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UserLoginPlace]    Script Date: 06/24/2019 15:33:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



ALTER PROC [dbo].[SP_UserLoginPlace]
	@UserName		NVARCHAR(21),		--用户名
	@MD5Password	NVARCHAR(50),		--密码，MD5值
	@UserLoginIP	NVARCHAR(16),		--用户登录的IP地址
	@UserToken		NVARCHAR(128),		--Token值
	@ZID			INT,				--登陆服务器ID 
	@HardID			NVARCHAR(64),		--客户端 硬盘 ID, 	
	@UserIDIN		INT,
	@Forced			INT,
	@LogonType		INT					--登录类型
AS
BEGIN
	
DECLARE @UserNameOut		NVARCHAR(21)
DECLARE @UserID				INT
DECLARE @Disabled			INT
DECLARE @Master				INT
DECLARE @UserPassword		NVARCHAR(50)
DECLARE @LogoID				INT
DECLARE @Wallet				BIGINT
DECLARE @BankMoney			BIGINT
DECLARE @Fascination		INT
DECLARE @timeismoney		INT
DECLARE @viptime			INT
DECLARE @DoublePointTime	INT
DECLARE @ProtectTime		INT
DECLARE @LastLoginIP		NVARCHAR(15)
DECLARE @nickName			NVARCHAR(20)
DECLARE @Name				NVARCHAR(20)
DECLARE	@Sex				INT

-- 新增用户资料
DECLARE @SignDescr			NVARCHAR(128) --个性签名
DECLARE @RealName			VARCHAR(20)	--真实姓名
DECLARE @ZJ_Number			VARCHAR(50)	--证件号码
DECLARE @PhoneNum			VARCHAR(50)	--移动电话号码
DECLARE @QQNum				VARCHAR(20)	--QQ号码
DECLARE	@AdrNation			VARCHAR(50)	--国籍
DECLARE @AdrProvince		VARCHAR(50)	--省份
DECLARE @AdrCity			VARCHAR(50) --城市
DECLARE	@ZipCode			VARCHAR(10) --邮政编码
DECLARE	@Email				VARCHAR(50)	--电子邮件
DECLARE	@Address			NVARCHAR(500)--联系地址
DECLARE	@Schooling			NVARCHAR(20) --文化程度
DECLARE	@HomePage			VARCHAR(128)--主页
DECLARE	@TelNo 				VARCHAR(20) --固定电话
DECLARE	@MSNID 				NVARCHAR(50)--MSN帐号


-- 新增安全登陆使用的数据
DECLARE	@DefHardID		NVARCHAR(64) --默认使用的硬盘ID
DECLARE	@IsRobot		SMALLINT	 --是否为机器人

-- 防止帐号重登录
DECLARE	@OnLineFlag				INT
--
DECLARE @NewsID				INT
DECLARE @HAVENEWS			INT
DECLARE @LimitedLogon		INT
DECLARE	@LockMathine		INT				--是否锁机
DECLARE	@BindMobile			INT				--是否绑定手机
DECLARE	@MatCode		NVARCHAR(64)	--机器码
DECLARE @StatusByGM		int
DECLARE @StatusTime		datetime
DECLARE @Salt			VARCHAR(20)
DECLARE @Lotteries		int			--奖券
DECLARE @AddFriendType	int
declare @HeadUrl		nvarchar(256)
declare @Jewels			int			--钻石
declare @CutRoomID      int			--断线重连房间号

set @CutRoomID=0
SET NOCOUNT ON;
set @HeadUrl=''
--IP地址限制检查
SELECT	@LimitedLogon=LimitedLogon 
FROM	TGameIPRule(NOLOCK) 
WHERE	AccessIPChar=@UserLoginIP 
		AND OverTime>=GETDATE()

IF @LimitedLogon IS NOT NULL AND @LimitedLogon<>0
BEGIN
	RETURN 1	--IP地址被限制
END

IF  @UserIDIN <= 0
BEGIN
	SELECT	@UserNameOut = a.UserName,
			@UserID=a.UserID,@Disabled=a.Disabled,@Sex=a.Sex, @Master=b.Master,
			@UserPassword=a.pass,@LogoID=a.logoid,@NickName=a.NickName,@IsRobot = a.IsRobot,@DefHardID = b.MyHardID,@AdrNation = b.AdrNation,@AdrProvince = b.AdrProvince,@AdrCity = b.AdrCity,
			@SignDescr = c.SignDescr,@RealName = c.RealName,@ZJ_Number = c.ZJ_Number,@PhoneNum = c.Phone,@QQNum = c.qqnum,
			@ZipCode = c.ZipCode,@Email = c.Email,@Address = c.Address,@Schooling = c.Schooling,@HomePage = c.HomePage,@TelNo = c.TelNo,@MSNID = c.MSNID,@BindMobile = isnull(c.PhoneValid, 0),
			@OnLineFlag=a.OnlineFlag,@StatusByGM=isnull(b.StatusByGM, 0), @StatusTime = isnull(b.StatusTime, '1970-01-01'),
			@Salt = a.Salt   ,@Lotteries = c.lotteries,@Jewels=c.Jewels
	FROM	TUsers a LEFT JOIN TUserInfo b on a.UserID = b.UserID
					 LEFT JOIN Web_Users c on a.UserID = c.UserID
	WHERE	UserName=@UserName
END
ELSE BEGIN
	SELECT	@UserNameOut = a.UserName,
			@UserID=a.UserID,@Disabled=a.Disabled,@Sex=a.Sex, @Master=b.Master,
			@UserPassword=a.pass,@LogoID=a.logoid,@NickName=a.NickName,@IsRobot = a.IsRobot,@DefHardID = b.MyHardID,@AdrNation = b.AdrNation,@AdrProvince = b.AdrProvince,@AdrCity = b.AdrCity,
			@SignDescr = c.SignDescr,@RealName = c.RealName,@ZJ_Number = c.ZJ_Number,@PhoneNum = c.Phone,@QQNum = c.qqnum,
			@ZipCode = c.ZipCode,@Email = c.Email,@Address = c.Address,@Schooling = c.Schooling,@HomePage = c.HomePage,@TelNo = c.TelNo,@MSNID = c.MSNID,@BindMobile = isnull(c.PhoneValid, 0),
			@OnLineFlag=a.OnlineFlag,@StatusByGM=isnull(b.StatusByGM, 0), @StatusTime = isnull(b.StatusTime, '1970-01-01'),
			@Salt = a.Salt   ,@Lotteries = c.lotteries,@Jewels=c.Jewels
	FROM	TUsers a LEFT JOIN TUserInfo b on a.UserID = b.UserID
					 LEFT JOIN Web_Users c on a.UserID = c.UserID
	WHERE	a.UserID=@UserIDIN
END

	IF @UserID IS NULL 
	BEGIN
		RETURN 2	--用户不存在
	END

	IF @StatusByGM = 4 and @StatusTime>GetDate()
	BEGIN
		RETURN 3	--用户不存在
	END
    declare @OnlineFalg int 
    select @OnLineFlag=ISNULL(OnlineFlag,0) from TUsers  WHERE  UserID =@UserID
	if EXISTS (SELECT  ZID FROM TZLoginRecord a  WHERE  a.UserID =@UserID)
	BEGIN
		IF @Forced<>0 and @OnLineFlag<>1
			DELETE FROM TZLoginRecord WHERE UserID=@UserID
		ELSE  
			RETURN 5
	end

	If EXISTS (SELECT LockCode FROM TLockCode WHERE  LockCode = @HardID OR LockHard = @HardID)
	begin
		RETURN 7
	end

	--游客账号限制登陆设备
	DECLARE @RegistType INT 
	SELECT @RegistType=RegistType,@IsRobot=IsRobot FROM dbo.TUsers WHERE UserID=@UserID
	IF @RegistType=0 AND @HardID<>(SELECT MathineCode FROM dbo.TUserInfo WHERE UserID=@UserID) AND @IsRobot=0
		RETURN 10
	
	-- 如果用户帐号不可用，或帐号拥有管理员权限，则禁止登录客户端大厅
	IF @Disabled<>0 OR @Master<>0
	BEGIN
		RETURN 3	--用户帐号禁止
	END

	--如果存在Salt，则采用另一种加密方式
	IF @Salt IS NOT NULL AND DATALENGTH(LTRIM(@Salt)) > 0
	BEGIN
		DECLARE @MD5Temp	VARCHAR(50) --NVARCHAR因为是Unicode编码，MD5后结果不一样
		SET @MD5Temp = @MD5Password
		SET @MD5Temp = sys.fn_VarBinToHexStr(HashBytes('MD5', @MD5Temp + @Salt))
		SET @MD5Temp = substring(@MD5Temp,charindex('x',@MD5Temp)+1,len(@MD5Temp))
		IF @MD5Temp<>@UserPassword
			RETURN 4	--密码不正确
	END
	ELSE IF @UserPassword<>@MD5Password
	BEGIN
		RETURN 4	--密码不正确
	END

	--在锁机的情况下，则return6通知客户端不能登陆
	SELECT @LockMathine=LockMathine, @MatCode=MathineCode FROM TUserInfo WHERE UserID=@UserID
	IF @IsRobot = 0
	BEGIN
		IF @LockMathine=1 AND @MatCode<>@HardID
		BEGIN
			SELECT  @UserID AS UserID,@PhoneNum AS PhoneNum
			RETURN 6
		END
	END

	--在线标志记录
	UPDATE TUsers Set OnlineFlag=1 where UserID = @UserID

	SELECT	@Wallet=WalletMoney,@BankMoney=BankMoney,
			@Fascination=Fascination,@TimeIsMoney=TimeIsMoney,
			@Viptime=viptime,@DoublePointTime=DoublePointTime,
			@ProtectTime=0,@LastLoginIP=LastLoginIP,
			@AddFriendType = AddFriendType
	FROM	TUserInfo
	WHERE	UserID=@UserID

	-- 清除记录(在线送金币记录)	
	UPDATE TUserInfo Set TimeIsMoney=0 where UserID = @UserID
	--
	IF @Wallet IS NULL
	BEGIN
		--如果用户信息表不存在，则需要添加一条记录
		INSERT INTO	TUserInfo(UserID)
			 VALUES (@UserID)
		--其它数据使用默认值
		--更新内存数据
		SET @Wallet			= 0
		SET @BankMoney		= 0
		SET @Fascination	= 0
		SET @TimeIsMoney	= 0
		SET @Viptime		= 0
		SET @DoublePointTime= 0
		SET	@ProtectTime	= 0
		SET	@Sex			= 0
		SET @LastLoginIP	= ''''
	END

	--更新用户信息
	UPDATE	TUserInfo 
	SET		AllLoginTime=AllLoginTime+1,
			LastLoginTM=GETDATE(),
			LastLoginIP=@UserLoginIP
	WHERE	UserID=@UserID

	--更新用户TOKEN
	UPDATE	TUsers
	SET		Token=@UserToken
	WHERE	UserID=@UserID

	--玩家从未填写证件号，则不进行验证；机器人不验证
	--本次登陆的机器HardID不是默认机ID，需要身份验证
	--有手机认证，无需身份验证
	IF @IsRobot = 0
		BEGIN
			--登陆的PC的HardID是本PC的HardID,则更改MyHardID
			UPDATE TUserInfo SET MyHardID = @HardID WHERE UserID = @UserID
		END


	--是否有在线公告
	--注意：是从网站数据表来的数据，不改变原来的结构
	SELECT	@NewsID=News_ID 
	FROM	Web_NewsType,Web_NewsData 
	WHERE	Web_NewsType.typeid=Web_NewsData.news_type 
			and typesort=100 
			and istop=1
	--添加登陆记录      -- 增加 客户端机器信息
	INSERT INTO [TLoginRecord]
           ([UserID]
			,[MachineCode]
			,[CPUID]
			,[HardID]
           ,[LastLoginIP])
     VALUES
           (@UserID
			,@HardID
			,@HardID
			,@HardID
           ,@UserLoginIP)
	---------------
	IF @NewsID is NULL
		SET @HAVENEWS = 0
	ELSE
		SET @HAVENEWS = 1
   	
   	IF  @IsRobot=0 AND (SELECT RegistType FROM dbo.TUsers WHERE UserID=@UserID) <> @LogonType
   		RETURN 11
   	
	if exists(select * from TWeiXinRegister where UserID=@UserID)
	BEGIN 
		SELECT @nickName=NickName,@Sex=Sex,@HeadUrl=HeadUrl FROM TWeiXinRegister WHERE UserID=@UserID
	END 
	else
	begin
		if @LogoID>100
			select @HeadUrl=PicPosition from T_HeadPic where userid=@UserID
	end

	IF EXISTS (SELECT * FROM TGameCutNetRecord WHERE UserID=@UserID) 
		OR EXISTS (SELECT * FROM TBuyGameDeskRecord WHERE MasterID=@UserID AND MasterState=0 and ClubID=0) 
		OR EXISTS (SELECT * FROM dbo.TWLoginRecord WHERE UserID=@UserID)
	BEGIN 
		SET @CutRoomID=1 
	END 
	ELSE IF EXISTS (SELECT * FROM dbo.TSignUp a,dbo.TContestUserRecord b 
					WHERE a.UserID=@UserID AND b.UserID=@UserID AND a.MatchID=b.MatchID AND a.ContestID=b.ContestID AND b.IsKick=0)
	BEGIN 
		DECLARE @NetCutTime DATETIME
		DECLARE @MatchID INT 
		SELECT @NetCutTime=b.NetCutTime,@MatchID=a.MatchID FROM dbo.TSignUp a,dbo.TContestUserRecord b 
					WHERE a.UserID=@UserID AND b.UserID=@UserID AND a.MatchID=b.MatchID AND a.ContestID=b.ContestID AND b.IsKick=0
		IF DATEDIFF(ss,@NetCutTime,GETDATE())>300
		BEGIN 
			SET @CutRoomID=0 
			UPDATE TContestUserRecord SET IsKick=1 WHERE UserID=@UserID AND MatchID=@MatchID
		END 
		ELSE 
			SET @CutRoomID=1 
	END 
	--按网站同事要求维护用户连续登陆天数
	
	declare @Update datetime       --玩家上次更新的时间
	select @Update=isnull(ContinueLoginUpdateTime,0) from TUserInfo where @UserID=UserID
	if @Update=0  --第一次登陆
	begin
		update TUserInfo set ContinueLoginUpdateTime=GETDATE(),ContinueLoginCount=1 where @UserID=UserID
	end
	else 
	begin
		declare @Day int
		select @Day=datediff(day, @Update,GETDATE()); --两日期间隔天
		if @Day<1  --表示今天已经登陆过  连续登陆天数不用增加  更新下登陆时间就行
		update TUserInfo set ContinueLoginUpdateTime=GETDATE() where @UserID=UserID
		if @Day=1  --表示上次登陆更新时间 距离现在一天 增加连续登陆天数  更新下登陆时间
		update TUserInfo set ContinueLoginUpdateTime=GETDATE(),ContinueLoginCount=ContinueLoginCount+1 where @UserID=UserID
		if @Day>1  --表示上次登陆更新时间 距离现在超过一天  中断连续登陆天数  更新下登陆时间
		update TUserInfo set ContinueLoginUpdateTime=GETDATE(),ContinueLoginCount=1 where @UserID=UserID
	end
	
	
	--输出变量
    SELECT  @UserID AS UserID,@Sex AS sex,
			@UserPassword AS UserPassword, @LogoID AS  LogoID,
			@Wallet AS  Wallet, @BankMoney AS  BankMoney,
			@Fascination AS Fascination, @timeismoney AS timeismoney,
			@viptime AS viptime, @DoublePointTime AS DoublePointTime,
			@ProtectTime AS ProtectTime, @LastLoginIP AS LastLoginIP,
			@nickName AS nickName, @UserNameOut AS  UserName,
			@SignDescr AS SignDescr,@RealName AS RealName,@ZJ_Number AS ZJ_Number,@PhoneNum AS PhoneNum,@QQNum AS QQNum,
			@ZipCode AS ZipCode,@Email AS Email,@Address AS Address,@Schooling AS Schooling,@HomePage AS HomePage,@TelNo AS TelNo,@MSNID AS MSNID,
			@AdrNation AS AdrNation,@AdrProvince AS AdrProvince,@AdrCity AS AdrCity,
			@HAVENEWS AS HAVENEWS,
			@LockMathine AS LockMathine,
			@BindMobile AS BindMobile,
			@AddFriendType as AddFriendType	,
			@Lotteries  as Lotteries,
			@LogonType  as LogonType,
			@Jewels     as Jewels,
			@HeadUrl	as HeadUrl,
			@CutRoomID  as CutRoomID
END
	RETURN 0













