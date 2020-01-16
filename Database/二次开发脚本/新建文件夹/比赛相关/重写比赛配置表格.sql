
/****** Object:  Table [dbo].[TContestInfo]    Script Date: 02/17/2017 09:46:24 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[TContestInfo]') AND type in (N'U'))
DROP TABLE [dbo].[TContestInfo]
GO

/****** Object:  Table [dbo].[TContestInfo]    Script Date: 02/17/2017 09:46:30 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[TContestInfo](
	[ContestID] [int] IDENTITY(1,1) NOT NULL,
	[ContestType] [int] NOT NULL,
	[GameID] [int] NOT NULL,
	[SendTypeID] [int] NOT NULL,
	[StartTime] [datetime] NOT NULL,
	[ContestTime] [int] NOT NULL,
	[Chip] [bigint] NOT NULL,
	[LowChip] [bigint] NOT NULL,
	[EntryFee] [bigint] NOT NULL,
	[EntryFeeType] [int] NOT NULL,
	[UpPeople] [int] NOT NULL,
	[MinPeople] [int] NOT NULL,
	[LeastPeople] [int] NOT NULL,
	[CircleTime] [int] NOT NULL,
 CONSTRAINT [PK_ContestInfo] PRIMARY KEY CLUSTERED 
(
	[ContestID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] 

GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赛事ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'ContestID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛类型 0-人满赛，1-定时赛, 2-循环赛' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'ContestType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'奖励方案ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'SendTypeID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛开始时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'StartTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛时间,单位分钟' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'ContestTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛积分' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'Chip'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'最少积分' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'LowChip'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报名费用' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'EntryFee'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'报名费用类型0-金币，1-钻石' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'EntryFeeType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'人数上限' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'UpPeople'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'最小开始人数' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'MinPeople'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'循环赛游戏间隔,单位分钟' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'CircleTime'
GO

ALTER TABLE [dbo].[TContestInfo] ADD  CONSTRAINT [DF_TContestInfo_AwardID]  DEFAULT ((0)) FOR [SendTypeID]
GO

ALTER TABLE [dbo].[TContestInfo] ADD  CONSTRAINT [DF_TContestInfo_LowChip]  DEFAULT ((0)) FOR [LowChip]
GO
