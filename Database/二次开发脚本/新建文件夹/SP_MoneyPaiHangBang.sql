SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROC [dbo].[SP_MoneyPaiHangBang]
    @Count INT ,      --排行前几
    @MoneyType INT --0:钱包+银行，1:钱包，2:银行 
AS 
    BEGIN
		
		set nocount on
		
        DECLARE @MoneySum BIGINT
        DECLARE @WalletMoney BIGINT
        DECLARE @BankMoney BIGINT
		
		declare @ContainRobot int
		set @ContainRobot=1
		
		declare @tmp table(LogoID int default(0), UserID int default(0), NickName varchar(50) default(''), WalletMoney bigint default(0), BankMoney bigint default(0), MoneySum bigint default(0)
		, HeadUrl varchar(256) default(''), SignDescr varchar(128) default(''))
		
	--目前只获取了前30名
		if @ContainRobot=1
		begin
			IF @MoneyType = 0 
				BEGIN
					insert into @tmp(LogoID, UserID, NickName, WalletMoney, BankMoney, MoneySum)
					EXEC ('SELECT TOP ' + @Count + 'B.LogoID, A.UserID, B.NickName, A.WalletMoney, A.BankMoney, A.WalletMoney+A.BankMoney AS MoneySum
					FROM dbo.TUserInfo A, dbo.TUsers B WHERE A.UserID=B.UserID ORDER BY MoneySum DESC' )
				END
			ELSE 
				IF @MoneyType = 1 
					BEGIN
						insert into @tmp(LogoID, UserID, NickName, WalletMoney, BankMoney)
						EXEC ('SELECT TOP ' + @Count + 'B.LogoID, A.UserID, B.NickName, A.WalletMoney, A.BankMoney
						FROM dbo.TUserInfo A, dbo.TUsers B WHERE A.UserID=B.UserID ORDER BY WalletMoney DESC' )
			
					END
				ELSE 
					IF @MoneyType = 2 
						BEGIN
							insert into @tmp(LogoID, UserID, NickName, WalletMoney, BankMoney)
							EXEC ('SELECT TOP ' + @Count + 'B.LogoID, A.UserID, B.NickName, A.WalletMoney, A.BankMoney
							FROM dbo.TUserInfo A, dbo.TUsers B WHERE A.UserID=B.UserID ORDER BY BankMoney DESC' )	
						END
					ELSE 
						BEGIN
			--没有考虑到的情形，直接返回失败
							RETURN 0
						END
        end
        else
        begin
			IF @MoneyType = 0 
				BEGIN
					insert into @tmp(LogoID, UserID, NickName, WalletMoney, BankMoney, MoneySum)
					EXEC ('SELECT TOP ' + @Count + 'B.LogoID, A.UserID, B.NickName, A.WalletMoney, A.BankMoney, A.WalletMoney+A.BankMoney AS MoneySum
					FROM dbo.TUserInfo A, dbo.TUsers B WHERE A.UserID=B.UserID  and B.IsRobot != 1 ORDER BY MoneySum DESC' )
				END
			ELSE 
				IF @MoneyType = 1 
					BEGIN
						insert into @tmp(LogoID, UserID, NickName, WalletMoney, BankMoney)
						EXEC ('SELECT TOP ' + @Count + 'B.LogoID, A.UserID, B.NickName, A.WalletMoney, A.BankMoney
						FROM dbo.TUserInfo A, dbo.TUsers B WHERE A.UserID=B.UserID  and B.IsRobot != 1 ORDER BY WalletMoney DESC' )
			
					END
				ELSE 
					IF @MoneyType = 2 
						BEGIN
							insert into @tmp(LogoID, UserID, NickName, WalletMoney, BankMoney)
							EXEC ('SELECT TOP ' + @Count + 'B.LogoID, A.UserID, B.NickName, A.WalletMoney, A.BankMoney
							FROM dbo.TUserInfo A, dbo.TUsers B WHERE A.UserID=B.UserID  and B.IsRobot != 1 ORDER BY BankMoney DESC' )	
						END
					ELSE 
						BEGIN
			--没有考虑到的情形，直接返回失败
							RETURN 0
						END
        end
        update a set a.HeadUrl=b.HeadUrl from @tmp a join TWeiXinRegister b on a.UserID=b.UserID         
        update a set a.HeadUrl=b.PicPosition from @tmp a join T_HeadPic b on a.UserID=b.UserID and IsMain=1
        update a set a.SignDescr=b.SignDescr from @tmp a join Web_Users b on a.UserID=b.UserID
        declare @rtcount int
        select @rtcount=COUNT(*) from @tmp
        select * from @tmp
        return @rtcount
    END



