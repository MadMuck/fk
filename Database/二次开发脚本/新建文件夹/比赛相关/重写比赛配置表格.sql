
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

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'����ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'ContestID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�������� 0-��������1-��ʱ��, 2-ѭ����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'ContestType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'SendTypeID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'������ʼʱ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'StartTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'����ʱ��,��λ����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'ContestTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'Chip'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'���ٻ���' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'LowChip'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'EntryFee'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'������������0-��ң�1-��ʯ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'EntryFeeType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'UpPeople'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��С��ʼ����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'MinPeople'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'ѭ������Ϸ���,��λ����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestInfo', @level2type=N'COLUMN',@level2name=N'CircleTime'
GO

ALTER TABLE [dbo].[TContestInfo] ADD  CONSTRAINT [DF_TContestInfo_AwardID]  DEFAULT ((0)) FOR [SendTypeID]
GO

ALTER TABLE [dbo].[TContestInfo] ADD  CONSTRAINT [DF_TContestInfo_LowChip]  DEFAULT ((0)) FOR [LowChip]
GO
