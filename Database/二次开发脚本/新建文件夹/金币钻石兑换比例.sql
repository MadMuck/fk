
ALTER TABLE dbo.Web_Config ADD Money2JewelRate INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'多少金币兑换1钻石，0为关闭此功能' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'Money2JewelRate'
GO


ALTER TABLE dbo.Web_Config ADD Jewel2MoneyRate INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'1钻石兑换多少金币，0为关闭此功能' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'Jewel2MoneyRate'
GO