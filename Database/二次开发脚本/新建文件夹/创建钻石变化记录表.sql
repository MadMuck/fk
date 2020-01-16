
GO

/****** Object:  Table [dbo].[Web_MoneyChangeLog]    Script Date: 10/25/2016 16:47:52 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[TJewelChangeLog](
	[ID] [int] IDENTITY(1,1) NOT FOR REPLICATION NOT NULL,
	[UserID] [int] NOT NULL,
	[StartJewel] [bigint] NOT NULL,
	[ChangeJewel] [bigint] NOT NULL,
	[ChangeType] [int] NOT NULL,
	[DateTime] [datetime] NOT NULL,
	[Remark] [nvarchar](100) NULL,
 CONSTRAINT [PK_TJewelChangeLog] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��ʯ�仯��־��ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'ID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�û�ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'UserID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�仯ǰ��ʯ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'StartJewel'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�仯����ʯ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'ChangeJewel'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��ʯ�仯����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'ChangeType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�仯ʱ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'DateTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��ע' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TJewelChangeLog', @level2type=N'COLUMN',@level2name=N'Remark'
GO


