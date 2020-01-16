
ALTER TABLE dbo.Web_Config ADD Invoice bit DEFAULT(0)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否可以代开：0不可以，1可以' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'Invoice'
GO

ALTER TABLE dbo.Web_Config ADD InvoiceNum INT DEFAULT(1)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'可代开房间数量' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'InvoiceNum'
GO

CREATE TABLE TBuyDeskUser(
	UserID INT,
	DeskPass NVARCHAR(20))
	
CREATE TABLE TBuyDeskUserDelete(
	UserID INT,
	DeskPass NVARCHAR(20))
	
	
ALTER TABLE dbo.TBuyGameDeskRecordDelete ADD IsPlay TINYINT DEFAULT(1)