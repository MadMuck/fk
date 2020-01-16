go
if exists(select * from dbo.sysobjects where id = object_id('SP_GetSPGameInfo'))  
drop procedure SP_GetSPGameInfo  
go

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE procedure SP_GetSPGameInfo
@RoomID int  
as
begin
	set nocount ON
	
	select * from TSPRoomConfigInfo where RoomID=@RoomID
	return 0
end