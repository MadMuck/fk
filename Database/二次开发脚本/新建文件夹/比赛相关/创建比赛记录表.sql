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
	

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'MatchID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'����ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'ContestID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'��������ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'RoomID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�������� 0-��������1-��ʱ��, 2-ѭ����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'ContestType'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'������ʼʱ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'BeginTime'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'״̬0δ��ʼ,1�ѿ�ʼ,2��ʼʧ��' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'TContestLog', @level2type=N'COLUMN',@level2name=N'ContestStat'
GO