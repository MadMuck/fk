

ALTER TABLE dbo.TUsers ADD RegistType INT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ע������ 1��ͨע��,0�ο͵�½,>=2������ע��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TUsers', @level2type=N'COLUMN',@level2name=N'RegistType'
GO