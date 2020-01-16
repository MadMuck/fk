

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_ClubReviewList
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
		
	IF @MasterID<>@UserID
		RETURN 3		--玩家不是会长
		
		
	DECLARE @Temp TABLE(UserID INT,LogoID INT,Sex INT,HeadURL NVARCHAR(256),NickName NVARCHAR(64),bTYPE INT )

	INSERT INTO @Temp(UserID,bTYPE) SELECT UserID,0 FROM dbo.TClubVerifyTable WHERE ClubID=@ClubID
	INSERT INTO @Temp(UserID,bTYPE) SELECT UserID,1 FROM dbo.TClubBlacklistTable WHERE ClubID=@ClubID
	
	UPDATE a SET a.LogoID=TUsers.LogoID,a.Sex=TUsers.Sex,a.NickName=TUsers.NickName FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.UserID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.UserID
	
	SELECT * FROM @Temp
	
	RETURN 0
END