
ALTER TABLE dbo.Web_Config ADD RegistLimitDaily INT DEFAULT(2),RegistLimitTotal INT DEFAULT(5),PhoneNumLimit INT DEFAULT(2)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'同一手机每日可注册上限,0为不限制' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'RegistLimitDaily'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'同一手机总共可注册上限,0为不限制' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'RegistLimitTotal'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'同一手机号码可绑定账号个数,0为不限制' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'PhoneNumLimit'
GO
