
ALTER TABLE dbo.TGameServerInfo ADD DeskMinPeople INT DEFAULT(0) NOT NULL , IsBRGame BIT DEFAULT(0) NOT NULL , IsFishGame BIT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'������С��ʼ����,0��ʾ���˲�����Ϸ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'DeskMinPeople'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�Ƿ��ǰ�����Ϸ��1�ǰ�����Ϸ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'IsBRGame'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�Ƿ��ǲ�����Ϸ��1�ǲ�����Ϸ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'IsFishGame'
GO