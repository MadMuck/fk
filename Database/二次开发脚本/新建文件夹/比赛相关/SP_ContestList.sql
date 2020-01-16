GO
/****** Object:  StoredProcedure [dbo].[SP_ContestList]    Script Date: 02/17/2017 10:33:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[SP_ContestList]
--@UserID INT --用户ID
AS 
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @ContestTable TABLE(
		ContestID INT ,
		RoomName NVARCHAR(100),
		ContestType INT ,
		BeginTime DATETIME,
		ContestAward INT ,
		AwardType INT ,
		OnlinePeople INT ,
		UpPeople INT ,
		ContestTime INT ,
		CircleTime  INT,
		EntryFee INT,
		EntryFeeType INT,
		Invalid INT,
		GameID INT )
	
	INSERT INTO @ContestTable( ContestID ,ContestType ,BeginTime ,ContestTime ,CircleTime,UpPeople,EntryFee,EntryFeeType)
	SELECT ContestID,ContestType,StartTime,ContestTime,CircleTime,UpPeople,EntryFee,EntryFeeType FROM TContestInfo WHERE ContestID IN  (SELECT a.ContestID from TGameRoomBase a 
	inner join TGameRoomInfo b on a.RoomID=b.RoomID and ((a.RoomRule&8388608)>0 OR (a.RoomRule&1048576)>0) AND UpdateTime >= DATEADD(MI, - 3, GETDATE()))
	
	DECLARE @tmpContestID INT 
	DECLARE @tmpContestType INT 
	DECLARE @tmpCircleTime INT 
	DECLARE @tmpContestTime INT 
	DECLARE @tmpBeginTime DATETIME
	
	DECLARE @GameNameID INT
	DECLARE @RoomName NVARCHAR(30)
	DECLARE @OnlinePeople INT 
	DECLARE @Invalid INT 
	
	DECLARE @ContestAward INT 
	DECLARE @AwardType INT 
	
	declare db_cursor CURSOR SCROLL for select ContestID,ContestType,ContestTime,CircleTime,BeginTime from @ContestTable
	open db_cursor
	fetch first from db_cursor into @tmpContestID,@tmpContestType,@tmpContestTime,@tmpCircleTime,@tmpBeginTime
	while @@fetch_status=0
	BEGIN
		SELECT TOP 1 @RoomName=b.RoomName,@GameNameID=b.GameNameID
		FROM TGameRoomBase a INNER JOIN TGameRoomInfo b ON a.RoomID=b.RoomID 
		WHERE a.ContestID=@tmpContestID
		
		SELECT @OnlinePeople=SUM(b.OnlineCount)
		FROM TGameRoomBase a INNER JOIN TGameRoomInfo b ON a.RoomID=b.RoomID 
		WHERE a.ContestID=@tmpContestID GROUP BY a.ContestID
	
		SELECT @ContestAward=b.GiveNum ,@AwardType=b.GiveType FROM dbo.TContestInfo a ,dbo.Web_MatchAwardConfig b 
		WHERE a.ContestID=@tmpContestID AND a.SendTypeID=b.TypeID AND b.Rank=1
	
		IF @tmpContestType=2 AND (GETDATE()>@tmpBeginTime)		--循环赛计算最新比赛时间
		BEGIN 
			DECLARE @Totalmi INT 
			DECLARE @CountTime INT 
			SET @Totalmi = DATEDIFF(mi,@tmpBeginTime,GETDATE())
			SET @CountTime= @tmpCircleTime-(@Totalmi%@tmpCircleTime)
			SET @tmpBeginTime = DATEADD(mi,(@Totalmi+@CountTime),@tmpBeginTime)
			
			UPDATE dbo.TContestInfo SET StartTime=@tmpBeginTime WHERE ContestID=@tmpContestID
		END 
		
		------------判断比赛是否可报名
		IF @tmpContestType=0
		BEGIN 
			IF EXISTS (SELECT * FROM dbo.TGameRoomBase a inner join TGameRoomInfo b on a.RoomID=b.RoomID WHERE ContestID=@tmpContestID 
			AND a.RoomID NOT IN (SELECT RoomID FROM dbo.TSignUp WHERE ContestID=@tmpContestID AND SignState=1)
			AND UpdateTime >= DATEADD(MI, - 3, GETDATE()) AND a.RoomRule&1048576>0)
				SET @Invalid=0
			ELSE 
				SET @Invalid=1
		END 
		ELSE IF @tmpContestType=1
		BEGIN 
			IF GETDATE()<@tmpBeginTime 
			AND EXISTS (SELECT * FROM dbo.TGameRoomBase a inner join TGameRoomInfo b on a.RoomID=b.RoomID WHERE ContestID=@tmpContestID 
				AND UpdateTime >= DATEADD(MI, - 3, GETDATE()) AND a.RoomRule&8388608>0)
				SET @Invalid=0
			ELSE 
				SET @Invalid=1
		END 
		ELSE IF @tmpContestType=2
		BEGIN 
			IF EXISTS (SELECT * FROM dbo.TContestCircleFreeRecord WHERE ContestID=@tmpContestID AND FREE<>2)
				SET @Invalid=0
			ELSE 
				SET @Invalid=1
		END 
		------------
		
		UPDATE @ContestTable SET RoomName=@RoomName, OnlinePeople=@OnlinePeople,ContestAward=@ContestAward ,
									AwardType=@AwardType,BeginTime=@tmpBeginTime,Invalid=@Invalid,GameID=@GameNameID
		WHERE ContestID=@tmpContestID
		
		fetch next from db_cursor into @tmpContestID,@tmpContestType,@tmpContestTime,@tmpCircleTime,@tmpBeginTime
	end
	close db_cursor
	deallocate db_cursor
	
	SELECT * FROM @ContestTable
	
END 