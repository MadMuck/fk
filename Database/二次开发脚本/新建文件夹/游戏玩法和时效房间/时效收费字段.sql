ALTER TABLE dbo.Web_Config ADD EveryJewelForMinutes int DEFAULT(60)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ʱЧ������ٷ���һ����ʯ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'EveryJewelForMinutes'
GO