
ALTER TABLE dbo.TGameServerInfo ADD FinishCondition INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'结束条件0-局数,1胡息,2-圈数' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'FinishCondition'
GO
