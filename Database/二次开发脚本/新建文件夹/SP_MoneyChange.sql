
ALTER PROCEDURE SP_MoneyChange
@UserID int,					--用户ID
@ChangeJewels BIGINT ,				--消耗或兑换的钻石数量
@Type INT						--0-金币兑换钻石，1-钻石兑换金币
AS 
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @ChangeRate INT 
	DECLARE @ChangeMoney BIGINT
	DECLARE @UserName NVARCHAR(20)
	DECLARE @JewelsBefore INT
	DECLARE @Money BIGINT
	DECLARE @Jewels INT
	DECLARE @WalletMoney BIGINT
	
	IF NOT EXISTS (SELECT * FROM dbo.TUserInfo WHERE UserID=@UserID)
		RETURN 1  --用户ID错误
	
	IF @Type=0
	BEGIN
		SELECT TOP 1 @ChangeRate=Money2JewelRate FROM dbo.Web_Config
		IF @ChangeRate=0
			RETURN 2  --此功能关闭
		SET @ChangeMoney = @ChangeJewels*@ChangeRate
		IF (SELECT WalletMoney FROM dbo.TUserInfo WHERE UserID=@UserID) < @ChangeMoney
			RETURN 3  --用户金币不足
		
		SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users WHERE UserID=@UserID
		UPDATE Web_Users SET Jewels=Jewels+@ChangeJewels WHERE UserID=@UserID
		INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
		VALUES(@UserID,@JewelsBefore,@ChangeJewels,2,GETDATE(),'金币兑换钻石')
		
		SELECT @Money=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
		SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
		UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney-@ChangeMoney WHERE UserID=@UserID
		INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
                VALUES  ( @UserID ,@UserName ,@Money ,-@ChangeMoney ,31 ,GETDATE() ,'金币兑换钻石')
	END 
	ELSE 
	BEGIN 
		SELECT TOP 1 @ChangeRate=Jewel2MoneyRate FROM dbo.Web_Config
		IF @ChangeRate=0
			RETURN 2  --此功能关闭
		SET @ChangeMoney = @ChangeJewels*@ChangeRate
		IF (SELECT Jewels FROM dbo.Web_Users WHERE UserID=@UserID) < @ChangeJewels
			RETURN 4  --用户钻石不足	
		
		SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users WHERE UserID=@UserID
		UPDATE Web_Users SET Jewels=Jewels-@ChangeJewels WHERE UserID=@UserID
		INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
		VALUES(@UserID,@JewelsBefore,-@ChangeJewels,3,GETDATE(),'钻石兑换金币')
		
		SELECT @Money=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
		SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
		UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@ChangeMoney WHERE UserID=@UserID
		INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
                VALUES  ( @UserID ,@UserName ,@Money ,@ChangeMoney ,32 ,GETDATE() ,'钻石兑换金币')
		
	END 
	
	SELECT @WalletMoney = WalletMoney FROM dbo.TUserInfo WHERE UserID=@UserID
	SELECT @Jewels = Jewels FROM dbo.Web_Users WHERE UserID=@UserID
	
	SELECT @WalletMoney AS WalletMoney,@Jewels AS Jewels
	RETURN 0
END 