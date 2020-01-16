

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_ClubUserLeave
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
		
	IF  EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE MasterID=@UserID AND ClubID=@ClubID)
		RETURN 4		--有未完成的游戏
	ELSE

	DECLARE @DeskPass NVARCHAR(20)
	SET @DeskPass=''
	SELECT @DeskPass=ISNULL(DeskPassword,'') FROM dbo.TGameCutNetRecord WHERE UserID=@UserID
	IF @DeskPass<>'' AND EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass AND ClubID=@ClubID)
		RETURN 4
	
	IF @MasterID=@UserID
		RETURN 5		--会长不能退出房间
	
	DELETE FROM dbo.TClubUserTable WHERE UserID=@UserID AND ClubID=@ClubID
	
	DECLARE @NickName NVARCHAR(64) 
	DECLARE @HeadURL NVARCHAR(256) 
	DECLARE @LogonID INT 
	DECLARE @Sex INT 
	SET @HeadURL=''
	
	SELECT @NickName=NickName,@LogonID=LogoID,@Sex=Sex FROM dbo.TUsers WHERE UserID=@UserID
	SELECT @HeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@UserID
	
	SELECT @NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex
	
	RETURN 0
END