

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE PROCEDURE SP_ClubChangeNotice
@UserID	INT ,
@ClubID INT ,
@ClubNotice NVARCHAR(100)
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--���ֲ�ID����
		
	IF @MasterID<>@UserID
		RETURN 3		--��Ҳ��ǻ᳤
		
	UPDATE dbo.TClubTable SET ClubNotice=@ClubNotice WHERE ClubID=@ClubID
	
	
	RETURN 0
END