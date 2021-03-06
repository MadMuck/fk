/****** Object:  StoredProcedure [dbo].[SP_ContestReturnFee]    Script Date: 01/04/2018 11:26:47 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_ContestReturnFee]
	@UserID			INT,			--用户ID
	@ContestID		INT,			--比赛ID
	@RoomID			INT,			--房间ID
	@GameID			INT				--游戏ID
AS
BEGIN

	declare @EntryFee INT
	DECLARE @EnterFeeType INT
	DECLARE @JewelsBefore INT 
	DECLARE @UserName NVARCHAR(20)
	DECLARE @Money BIGINT
	set @EntryFee = 0
	set @EnterFeeType=0

	select @EntryFee=EntryFee from TSignUp where GameNameID=@GameID and ContestID=@ContestID and RoomID=@RoomID and UserID=@UserID
	SELECT @EnterFeeType=EntryFeeType FROM dbo.TContestInfo WHERE ContestID=@ContestID
	if @EntryFee > 0
	begin
		--将报名费退回到用户的钱包
		--update TUserInfo set WalletMoney=@EntryFee+WalletMoney where UserID=@UserID	
		------------------------------------------
		IF @EnterFeeType=0
				BEGIN 
					SELECT @Money=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
					SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
					UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@EntryFee WHERE UserID=@UserID
					INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
							VALUES  ( @UserID ,@UserName ,@Money ,@EntryFee ,22 ,GETDATE() ,'用户断线比赛退赛')
				END 
		ELSE IF @EnterFeeType=1
				BEGIN
					SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
					update Web_Users set Jewels=Jewels+@EntryFee where UserID=@UserID
					INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
					VALUES(@UserID,@JewelsBefore,@EntryFee,5,GETDATE(),'用户断线比赛退赛返还钻石')
				END 
	end
	--删除该用户的报名信息
	delete from TSignUp where GameNameID=@GameID and ContestID=@ContestID and RoomID=@RoomID and UserID=@UserID
	
END
