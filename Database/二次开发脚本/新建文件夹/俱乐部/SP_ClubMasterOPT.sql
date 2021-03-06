USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubMasterOPT]    Script Date: 05/10/2019 09:26:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_ClubMasterOPT]
@UserID	INT ,
@ClubID INT ,
@TargetID INT ,
@Type INT			--0-同意，1-拒绝，2-移入黑名单，3-移出黑名单
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	DECLARE @ClubJoinLimit INT 
	DECLARE	@ClubUserLimit INT 
	
	SELECT TOP 1 @ClubJoinLimit=ClubJoinLimit,@ClubUserLimit=ClubUserLimit FROM dbo.Web_Config
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF @MasterID<>@UserID
		RETURN 3		--玩家不是会长
		
	IF EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@TargetID)
	BEGIN 
		DELETE FROM dbo.TClubVerifyTable WHERE UserID=@TargetID AND ClubID=@ClubID
		DELETE FROM dbo.TClubBlacklistTable WHERE UserID=@TargetID AND ClubID=@ClubID
		RETURN 5		--玩家已加入该工会
	END 
	
	IF NOT EXISTS (SELECT * FROM dbo.TUsers WHERE UserID=@TargetID)
		RETURN 7
		
	IF @Type=0
	BEGIN 
		IF NOT EXISTS (SELECT * FROM dbo.TClubVerifyTable WHERE UserID=@TargetID AND ClubID=@ClubID)
			RETURN 6
			
		IF @ClubJoinLimit<>0 AND (SELECT COUNT(*) FROM TClubUserTable WHERE UserID=@TargetID) >= @ClubJoinLimit
			RETURN 8		--超过加入俱乐部限制
		
		IF (SELECT COUNT(*) FROM dbo.TClubUserTable WHERE ClubID=@ClubID) >= @ClubUserLimit
			RETURN 9		--俱乐部人数超过限制
	
		DELETE FROM dbo.TClubVerifyTable WHERE UserID=@TargetID AND ClubID=@ClubID
		INSERT INTO dbo.TClubUserTable(ClubID, UserID) VALUES(@ClubID,@TargetID)
	END 
	ELSE IF @Type=1
	BEGIN 
		DELETE FROM dbo.TClubVerifyTable WHERE UserID=@TargetID AND ClubID=@ClubID
	END 
	ELSE IF @Type=2
	BEGIN 
		DELETE FROM dbo.TClubVerifyTable WHERE UserID=@TargetID AND ClubID=@ClubID
		INSERT INTO dbo.TClubBlacklistTable(ClubID, UserID) VALUES(@ClubID,@TargetID)
	END 
	ELSE IF @Type=3
	BEGIN 
		DELETE FROM dbo.TClubBlacklistTable WHERE UserID=@TargetID AND ClubID=@ClubID
	END 
	ELSE 
		RETURN 4
		
	DECLARE @NickName NVARCHAR(64) 
	DECLARE @HeadURL NVARCHAR(256) 
	DECLARE @LogonID INT 
	DECLARE @Sex INT 
	DECLARE @MasterNickName NVARCHAR(64) 
	DECLARE @MasterHeadURL NVARCHAR(256) 
	DECLARE @MasterLogonID INT 
	DECLARE @MasterSex INT 
	DECLARE @UserNum INT 
	DECLARE @RoomNum INT
	DECLARE @ClubName NVARCHAR(64)
	------
	declare @GroupCount int 
	DECLARE @MasterName NVARCHAR(32)
	declare @ClubIsOpen int
	declare @GameID int
	 set @GroupCount=0
	 set @MasterName=''
	SET @HeadURL=''
	SET @MasterHeadURL=''
	
	SELECT @NickName=NickName,@LogonID=LogoID,@Sex=Sex FROM dbo.TUsers WHERE UserID=@TargetID
	SELECT @HeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@TargetID
	SELECT @MasterNickName=NickName,@MasterName=NickName,@MasterLogonID=LogoID,@MasterSex=Sex FROM dbo.TUsers WHERE UserID=@UserID
	SELECT @MasterHeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@UserID
	
	
	
	SELECT @UserNum=COUNT(*) FROM dbo.TClubUserTable WHERE ClubID=@ClubID
	SELECT @RoomNum=COUNT(*) FROM dbo.TBuyGameDeskRecord WHERE ClubID=@ClubID
	select @GroupCount=COUNT(*)from TClubPlayInfo where ClubID=@ClubID
	SELECT @ClubName=ClubName,@ClubIsOpen=ClubIsOpen FROM dbo.TClubTable WHERE ClubID=@ClubID
	select @GameID=GameID from TClubPlayInfo where ClubID=@ClubID and GroupID=1
	SELECT @NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex,
			@MasterNickName AS MasterNickName,@MasterHeadURL AS MasterHeadURL,@MasterLogonID AS MasterLogoID,@MasterSex AS MasterSex,
			@UserNum AS UserNum,@RoomNum AS RoomNum,@ClubName AS ClubName,@GroupCount as GroupCount,@MasterName as MasterName,@ClubIsOpen as ClubIsOpen,
			@GameID as GameID
	
	RETURN 0
END