
set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go 
ALTER PROCEDURE SP_ReturnDesk
@RoomID INT ,   
@DeskID INT 
AS 
BEGIN 
	SET NOCOUNT ON
	
	IF NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE RoomID=@RoomID AND TableNumber=@DeskID)
		RETURN 1  --
	
	DECLARE @Password NVARCHAR(20)
	DECLARE @RunCount INT 
	DECLARE @ID INT 
	DECLARE @BasePoint BIGINT
	DECLARE @RoomRule INT 
	DECLARE @UserInfo TABLE(UserID INT,Score BIGINT ,Station INT ,Nickname nvarchar(50),HeadUrl nvarchar(256),LogoID INT , RunCount INT )
	
	SELECT @Password=szDeskPassWord,@RunCount=RunCount,@ID=ID FROM dbo.TBuyGameDeskRecord WHERE RoomID=@RoomID AND TableNumber=@DeskID
	
	INSERT INTO @UserInfo( UserID ,Station ,RunCount) SELECT UserID,UserStation,@RunCount FROM TGameCutNetRecord WHERE DeskPassword=@Password
	
	UPDATE b SET b.Nickname=a.NickName,b.LogoID=a.LogoID  FROM  dbo.TUsers a,@UserInfo b WHERE a.UserID=b.UserID
	
	UPDATE b SET b.HeadUrl=a.PicPosition  FROM  dbo.T_HeadPic a,@UserInfo b WHERE a.UserID=b.UserID
	
	UPDATE b SET b.HeadUrl=a.HeadUrl  FROM  dbo.TWeiXinRegister a,@UserInfo b WHERE a.UserID=b.UserID
	
	SELECT @RoomRule=RoomRule,@BasePoint=ExercisePoint FROM dbo.TGameRoomBase WHERE RoomID=@RoomID
	IF(@RoomRule & 8192)>0
	BEGIN 
		UPDATE @UserInfo SET Score=@BasePoint
		UPDATE b SET b.Score=a.UserScore+b.Score FROM dbo.TDeskTotleRecord a,@UserInfo b WHERE a.DeskUserID=b.UserID AND a.DeskPassword=@Password
	END 
	ELSE 
		UPDATE b SET b.Score=a.WalletMoney  FROM  dbo.TUserInfo a,@UserInfo b WHERE a.UserID=b.UserID
		
	SELECT * FROM @UserInfo
	          
	RETURN 0
END 