



set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER procedure SP_RecordNetCut
@DeskPassWord varchar(20),   --×À×ÓÃÜÂë 
@DeskUserID NVARCHAR(1000),
@UserStation NVARCHAR(1000)
as
begin
	set nocount ON
	
	DECLARE @LockType INT 
	SELECT @LockType=LockUserType FROM dbo.TGameRoomInfo WHERE RoomID=(SELECT RoomID FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPassWord)
	
	IF @LockType=2 OR @LockType=3
		DELETE FROM TGameCutNetRecord WHERE DeskPassword = @DeskPassWord
	
	INSERT INTO TGameCutNetRecord(DeskPassword ,UserID ,UserStation) 
	SELECT @DeskPassWord,a.RS,b.RS FROM  dbo.Split(@DeskUserID,',') a,dbo.Split(@UserStation,',') b WHERE a.ID=b.ID AND a.RS<>0
		AND a.RS NOT IN (SELECT UserID FROM TGameCutNetRecord WHERE DeskPassword = @DeskPassWord)
		
	INSERT INTO TBuyDeskUserDelete(UserID,DeskPass) 
	SELECT RS,@DeskPassWord FROM dbo.Split(@DeskUserID,',') WHERE RS NOT IN (SELECT UserID FROM TBuyDeskUserDelete WHERE DeskPass=@DeskPassWord)
		
	DELETE FROM TBuyDeskUser WHERE DeskPass=@DeskPassWord
end