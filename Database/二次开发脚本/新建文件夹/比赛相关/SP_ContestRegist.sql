
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestRegist]    Script Date: 02/21/2017 17:05:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROC [dbo].[SP_ContestRegist]
    @RoomID INT ,
    @ContestID INT
AS 
BEGIN
    SET NOCOUNT ON ;
	
	--校验数据
	IF NOT EXISTS (SELECT * FROM dbo.TGameRoomBase WHERE RoomID=@RoomID AND ContestID=@ContestID)
		RETURN 1		--房间初始化信息错误
	IF (SELECT ContestType FROM dbo.TContestInfo WHERE ContestID=@ContestID) <> 2
		RETURN 2		--比赛不是循环赛
   
   DELETE FROM dbo.TContestCircleFreeRecord WHERE RoomID=@RoomID
   
   INSERT INTO dbo.TContestCircleFreeRecord(RoomID ,ContestID ,FREE) VALUES(@RoomID,@ContestID,0)
   RETURN 0
   
END
