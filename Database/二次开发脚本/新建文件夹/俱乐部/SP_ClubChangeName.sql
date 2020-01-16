

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_ClubChangeName
@UserID	INT ,
@ClubID INT ,
@ClubName NVARCHAR(32)
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF @MasterID<>@UserID
		RETURN 3		--玩家不是会长
		
	IF EXISTS (SELECT * FROM dbo.TClubTable WHERE ClubName=@ClubName)
		RETURN 4
		
	UPDATE dbo.TClubTable SET ClubName=@ClubName WHERE ClubID=@ClubID
	
	
	RETURN 0
END