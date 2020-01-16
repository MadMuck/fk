go
if exists(select * from dbo.sysobjects where id = object_id('SP_SetGameDynamicInfo'))  
drop procedure SP_SetGameDynamicInfo  
go

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE procedure SP_SetGameDynamicInfo
@GameID int,  
@DynamicInfo1 VARBINARY(128)
as
begin
	set nocount ON
	
	if exists( select * from TGameDynamicInfo where GameID=@GameID)
	begin
		delete from TGameDynamicInfo where GameID=@GameID
		insert into TGameDynamicInfo values(@GameID,@DynamicInfo1)
	end
	else
	begin
		insert into TGameDynamicInfo values(@GameID,@DynamicInfo1)
	end
	
	return 0
end