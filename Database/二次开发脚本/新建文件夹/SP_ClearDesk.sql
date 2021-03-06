USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClearDesk]    Script Date: 05/16/2019 18:17:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[SP_ClearDesk]
@DeskPassWord varchar(20)   --桌子密码 
as
begin
	set nocount ON
	
	DECLARE @CreateTime DATETIME
	DECLARE @ID INT 
	DECLARE @MasterID INT 
	DECLARE @GameID INT 
	DECLARE @ToTalCount INT
	DECLARE @RoomID INT 
	DECLARE @DeskID INT 
	DECLARE @IsPlay TINYINT
	DECLARE @ClubID INT 
	
	SELECT @CreateTime=BuyTime,@ID=ID,@MasterID=MasterID,@GameID=GameID,@ToTalCount=BuyGameCount,@RoomID=RoomID,@DeskID=TableNumber,@IsPlay=IsPlay,@ClubID=ClubID
	FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPassWord and IsPlay=1
	---在游戏结束或者解散桌子的时候  抽取玩家碎钻给俱乐部群主  
	--1 俱乐部房间  2--多个大赢家平分               Web_Config表 UserScoreDisable--抽水的数量
	if @ClubID<>0   --俱乐部房间
		begin
		  --找到在这个桌子上最高得分
		  declare @UserMAXscore int
		  select @UserMAXscore=MAX(UserScore) from  TDeskTotleRecord where DeskPassword=@DeskPassWord and ClubID=@ClubID
		  --找到这个桌子上得到最高分的人数
		  declare @iCount int 
		  select @iCount=COUNT(*) from TDeskTotleRecord where DeskPassword=@DeskPassWord and ClubID=@ClubID and @UserMAXscore=UserScore and @UserMAXscore<>0
		  --获取抽水总数
		  declare @UserScoreDisable int 
		  select TOP 1 @UserScoreDisable=ISNULL(UserScoreDisable,0) from Web_Config 
		  if @UserScoreDisable<>0 and @UserMAXscore<>0 --   @UserScoreDisable=0  就不用抽了
		  begin
			declare @Avreage int   --每人要抽的　
			set @Avreage=@UserScoreDisable/@iCount   --小数没算　
			declare @Tmp table (UserID int)  --要写进变化日至　搞个临时表　多个或一个大赢家　都放里面
			insert into @Tmp(UserID) select DeskUserID from TDeskTotleRecord where DeskPassword=@DeskPassWord and ClubID=@ClubID and @UserMAXscore=UserScore
			
			declare @UserIDTmp int ---定义变量
			 declare cur cursor--定义一个游标
			for select UserID from @Tmp
			 open cur--打开游标 
			  fetch next from cur into @UserIDTmp ---给变量赋值
			  while @@FETCH_STATUS=0 --循环
				 begin
					declare @CrushingDrillBegin bigint	--玩家之前身上碎钻（奖券）数量
					select @CrushingDrillBegin=Lotteries from Web_Users where UserID=@UserIDTmp
					
					update Web_Users set Lotteries=Lotteries-@Avreage where UserID=@UserIDTmp   --玩家减　
					
					---插入变化日志表  --碎钻变化日志
					DECLARE @UserName NVARCHAR(64)
					declare @CrushingDrill bigint		--玩家之后身上碎钻（奖券）数量
					SELECT @UserName=UserName from dbo.TUsers WHERE UserID=@UserIDTmp
					select @CrushingDrill=Lotteries from Web_Users where UserID=@UserIDTmp
					
					INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
					VALUES  (@UserIDTmp, @UserName ,@CrushingDrillBegin ,-@Avreage ,@CrushingDrill,GETDATE(),'抽水扣税',7)
					
					
					select @CrushingDrillBegin=Lotteries from Web_Users where UserID=@MasterID  --群主之前的
					update Web_Users set Lotteries=Lotteries+@Avreage where UserID=@MasterID   --群主加
					SELECT @UserName=UserName from dbo.TUsers WHERE UserID=@MasterID
					select @CrushingDrill=Lotteries from Web_Users where UserID=@MasterID       --加过之后的
					INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
					VALUES  (@MasterID, @UserName ,@CrushingDrillBegin ,@Avreage ,@CrushingDrill,GETDATE(),'抽水获得',8)
					
					fetch next from cur into @UserIDTmp --往下
				 end
      
		     close cur--关闭游标
			 deallocate cur--删除游标
			
		  end
	    end
	    
	    
	delete from TBuyGameDeskRecord where szDeskPassWord=@DeskPassWord
	insert into TBuyGameDeskRecordDelete(ID,DeskPassword,GameID,DeskMaster,ToTalCount,DeleteTime,CreateTime,RoomID,DeskID,IsPlay,ClubID) 
	values(@ID,@DeskPassWord,@GameID,@MasterID,@ToTalCount,getdate(),@CreateTime,@RoomID,@DeskID,@IsPlay,@ClubID)
	delete from TGameCutNetRecord where DeskPassWord=@DeskPassWord
	DELETE FROM TRecordGameMSG WHERE DeskPassWord=@DeskPassWord
	DELETE FROM TBuyDeskUser WHERE DeskPass=@DeskPassWord
end