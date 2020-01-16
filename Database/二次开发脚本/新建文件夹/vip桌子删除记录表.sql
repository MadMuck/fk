CREATE TABLE TBuyGameDeskRecordDelete
(
		ID INT NOT NULL,--ID号
		DeskPassword NVARCHAR(20) NOT NULL,--桌子密码
		GameID INT NOT NULL ,
		DeskMaster INT NOT NULL,--桌主
		ToTalCount INT NOT NULL ,
		DeleteTime DATETIME,--清除时间
		CreateTime DATETIME --创建时间
)
		

ALTER TABLE dbo.TBuyGameDeskRecordDelete ADD RoomID INT DEFAULT(0),DeskID INT DEFAULT(0)