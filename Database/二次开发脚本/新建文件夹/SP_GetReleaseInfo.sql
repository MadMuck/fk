

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER procedure SP_GetReleaseInfo
@RoomID INT ,   
@DeskID INT  
as
begin
	set nocount on
	
	DECLARE @GetReleaseInfo TABLE(UserID INT ,GameInfo VARBINARY(MAX) ,UserStation INT )
	
	DECLARE @DeskPass NVARCHAR(20)
	SELECT @DeskPass=szDeskPassWord FROM dbo.TBuyGameDeskRecord WHERE RoomID=@RoomID AND TableNumber=@DeskID
	IF @DeskPass IS NULL 
		RETURN 1
	
	INSERT INTO @GetReleaseInfo(UserID,GameInfo) SELECT UserID,GameInfo FROM dbo.TRecordGameMSG WHERE DeskPassword=@DeskPass
	
	UPDATE b SET b.UserStation=a.UserStation FROM dbo.TGameCutNetRecord a INNER JOIN @GetReleaseInfo b ON a.UserID=b.UserID WHERE a.DeskPassword=@DeskPass
	
	--DELETE FROM TRecordGameMSG WHERE DeskPassword=@DeskPass
	
	SELECT * FROM @GetReleaseInfo
end