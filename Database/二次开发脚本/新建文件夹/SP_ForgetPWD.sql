

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
go 
CREATE PROCEDURE SP_ForgetPWD
	@UseID INT ,		--找回方式 0-使用用户名，1-使用用户ID
	@UserID INT ,
	@UserName NVARCHAR(64),
	@PhoneNum NVARCHAR(20),
	@NewPassword NVARCHAR(64)
AS
BEGIN
	SET NOCOUNT ON
	
	IF @UseID=0
		SELECT @UserID=UserID FROM dbo.TUsers WHERE UserName=@UserName
		
	IF @UserID=NULL OR NOT EXISTS (SELECT * FROM dbo.TUsers WHERE UserID=@UserID)
		RETURN 1
		
	IF (SELECT PhoneValid FROM dbo.Web_Users WHERE UserID=@UserID) = 0
		RETURN 2
	
	IF (SELECT Phone FROM dbo.Web_Users WHERE UserID=@UserID) <> @PhoneNum
		RETURN 3
		
	IF (SELECT RegistType FROM dbo.TUsers WHERE UserID=@UserID) <> 1
		RETURN 4
		
	UPDATE dbo.TUsers SET Pass=@NewPassword WHERE UserID=@UserID
		
	RETURN 0
	
END 