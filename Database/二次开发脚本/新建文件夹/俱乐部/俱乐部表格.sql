
ALTER TABLE dbo.Web_Config ADD ClubLimit INT DEFAULT(5) NOT NULL ,CanUserCreate TINYINT DEFAULT(1) NOT NULL ,ClubJoinLimit INT DEFAULT(10) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'每个ID可创建俱乐部数量，0不限制' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'ClubLimit'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'创建俱乐部限制，0-代理商可创建，1-所有人可创建' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'CanUserCreate'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'每个ID可加入的俱乐部限制，0不限制' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'ClubJoinLimit'
GO


ALTER TABLE dbo.Web_Config ADD ClubUserLimit INT DEFAULT(100) NOT NULL 
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'每个俱乐部可加入的玩家数量' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'ClubUserLimit'
GO

---俱乐部表
CREATE TABLE TClubTable(
		ClubID INT NOT NULL ,
		ClubName NVARCHAR(40) NOT NULL ,
		MasterID INT NOT NULL,
		ClubNotice NVARCHAR(100) DEFAULT('') NOT NULL
		)
		
CREATE TABLE TClubUserTable(
		ClubID INT NOT NULL ,
		UserID INT NOT NULL
		)
		
CREATE TABLE TClubVerifyTable(
		ClubID INT NOT NULL ,
		UserID INT NOT NULL
		)	

CREATE TABLE TClubBlacklistTable(
		ClubID INT NOT NULL ,
		UserID INT NOT NULL
		)
		
CREATE TABLE TClubBuyDeskRecord(
		ClubID INT NOT NULL ,
		UserID INT NOT NULL ,
		BuyTime DATETIME,
		CostJewels INT ,
		DeskPass NVARCHAR(20),
		IsCost BIT 
		)
		
ALTER TABLE dbo.TBuyGameDeskRecord ADD ClubID INT DEFAULT(0) NOT NULL 	
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'俱乐部ID，0普通购买房间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecord', @level2type=N'COLUMN',@level2name=N'ClubID'
GO	

ALTER TABLE dbo.TBuyGameDeskRecordDelete ADD ClubID INT DEFAULT(0) NOT NULL 	
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'俱乐部ID，0普通购买房间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecordDelete', @level2type=N'COLUMN',@level2name=N'ClubID'
GO	

ALTER TABLE dbo.TBuyGameDeskRecord ADD ClubPayType INT DEFAULT(0) NOT NULL 	
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'俱乐部开饭类型，0-房主付，1-AA，仅做标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecord', @level2type=N'COLUMN',@level2name=N'ClubPayType'
GO	

ALTER TABLE dbo.TBuyGameDeskRecordDelete ADD ClubPayType INT DEFAULT(0) NOT NULL 	
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'俱乐部开饭类型，0-房主付，1-AA，仅做标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecordDelete', @level2type=N'COLUMN',@level2name=N'ClubPayType'
GO		

ALTER TABLE dbo.TDeskTotleRecord ADD ClubID INT DEFAULT(0) NOT NULL 

ALTER TABLE dbo.TDeskTotleRecord ADD MasterID INT DEFAULT(0) NOT NULL 