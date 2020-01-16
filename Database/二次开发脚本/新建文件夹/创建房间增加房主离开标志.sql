
ALTER TABLE dbo.TBuyGameDeskRecord ADD MasterState bit DEFAULT(0)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'房主状态：0在房间，1离开状态' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TBuyGameDeskRecord', @level2type=N'COLUMN',@level2name=N'MasterState'
GO
