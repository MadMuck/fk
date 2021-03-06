USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UserIntoClub]    Script Date: 05/10/2019 11:21:33 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-12
-- Description:	俱乐部拉人
-- =============================================
ALTER PROCEDURE [dbo].[SP_UserIntoClub]
@UserID	INT ,
@ClubID INT ,
@MasterID INT 
AS
BEGIN
	SET NOCOUNT ON;
		
	DECLARE @ClubJoinLimit INT 
	DECLARE	@ClubUserLimit INT 
	
	SELECT TOP 1 @ClubJoinLimit=ClubJoinLimit,@ClubUserLimit=ClubUserLimit FROM dbo.Web_Config
	
	if not exists(select * from TClubTable where @ClubID=ClubID and @MasterID=MasterID)
	return 1  --俱乐部ID错误  玩家不是会长
	
	
		
	IF EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
	RETURN 2		--玩家已加入该工会

	
	IF NOT EXISTS (SELECT * FROM dbo.TUsers WHERE UserID=@UserID)
		RETURN 3    --没有这个玩家ID
		
		
	IF @ClubJoinLimit<>0 AND (SELECT COUNT(*) FROM TClubUserTable WHERE UserID=@UserID) >= @ClubJoinLimit
		RETURN 4		--该玩家超过加入俱乐部限制  拉人失败
	
	IF (SELECT COUNT(*) FROM dbo.TClubUserTable WHERE ClubID=@ClubID) >= @ClubUserLimit
		RETURN 5		--俱乐部人数超过限制  拉人失败

	
	INSERT INTO dbo.TClubUserTable(ClubID, UserID) VALUES(@ClubID,@UserID)
	
	------
		
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
	SET @HeadURL=''
	SET @MasterHeadURL=''
	----------------
	declare @GroupCount int 
	DECLARE @MasterName NVARCHAR(32)
	declare @ClubIsOpen int
	declare @GameID int
	 set @GroupCount=0
	 set @MasterName=''
	select @MasterName=NickName from TUsers where @MasterID=UserID
	select @GroupCount=COUNT(*) from TClubPlayInfo where @ClubID=ClubID
	SELECT @NickName=NickName,@LogonID=LogoID,@Sex=Sex FROM dbo.TUsers WHERE UserID=@UserID
	SELECT @HeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@UserID
	SELECT @MasterNickName=NickName,@MasterLogonID=LogoID,@MasterSex=Sex FROM dbo.TUsers WHERE UserID=@UserID
	SELECT @MasterHeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@UserID
	
	SELECT @UserNum=COUNT(*) FROM dbo.TClubUserTable WHERE ClubID=@ClubID
	SELECT @RoomNum=COUNT(*) FROM dbo.TBuyGameDeskRecord WHERE ClubID=@ClubID
	SELECT @ClubName=ClubName,@ClubIsOpen=ClubIsOpen FROM dbo.TClubTable WHERE ClubID=@ClubID
	select @GameID=GameID from TClubPlayInfo where ClubID=@ClubID and GroupID=1
	SELECT @MasterID AS MasterID,@NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex,
			@MasterNickName AS MasterNickName,@MasterHeadURL AS MasterHeadURL,@MasterLogonID AS MasterLogoID,@MasterSex AS MasterSex,
			@UserNum AS UserNum,@RoomNum AS RoomNum,@ClubName AS ClubName,@GroupCount as GroupCount,@MasterName as MasterName,@ClubIsOpen as ClubIsOpen,
			@GameID as GameID
	return 0
END
