CREATE TABLE TBuyGameDeskRecordDelete
(
		ID INT NOT NULL,--ID��
		DeskPassword NVARCHAR(20) NOT NULL,--��������
		GameID INT NOT NULL ,
		DeskMaster INT NOT NULL,--����
		ToTalCount INT NOT NULL ,
		DeleteTime DATETIME,--���ʱ��
		CreateTime DATETIME --����ʱ��
)
		

ALTER TABLE dbo.TBuyGameDeskRecordDelete ADD RoomID INT DEFAULT(0),DeskID INT DEFAULT(0)