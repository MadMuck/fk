IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[TContestCircleFreeRecord]') AND type in (N'U'))
DROP TABLE [dbo].[TContestCircleFreeRecord]
GO

CREATE TABLE TContestCircleFreeRecord(
	RoomID INT NOT NULL ,
	ContestID INT NOT NULL ,
	FREE INT NOT NULL,
	BeginTime DATETIME,
	MatchID INT )
	

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否空闲0-空闲状态，1-报名状态，2已开始' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestCircleFreeRecord', @level2type=N'COLUMN',@level2name=N'FREE'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestCircleFreeRecord', @level2type=N'COLUMN',@level2name=N'ContestID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'比赛房间ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestCircleFreeRecord', @level2type=N'COLUMN',@level2name=N'RoomID'
GO
