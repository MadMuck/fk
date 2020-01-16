
--���߼�¼��
CREATE TABLE TGameCutNetRecord(
	DeskPassword NVARCHAR(20),
	UserID INT ,
	UserStation INT,
)

--������ս��
CREATE TABLE TDeskTotleRecord(
	ID INT ,
	DeskPassword NVARCHAR(20),
	DeskUserID INT ,
	UserScore INT ,
	CreateTime DATETIME,
	GameID INT ,
	PlayCount INT DEFAULT(1) NOT NULL
)

--����ս��
CREATE TABLE TUserGameRecord(
	ID INT ,
	GameID INT ,
	DeskPassword NVARCHAR(20),
	DeskUserID INT,
	UserScore INT,
	PlayCount INT,
	StartTime DATETIME,
	RecordNum NVARCHAR(20),
	GameType INT --0��ͨ����1�����䣬2����
)

--��Ϸ��ˮ��
CREATE TABLE TGameSNRecord(
	ID INT IDENTITY(1000,1),
	GameSN NVARCHAR(20),
	RoomID INT ,
	DeskID INT,
	StartTime DATETIME
)

--��Ϸ��¼��
CREATE TABLE TRecordGameMSG(
	UserID INT ,
	GameInfo VARBINARY(MAX),
	DeskPassword NVARCHAR(20)
)


--������������
--CREATE TABLE TBuyDeskConfig(
	
--)

ALTER TABLE dbo.Web_Config ADD RegJewels INT DEFAULT(1000) NOT NULL