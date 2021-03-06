/****** Object:  StoredProcedure [dbo].[SP_PhoneAddUser]    Script Date: 04/17/2019 14:31:10 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
ALTER PROCEDURE [dbo].[SP_PhoneAddUser]
(	
	@FastRegister int, --0-快速注册，1-普通注册,2-微信注册，3-QQ注册, 4-手机号注册
	@UserName nvarchar(64),
	@Pass nvarchar(64),
	@HardID nvarchar(64),
	@WeiXinName nvarchar(50), --微信登录昵称
	@Boy smallint,			  --性别
	@HeadUrl nvarchar(256),   --图像地址
	@UionID  nvarchar(64)     --微信,QQ标识码,手机号
) 
AS
BEGIN
	SET NOCOUNT ON;
	
	declare @UserID int,
	@UserNameTmp nvarchar(64),
	@NickName nvarchar(20) ,
	@TwoPassword nvarchar(50),
	@Sex int ,	-- 
	@LogoId int ,	-- 		
	@ZJ_Number varchar(150),	-- 
	@RegisterIP nvarchar(150),	-- 	
	@PhoneNum varchar(50), 
	@Email nvarchar(150), 
	@QQNum varchar(20),
	@RealName varchar(20),
	@AgencyID int, --代理商ID
	@AgencyName nvarchar(50), --代理商帐号		
	@SignDescr NVARCHAR(500),--个性签名
	@PhoneValid INT
	

	select @NickName  ='',
	@UserNameTmp = '',
	@TwoPassword ='',
	@Sex = 1,	-- 
	@LogoId  = 1,	-- 		
	@ZJ_Number ='',	-- 
	@RegisterIP ='',	-- 	
	@PhoneNum ='', 
	@Email ='', 
	@QQNum ='',
	@RealName ='',
	@AgencyID = 1, --代理商ID
	@AgencyName ='admin', --代理商帐号		
	@SignDescr ='',--个性签名
	@PhoneValid=0
	
	IF (@FastRegister = 4)
	BEGIN
		if (@UionID = '')
			return 0
		
		SET @PhoneNum=@UionID
		SET @PhoneValid=1
		IF EXISTS 
		(
			select *
			from TUsers a 
			inner join Web_users b 
			on a.UserID=b.UserID 
			where b.Phone=@UionID AND a.RegistType=@FastRegister
		)
		begin
			select @UserName=a.UserName, @Pass=a.Pass 
			from TUsers a 
			inner join Web_users b 
			on a.UserID=b.UserID 
			where b.Phone=@UionID AND a.RegistType=@FastRegister
			
			select @UserName as UserName, @Pass as Pswd
			return 1
		end
	END
	else if (@FastRegister = 1)
	begin
		if (exists (select top 1 * from tusers where username=@UserName))
		begin
			return 2
		end
		--过滤敏感词
		if exists(select * from tempdb..sysobjects where id=object_id('tempdb..#regname'))
		BEGIN 
			drop table #regname
		END 
		DECLARE @BadName nvarchar(max)
		select @BadName =  badnickname from web_config
		select tmp.RS as num into #regname from (Select RS from dbo.Split(@BadName,'|')) as tmp
		if exists(select *from #regname where num IS NOT NULL AND num <>'' AND @UserName like '%'+num+'%')
		BEGIN 
			return 3--非法昵称
		END
		DECLARE @RegistLimitDaily INT
		DECLARE @RegistLimitTotal INT
		SELECT TOP 1 @RegistLimitDaily=RegistLimitDaily,@RegistLimitTotal=RegistLimitTotal FROM dbo.Web_Config
		IF (SELECT COUNT(*) FROM dbo.TUsers a INNER JOIN dbo.TUserInfo b ON a.UserID=b.UserID WHERE b.MathineCode=@HardID AND a.RegistType=@FastRegister) >= @RegistLimitTotal
		BEGIN 
			RETURN 4
		END 
		IF (SELECT COUNT(*) FROM dbo.TUsers a INNER JOIN dbo.TUserInfo b
			 ON a.UserID=b.UserID  WHERE b.MathineCode=@HardID AND a.RegistType=@FastRegister
			 AND a.UserID IN (SELECT UserID FROM Web_Users WHERE DATEDIFF(dd,RegisterTM,GETDATE())<1)) >= @RegistLimitDaily
		BEGIN 
			RETURN 5
		END 
	end
	else if @FastRegister=0 
	begin
		if (@HardID = '')
			return 0

		if exists(select * from TUserInfo inner join TUsers on TUserInfo.UserID=TUsers.UserID where MathineCode = @HardID AND RegistType=@FastRegister)
		begin
							
			select @UserName=a.UserName, @Pass=a.Pass from TUsers a inner join TUserInfo b on a.UserID=b.UserID where b.MathineCode=@HardID AND a.RegistType=@FastRegister

			select @UserName as UserName, @Pass as Pswd
		
			return 1
		end
	end
	else if @FastRegister=2
	begin
		if (@UionID = '')
			return 0
		set @HardID=''
		set @NickName=@WeiXinName
		set @Sex=@Boy 
		if exists(select * from TWeiXinRegister where UionID=@UionID)
		begin				
			select @UserName=a.UserName, @Pass=a.Pass from TUsers a inner join TUserInfo b on a.UserID=b.UserID 
			inner join TWeiXinRegister c on a.UserID=c.UserID where UionID=@UionID
			select @UserID=UserID from TWeiXinRegister where UionID=@UionID
			if exists (select * from Tusers where nickname=@NickName and  UserID<>@UserID )
				select @NickName = @NickName + cast(@UserID as nvarchar(10))

			update TUsers set NickName=@NickName,Sex=@Sex where UserID=@UserID
			update TWeiXinRegister set NickName=@NickName,Sex=@Sex,HeadUrl=@HeadUrl where UionID=@UionID
			select @UserName as UserName, @Pass as Pswd
			return 1
		end
	end

	declare @RegTime datetime,@RegWallet int
	set @UserID = 0;	
	set @RegWallet = 0;
	set @RegTime = getdate();
	select top 1 @RegWallet=RegWallet from Web_Config

	select @AgencyID=AgencyID from Web_Agency where AgencyName=@AgencyName

	Declare @Regex varchar(500)
	Declare @GoodsCode varchar(500)
	Declare @Price int
	declare @i int;
	set @i=0;
	INSERT INTO Web_CreateID  default values
	Select @UserID=ident_current('Web_CreateID')
	Select @Regex=FineryIDRegex,@Price=FineryIDDefaultPrice from Web_Config
	
	--分割正则,判断是否为靓号
	declare @regID int,@maxID int,@Reg nvarchar(max),@d int
	set @regID = 0;
	set @d = 0;
	select id=identity(int,0,1),tmp.RS as num into #regtmp from (Select RS from dbo.split(@Regex,'-')) as tmp
	select @maxID = max(id) from #regtmp
	while(@i=0)
	begin
		if exists(select * from TUsers where UserID=@UserID)
		begin
			INSERT INTO Web_CreateID  default values
			Select @UserID=ident_current('Web_CreateID')
			continue
		end
		if exists(select * from Web_FineryID where FineryID=@UserID)
		begin
			INSERT INTO Web_CreateID  default values
			Select @UserID=ident_current('Web_CreateID')
			continue
		end
		while(@regID <= @maxID)
		begin
			select @Reg = num from #regtmp where id=@regID;
			if dbo.regex_ismatch(@UserID,@Reg) = 1
			begin
				if not exists(select FineryID from Web_FineryID where FineryID=@UserID)
				begin
					INSERT INTO Web_FineryID(FineryID, Price, IsBuy, IsUse) values(@UserID,@Price,0,0)
				end
				INSERT INTO Web_CreateID default values
				Select @UserID=ident_current('Web_CreateID')
				set @d = 1
				set @regID = 0
				break
			end
			set @regID = @regID + 1
		end
		if @d=1
		begin
			set @d=0
			continue
		end
		set @i = 1
	end
	DECLARE @BankVersion INT
	SET @BankVersion=1
	SELECT @BankVersion=BankVersion  FROM dbo.Web_Config

	--如果是手游端注册，则组合用户名和昵称

	if (@UserName = '')
	begin
		select @UserName = 'Phone' + cast(@UserID as nvarchar(10))			
	end
	if (@NickName = '')
	begin
		select @NickName = @UserName
	end
	if (@Pass = '')
	begin
		select @Pass = 'e10adc3949ba59abbe56e057f20f883e'				   
	end
	if (@TwoPassword = '')
	begin
		select @TwoPassword = 'e10adc3949ba59abbe56e057f20f883e'
	end
		
	if @FastRegister=2--插入微信登录信息
	BEGIN
		if EXISTS (select * from TUsers where NickName=@NickName)
			select @NickName=@NickName+cast(@UserID as nvarchar(10))

		insert into TWeiXinRegister(UserID,UionID,NickName,Sex,HeadUrl) 
		VALUES(@UserID,@UionID,@NickName,@Sex,@HeadUrl)
	end
	--插入用户信息
	INSERT TUsers (UserID,UserName,Pass,TwoPassword,NickName,Sex,LogoID,AgencyID,RegistType) 
		VALUES (@UserID,@UserName,@Pass,@TwoPassword,@NickName,@Sex,@LogoId,@AgencyID,@FastRegister); 
	IF(@BankVersion=2)--此版本银行walletmoney字段无效
	BEGIN
		INSERT TUserInfo (UserID,BankMoney,LastLoginIP,LastLoginTM,PhoneNum,MyHardID) 
			VALUES (@UserID,@RegWallet,@RegisterIP,@RegTime,@PhoneNum, 'ninini');
	END
	ELSE
	begin
		INSERT TUserInfo (UserID,WalletMoney,LastLoginIP,LastLoginTM,PhoneNum, MathineCode) 
			VALUES (@UserID,@RegWallet,@RegisterIP,@RegTime,@PhoneNum, @HardID);
	END
	--注册赠送金币后，添加到金币变化日志表
	IF(@RegWallet>0)
	BEGIN 
	INSERT INTO dbo.Web_MoneyChangeLog
			( UserID ,
			  UserName ,
			  StartMoney ,
			  ChangeMoney ,
			  ChangeType ,
			  DateTime ,
			  Remark
			)
	VALUES  ( @UserID , -- UserID - int
			  @UserName , -- UserName - nvarchar(50)
			  0 , -- StartMoney - bigint
			  @RegWallet , -- ChangeMoney - bigint
			  26, -- ChangeType - int
			 GETDATE() , -- DateTime - datetime
			  '注册赠送'  -- Remark - nvarchar(100)
			)
	END
	DECLARE @RegJewels int 
	SELECT @RegJewels=RegJewels FROM web_config
	
	DELETE Web_Users WHERE UserID=@UserID; 
	
	INSERT Web_Users (UserID,RegisterTM,Email,RegisterIP,ZJ_type,ZJ_Number,Phone,qqNum,RealName,SignDescr,Jewels,PhoneValid) 
		VALUES (@UserID,@RegTime,@Email,@RegisterIP,1,@ZJ_Number,@PhoneNum,@QQNum,@RealName,@SignDescr,@RegJewels,@PhoneValid);
	
        -----------------2017.05.09 添加推广码------------------------------
		DECLARE @PromotionCode VARCHAR(6) --玩家的推广码
		select @PromotionCode=DBO.f_GetRandNum(6,3);
		--添加玩家推广信息
		insert Web_PromotionInfo(UserID,[Money],[Jewels],[Code]) values(@UserID,0,0,@PromotionCode);
		-----------------2017.05.09 添加推广码-------------------------------

	--返回玩家用户名和密码
	select @UserName as UserName, @Pass as Pswd
	return 1

END
