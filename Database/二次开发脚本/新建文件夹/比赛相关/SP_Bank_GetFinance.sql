USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_Bank_GetFinance]    Script Date: 03/03/2017 15:55:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



-- Stored Procedure

-- ======================================================
-- Author:		JianGuankun
-- Create date: 2012-7-12
-- Description:	获取用户的财务状况，如银行/钱包，奖券等
-- ======================================================
ALTER PROCEDURE [dbo].[SP_Bank_GetFinance]
	@UserID	INT			--用户ID
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE	@BankVer		INT

	DECLARE @UserIDFind		INT
	DECLARE @BankMoney		BIGINT
	DECLARE @WalletMoney	BIGINT
	DECLARE @Lotteries		INT
	DECLARE @UserJewels		INT 

	SET @BankMoney = 0
	SET @WalletMoney = 0
	SET @Lotteries = 0
	SET @UserJewels = 0

	-- 先判断密码是否正确
	SELECT @UserIDFind = UserID FROM TUsers WHERE UserID = @UserID

	IF @UserIDFind IS NULL
	BEGIN
		--用户不存在
		RETURN 1
	END

	--获取银行的版本
	SELECT TOP 1 @BankVer = BankVersion FROM Web_Config 

	--获取银行存款
	SELECT @BankMoney = BankMoney FROM TUserInfo WHERE UserID = @UserID

	--获取取钱包
	IF @BankVer = 1
		BEGIN		
			SELECT @WalletMoney = WalletMoney FROM TUserInfo WHERE UserID = @UserID
		END
	ELSE IF @BankVer = 2
		BEGIN
			SELECT @WalletMoney = SUM(WalletMoney) FROM TBankWallet WHERE UserID = @UserID
		END

	--获取奖券
	SELECT @Lotteries = Lotteries FROM Web_Users WHERE UserID = @UserID
	--获取钻石
	SELECT @UserJewels=Jewels FROM Web_Users where UserID=@UserID

	--返回结果集
	SELECT @BankMoney AS BankMoney,
		   @WalletMoney AS WalletMoney,
		   @Lotteries AS Lotteries,
		   @UserJewels AS UserJewels

END
	RETURN 0




