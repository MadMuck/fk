go
if exists(select * from dbo.sysobjects where id = object_id('SP_SetSPGameInfo'))  
drop procedure SP_SetSPGameInfo  
go

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE procedure SP_SetSPGameInfo
@RoomID int,
@RoomConfig varbinary(MAX)
as
begin
	set nocount ON
	
	delete from TSPRoomConfigInfo where RoomID=@RoomID
	insert into TSPRoomConfigInfo values(@RoomID,@RoomConfig,GETDATE())
	
	return 0
end