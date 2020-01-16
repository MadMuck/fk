
ALTER PROCEDURE SP_CostJewels
@DeskPass NVARCHAR(20),
@Type INT  --0扣费 1退款
AS 
BEGIN 
	SET NOCOUNT ON 
	DECLARE @CostJewels INT 
	DECLARE @PayType INT 
	DECLARE @MasterID INT 
	DECLARE @DeskID INT 
	DECLARE @Jewels INT 
	DECLARE @LockJewels INT
	DECLARE @ClubID INT 
	DECLARE @ClubMasterID INT 
	
	SELECT @CostJewels=Jewels,@PayType=PayType,@MasterID=MasterID,@ClubID=ClubID FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass
	
	IF @CostJewels=0
		RETURN 1
	
	DECLARE @NoticeTable TABLE(UserID INT ,Jewels INT ,LockJewels INT,PayType INT)
	
	IF @ClubID<>0
	BEGIN 
		SET @ClubMasterID=0
		SELECT @ClubMasterID=ISNULL(MasterID,0) FROM dbo.TClubTable WHERE ClubID=@ClubID
		IF @ClubMasterID=0
			RETURN 1
	END 
	
	IF @Type=0
	BEGIN 
		DECLARE @JewelsBefore INT 
		IF @ClubID<>0
		BEGIN 
			SELECT @JewelsBefore=Jewels+LockJewels,@Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@ClubMasterID
			UPDATE Web_Users SET LockJewels=LockJewels-@CostJewels WHERE UserID=@ClubMasterID
			INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
			VALUES(@MasterID,@JewelsBefore,-@CostJewels,13,GETDATE(),'俱乐部创建房间消耗钻石')
			INSERT INTO @NoticeTable(UserID,Jewels,LockJewels,PayType) VALUES(@ClubMasterID,@Jewels,@LockJewels-@CostJewels,@PayType)
			UPDATE dbo.TBuyGameDeskRecord SET Jewels=0 WHERE szDeskPassWord=@DeskPass
			UPDATE TClubBuyDeskRecord SET IsCost=1 WHERE DeskPass=@DeskPass AND ClubID=@ClubID
		END 
		ELSE IF @PayType=0
		BEGIN 
			SELECT @JewelsBefore=Jewels+LockJewels,@Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@MasterID
			UPDATE Web_Users SET LockJewels=LockJewels-@CostJewels WHERE UserID=@MasterID
			INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
			VALUES(@MasterID,@JewelsBefore,-@CostJewels,1,GETDATE(),'创建房间消耗钻石')
			INSERT INTO @NoticeTable(UserID,Jewels,LockJewels,PayType) VALUES(@MasterID,@Jewels,@LockJewels-@CostJewels,@PayType)
			UPDATE dbo.TBuyGameDeskRecord SET Jewels=0 WHERE szDeskPassWord=@DeskPass
		END 
		ELSE IF @PayType=2
		BEGIN 
			DECLARE @UserID INT 
			DECLARE db_cursor CURSOR SCROLL FOR SELECT UserID FROM dbo.TGameCutNetRecord WHERE DeskPassword=@DeskPass
			OPEN db_cursor
			FETCH FIRST FROM db_cursor INTO @UserID
			WHILE @@fetch_status=0
			BEGIN
				SELECT @JewelsBefore=Jewels+LockJewels,@Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@UserID
				UPDATE Web_Users SET Jewels=Jewels-@CostJewels WHERE UserID=@UserID
				INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
				VALUES(@UserID,@JewelsBefore,-@CostJewels,1,GETDATE(),'创建房间消耗钻石')
				INSERT INTO @NoticeTable(UserID,Jewels,LockJewels,PayType) VALUES(@UserID,@Jewels-@CostJewels,@LockJewels,@PayType)
				
				FETCH NEXT FROM db_cursor INTO @UserID
			END
			CLOSE db_cursor
			DEALLOCATE db_cursor
			
			UPDATE dbo.TBuyGameDeskRecord SET Jewels=0 WHERE szDeskPassWord=@DeskPass
		END 
		ELSE 
			RETURN 1
	END 
	ELSE IF @Type=1
	BEGIN 
		IF @ClubID<>0
		BEGIN 
			SELECT @Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@ClubMasterID
			
			UPDATE Web_Users SET LockJewels=LockJewels-@CostJewels WHERE UserID=@ClubMasterID
			UPDATE Web_Users SET Jewels=Jewels+@CostJewels WHERE UserID=@ClubMasterID
			INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
			VALUES(@ClubMasterID,@Jewels+@LockJewels,@CostJewels,1,GETDATE(),'创建房间归还钻石')
			INSERT INTO @NoticeTable(UserID,Jewels,LockJewels,PayType) VALUES(@ClubMasterID,@Jewels+@CostJewels,@LockJewels-@CostJewels,@PayType)
		END 
		ELSE IF @PayType=0
		BEGIN 
			SELECT @Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@MasterID
			UPDATE Web_Users SET LockJewels=LockJewels-@CostJewels WHERE UserID=@MasterID
			UPDATE Web_Users SET Jewels=Jewels+@CostJewels WHERE UserID=@MasterID
			INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
			VALUES(@MasterID,@Jewels+@LockJewels,@CostJewels,1,GETDATE(),'创建房间归还钻石')
			INSERT INTO @NoticeTable(UserID,Jewels,LockJewels,PayType) VALUES(@UserID,@Jewels+@CostJewels,@LockJewels-@CostJewels,@PayType)
		END 
		ELSE 
			RETURN 1
	END 
	ELSE
		RETURN 1
	
	UPDATE dbo.TBuyGameDeskRecord SET Jewels=0 WHERE szDeskPassWord=@DeskPass
	
	SELECT * FROM @NoticeTable
	RETURN 0

END 