SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		
-- Create date: 
-- Description:	
-- =============================================
ALTER FUNCTION HNF_DistributeTable
(
	@GameID varchar(15)	--游戏名字
)
RETURNS 
@Temp TABLE 
(
	RoomID INT ,
	DeskID INT 
)
AS
BEGIN
	declare @CanBuyDesk table(RoomID int not null,DeskCount int null)--可以购买的房间信息
	insert into @CanBuyDesk 
	select a.RoomID,a.DeskCount from TGameRoomBase a 
	inner join TGameRoomInfo b on a.RoomID=b.RoomID and (a.RoomRule&33554432)>0 AND UpdateTime >= DATEADD(MI, - 3, GETDATE()) AND b.GameNameID=@GameID AND IsConnected=1
	declare @tempRoomID int,@tempDeskCount int,@index int
	declare @szTempPassWord varchar(20)
	declare db_cursor CURSOR SCROLL for select * from @CanBuyDesk
	open db_cursor
	fetch first from db_cursor into @tempRoomID,@tempDeskCount
	while @@fetch_status=0
	begin
		set @index=0
		while @index<@tempDeskCount
		begin
			if not exists(select * from TBuyGameDeskRecord where RoomID=@tempRoomID and TableNumber=@index)--找到没有购买的桌子
			BEGIN
				INSERT INTO @Temp(RoomID,DeskID) VALUES(@tempRoomID,@index)
			
				close db_cursor
				deallocate db_cursor
				
				RETURN 
			end
			set @index=@index+1
		end
		fetch next from db_cursor into @tempRoomID,@tempDeskCount
	end
	close db_cursor
	deallocate db_cursor
	
	INSERT INTO @Temp(RoomID,DeskID) VALUES(255,255)
	RETURN 
END
GO