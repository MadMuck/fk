
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



-- Stored Procedure

-- ======================================================
-- Author:		
-- Create date: 
-- Description:	
-- ======================================================
ALTER PROCEDURE [dbo].[SP_SignAward]
	@UserID	INT,			--用户ID
	@Type	INT				--操作类型：0-查询，1-执行
AS
BEGIN
	SET NOCOUNT ON;
	
	DECLARE @CountDay INT
	DECLARE @HaveSign INT 
	DECLARE @LastSignTime DATETIME 
	DECLARE @AwardMoney INT 
	DECLARE @AwardJewels INT 
	DECLARE @AwardLotteries INT 
	DECLARE @TotalMoney BIGINT
	DECLARE @UserName NVARCHAR(64)
	SET @CountDay=0
	SET @HaveSign=0
	SET @AwardMoney=0
	SET @AwardJewels=0
	SET @AwardLotteries=0
	
	SELECT TOP 1 @CountDay=ISNULL(SignDay,0),@LastSignTime=ISNULL(LastSignTime,'1900-01-01 00:00:00.000') FROM dbo.TSignAwardLog WHERE UserID=@UserID ORDER BY LastSignTime DESC
	IF DATEDIFF(dd,@LastSignTime,GETDATE()) = 0
	BEGIN 
		SET @HaveSign=1
	END 
	ELSE IF DATEDIFF(dd,@LastSignTime,GETDATE()) = 1
	BEGIN
		SET @HaveSign=0
		SET @CountDay=@CountDay+1
		IF @CountDay > 7
			SET @CountDay=1
	END 
	ELSE
	BEGIN 
		SET @CountDay=1
		SET @HaveSign=0
	END 
	
	IF @Type=0
	BEGIN 
		SELECT TOP 7 SignDay,AwardMoney,AwardJewels,AwardLotteries,@CountDay AS CountDay,@HaveSign AS HaveSign FROM dbo.TSignAward ORDER BY SignDay ASC
	END 
	ELSE IF @Type=1
	BEGIN 
		IF @HaveSign=1
		BEGIN 
			SELECT AwardMoney,AwardJewels,AwardLotteries,@HaveSign AS HaveSign FROM dbo.TSignAward WHERE SignDay=@CountDay
			RETURN 1	--已签到
		END 
		--记录签到信息
		IF EXISTS (SELECT * FROM dbo.TSignAwardLog WHERE UserID=@UserID)
			UPDATE dbo.TSignAwardLog SET SignDay=@CountDay,LastSignTime=GETDATE() WHERE UserID=@UserID
		ELSE 
			INSERT INTO dbo.TSignAwardLog(UserID, LastSignTime, SignDay) VALUES(@UserID,GETDATE(),@CountDay)
		SELECT @AwardMoney=AwardMoney,@AwardJewels=AwardJewels,@AwardLotteries=AwardLotteries FROM dbo.TSignAward WHERE SignDay=@CountDay
		SELECT @UserName=UserName from dbo.TUsers WHERE UserID=@UserID
		
		IF @AwardMoney > 0			--奖励金币
		BEGIN 
			SELECT @TotalMoney=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
			UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@AwardMoney WHERE UserID=@UserID
			INSERT INTO dbo.Web_MoneyChangeLog(UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
			VALUES  (@UserID, @UserName ,@TotalMoney ,@AwardMoney ,50 ,GETDATE(),'签到奖励金币')
		END 
		IF @AwardJewels > 0		--奖励钻石
		BEGIN 
			SELECT @TotalMoney=Jewels+LockJewels FROM dbo.Web_Users WHERE UserID=@UserID
			UPDATE dbo.Web_Users SET Jewels=Jewels+@AwardJewels WHERE UserID=@UserID
			INSERT INTO dbo.TJewelChangeLog(UserID ,StartJewel ,ChangeJewel ,ChangeType ,DateTime ,Remark)
			VALUES  (@UserID,@TotalMoney ,@AwardJewels ,6 ,GETDATE(),'签到奖励钻石')
		END 
		IF @AwardLotteries > 0		--奖励奖券
		BEGIN 
			SELECT @TotalMoney=Lotteries FROM dbo.Web_Users WHERE UserID=@UserID
			UPDATE dbo.Web_Users SET Lotteries=Lotteries+@AwardLotteries WHERE UserID=@UserID
			INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
			VALUES  (@UserID, @UserName ,@TotalMoney ,@AwardLotteries ,@TotalMoney+@AwardLotteries,GETDATE(),'签到奖励奖券',5)
		END 
		
		SELECT @AwardMoney AS AwardMoney,@AwardJewels AS AwardJewels,@AwardLotteries AS AwardLotteries
	END 
	
	RETURN 0
END 