USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_BankMoneyOpera]    Script Date: 01/30/2018 15:08:31 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO






-- Stored Procedure

-- =============================================
-- Author:		Fred Huang
-- Create date: 2008-12-03
-- Description:	银行存取款
-- =============================================
ALTER PROCEDURE [dbo].[SP_BankMoneyOpera]
	@UserID			INT,			--用户ID号
	@MoneyQuantitys	NVARCHAR(32),	--存取款的金币数
	@RoomID			INT,			--在哪个房间进行操作
	@IsSave			INT,			--是存款还是取款-- 1：存款，2：取款
	@MD5Pass		NVARCHAR(50)	--银行密码，只有取款时用得着


AS
BEGIN
SET NOCOUNT ON;

DECLARE	@MoneyInBank	BIGINT			--银行中的金币
DECLARE	@MoneyInWallet	BIGINT			--用户身上的金币
DECLARE	@MoneyQuantity	BIGINT			--用户身上的金币
DECLARE @Salt			VARCHAR(20)
DECLARE	@BankVer		INT --银行版本 JianGuankun 2011.12.16
DECLARE @GameID BIGINT
SET @MoneyInBank=0
SET @MoneyInWallet=0
SET @MoneyQuantity=0
	SELECT TOP 1 @BankVer = BankVersion FROM Web_Config --获取银行的版本
	SELECT @GameID = GameNameID FROM TGameRoomInfo WHERE RoomID = @RoomID --获取游戏NameID
	SET @MoneyQuantity= convert(bigint, @MoneyQuantitys)

 

	IF @BankVer = 1
	BEGIN
		DECLARE	@InWServerNums	INT
		SET @InWServerNums = 0
		SELECT @InWServerNums = COUNT(DISTINCT WID) FROM TWLoginRecord WHERE UserID = @UserID
		IF @InWServerNums > 1
		BEGIN
			RETURN 5
		END
	END

 
	--检查数据库中的金币
	IF @IsSave = 1
		BEGIN	--存款

			--不同银行的版本的存取方式有所差异 JianGuankun 2011.12.16
			IF @BankVer = 1
			BEGIN		
				SELECT	@MoneyInBank = BankMoney,@MoneyInWallet = WalletMoney FROM TUserInfo WITH(NOLOCK) WHERE UserID = @UserID
			END
			ELSE IF @BankVer = 2
			BEGIN
				SELECT @MoneyInBank = BankMoney FROM TUserInfo WITH(NOLOCK) WHERE UserID = @UserID
				SELECT @MoneyInWallet = WalletMoney FROM TBankWallet WITH(NOLOCK) WHERE UserID = @UserID AND GameID = @GameID
			END
		 

	 			--没有记录，返回错误
			IF @MoneyInBank IS NULL
			BEGIN
				RETURN 1
			END

			IF @MoneyInWallet < 0	--身上无现金
			BEGIN
				RETURN 2
			END

			IF @MoneyInWallet < @MoneyQuantity	--身上钱没那么多
			BEGIN
				SET @MoneyQuantity = @MoneyInWallet	--只能存身上的金币
			END

			--不同银行的版本的存取方式有所差异 JianGuankun 2011.12.16
			IF @BankVer = 1
			BEGIN

				--更新数据			
				UPDATE	TUserInfo
				SET		WalletMoney=WalletMoney-@MoneyQuantity,
						BankMoney=BankMoney+@MoneyQuantity
				WHERE	UserID=@UserID
			END
			ELSE IF @BankVer = 2
			BEGIN
				--更新数据	
				UPDATE TUserInfo SET BankMoney = BankMoney + @MoneyQuantity WHERE UserID = @UserID
				UPDATE TBankWallet SET WalletMoney = WalletMoney - @MoneyQuantity WHERE UserID = @UserID AND GameID = @GameID
			END

			--记录
			INSERT INTO TMoneyChangeRecord (
							UserID,moneyInBank,
							MoneyInRoom,OutMoney,
							InMoney,RoomID,
							TimeEx,Error) 
			VALUES		(@UserID,@MoneyInBank,
						 @MoneyInWallet,@MoneyQuantity,
						 0,@RoomID,
						 GETDATE(),8)
		END
	ELSE
		BEGIN	--取款
			-- 使用密码来取款

			DECLARE @TwoPassWd NVARCHAR(50)

			-- 先判断密码是否正确 
			SELECT @TwoPassWd = ISNULL(TwoPassword, ''),@Salt = Salt from TUsers 
			WHERE UserID = @UserID 

			--银行密码匹配2次MD5或者1次MD5 modify by wyl  2011/5/3
			IF @Salt IS NOT NULL AND DATALENGTH(LTRIM(@Salt)) > 0
			BEGIN
				DECLARE @MD5Temp	VARCHAR(50) --NVARCHAR因为是Unicode编码，MD5后结果不一样
				SET @MD5Temp = @MD5Pass
				SET @MD5Temp = sys.fn_VarBinToHexStr(HashBytes('MD5', @MD5Temp + @Salt))
				SET @MD5Temp = substring(@MD5Temp,charindex('x',@MD5Temp)+1,len(@MD5Temp))

				SELECT	@MoneyInBank = a.BankMoney
				FROM	TUserInfo as a WITH(NOLOCK) inner join TUsers as b ON a.UserID=b.UserID
				WHERE	a.UserID=@UserID and b.TwoPassword = @MD5Temp
				IF @MD5Temp<>@TwoPassWd
				BEGIN
					RETURN 2 --银行密码不对 
				END 
			END
			ELSE
			BEGIN
				SELECT	@MoneyInBank = a.BankMoney
				FROM	TUserInfo as a WITH(NOLOCK) inner join TUsers as b ON a.UserID=b.UserID
				WHERE	a.UserID=@UserID and b.TwoPassword = @MD5Pass
				IF @MD5Pass<>@TwoPassWd 
				BEGIN 
					RETURN 2 --银行密码不对 
				END 
			END

			

			--不同银行的版本的存取方式有所差异 JianGuankun 2011.12.16
			IF @BankVer = 1
			BEGIN		
				SELECT @MoneyInWallet = WalletMoney FROM TUserInfo WITH(NOLOCK) WHERE UserID = @UserID
			END
			ELSE IF @BankVer = 2
			BEGIN
				SELECT @MoneyInWallet = WalletMoney FROM TBankWallet WITH(NOLOCK) WHERE UserID = @UserID AND GameID = @GameID
			END

 
			--没有记录，返回错误
			IF @MoneyInBank IS NULL
			BEGIN
				RETURN 1
			END

			IF @MoneyInBank < 0	--银行中无存款
			BEGIN
				RETURN 2
			END

			IF @MoneyInBank < @MoneyQuantity	--银行中取款不足
			BEGIN
				SET @MoneyQuantity = @MoneyInBank	--只能取银行中剩余的金币
			END

			--不同银行的版本的存取方式有所差异 JianGuankun 2011.12.16
			IF @BankVer = 1
			BEGIN
				--更新数据			
				UPDATE	TUserInfo
				SET		WalletMoney=WalletMoney+@MoneyQuantity,
						BankMoney=BankMoney-@MoneyQuantity
				WHERE	UserID=@UserID
			END
			ELSE IF @BankVer = 2
			BEGIN
				DECLARE @WalletInGame BIGINT
				SELECT @WalletInGame = WalletMoney FROM TBankWallet WHERE UserID = @UserID AND GameID = @GameID
				IF @WalletInGame IS NULL
				BEGIN
					INSERT INTO TBankWallet(UserID,GameID,WalletMoney) VALUES(@UserID,@GameID,@MoneyQuantity)
				END
				ELSE
				BEGIN
					UPDATE TBankWallet SET WalletMoney = WalletMoney + @MoneyQuantity WHERE UserID = @UserID AND GameID = @GameID
				END
				UPDATE TUserInfo SET BankMoney = BankMoney - @MoneyQuantity WHERE UserID = @UserID
			END

			--记录
			INSERT INTO TMoneyChangeRecord (
							UserID,moneyInBank,
							MoneyInRoom,OutMoney,
							InMoney,RoomID,
							TimeEx,Error) 
			VALUES		(@UserID,@MoneyInBank,
						 @MoneyInWallet,0,
						 @MoneyQuantity,@RoomID,
						 GETDATE(),8)
		END


	SELECT @MoneyInBank = BankMoney FROM TUserInfo WHERE UserID = @UserID

	--不同银行的版本的存取方式有所差异 JianGuankun 2011.12.16
	IF @BankVer = 1
	BEGIN		
		SELECT @MoneyInWallet = WalletMoney FROM TUserInfo WITH(NOLOCK) WHERE UserID = @UserID
	END
	ELSE IF @BankVer = 2
	BEGIN
		SELECT @MoneyInWallet = WalletMoney FROM TBankWallet WITH(NOLOCK) WHERE UserID = @UserID AND GameID = @GameID
	END
	  

	SELECT	@MoneyInBank AS MoneyInBank,
			@MoneyQuantity AS MoneyChange,
			@MoneyInWallet AS MoneyInWallet
END
	RETURN 0

