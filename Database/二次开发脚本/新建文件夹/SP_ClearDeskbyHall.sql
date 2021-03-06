USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClearDeskbyHall]    Script Date: 06/03/2019 14:21:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_ClearDeskbyHall]
@UserID	INT ,
@DeskPass varchar(20)   --桌子密码 
AS 
BEGIN 
	SET NOCOUNT ON
	declare @ClubID int
	set @ClubID=0
	declare @MasterID int 
	IF NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass)
		RETURN 3
		
	SELECT @ClubID=ISNULL(ClubID,0) FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass
	if @ClubID<>0
	begin
		select @MasterID=MasterID from TClubTable where @ClubID=ClubID
		if @MasterID<>@UserID
		RETURN 2
	end
		
		
	DECLARE @CreateTime DATETIME
	DECLARE @ID INT 
	DECLARE @GameID INT 
	DECLARE @ToTalCount INT
	DECLARE @RoomID INT 
	DECLARE @DeskID INT 
	DECLARE @PayType INT 
	DECLARE @CostJewels INT 
	DECLARE @IsPlay TINYINT
	--DECLARE @ClubID INT
	
	SELECT @CreateTime=BuyTime,@ID=ID,@GameID=GameID,@ToTalCount=BuyGameCount,@RoomID=RoomID,@DeskID=TableNumber,@IsPlay=IsPlay,@PayType=PayType,@CostJewels=Jewels,@ClubID=ClubID
	FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass
	
	IF @IsPlay=1 AND @RoomID<>255
		RETURN 4
	
	IF @RoomID=255
	BEGIN 
		DELETE FROM TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass
		INSERT INTO TBuyGameDeskRecordDelete(ID,DeskPassword,GameID,DeskMaster,ToTalCount,DeleteTime,CreateTime,RoomID,DeskID,IsPlay,ClubID) 
		VALUES(@ID,@DeskPass,@GameID,@UserID,@ToTalCount,getdate(),@CreateTime,@RoomID,@DeskID,@IsPlay,@ClubID)
		DELETE FROM TGameCutNetRecord WHERE DeskPassWord=@DeskPass
		DELETE FROM TRecordGameMSG WHERE DeskPassWord=@DeskPass
		DELETE FROM TBuyDeskUser WHERE DeskPass=@DeskPass
		
		IF @IsPlay=0 AND @PayType=0	--游戏没开始返还房卡
		BEGIN 
			DECLARE @Jewels INT 
			DECLARE @LockJewels INT
			SELECT @Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@UserID
			UPDATE Web_Users SET LockJewels=LockJewels-@CostJewels WHERE UserID=@UserID
			UPDATE Web_Users SET Jewels=Jewels+@CostJewels WHERE UserID=@UserID
		END 
	END 
	
	SELECT @RoomID AS RoomID ,@DeskID AS DeskID
	
	RETURN 0
END 