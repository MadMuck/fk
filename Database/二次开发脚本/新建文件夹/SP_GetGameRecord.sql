USE [sql]
GO
/****** Object:  StoredProcedure [dbo].[SP_GetGameRecord]    Script Date: 04/18/2019 10:10:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_GetGameRecord]
@UserID INT ,   
@BeginNum INT ,  --0--我的战绩 -1--亲友战绩-   非0非1----就是具体某个成员的战绩
@GameID INT ,	--查询单局战绩时需要的唯一标识
@Optype INT	,	--0查询房卡场的，2查询单局战绩,1查询普通房间战绩
@ClubID INT	
AS 
BEGIN 
	SET NOCOUNT ON
	
	IF NOT EXISTS (SELECT * FROM dbo.TUsers WHERE UserID=@UserID)
		RETURN 1  --没有指定用户
		
	DECLARE @iCount INT  
	SET @iCount=0
		
	IF @Optype=0---房卡场
	BEGIN 
		DECLARE @ToTalRecord TABLE(ID INT ,DeskPassword NVARCHAR(20),DeskUserID INT ,NickName NVARCHAR(64),UserScore INT ,CreateTime DATETIME,iCount INT, 
			GameID INT ,GameName NVARCHAR(20),PlayNum INT ,PlayCount INT ,TotalCount INT ) 
		
		DECLARE @DeskPassTable TABLE (DeskPassword NVARCHAR(20),CreateTime DATETIME)
		
		IF @ClubID<>0 AND @BeginNum=0 --我的战绩
		BEGIN 
			SELECT @iCount=COUNT(DISTINCT DeskPassword) FROM TDeskTotleRecord WHERE ClubID=@ClubID
			IF @iCount>30
				SET @iCount=30
		
			INSERT INTO @DeskPassTable(DeskPassword,CreateTime) SELECT DISTINCT DeskPassword,CreateTime FROM TDeskTotleRecord 
			WHERE ClubID=@ClubID and DeskUserID=@UserID ORDER BY CreateTime DESC
		END 
		ELSE if @ClubID<>0 AND @BeginNum=1 --亲友战绩
		BEGIN 
			SELECT @iCount=COUNT(DISTINCT DeskPassword) FROM TDeskTotleRecord WHERE (DeskUserID=@UserID OR MasterID=@UserID) AND ClubID=@ClubID
			IF @iCount>30
				SET @iCount=30
		
			INSERT INTO @DeskPassTable(DeskPassword,CreateTime) SELECT DISTINCT DeskPassword,CreateTime FROM TDeskTotleRecord 
			WHERE DeskUserID<>@UserID  AND ClubID=@ClubID ORDER BY CreateTime DESC
		END 
		ELSE if @ClubID<>0 AND @BeginNum<>1 and @BeginNum<>0--查询某个玩家的战绩
		BEGIN 
				IF NOT EXISTS (SELECT * FROM dbo.TUsers WHERE UserID=@BeginNum)
				RETURN 1  --没有指定用户
			SELECT @iCount=COUNT(DISTINCT DeskPassword) FROM TDeskTotleRecord WHERE (DeskUserID=@BeginNum OR MasterID=@BeginNum) AND ClubID=@ClubID
			IF @iCount>30
				SET @iCount=30
		
			INSERT INTO @DeskPassTable(DeskPassword,CreateTime) SELECT DISTINCT DeskPassword,CreateTime FROM TDeskTotleRecord 
			WHERE DeskUserID=@BeginNum  AND ClubID=@ClubID ORDER BY CreateTime DESC
		END
		else   --大厅房卡场
		begin
			SELECT @iCount=COUNT(DISTINCT DeskPassword) FROM TDeskTotleRecord WHERE (DeskUserID=@UserID OR MasterID=@UserID) AND ClubID=@ClubID
			IF @iCount>30
				SET @iCount=30
		
			INSERT INTO @DeskPassTable(DeskPassword,CreateTime) SELECT DISTINCT DeskPassword,CreateTime FROM TDeskTotleRecord 
			WHERE (DeskUserID=@UserID OR MasterID=@UserID) AND ClubID=@ClubID ORDER BY CreateTime DESC
		end
		INSERT INTO @ToTalRecord(ID,DeskPassword,DeskUserID,UserScore,CreateTime,GameID,PlayCount,iCount)   --插入基本数据
		SELECT ID,DeskPassword,DeskUserID,UserScore,CreateTime,GameID,PlayCount,@iCount FROM TDeskTotleRecord WHERE DeskPassword IN 
		(SELECT t.DeskPassword FROM (select row_number() OVER (ORDER BY CreateTime DESC) as RowNum,DeskPassword FROM @DeskPassTable)AS t
		WHERE RowNum between 1 and 31)
		
		UPDATE @ToTalRecord SET NickName=b.NickName FROM @ToTalRecord a,TUsers b WHERE a.DeskUserID=b.UserID
		UPDATE @ToTalRecord SET PlayNum=b.DeskPeople,GameName=b.GameName FROM @ToTalRecord a,TGameServerInfo b WHERE a.GameID=b.NameID
		
		UPDATE @ToTalRecord SET TotalCount=b.BuyGameCount FROM @ToTalRecord a,dbo.TBuyGameDeskRecord b WHERE a.ID = b.ID
		UPDATE @ToTalRecord SET TotalCount=b.ToTalCount FROM @ToTalRecord a,dbo.TBuyGameDeskRecordDelete b WHERE a.ID = b.ID
		
		SELECT * FROM @ToTalRecord ORDER BY ID DESC
		
		RETURN 0
	END 
	ELSE IF @Optype=2  ---单局战绩
	BEGIN 
		IF @GameID = 0
			RETURN 2  --数据错误
		SELECT @iCount=ISNULL(MAX(PlayCount),0) FROM TUserGameRecord WHERE ID=@GameID
		
		DECLARE @UserRecord TABLE(ID INT,DeskPassword NVARCHAR(20),DeskUserID INT,NickName NVARCHAR(64),UserScore INT,
			PlayCount INT,EndTime DATETIME,iCount INT,GameSN NVARCHAR(20),GameID INT ,PlayNum INT,CanPlayback BIT)
		
		INSERT INTO @UserRecord(ID,DeskPassword,DeskUserID,UserScore,PlayCount,EndTime,iCount,GameSN,GameID)
		SELECT ID,DeskPassword,DeskUserID,UserScore,PlayCount,Starttime,@iCount,RecordNum,GameID FROM TUserGameRecord WHERE ID=@GameID
			AND PlayCount>=@BeginNum AND PlayCount<=(@BeginNum+19)
		
		UPDATE @UserRecord SET NickName=b.NickName FROM @UserRecord a,dbo.TUsers b WHERE a.DeskUserID = b.UserID
		UPDATE @UserRecord SET PlayNum=b.DeskPeople,CanPlayback=b.CanPalyback FROM @UserRecord a,TGameServerInfo b WHERE a.GameID=b.NameID
		
		SELECT * FROM @UserRecord ORDER BY PlayCount ASC
		
		RETURN 0 
	END 
	ELSE IF @Optype=1   ---金币场
	BEGIN
		SELECT @iCount=ISNULL(COUNT(*),0) FROM dbo.TUserGameRecord WHERE DeskUserID=@UserID AND GameType=0
		IF @iCount>30
			SET @iCount=30
		
		DECLARE @NormalRecord TABLE(ID INT ,DeskUserID INT ,NickName NVARCHAR(64),UserScore INT ,CreateTime DATETIME,iCount INT, 
			GameID INT ,GameName NVARCHAR(20),PlayNum INT) 
		
		DECLARE @GetRecord TABLE(ID INT,Starttime DATETIME)
		
		INSERT INTO @GetRecord(ID,Starttime) 
		SELECT t.RowNum,t.Starttime FROM (select row_number() OVER (ORDER BY Starttime DESC) as RowNum,TUserGameRecord.Starttime 
		FROM TUserGameRecord WHERE DeskUserID=@UserID AND GameType=0)AS t
		WHERE RowNum between 1 and 31
		
		INSERT INTO @NormalRecord(ID,DeskUserID,UserScore,CreateTime,GameID,iCount)   --插入基本数据
		SELECT a.ID,DeskUserID,UserScore,TUserGameRecord.Starttime,GameID,@iCount FROM TUserGameRecord LEFT JOIN @GetRecord a ON dbo.TUserGameRecord.StartTime = a.Starttime 
		WHERE TUserGameRecord.Starttime IN (SELECT Starttime FROM @GetRecord)
		
		
		UPDATE @NormalRecord SET NickName=b.NickName FROM @NormalRecord a,TUsers b WHERE a.DeskUserID=b.UserID
		UPDATE @NormalRecord SET PlayNum=b.DeskPeople,GameName=b.GameName FROM @NormalRecord a,TGameServerInfo b WHERE a.GameID=b.NameID
		
		SELECT * FROM @NormalRecord ORDER BY CreateTime DESC
		
		RETURN 0 
	END 
	ELSE 
		RETURN 2 --数据错误
	
END 