
ALTER TABLE dbo.TGameServerInfo ADD FinishCondition INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������0-����,1��Ϣ,2-Ȧ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'FinishCondition'
GO
