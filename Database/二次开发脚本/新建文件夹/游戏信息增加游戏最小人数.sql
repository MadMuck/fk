
ALTER TABLE dbo.TGameServerInfo ADD DeskMinPeople INT DEFAULT(0) NOT NULL , IsBRGame BIT DEFAULT(0) NOT NULL , IsFishGame BIT DEFAULT(0) NOT NULL 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'桌子最小开始人数,0表示满人才能游戏' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'DeskMinPeople'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否是百人游戏，1是百人游戏' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'IsBRGame'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否是捕鱼游戏，1是捕鱼游戏' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TGameServerInfo', @level2type=N'COLUMN',@level2name=N'IsFishGame'
GO