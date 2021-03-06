USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubList]    Script Date: 05/10/2019 09:08:58 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_ClubList]
@UserID	INT 
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @Temp TABLE(ClubID INT,MasterID INT DEFAULT(0)NOT NULL ,UserNum INT ,RoomNum INT,LogoID INT,Sex INT,HeadURL NVARCHAR(256),
	ClubName NVARCHAR(64),GroupCount int,MasterName nvarchar(32),ClubIsOpen int,GameID int)

	INSERT INTO @Temp(ClubID) SELECT ClubID FROM dbo.TClubUserTable WHERE UserID=@UserID
	
	UPDATE a SET a.MasterID=TClubTable.MasterID,a.ClubName=TClubTable.ClubName ,a.ClubIsOpen=TClubTable.ClubIsOpen
	FROM @Temp a, dbo.TClubTable WHERE dbo.TClubTable.ClubID=a.ClubID
	DELETE FROM @Temp WHERE MasterID=0
	
	UPDATE a SET a.LogoID=TUsers.LogoID,a.Sex=TUsers.Sex,a.MasterName=TUsers.NickName FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.MasterID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.MasterID
	
	DECLARE @ClubID INT 
	declare db_cursor CURSOR SCROLL for select ClubID from @Temp
	open db_cursor
	fetch first from db_cursor into @ClubID
	while @@fetch_status=0
	begin
		DECLARE @UserNum INT 
		DECLARE @RoomNum INT
		declare @GroupCount int
		declare @GameID int
		SELECT @UserNum=COUNT(*) FROM dbo.TClubUserTable WHERE ClubID=@ClubID
		SELECT @RoomNum=COUNT(*) FROM dbo.TBuyGameDeskRecord WHERE ClubID=@ClubID
		select @GroupCount=COUNT(*) from TClubPlayInfo where ClubID=@ClubID
		select @GameID=GameID from TClubPlayInfo where ClubID=@ClubID and GroupID=1
		UPDATE @Temp SET UserNum=@UserNum,RoomNum=@RoomNum,GroupCount=@GroupCount,GameID=@GameID WHERE ClubID=@ClubID
		
		fetch next from db_cursor into @ClubID
	end
	close db_cursor
	deallocate db_cursor
	
	SELECT * FROM @Temp
	
	RETURN 0
END