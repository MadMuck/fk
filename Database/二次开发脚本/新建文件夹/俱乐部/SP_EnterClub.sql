USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_EnterClub]    Script Date: 05/16/2019 17:11:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_EnterClub]
@UserID	INT ,
@ClubID INT 
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 3		--玩家未加入该工会

	declare @Money bigint
	declare @UserPoint bigint
	select @Money=WalletMoney from TUserInfo where UserID=@UserID
	select @UserPoint=ClubPoints from TClubUserTable where UserID=@UserID and ClubID=@ClubID
	SELECT @Money as Money,@UserPoint as UserPoint,ClubNotice,MasterID FROM dbo.TClubTable WHERE ClubID=@ClubID
	
	RETURN 0
END