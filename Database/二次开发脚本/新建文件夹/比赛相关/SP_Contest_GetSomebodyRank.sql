USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_Contest_GetSomebodyRank]    Script Date: 05/25/2017 11:56:47 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO









-- Stored Procedure

-- ======================================================
-- Author:		JianGuankun
-- Create date: 2012-7-12
-- Description:	获取部分玩家的比赛排名
-- ======================================================
ALTER PROCEDURE [dbo].[SP_Contest_GetSomebodyRank]
	@ContestID	INT,
	@MatchID	BIGINT,
	@GameID		INT,
	@RoomID		INT
AS
BEGIN
	SET NOCOUNT ON;
	declare @MaxRank int;
	set @MaxRank = 0
	
	declare @TotalTable table(GameID INT, RoomID INT, UserID INT, ContestID INT, MatchID BIGINT,RankNum INT,Score BIGINT,CCount INT,LeftTime DATETIME)	--总榜变量表	
	declare @KickTable table(GameID INT, RoomID INT, UserID INT, ContestID INT, MatchID BIGINT,RankNum INT,Score BIGINT,CCount INT,LeftTime DATETIME)	--被淘汰榜变量表	
	
	--获取所有用户的排名
	insert into @TotalTable(GameID,RoomID,UserID,ContestID,MatchID,RankNum,Score,CCount,LeftTime)
	select GameID,a.RoomID,a.UserID,a.ContestID,a.MatchID,ROW_NUMBER() over (order by ContestScore desc,ContestCount desc,LeftTime DESC,TSignUp.SignTime ASC) as RankNum,
	ContestScore,ContestCount,LeftTime
	from TContestUserRecord a, dbo.TSignUp where GameID=@GameID and a.RoomID=@RoomID and a.ContestID=@ContestID and a.MatchID=@MatchID and IsKick=0 
	AND a.UserID=TSignUp.UserID AND a.MatchID = TSignUp.MatchID
	
	--获取最大的排名，然后被淘汰用户的排名以此为基数递增
	select @MaxRank=Count(UserID) from @TotalTable	

	--获取被淘汰用户的排名
	insert into @KickTable(GameID,RoomID,UserID,ContestID,MatchID,RankNum,Score,CCount,LeftTime)
	select GameID,RoomID,UserID,ContestID,MatchID,ROW_NUMBER() over (order by LeftTime desc,ContestScore desc,ContestCount desc) as RankNum,
	ContestScore,ContestCount,LeftTime
	from TContestUserRecord where GameID=@GameID and RoomID=@RoomID and ContestID=@ContestID and MatchID=@MatchID and IsKick<>0
	
	update @KickTable set RankNum=RankNum+@MaxRank

	--清空比赛结果记录中此场比赛的记录
	if exists (select * from TContestRecord_New
				where GameNameID=@GameID and RoomID=@RoomID and ContestID = @ContestID and MatchID = @MatchID)
	begin
		delete from TContestRecord_New where GameNameID=@GameID and RoomID=@RoomID and ContestID = @ContestID and MatchID = @MatchID
	end
	
	--将比赛结果插入到记录表中
	insert into TContestRecord_New select * from @TotalTable union select * from @KickTable
	
	--返回比赛结果
	select *,@MaxRank AS RemainPeople from TContestRecord_New where GameNameID=@GameID and RoomID=@RoomID and ContestID = @ContestID and MatchID = @MatchID

	--返回当前正在比赛的人数
	return @MaxRank
END







