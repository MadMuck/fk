
ALTER TABLE dbo.Web_Config ADD Money2JewelRate INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'���ٽ�Ҷһ�1��ʯ��0Ϊ�رմ˹���' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'Money2JewelRate'
GO


ALTER TABLE dbo.Web_Config ADD Jewel2MoneyRate INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'1��ʯ�һ����ٽ�ң�0Ϊ�رմ˹���' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'Jewel2MoneyRate'
GO