IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[TContestLog]') AND type in (N'U'))
DROP TABLE [dbo].[TContestLog]
GO

CREATE TABLE TContestLog(
	MatchID INT IDENTITY(1,1) ,
	ContestID INT NOT NULL ,
	RoomID INT ,
	ContestType INT NOT NULL ,
	BeginTime DATETIME,
	ContestStat INT )
	

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛编号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'MatchID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'ContestID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛房间ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'RoomID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛类型 0-人满赛，1-定时赛, 2-循环赛' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'ContestType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛开始时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'BeginTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'状态0未开始,1已开始,2开始失败' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'ContestStat'
GO