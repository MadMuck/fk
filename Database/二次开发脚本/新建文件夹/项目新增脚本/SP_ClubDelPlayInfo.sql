
/****** Object:  StoredProcedure [dbo].[SP_ClubDelPlayInfo]    Script Date: 11/27/2018 16:20:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[SP_ClubDelPlayInfo]
@ClubID int,					--俱乐部ID
@GroupID int,                   --楼层ID
@UserID  int					--用户ID
AS 
begin
	SET NOCOUNT ON
	DECLARE @MasterID INT 

	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0)FROM TClubTable WHERE @ClubID=ClubID
	if @GroupID=1
		return 7      --不能删除1楼
	IF @MasterID=0
		RETURN 2		--俱乐部群ID错误
		
	IF @MasterID<>@UserID
		RETURN 3		--玩家不是会长
	
	if not exists (select * from TClubPlayInfo where GroupID=@GroupID and ClubID=@ClubID)
	    return 5       --该俱乐部没有这个楼层
	
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
	
	--删除俱乐部楼层
 
	DELETE FROM dbo.TClubPlayInfo WHERE GroupID=@GroupID and ClubID=@ClubID
	--申请解散未开始的房间
	SELECT RoomID,TableNumber,szDeskPassWord FROM @Temp WHERE IsPlay=0 AND RoomID<>255 AND TableNumber<>255
	
	RETURN 0
	
	
end