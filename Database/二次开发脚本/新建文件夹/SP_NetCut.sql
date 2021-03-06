
GO
/****** Object:  StoredProcedure [dbo].[SP_NetCut]    Script Date: 10/28/2016 16:49:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_NetCut]
@UserID INT,--用户ID
@Type INT  --0,全部,1,比赛场,2VIP场
AS 
BEGIN 
	declare @CutRoomID INT
	DECLARE @DeskNum INT 
	declare @BuyRoom TINYINT
	DECLARE @iUserStation INT 
	DECLARE @GameID INT 
	declare @PassWord varchar(20)
	DECLARE @ReturnValue INT 
	DECLARE @PayType INT 
	DECLARE @Jewels INT 
	DECLARE @IsPaly INT 
	DECLARE @PositionLimit INT 
	set @CutRoomID=0
	set @BuyRoom=0
	SET @iUserStation=0
	set @PassWord=''
	SET @ReturnValue=0
	
	SET NOCOUNT ON 
	
	IF EXISTS (SELECT * FROM TGameCutNetRecord WHERE UserID=@UserID)  OR EXISTS (SELECT * FROM TBuyGameDeskRecord WHERE MasterID=@UserID AND MasterState=0)
		AND (@Type=0 OR @Type=2)   
	BEGIN   --创建房间断线信息
		set @BuyRoom=1
		SELECT @PassWord=DeskPassword FROM TGameCutNetRecord WHERE UserID=@UserID
		IF @PassWord=''
			SELECT TOP 1 @PassWord=szDeskPassWord FROM TBuyGameDeskRecord WHERE MasterID=@UserID AND MasterState=0 ORDER BY BuyTime DESC
			
		select @CutRoomID=RoomID,@DeskNum=TableNumber,@GameID=GameID,@PayType=PayType,@Jewels=Jewels,@IsPaly=IsPlay,@PositionLimit=PositionLimit
		 from TBuyGameDeskRecord where szDeskPassWord=@PassWord
		
		IF @PayType=2 AND @IsPaly=0
		BEGIN 
			IF @Jewels>(SELECT Jewels FROM dbo.Web_Users WHERE UserID=@UserID)
				RETURN 5
		END 
		
		IF @CutRoomID<>255 AND @DeskNum<>255  AND EXISTS (SELECT * FROM dbo.TGameRoomInfo WHERE RoomID=@CutRoomID AND UpdateTime >= DATEADD(MI, - 3, GETDATE()) AND IsConnected=1)
		BEGIN 
			SELECT @iUserStation=UserStation FROM dbo.TGameCutNetRecord WHERE UserID=@UserID
			SELECT @CutRoomID AS RoomID,@DeskNum AS TableNumber,@iUserStation AS UserStation,@PassWord AS PassWord,@PositionLimit AS PositionLimit
			return 1-- 创建房间重连成功
		END 
		ELSE  --重新分配桌子资源
		BEGIN 
			SELECT @CutRoomID=RoomID ,@DeskNum=DeskID FROM HNF_DistributeTable(@GameID)
			IF @CutRoomID<>255 AND @DeskNum<>255   
			BEGIN 
				UPDATE TBuyGameDeskRecord SET RoomID=@CutRoomID ,TableNumber=@DeskNum WHERE szDeskPassWord=@PassWord
				DECLARE @UserStation INT 
				SELECT @UserStation=UserStation FROM TGameCutNetRecord WHERE DeskPassword=@PassWord
				SELECT RoomID,TableNumber,@UserStation AS UserStation,@PassWord AS PassWord,@PositionLimit AS PositionLimit from TBuyGameDeskRecord where szDeskPassWord=@PassWord
				RETURN 2   --重新分配资源
			END 
			ELSE 
				RETURN 3  --没有空闲桌子
		END 
		
	END 
	ELSE IF EXISTS (SELECT * FROM dbo.TWLoginRecord WHERE UserID=@UserID)
	BEGIN
		SELECT @CutRoomID=WID FROM dbo.TWLoginRecord WHERE UserID=@UserID
		IF DATEDIFF(MI,(SELECT UpdateTime FROM dbo.TGameRoomInfo WHERE RoomID=@CutRoomID),GETDATE())>3 
		BEGIN 
			DELETE FROM dbo.TWLoginRecord WHERE UserID=@UserID AND WID=@CutRoomID
			RETURN 0
		END 
		
		IF ((SELECT RoomRule FROM dbo.TGameRoomBase WHERE RoomID=@CutRoomID) & 33554432) > 0
		BEGIN 
			DELETE FROM dbo.TWLoginRecord WHERE UserID=@UserID AND WID=@CutRoomID
			RETURN 7
		END 
		
		IF EXISTS (SELECT * FROM dbo.TContestInfo WHERE ContestID=(SELECT ContestID FROM dbo.TGameRoomBase WHERE RoomID=@CutRoomID))
		BEGIN	-- 比赛房间（游戏中）
			IF @Type<>0 AND @Type<>1
				RETURN 0
			
			SET @Type=1
		END 
		ELSE
		BEGIN --普通房间
			IF @Type<>0
				RETURN 0
			SET @Type=0
		END 	
		
		SELECT @CutRoomID AS RoomID,@Type AS Type
		
		RETURN 4
		
	END 
	ELSE IF (@Type=0 OR @Type=1)
	BEGIN  --比赛房间（排队中）
		DECLARE @NetCutTime DATETIME
		DECLARE @MatchID INT 
		SELECT @CutRoomID=ISNULL(a.RoomID,0),@NetCutTime=b.NetCutTime,@MatchID=a.MatchID FROM dbo.TSignUp a,dbo.TContestUserRecord b 
					WHERE a.UserID=@UserID AND b.UserID=@UserID AND a.MatchID=b.MatchID AND a.ContestID=b.ContestID AND b.IsKick=0
					
		IF @CutRoomID<>0
		BEGIN 
			IF DATEDIFF(ss,@NetCutTime,GETDATE())>300
			BEGIN 
				UPDATE TContestUserRecord SET IsKick=1 WHERE UserID=@UserID AND MatchID=@MatchID
				RETURN 0
			END 
			ELSE 
			BEGIN 
				SET @Type=1
				SELECT @CutRoomID AS RoomID,@Type AS TYPE
				RETURN 4
			END 
		
			
		END 
	END 
	IF @Type=1
		RETURN 6
	
	return 0
end