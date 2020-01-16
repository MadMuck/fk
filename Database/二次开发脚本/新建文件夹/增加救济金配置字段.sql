

ALTER TABLE dbo.Web_Config ADD SendCount_Alms INT DEFAULT(0) NOT NULL,SendMoney_Alms BIGINT DEFAULT(0) NOT NULL

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'救济金赠送次数' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'SendCount_Alms'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'救济金赠送数量' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'SendMoney_Alms'
GO
