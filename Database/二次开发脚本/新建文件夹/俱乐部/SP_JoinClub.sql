

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_JoinClub
@UserID	INT ,
@ClubID INT 
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @ClubJoinLimit INT 
	DECLARE	@ClubUserLimit INT 
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT TOP 1 @ClubJoinLimit=ClubJoinLimit,@ClubUserLimit=ClubUserLimit FROM dbo.Web_Config
	
	IF @ClubJoinLimit<>0 AND (SELECT COUNT(*) FROM TClubUserTable WHERE UserID=@UserID) >= @ClubJoinLimit
		RETURN 2		--����������ֲ�����
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM dbo.TClubTable WHERE ClubID=@ClubID
	IF @MasterID=0
		RETURN 3		--���ֲ�ID����
	
	IF (SELECT COUNT(*) FROM dbo.TClubUserTable WHERE ClubID=@ClubID) >= @ClubUserLimit
		RETURN 4		--���ֲ�������������
	
	IF EXISTS (SELECT * FROM dbo.TClubBlacklistTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 5		--�ѱ����������
		
	IF EXISTS (SELECT * FROM TClubUserTable WHERE UserID=@UserID AND ClubID=@ClubID)
		RETURN 6		--�Ѿ��ھ��ֲ���
	
	IF EXISTS (SELECT * FROM TClubVerifyTable WHERE UserID=@UserID AND ClubID=@ClubID)
		RETURN 7		--�Ѿ������
	
	INSERT INTO dbo.TClubVerifyTable(ClubID, UserID) VALUES(@ClubID,@UserID)
	
	DECLARE @NickName NVARCHAR(64) 
	DECLARE @HeadURL NVARCHAR(256) 
	DECLARE @LogonID INT 
	DECLARE @Sex INT 
	SET @HeadURL=''
	
	SELECT @NickName=NickName,@LogonID=LogoID,@Sex=Sex FROM dbo.TUsers WHERE UserID=@UserID
	SELECT @HeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@UserID
	
	SELECT @MasterID AS MasterID,@NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex
	
	RETURN 0
END