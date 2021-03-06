set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
GO


-- Stored Procedure

-- =============================================
-- Author:		PengJiLin
-- Create date: 2011-5-23
-- Description:	购买VIP、赠送VIP功能
--              购买是自己购买自己使用，
--              赠送是自己购买目标是用
-- =============================================
ALTER PROCEDURE [dbo].[SP_VipOpera]
	@UserID			INT,			--用户ID号
	@TargetNickName	NVARCHAR(50),   --目标用户昵称
    @VipType		INT             --VIP 类型：一天(0)、一周(1)、一月(2)、半年(3)、一年(4) 共5种
AS
BEGIN
	SET NOCOUNT ON;

	--CREATE TABLE chs_vip(UserID int,Targetnickname nvarchar(50),viptype int)
	--insert into chs_vip values(@UserID	,@TargetNickName,  @VipType)

	DECLARE	@BankVer		INT --银行版本
	SELECT TOP 1 @BankVer = BankVersion FROM Web_Config

	-- 根据VIP类型获取VIP价格，和确定VIP时长(秒)
	DECLARE	@VipMoney	INT
	DECLARE @VipTimeSec	INT
	DECLARE @SumMoney BIGINT
	DECLARE @UserName NVARCHAR(50)
	IF @VipType=1					-- 一天VIP
	BEGIN
		SELECT @VipMoney=MoneyToViptimeDay FROM Web_Config
		SET @VipTimeSec=86400		-- 24*60*60
	END
	ELSE IF @VipType=7				-- 一周VIP
	BEGIN
		SELECT @VipMoney=MoneyToViptimeWeek FROM Web_Config
		SET @VipTimeSec=604800		-- 7*24*60*60
	END
	ELSE IF @VipType=30				-- 一月VIP
	BEGIN
		SELECT @VipMoney=MoneyToViptimeMonth FROM Web_Config
		SET @VipTimeSec=2592000		-- 30*24*60*60
	END
	ELSE IF @VipType=180				-- 半年VIP
	BEGIN
		SELECT @VipMoney=MoneyToViptimeHYear FROM Web_Config
		SET @VipTimeSec=15552000	-- 6*30*24*60*60
	END
	ELSE							-- 一年VIP
	BEGIN
		SELECT @VipMoney=MoneyToViptimeYear FROM Web_Config
		SET @VipTimeSec=31104000	-- 12*30*24*60*60
	END

	-- 用户金币是否够(银行)
	DECLARE	@UserBankMoney	 BIGINT
	DECLARE @UserWalletMoney BIGINT
	SELECT @UserBankMoney=ISNULL(BankMoney,0),@UserWalletMoney=ISNULL(WalletMoney,0) FROM TUserInfo WHERE @UserID=UserID
	--总金币
	IF @BankVer = 1
		BEGIN
			SET @SumMoney = @UserBankMoney + @UserWalletMoney
		END
	ELSE IF @BankVer = 2
		BEGIN
			SELECT @SumMoney=@UserBankMoney+ ISNULL(SUM(WalletMoney),0) FROM TBankWallet WHERE UserID = @UserID
		END
	
	SELECT @UserName=UserName FROM TUsers WHERE UserID = @UserID
	IF @UserBankMoney<@VipMoney
	BEGIN
		RETURN		1				-- 金币不够
	END

	-- 根据是否有昵称
	DECLARE @NickNameLength INT
	SELECT @NickNameLength = len(@TargetNickName)

	-- 操作结果
	DECLARE @Result		INT

	-- 确定受惠目标用户
	DECLARE @BenUserID	INT
	DECLARE @SaveUserID	INT			-- 记录使用，0=自己使用，非0=赠送给别人
	IF @NickNameLength=0			-- 购买
	BEGIN
		SET @BenUserID=@UserID
		SET @SaveUserID=0
		SET @Result=0				-- 购买结果
	END
	ELSE							-- 赠送
	BEGIN
		DECLARE @TargerUserID	INT
		SELECT @TargerUserID=UserID FROM TUsers WHERE @TargetNickName=NickName
		SET @BenUserID=@TargerUserID
		SET @SaveUserID=@TargerUserID
		SET @Result=10				-- 赠送结果
	END

	-- 受惠用户是否存在
	IF NOT EXISTS (SELECT UserID FROM TUsers WHERE @BenUserID=UserID)
	BEGIN
		RETURN		2				-- 目标用户不存在
	END

	-- 当前时间(秒)
	DECLARE	@CurTimeSecs	INT
	SET @CurTimeSecs=datediff([second],'1970-01-01 08:00:00',getdate())

	-- 受惠用户已有VIP时间
	DECLARE @HadVipSecs		INT
	SELECT @HadVipSecs=Viptime FROM TUserInfo WHERE @BenUserID=UserID
	IF @HadVipSecs<=@CurTimeSecs
	BEGIN
		SET @HadVipSecs=0
	END
	ELSE
	BEGIN
		SET @HadVipSecs=@HadVipSecs-@CurTimeSecs
	END

	--VIP事务
	BEGIN TRAN

	-- 扣金币
	UPDATE TuserInfo
	SET BankMoney=BankMoney-@VipMoney
	WHERE @UserID=UserID
	Insert into Web_MoneyChangeLog(UserID,UserName,StartMoney,ChangeMoney,ChangeType,DateTime,Remark) values(@UserID,@UserName,@SumMoney,-@VipMoney,6,getdate(),'购买VIP');

	-- 加VIP时间
	UPDATE TUserInfo
	SET Viptime=@CurTimeSecs+@HadVipSecs+@VipTimeSec
	WHERE @BenUserID=UserID

	-- 写记录
	INSERT INTO Web_UserApplyVipRecord (UserID,[Money],AddVipTime,AddTime,TargetUserID) 
	VALUES(@UserID,@VipMoney,@VipTimeSec,getdate(),@SaveUserID)

	COMMIT TRAN

	RETURN	@Result
END







