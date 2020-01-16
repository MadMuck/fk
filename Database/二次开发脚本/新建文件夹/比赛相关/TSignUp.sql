IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[TSignUp]') AND type in (N'U'))
DROP TABLE [dbo].TSignUp
GO

/****** Object:  Table [dbo].[TSignUp]    Script Date: 02/22/2017 13:49:27 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[TSignUp](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[GameNameID] [int] NOT NULL,
	[ContestID] [int] NOT NULL,
	[RoomID] [int] NOT NULL,
	[UserID] [int] NOT NULL,
	[SignState] [int] NOT NULL,
	[SignTime] [datetime] NULL,
	[EntryFee] [int] NULL,
	[MatchID] [int] NULL,
 CONSTRAINT [PK_TSignUp] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'自动增长列' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'ID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'游戏名称对应的ID，每个游戏都有一个唯一的ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'GameNameID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'房间ID，每个房间都有一个唯一的ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'RoomID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用户ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'UserID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛状态，0-未开始，1-已开始，2-人数已满' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'SignState'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报名时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'SignTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报名费' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TSignUp', @level2type=N'COLUMN',@level2name=N'EntryFee'
GO

ALTER TABLE [dbo].[TSignUp] ADD  CONSTRAINT [DF_TSignUp_ContestID]  DEFAULT ((1)) FOR [ContestID]
GO


