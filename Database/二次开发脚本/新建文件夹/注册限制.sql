
ALTER TABLE dbo.Web_Config ADD RegistLimitDaily INT DEFAULT(2),RegistLimitTotal INT DEFAULT(5),PhoneNumLimit INT DEFAULT(2)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ͬһ�ֻ�ÿ�տ�ע������,0Ϊ������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'RegistLimitDaily'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ͬһ�ֻ��ܹ���ע������,0Ϊ������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'RegistLimitTotal'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ͬһ�ֻ�����ɰ��˺Ÿ���,0Ϊ������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_Config', @level2type=N'COLUMN',@level2name=N'PhoneNumLimit'
GO
