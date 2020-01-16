
ALTER PROCEDURE SP_MasterLeave
@UserID INT ,
@DeskID INT,
@RoomID INT,
@Leave INT,
@bMaster INT 
AS 
BEGIN 
	SET NOCOUNT ON 
	
	DECLARE	@IsPlay TINYINT
	DECLARE @DeskPass NVARCHAR(20)
	SET @DeskPass=''
	
	SELECT @DeskPass=ISNULL(szDeskPassWord,'') FROM dbo.TBuyGameDeskRecord WHERE TableNumber=@DeskID AND RoomID=@RoomID
	IF @DeskPass=''
		RETURN 2
	
	--IF @bMaster<>0 AND NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE MasterID=@UserID AND TableNumber=@DeskID AND RoomID=@RoomID)
	--	RETURN 6   --房主不能离开
	
	IF @bMaster<>0 AND (SELECT TOP 1 Invoice FROM dbo.Web_Config) = 0	
		RETURN 5
		
	SELECT @IsPlay=IsPlay FROM dbo.TBuyGameDeskRecord WHERE TableNumber=@DeskID AND RoomID=@RoomID
	
	IF @bMaster<>0 AND @Leave=0
		UPDATE dbo.TBuyGameDeskRecord SET MasterState=0 WHERE MasterID=@UserID AND TableNumber=@DeskID AND RoomID=@RoomID
	ELSE IF @bMaster<>0
		UPDATE dbo.TBuyGameDeskRecord SET MasterState=1 WHERE MasterID=@UserID AND TableNumber=@DeskID AND RoomID=@RoomID
		
	IF @Leave=0 --AND @IsPlay=0
	BEGIN 
		IF NOT EXISTS (SELECT * FROM TBuyDeskUser WHERE UserID=@UserID AND DeskPass=@DeskPass)
			INSERT INTO TBuyDeskUser(UserID,DeskPass) VALUES(@UserID,@DeskPass)
	END 
	ELSE IF @Leave<>0 --AND @IsPlay=0
		begin
		DELETE FROM TBuyDeskUser WHERE UserID=@UserID AND DeskPass=@DeskPass
		delete from TGameCutNetRecord where UserID=@UserID and DeskPassword=@DeskPass
		end
	RETURN 1

END 