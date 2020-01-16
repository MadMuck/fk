set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
if exists(select * from dbo.sysobjects where id = object_id('SP_GetVipDeskConfig'))  
drop procedure SP_GetVipDeskConfig  
go
CREATE PROCEDURE SP_GetVipDeskConfig
@GameID int			--ÓÎÏ·ID
AS 
begin
	SET NOCOUNT ON
	declare @RtTable table(ID int, BuyGameCount int, NeedJewels int, GameID int, AANeedJewels int,Invoice int, EveryJewelForMinutes int, FinishCondition int
	, DynamicInfo1 VARBINARY(max))
	insert into @RtTable(ID,BuyGameCount,NeedJewels,GameID,AANeedJewels)
	select ROW_NUMBER() over (order by ID asc),BuyGameCount,NeedJewels,GameID,AANeedJewels from TVIPDeskToCount a where GameID=@GameID order by BuyGameCount asc
		
	declare @Invoice int
	declare @EveryJewelForMinutes int
	declare @FinishCondition int
	declare @DynamicInfo1 VARBINARY(128)
	declare @DynamicInfo2 VARBINARY(128)
	declare @DynamicInfo3 VARBINARY(128)
	select top 1 @Invoice=isnull(Invoice,0),@EveryJewelForMinutes=EveryJewelForMinutes from Web_Config
	select @DynamicInfo1=DynamicInfo1 from TGameDynamicInfo where GameID=@GameID
	select top 1 @FinishCondition=isnull(FinishCondition,0) from TGameServerInfo where ServerInfoID=@GameID
	
	declare @Count int
	select @Count=COUNT(*) from @RtTable
	if @Count > 0
	begin
		update @RtTable set Invoice=@Invoice,DynamicInfo1=@DynamicInfo1,FinishCondition=@FinishCondition,EveryJewelForMinutes=@EveryJewelForMinutes
	end
	else
	begin
		insert into @RtTable(ID,BuyGameCount,NeedJewels,GameID,AANeedJewels,Invoice,EveryJewelForMinutes,FinishCondition,DynamicInfo1)
		select 1,0,0,@GameID,0,@Invoice,@EveryJewelForMinutes,@FinishCondition,@DynamicInfo1
	end

	select * from @RtTable 
	return 0

end