go
if exists(select * from dbo.sysobjects where id = object_id('SP_DissmissDeskByMS'))  
drop procedure SP_DissmissDeskByMS  
go

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE procedure SP_DissmissDeskByMS
as
begin
	set nocount ON
	declare @rttable table(RoomID int, deskNo int, deskpwd varchar(20), MasterID int, PositionLimit int)
	
	declare @tmptable table(GameID int, deskpwd varchar(20), MasterID int, PositionLimit int)

	insert into @tmptable 
	select GameID,szDeskPassWord,MasterID,PositionLimit from TBuyGameDeskRecord where RoomID=255 and TableNumber=255 and FinishCondition=3 
		and IsPlay <> 0 and (datediff(second,BeginTime, getdate()) >= BuyMinutes * 60)
	
	declare @tmpRoomID int
	declare @tmpdeskNo int
	declare @tmpMasterID int
	declare @tmpPositionLimit int
	
	declare @tmpGameID int
	declare @tmpdeskpwd varchar(20)
	declare db_cursor CURSOR SCROLL for select * from @tmptable
	open db_cursor
	fetch first from db_cursor into @tmpGameID,@tmpdeskpwd,@tmpMasterID,@tmpPositionLimit
	while @@fetch_status=0
	begin
		SELECT @tmpRoomID=RoomID ,@tmpdeskNo=DeskID FROM HNF_DistributeTable(@tmpGameID)
		if @tmpRoomID<>255 AND @tmpRoomID<>255
		begin
			UPDATE TBuyGameDeskRecord SET RoomID=@tmpRoomID ,TableNumber=@tmpdeskNo WHERE szDeskPassWord=@tmpdeskpwd
			insert into @rttable values(@tmpRoomID,@tmpdeskNo,@tmpdeskpwd,@tmpMasterID,@tmpPositionLimit)
		end
		fetch next from db_cursor into @tmpGameID,@tmpdeskpwd,@tmpMasterID,@tmpPositionLimit
	end
	close db_cursor
	deallocate db_cursor
	
	select * from  @rttable
	return 0
end