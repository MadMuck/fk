

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_ClubKickUser
@UserID	INT ,
@ClubID INT ,
@TargetID INT 
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF @MasterID<>@UserID
		RETURN 3		--玩家不是会长
		
	DECLARE @NickName NVARCHAR(64) 
	DECLARE @HeadURL NVARCHAR(256) 
	DECLARE @LogonID INT 
	DECLARE @Sex INT 
	SET @HeadURL=''
	
	SELECT @NickName=NickName,@LogonID=LogoID,@Sex=Sex FROM dbo.TUsers WHERE UserID=@TargetID
	SELECT @HeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@TargetID	
	
	IF NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE ClubID=@ClubID AND MasterID=@TargetID AND ((RoomID=255 AND TableNumber=255) OR IsPlay=0))
	BEGIN 
		DELETE FROM dbo.TClubUserTable WHERE UserID=@TargetID AND ClubID=@ClubID
		SELECT @NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex
		RETURN 0 --不需要解散房间
	END 
	
	--解散已释放的房间
	DECLARE @Temp TABLE(CreateTime DATETIME,ID INT,MasterID INT,GameID INT,ToTalCount INT,RoomID INT,TableNumber INT,szDeskPassWord NVARCHAR(20),IsPlay INT)

	INSERT INTO @Temp(ID,szDeskPassWord,GameID,MasterID,ToTalCount,CreateTime,RoomID,TableNumber,IsPlay)
	SELECT ID,szDeskPassWord,GameID,MasterID,BuyGameCount,BuyTime,RoomID,TableNumber,IsPlay FROM dbo.TBuyGameDeskRecord
		WHERE ClubID=@ClubID AND MasterID=@TargetID AND ((RoomID=255 AND TableNumber=255) OR IsPlay=0)
	
	DECLARE @DeskPass NVARCHAR(20)
	DECLARE @NoticeTable TABLE(UserID INT ,Jewels INT ,LockJewels INT,PayType INT)
	DECLARE db_cursor CURSOR SCROLL FOR select szDeskPassWord from @Temp
	OPEN db_cursor
	FETCH FIRST FROM db_cursor INTO @DeskPass
	WHILE @@fetch_status=0
	BEGIN
		INSERT @NoticeTable
		EXEC dbo.SP_CostJewels @DeskPass,1 
		FETCH NEXT FROM db_cursor INTO @DeskPass
	END
	CLOSE db_cursor
	DEALLOCATE db_cursor

	DELETE FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord IN (SELECT szDeskPassWord FROM @Temp)
	INSERT INTO dbo.TBuyGameDeskRecordDelete(ID ,DeskPassword ,GameID ,DeskMaster ,ToTalCount ,DeleteTime ,CreateTime ,RoomID ,DeskID)
	SELECT ID,szDeskPassWord,GameID,MasterID,ToTalCount,GETDATE(),CreateTime,RoomID,TableNumber FROM @Temp
	DELETE FROM dbo.TGameCutNetRecord WHERE DeskPassWord IN (SELECT szDeskPassWord FROM @Temp)
	DELETE FROM dbo.TRecordGameMSG WHERE DeskPassWord IN (SELECT szDeskPassWord FROM @Temp)
	
	DELETE FROM dbo.TClubUserTable WHERE UserID=@TargetID AND ClubID=@ClubID	
	
	--申请解散未开始的房间
	SELECT RoomID,TableNumber,szDeskPassWord,@NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex FROM @Temp WHERE IsPlay=0 AND RoomID<>255 AND TableNumber<>255
	
	RETURN 4 --需要解散房间
END