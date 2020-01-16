
--断线记录表
CREATE TABLE TGameCutNetRecord(
	DeskPassword NVARCHAR(20),
	UserID INT ,
	UserStation INT,
)

--房间总战绩
CREATE TABLE TDeskTotleRecord(
	ID INT ,
	DeskPassword NVARCHAR(20),
	DeskUserID INT ,
	UserScore INT ,
	CreateTime DATETIME,
	GameID INT ,
	PlayCount INT DEFAULT(1) NOT NULL
)

--单局战绩
CREATE TABLE TUserGameRecord(
	ID INT ,
	GameID INT ,
	DeskPassword NVARCHAR(20),
	DeskUserID INT,
	UserScore INT,
	PlayCount INT,
	StartTime DATETIME,
	RecordNum NVARCHAR(20),
	GameType INT --0普通场，1开房间，2比赛
)

--游戏流水号
CREATE TABLE TGameSNRecord(
	ID INT IDENTITY(1000,1),
	GameSN NVARCHAR(20),
	RoomID INT ,
	DeskID INT,
	StartTime DATETIME
)

--游戏记录表
CREATE TABLE TRecordGameMSG(
	UserID INT ,
	GameInfo VARBINARY(MAX),
	DeskPassword NVARCHAR(20)
)


--创建房间设置
--CREATE TABLE TBuyDeskConfig(
	
--)

ALTER TABLE dbo.Web_Config ADD RegJewels INT DEFAULT(1000) NOT NULL