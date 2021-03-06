
GO
/****** Object:  StoredProcedure [dbo].[SP_BankOpNormal]    Script Date: 03/21/2017 14:05:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_BankOpNormal]
    @UserID INT , --用户ID 
    @GameID INT ,
    @OperateType INT ,
    @Money BIGINT ,
    @MD5Pass NVARCHAR(50) --银行密码，只有取款或转帐时用得着 
AS 
    BEGIN 
        SET NOCOUNT ON ; 
        BEGIN TRAN

        DECLARE @InWServerNums INT

        DECLARE @RoomID INT --在哪个房间进行操作 

        SELECT TOP 1
                @RoomID = RoomID
        FROM    TGameRoomInfo
        WHERE   GameNameID = @GameID 
        IF @RoomID IS NULL 
            BEGIN
                SET @RoomID = 0
            END

        DECLARE @BankVer INT --银行版本 JianGuankun 2011.12.16
        SELECT TOP 1
                @BankVer = BankVersion
        FROM    Web_Config --获取银行的版

        DECLARE @_money_op BIGINT 
        DECLARE @_money_userid INT      --钱包userid 
        DECLARE @_money_bank BIGINT --银行中的金币 
        DECLARE @_money_wallet BIGINT --钱包中的金币 

        DECLARE @NewMoneyInBank BIGINT --钱包中的金币 
        DECLARE @NewMoneyInGame BIGINT --钱包中的金币 
        DECLARE @SumMoney BIGINT --总金币数 byyao 
 

        SET @_money_op = 0 
        SET @_money_op = @Money 
        SET @SumMoney = 0 
        SET @_money_wallet = 0 

        IF @Money <= 0 
            BEGIN
                COMMIT TRAN
                RETURN 1
            END

        SELECT  @_money_bank = BankMoney
        FROM    TUserInfo WITH ( NOLOCK )
        WHERE   UserID = @UserID

        IF @GameID > 0 
            BEGIN
                SELECT  @SumMoney = ISNULL(SUM(WalletMoney), 0)
                FROM    TBankWallet WITH ( NOLOCK )
                WHERE   UserID = @UserID 
            END
        ELSE 
            BEGIN
                SELECT  @SumMoney = ISNULL(WalletMoney, 0)
                FROM    TUserInfo WITH ( NOLOCK )
                WHERE   UserID = @UserID 
            END
	
        SET @SumMoney = @SumMoney + @_money_bank  
        IF @_money_bank IS NULL 
            BEGIN 
                COMMIT TRAN
                RETURN 1 
            END 

        IF @GameID > 0 
            BEGIN
                SELECT  @_money_userid = UserID ,
                        @_money_wallet = WalletMoney
                FROM    TBankWallet WITH ( NOLOCK )
                WHERE   UserID = @UserID
                        AND GameID = @GameID 
            END
        ELSE 
            BEGIN
                SELECT  @_money_userid = UserID ,
                        @_money_wallet = WalletMoney
                FROM    TUserInfo WITH ( NOLOCK )
                WHERE   UserID = @UserID
            END
	
        IF @OperateType = 1 --取金币 
            BEGIN

                IF @BankVer = 1
                    AND @RoomID = 0 
                    BEGIN
                        SET @InWServerNums = 0
                        SELECT  @InWServerNums = COUNT(WID)
                        FROM    TWLoginRecord
                        WHERE   UserID = @UserID
                        IF @InWServerNums >= 1 
                            BEGIN
                                COMMIT TRAN
                                RETURN 5
                            END
                    END

                DECLARE @TwoPassWd NVARCHAR(50) 
                DECLARE @Salt VARCHAR(20) 

		-- 先判断密码是否正确 
                SELECT  @TwoPassWd = ISNULL(TwoPassword, '') ,
                        @Salt = Salt
                FROM    TUsers
                WHERE   UserID = @UserID 

		---银行密码匹配2次MD5或者1次MD5 modify by wyl  2011/5/3 
                IF @Salt IS NOT NULL
                    AND DATALENGTH(LTRIM(@Salt)) > 0 
                    BEGIN 
                        DECLARE @MD5Temp VARCHAR(50) --NVARCHAR因为是Unicode编码，MD5后结果不一样 
                        SET @MD5Temp = @MD5Pass 
                        SET @MD5Temp = sys.fn_VarBinToHexStr(HashBytes('MD5',
                                                              @MD5Temp + @Salt)) 
                        SET @MD5Temp = SUBSTRING(@MD5Temp,
                                                 CHARINDEX('x', @MD5Temp) + 1,
                                                 LEN(@MD5Temp)) 
                        IF @MD5Temp <> @TwoPassWd 
                            BEGIN 
                                COMMIT TRAN
                                RETURN 2 --银行密码不对 
                            END 
                    END 
                ELSE 
                    IF @MD5Pass <> @TwoPassWd 
                        BEGIN 
                            COMMIT TRAN
                            RETURN 2 --银行密码不对 
                        END 

                IF @_money_bank < @_money_op 
                    BEGIN 
			-- SET @_money_op = @_money_bank   
                        COMMIT TRAN
                        RETURN 4 
                    END 

		--更新数据 

                SET @NewMoneyInBank = @_money_bank - @_money_op 
                UPDATE  TUserInfo
                SET     BankMoney = @NewMoneyInBank
                WHERE   UserID = @UserID 

                IF @GameID > 0 
                    BEGIN
                        IF @_money_userid IS NULL 
                            BEGIN 
                                INSERT  INTO TBankWallet
                                        ( UserID, GameID, WalletMoney )
                                VALUES  ( @UserID, @GameID, @_money_op ) 
                                SET @NewMoneyInGame = @_money_op 
                            END 
                        ELSE 
                            BEGIN 
                                SET @NewMoneyInGame = @_money_wallet
                                    + @_money_op 
                                UPDATE  TBankWallet
                                SET     WalletMoney = @NewMoneyInGame
                                WHERE   UserID = @UserID
                                        AND GameID = @GameID 
                            END 
                    END
                ELSE 
                    BEGIN	
                        UPDATE  TUserInfo
                        SET     WalletMoney = WalletMoney + @_money_op
                        WHERE   UserID = @UserID
                        SET @NewMoneyInGame = @_money_wallet + @_money_op 
                    END
		
		--记录 
                INSERT  INTO TMoneyChangeRecord
                        ( UserID ,
                          moneyInBank ,
                          MoneyInRoom ,
                          OutMoney ,
                          InMoney ,
                          RoomID ,
                          TimeEx ,
                          Error
                            
                        )
                VALUES  ( @UserID ,
                          @NewMoneyInBank ,
                          @NewMoneyInGame ,
                          0 ,
                          @_money_op ,
                          @RoomID ,
                          GETDATE() ,
                          8
                            
                        ) 
		
                DECLARE @AfterSumMoney BIGINT 
                SET @AfterSumMoney = 0 
                IF @GameID > 0 
                    BEGIN
                        SELECT  @AfterSumMoney = ISNULL(SUM(WalletMoney), 0)
                        FROM    TBankWallet WITH ( NOLOCK )
                        WHERE   UserID = @UserID 
                    END
                ELSE 
                    BEGIN	
                        SELECT  @AfterSumMoney = ISNULL(WalletMoney, 0)
                        FROM    TUserInfo WITH ( NOLOCK )
                        WHERE   UserID = @UserID 
                    END

                SET @AfterSumMoney = @AfterSumMoney + @NewMoneyInBank 
            END
 
        ELSE 
            BEGIN 

                IF @OperateType = 2 --存金币 
                    BEGIN 

                        IF @BankVer = 1
                            AND @RoomID = 0 
                            BEGIN
				
                                SET @InWServerNums = 0
                                SELECT  @InWServerNums = COUNT(WID)
                                FROM    TWLoginRecord
                                WHERE   UserID = @UserID
                                IF @InWServerNums >= 1 
                                    BEGIN
                                        COMMIT TRAN
                                        RETURN 5
                                    END
                            END
                        IF @_money_wallet < @_money_op 
                            BEGIN 
			-- SET @_money_op = @_money_wallet 
                                COMMIT TRAN
                                RETURN 1 
                            END 

                        IF @_money_op IS NULL 
                            BEGIN 
                                COMMIT TRAN
                                RETURN 2 ; 
                            END 

			--更新数据 

                        SET @NewMoneyInBank = @_money_bank + @_money_op 
                        UPDATE  TUserInfo
                        SET     BankMoney = @NewMoneyInBank
                        WHERE   UserID = @UserID 

                        IF @GameID > 0 
                            BEGIN

                                SET @NewMoneyInGame = @_money_wallet
                                    - @_money_op 
                                UPDATE  TBankWallet
                                SET     WalletMoney = @NewMoneyInGame
                                WHERE   UserID = @UserID
                                        AND GameID = @GameID 
                                SET @AfterSumMoney = 0 
                                SELECT  @AfterSumMoney = ISNULL(SUM(WalletMoney),
                                                              0)
                                FROM    TBankWallet WITH ( NOLOCK )
                                WHERE   UserID = @UserID 
                                SET @AfterSumMoney = @AfterSumMoney
                                    + @NewMoneyInBank 

                            END
                        ELSE 
                            BEGIN	
                                SET @NewMoneyInGame = @_money_wallet
                                    - @_money_op 
                                UPDATE  TUserInfo
                                SET     WalletMoney = WalletMoney - @_money_op
                                WHERE   UserID = @UserID
                                SET @AfterSumMoney = @NewMoneyInGame
                                    + @NewMoneyInBank 
                            END

			--记录 
                        INSERT  INTO TMoneyChangeRecord
                                ( UserID ,
                                  moneyInBank ,
                                  MoneyInRoom ,
                                  OutMoney ,
                                  InMoney ,
                                  RoomID ,
                                  TimeEx ,
                                  Error
                                    
                                )
                        VALUES  ( @UserID ,
                                  @NewMoneyInBank ,
                                  @NewMoneyInGame ,
                                  @_money_op ,
                                  0 ,
                                  @RoomID ,
                                  GETDATE() ,
                                  8
                                    
                                ) 
		 
                    END 
            END 

        SELECT  @_money_op AS MoneyOp

        COMMIT TRAN
        RETURN 0 
    END 

-- exec SP_BankOpNormal 10037, 20015400, 1, '1000','d0970714757783e6cf17b26fb8e2298f'
