ALTER TABLE dbo.TBuyGameDeskRecord ADD BuyMinutes int DEFAULT(0)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'����ʱ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecord', @level2type=N'COLUMN',@level2name=N'BuyMinutes'
GO

ALTER TABLE dbo.TBuyGameDeskRecord ADD BeginTime DATETIME

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��ʼʱ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecord', @level2type=N'COLUMN',@level2name=N'BeginTime'
GO