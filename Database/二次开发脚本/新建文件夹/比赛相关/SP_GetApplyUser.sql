
GO
/****** Object:  StoredProcedure [dbo].[SP_GetApplyUser]    Script Date: 02/21/2017 17:05:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[SP_GetApplyUser]
    @RoomID INT ,
    @ContestID INT
AS 
BEGIN
    SET NOCOUNT ON ;
	DECLARE @CountTime INT 
	DECLARE @RoomName NVARCHAR(50)
	DECLARE @BeginTime DATETIME
	DECLARE @MatchID INT 
	--校验数据
	IF NOT EXISTS (SELECT * FROM dbo.TGameRoomBase WHERE RoomID=@RoomID AND ContestID=@ContestID)
		RETURN 1		--房间初始化信息错误
		
	SELECT @RoomName=RoomName FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID
	--校验时间
	IF (SELECT ContestType FROM dbo.TContestInfo WHERE ContestID=@ContestID) = 1
	BEGIN 
		SELECT @BeginTime = StartTime FROM dbo.TContestInfo WHERE ContestID=@ContestID
		IF @BeginTime<=GETDATE()
			RETURN 3
		ELSE 
			SET @CountTime = DATEDIFF(ss,@BeginTime,GETDATE())
			SELECT UserID,@RoomName AS RoomName,@BeginTime AS BeginTime FROM dbo.TSignUp WHERE ContestID = @ContestID AND SignState<>1
	END 
	ELSE IF (SELECT ContestType FROM dbo.TContestInfo WHERE ContestID=@ContestID) = 2
	BEGIN 
		SELECT @BeginTime=BeginTime,@MatchID=MatchID FROM dbo.TContestCircleFreeRecord WHERE RoomID=@RoomID AND ContestID=@ContestID
		IF @BeginTime = NULL OR @BeginTime<=GETDATE()
			RETURN 3
		ELSE
			SET @CountTime = DATEDIFF(ss,@BeginTime,GETDATE())
			SELECT UserID,@RoomName AS RoomName,@BeginTime AS BeginTime FROM dbo.TSignUp WHERE ContestID = @ContestID AND MatchID=@MatchID AND SignState<>1
	END 
	ELSE
		RETURN 2
   
	RETURN 0
   
END
