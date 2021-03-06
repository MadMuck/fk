
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestBegin]    Script Date: 03/03/2017 16:20:09 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	比赛开始时，获取该场比赛的比赛ID
-- =============================================
ALTER PROCEDURE [dbo].[SP_ContestBegin] 
	@ContestID	INT,
	@GameID		INT,
	@RoomID		INT
AS
BEGIN

	SET NOCOUNT ON;
	declare @MatchID INT
	DECLARE @ContestType INT 
	
	SET @ContestType=0
	SET @MatchID = 0
	--获取比赛记录表中最大的比赛ID
	select top 1 @MatchID=MatchID from dbo.TSignUp WHERE ContestID=@ContestID AND GameNameID=@GameID AND RoomID=@RoomID

	delete from TContestUserRecord where MatchID=@MatchID
	--设置报名表用户状态
	update TSignUp set SignState=1,EntryFee=0 where RoomID=@RoomID and ContestID=@ContestID and GameNameID=@GameID
	
	--查找比赛初始积分
	declare @ContestScore int
	select @ContestScore=ISNULL(Chip,0),@ContestType=ISNULL(ContestType,0) from TContestInfo where ContestID=@ContestID
	--为报名用户设置初始比赛记录
	declare @UserID int ---定义变量
	   declare cur cursor--定义一个游标
		for select UserID from TSignUp where GameNameID=@GameID and RoomID=@RoomID and ContestID=@ContestID
		 open cur--打开游标 
		  fetch next from cur into @UserID ---给变量赋值
		   while @@FETCH_STATUS=0 --循环
			  begin
				insert into TContestUserRecord(GameID,RoomID,UserID,ContestID,MatchID,ContestScore,ContestCount,LeftTime) 
				values(@GameID,@RoomID,@UserID,@ContestID,@MatchID,@ContestScore,0,getdate())
				fetch next from cur into @userID --往下
			  end
      
      close cur--关闭游标
      deallocate cur--删除游标

	--删除未报名用户的登陆信息	
	delete from TWLoginRecord where WID=@RoomID and UserID not in 
	(select distinct UserID from TSignUp where RoomID=@RoomID and ContestID=@ContestID and GameNameID=@GameID) 
	
	IF @ContestType=2
		UPDATE dbo.TContestCircleFreeRecord SET FREE=2 WHERE ContestID=@ContestID AND RoomID=@RoomID

	--返回所有已报名的用户ID
	select distinct UserID from TSignUp	where GameNameID=@GameID and ContestID=@ContestID and RoomID=@RoomID

	return @MatchID
	
END












