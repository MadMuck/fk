IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[TContestCircleFreeRecord]') AND type in (N'U'))
DROP TABLE [dbo].[TContestCircleFreeRecord]
GO

CREATE TABLE TContestCircleFreeRecord(
	RoomID INT NOT NULL ,
	ContestID INT NOT NULL ,
	FREE INT NOT NULL,
	BeginTime DATETIME,
	MatchID INT )
	

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�Ƿ����0-����״̬��1-����״̬��2�ѿ�ʼ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestCircleFreeRecord', @level2type=N'COLUMN',@level2name=N'FREE'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'����ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestCircleFreeRecord', @level2type=N'COLUMN',@level2name=N'ContestID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestCircleFreeRecord', @level2type=N'COLUMN',@level2name=N'RoomID'
GO
