
GO
/****** Object:  StoredProcedure [dbo].[SP_GetGameName]    Script Date: 11/22/2016 11:19:21 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Fred Huang
-- Create date: 2008-11-18
-- Description:	获取游戏名称列表
-- =============================================
ALTER PROCEDURE [dbo].[SP_GetGameName]
AS
BEGIN

	SET NOCOUNT ON;
	DECLARE @NameTable TABLE (NameID INT,CanBuy INT DEFAULT(0) NOT NULL)
	INSERT INTO @NameTable(NameID) SELECT NameID FROM dbo.TGameNameInfo
	
	DECLARE @NameID INT 
	DECLARE @CanBuy INT 
	declare db_cursor CURSOR SCROLL for select NameID from @NameTable
	open db_cursor
	fetch first from db_cursor into @NameID
	while @@fetch_status=0
	begin
		set @CanBuy=0
		IF NOT EXISTS (SELECT * FROM dbo.TGameRoomInfo a INNER JOIN dbo.TGameRoomBase b ON a.RoomID = b.RoomID WHERE a.GameNameID=@NameID AND (b.RoomRule&33554432)>0)
			SET @CanBuy=1
		ELSE IF EXISTS (SELECT * FROM dbo.TGameRoomInfo a INNER JOIN dbo.TGameRoomBase b ON a.RoomID = b.RoomID WHERE a.GameNameID=@NameID AND (b.RoomRule&33554432)<=0)
			SET @CanBuy=3
		ELSE 
			SET @CanBuy=2
			
		UPDATE @NameTable SET CanBuy=@CanBuy WHERE NameID=@NameID
		
		fetch next from db_cursor into @NameID
	end
	close db_cursor
	deallocate db_cursor
	
	SELECT * FROM TGameNameInfo(NOLOCK) a INNER JOIN @NameTable b ON a.NameID = b.NameID WHERE Enable=1 ORDER BY IDSort

END
