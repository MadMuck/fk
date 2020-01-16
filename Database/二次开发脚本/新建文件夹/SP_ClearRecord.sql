
ALTER PROCEDURE SP_ClearRecord
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @Temp TABLE(CreateTime DATETIME,ID INT,MasterID INT,GameID INT,ToTalCount INT,RoomID INT,DeskID INT,Pass NVARCHAR(20),IsPlay TINYINT)

	INSERT INTO @Temp(ID,Pass,GameID,MasterID,ToTalCount,CreateTime,RoomID,DeskID,IsPlay)
	SELECT ID,szDeskPassWord,GameID,MasterID,BuyGameCount,BuyTime,RoomID,TableNumber,IsPlay FROM dbo.TBuyGameDeskRecord
		WHERE DATEDIFF(dd,BuyTime,GETDATE())>=2 AND RoomID=255 AND TableNumber=255

	DELETE FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord IN (SELECT Pass FROM @Temp)
	INSERT INTO dbo.TBuyGameDeskRecordDelete(ID ,DeskPassword ,GameID ,DeskMaster ,ToTalCount ,DeleteTime ,CreateTime ,RoomID ,DeskID,IsPlay)
	SELECT ID,Pass,GameID,MasterID,ToTalCount,GETDATE(),CreateTime,RoomID,DeskID,IsPlay FROM @Temp
	DELETE FROM dbo.TGameCutNetRecord WHERE DeskPassWord IN (SELECT Pass FROM @Temp)
	DELETE FROM dbo.TRecordGameMSG WHERE DeskPassWord IN (SELECT Pass FROM @Temp)

	DELETE FROM TDeskTotleRecord WHERE DATEDIFF(dd,CreateTime,GETDATE())>=2 AND DeskPassword NOT IN (SELECT szDeskPassWord FROM dbo.TBuyGameDeskRecord)

	DELETE FROM TUserGameRecord WHERE DATEDIFF(dd,StartTime,GETDATE())>=2 AND DeskPassword NOT IN (SELECT szDeskPassWord FROM dbo.TBuyGameDeskRecord)

END 
