
/****** Object:  StoredProcedure [dbo].[SP_ClubAddPlayInfo]    Script Date: 12/04/2018 09:50:46 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[SP_ClubAddPlayInfo]
@ClubID int,					--俱乐部ID
@BuyCount INT ,					--购买局数
@PayType INT,
@GameID int,					--游戏ID
@FinishCondition INT,			--房间结束方式
@PositionLimit INT ,			--是否开启定位
@PlayerNum INT ,				--游戏限制人数
@MidEnter INT,
@BuyMinutes int,				--购买时长
@DeskConfig VARBINARY(MAX),
@PreventCheating int,				--0不开启防作弊检查  1开启防作弊检查
@CurrencyType	int	,					--结算方式   0--金币结算    1---分数结算
@GroupID int,                        --楼层ID
@Type int						--0：添加楼层玩法  1：修改楼层玩法
AS 
begin
	SET NOCOUNT ON	
	 
	declare @GroupName nvarchar(64)
	select @GroupName=ComName from TGameNameInfo where @GameID=NameID 
	if @Type=0
	begin 
		if exists (select * from TClubPlayInfo where @GroupID=GroupID and ClubID=@ClubID)
		begin
			return 1  --楼层已经存在   楼层ID重复
		end
		declare @count int
		set @count=0
		select @count=COUNT(*) from TClubPlayInfo where ClubID=@ClubID
		if @count>=5
		return 2      --该俱乐部已经有5层楼了   
		
		
		insert into TClubPlayInfo(GroupID,GroupName,ClubID,GameID,PlayCount,bFinishCondition,bPayType,bPostionLimit,bPlayNumber,bMidEnter,bMinutes,
		DeskConfig,PreventCheating,CurrencyType) 
		values(@GroupID,@GroupName,@ClubID,@GameID,@BuyCount,@FinishCondition,@PayType,@PositionLimit,@PlayerNum,@MidEnter,@BuyMinutes,@DeskConfig,
		@PreventCheating,@CurrencyType)
		
		select ID ,GroupName from TClubPlayInfo where GroupID=@GroupID and @ClubID=@ClubID and GameID=@GameID and PlayCount=@BuyCount and bFinishCondition=@FinishCondition
		and bPayType=@PayType and bPostionLimit=@PositionLimit and bPlayNumber=@PlayerNum and bMidEnter=@MidEnter and bMinutes=@BuyMinutes and DeskConfig=@DeskConfig
		return 0
	end
	else
	begin
		if not exists (select * from TClubPlayInfo where @GroupID=GroupID and ClubID=@ClubID)
		begin
			return 3  --没有该楼层 无法修改玩法
		end
		IF EXISTS(select * from TBuyGameDeskRecord where GroupID=@GroupID and @ClubID=ClubID and IsPlay=1)
		 return 4        --该群有正在游戏的房间
		 		--解散已释放的房间
		DECLARE @Temp TABLE(CreateTime DATETIME,ID INT,MasterID INT,GameID INT,ToTalCount INT,RoomID INT,TableNumber INT,szDeskPassWord NVARCHAR(20),IsPlay INT)

		INSERT INTO @Temp(ID,szDeskPassWord,GameID,MasterID,ToTalCount,CreateTime,RoomID,TableNumber,IsPlay)
		SELECT ID,szDeskPassWord,GameID,MasterID,BuyGameCount,BuyTime,RoomID,TableNumber,IsPlay FROM dbo.TBuyGameDeskRecord
			WHERE ClubID=@ClubID AND ((RoomID=255 AND TableNumber=255) OR IsPlay=0) AND GroupID=@GroupID
			
			
		DECLARE @DeskPass NVARCHAR(20)
		DECLARE @NoticeTable TABLE(UserID INT ,Jewels INT ,LockJewels INT,PayType INT)
		DECLARE db_cursor CURSOR SCROLL FOR select szDeskPassWord from @Temp  --声明游标
		OPEN db_cursor
		FETCH FIRST FROM db_cursor INTO @DeskPass
		WHILE @@fetch_status=0
		BEGIN
			INSERT @NoticeTable
			EXEC dbo.SP_CostJewels @DeskPass,1						  --执行 	SP_CostJewels 退费	--返回	@NoticeTable		
			FETCH NEXT FROM db_cursor INTO @DeskPass                  --读取游标下一行
		END
		CLOSE db_cursor
		DEALLOCATE db_cursor

		DELETE FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord IN (SELECT szDeskPassWord FROM @Temp)
		INSERT INTO dbo.TBuyGameDeskRecordDelete(ID ,DeskPassword ,GameID ,DeskMaster ,ToTalCount ,DeleteTime ,CreateTime ,RoomID ,DeskID)
		SELECT ID,szDeskPassWord,GameID,MasterID,ToTalCount,GETDATE(),CreateTime,RoomID,TableNumber FROM @Temp
		DELETE FROM dbo.TGameCutNetRecord WHERE DeskPassWord IN (SELECT szDeskPassWord FROM @Temp)
		DELETE FROM dbo.TRecordGameMSG WHERE DeskPassWord IN (SELECT szDeskPassWord FROM @Temp)

		
		--已分配的房间解除绑定
		UPDATE dbo.TBuyGameDeskRecord SET ClubID=0 WHERE ClubID=@ClubID AND RoomID<>255 AND TableNumber<>255 AND IsPlay=1
		
		--修改俱乐部楼层
	 
		update dbo.TClubPlayInfo 
		set GroupName=@GroupName,GameID=@GameID,PlayCount=@BuyCount,bFinishCondition=@FinishCondition,bPayType=@PayType,bPostionLimit=@PositionLimit,
		bPlayNumber=@PlayerNum,bMidEnter=@MidEnter,bMinutes=@BuyMinutes,DeskConfig=@DeskConfig,PreventCheating=@PreventCheating,CurrencyType=@CurrencyType
		WHERE GroupID=@GroupID and ClubID=@ClubID
		if @GroupID=1
		begin
			update TClubTable set ClubName=@GroupName where ClubID=@ClubID  --俱乐部的名字改成楼层的名字
		end
		declare @ID int 
		select @ID=ID from TClubPlayInfo where GroupID=@GroupID and @ClubID=@ClubID and GameID=@GameID and PlayCount=@BuyCount and bFinishCondition=@FinishCondition
		and bPayType=@PayType and bPostionLimit=@PositionLimit and bPlayNumber=@PlayerNum and bMidEnter=@MidEnter and bMinutes=@BuyMinutes and DeskConfig=@DeskConfig
	
		--申请解散未开始的房间
		SELECT @ID as ID,@GroupName as GroupName,RoomID,TableNumber,szDeskPassWord FROM @Temp WHERE IsPlay=0 AND RoomID<>255 AND TableNumber<>255
		
		RETURN 0
	end
end