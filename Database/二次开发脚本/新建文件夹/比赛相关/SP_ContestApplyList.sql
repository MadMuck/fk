GO
/****** Object:  StoredProcedure [dbo].[SP_ContestApplyList]    Script Date: 02/17/2017 10:33:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[SP_ContestApplyList]
@UserID INT --ÓÃ»§ID
AS 
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @ContestID INT 
	DECLARE @ContestType INT 
	DECLARE @BeginTime DATETIME
	DECLARE @RoomName NVARCHAR(30)
	DECLARE	@RoomID INT 
	
	DECLARE @ContestTable TABLE(ContestID INT,RoomID INT ,RoomName NVARCHAR(30),BeginTime DATETIME)
	DECLARE db_cursor CURSOR SCROLL FOR SELECT ContestID,RoomID FROM dbo.TSignUp WHERE UserID=@UserID AND (SignState<>1)
	OPEN db_cursor
	FETCH FIRST FROM db_cursor INTO @ContestID,@RoomID
	WHILE @@fetch_status=0
	BEGIN
		SELECT @ContestType=ContestType,@BeginTime=StartTime FROM dbo.TContestInfo WHERE ContestID=@ContestID
		SELECT @RoomName=RoomName FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID
		IF @ContestType=1
		BEGIN 
			IF DATEDIFF(mi,GETDATE(),@BeginTime)<5 AND DATEDIFF(s,GETDATE(),@BeginTime)>5
			BEGIN 
				INSERT INTO @ContestTable( ContestID,RoomID, RoomName, BeginTime )
				VALUES  (@ContestID,@RoomID,@RoomName,@BeginTime)
			END 
		END 
		ELSE IF @ContestType=2
		BEGIN 
			SELECT @BeginTime=BeginTime FROM dbo.TContestCircleFreeRecord WHERE ContestID=@ContestID AND RoomID=@RoomID AND FREE=1
			IF DATEDIFF(mi,GETDATE(),@BeginTime)<5 AND DATEDIFF(s,GETDATE(),@BeginTime)>5
			BEGIN 
				INSERT INTO @ContestTable( ContestID,RoomID, RoomName, BeginTime )
				VALUES  (@ContestID,@RoomID,@RoomName,@BeginTime)
			END 
		END 
		
		FETCH NEXT FROM db_cursor INTO @ContestID,@RoomID
	END
	close db_cursor
	deallocate db_cursor
	
	SELECT * FROM @ContestTable
	
END 