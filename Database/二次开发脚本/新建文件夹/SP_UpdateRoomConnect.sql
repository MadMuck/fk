
set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE PROCEDURE SP_UpdateRoomConnect
@RoomList varchar(512)
AS 
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @RoomTable TABLE(RoomID int)
	
    INSERT INTO @RoomTable(RoomID) SELECT a.RS FROM dbo.Split(@RoomList,',') a

	UPDATE dbo.TGameRoomInfo SET IsConnected=1 WHERE RoomID IN (SELECT * FROM @RoomTable)
	UPDATE dbo.TGameRoomInfo SET IsConnected=0 WHERE RoomID NOT IN (SELECT * FROM @RoomTable)
		
END 