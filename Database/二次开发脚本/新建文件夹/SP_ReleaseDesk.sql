

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER procedure SP_ReleaseDesk
@DeskPassWord varchar(20),   --×À×ÓÃÜÂë
@RunCount INT  
as
begin
	set nocount on
	UPDATE dbo.TBuyGameDeskRecord SET RoomID=255,TableNumber=255,RunCount=@RunCount WHERE szDeskPassWord=@DeskPassWord
	
	--IF @RunCount=0
	--	DELETE FROM dbo.TGameCutNetRecord WHERE DeskPassword=@DeskPassWord
end