
GO
/****** Object:  StoredProcedure [dbo].[SP_CreateGameSN]    Script Date: 11/27/2016 15:04:39 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[SP_CreateGameSN]
@RoomID INT ,  
@DeskID INT 
as
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @Index INT 
	DECLARE @GameSN NVARCHAR(20)
	DECLARE @NowTime DATETIME
	SET @NowTime = GETDATE()
	
	INSERT INTO TGameSNRecord(RoomID,DeskID,StartTime) VALUES(@RoomID,@DeskID,@NowTime)
	SET @Index = @@IDENTITY
	
	WHILE 1=1
	BEGIN 
		set @GameSN=RIGHT(100000000 + CONVERT(bigint, ABS(CHECKSUM(NEWID()))), 7)
		IF NOT EXISTS(SELECT * FROM TGameSNRecord WHERE GameSN=@GameSN)
			BREAK
	END 
	--SET @GameSN = REPLACE(REPLACE(REPLACE(CONVERT(NVARCHAR(17), @NowTime, 120 ),'-',''),' ',''),':','')+@GameSN
	
	UPDATE TGameSNRecord SET GameSN=@GameSN WHERE RoomID=@RoomID AND DeskID=@DeskID AND StartTime=@NowTime
	
	SELECT @GameSN AS GameSN
	
END 