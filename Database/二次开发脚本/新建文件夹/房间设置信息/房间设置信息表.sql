go
if exists(select * from dbo.sysobjects where id = object_id('TSPRoomConfigInfo'))  
drop table TSPRoomConfigInfo  
go
CREATE TABLE TSPRoomConfigInfo
(
		RoomID int,
		RoomConfig varbinary(MAX),
		UpdateTime datetime,
)