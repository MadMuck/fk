
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestAbandon]    Script Date: 02/21/2017 17:05:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[SP_ContestAbandon]
    @gameID INT ,
    @roomID INT ,
    @ContestID INT
AS 
    BEGIN
        SET NOCOUNT ON ;
		
		DECLARE @UserIDTable TABLE(UserID INT,UserMoney BIGINT,UserJewels BIGINT)
		DECLARE @UserID INT 
		DECLARE @EnterFee INT 
		DECLARE @EnterFeeType INT 
		DECLARE @UserMoney BIGINT
		DECLARE @UserJewels BIGINT
		DECLARE @UserName NVARCHAR(20)
		
		SELECT @EnterFee=EntryFee,@EnterFeeType=EntryFeeType FROM dbo.TContestInfo WHERE ContestID=@ContestID
		
		declare db_cursor CURSOR SCROLL for select UserID from dbo.TSignUp WHERE ContestID=@ContestID AND RoomID=@roomID AND GameNameID=@gameID
		open db_cursor
		fetch first from db_cursor into @UserID
		while @@fetch_status=0
		BEGIN
			--退还报名费
			IF @EnterFeeType=0
			BEGIN 
				SELECT @UserMoney=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
				SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
				UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@EnterFee WHERE UserID=@UserID
				INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
						VALUES  ( @UserID ,@UserName ,@UserMoney ,@EnterFee ,22 ,GETDATE() ,'比赛退赛')
			END 
			--删除报名信息
			DELETE FROM dbo.TSignUp WHERE ContestID=@ContestID AND RoomID=@roomID AND GameNameID=@gameID AND UserID=@UserID
			--循环赛重置房间信息
			UPDATE dbo.TContestCircleFreeRecord SET FREE=0 WHERE ContestID=@ContestID AND RoomID=@roomID
			
			SELECT @UserMoney=WalletMoney FROM dbo.TUserInfo WHERE UserID=@UserID
			SELECT @UserJewels=Jewels FROM Web_Users where UserID=@UserID
        
			INSERT INTO @UserIDTable ( UserID, UserMoney, UserJewels )
			VALUES  (@UserID,@UserMoney,@UserJewels)
			
			fetch next from db_cursor into @UserID
		end
		close db_cursor
		deallocate db_cursor
	
		SELECT * FROM @UserIDTable
      
    END
