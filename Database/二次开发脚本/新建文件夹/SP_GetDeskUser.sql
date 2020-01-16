

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE PROCEDURE SP_GetDeskUser
@DeskPass varchar(20)   --桌子密码 
as
begin
	SET NOCOUNT ON
	
	DECLARE @UserID_tmp TABLE (UserID INT)
	IF EXISTS (SELECT * FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass) 
	BEGIN 
		DECLARE @IsPlay TINYINT
		SELECT @IsPlay=IsPlay FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPass
		IF @IsPlay=0
		BEGIN 
			INSERT INTO @UserID_tmp(UserID) SELECT UserID FROM TBuyDeskUser WHERE DeskPass=@DeskPass
		END 
		ELSE 
		BEGIN 
			INSERT INTO @UserID_tmp(UserID) SELECT UserID FROM dbo.TGameCutNetRecord WHERE DeskPassword=@DeskPass
		END 
	END 
	ELSE IF EXISTS (SELECT * FROM dbo.TBuyGameDeskRecordDelete WHERE DeskPassword=@DeskPass)
	BEGIN 
		INSERT INTO @UserID_tmp(UserID) SELECT UserID FROM dbo.TBuyDeskUserDelete WHERE DeskPass=@DeskPass
	END 
	ELSE 
		RETURN 1		--桌子不存在
		
	SELECT [@UserID_tmp].UserID,dbo.TUsers.Sex,LogoID,dbo.TUsers.NickName,HeadUrl 
			FROM @UserID_tmp INNER JOIN dbo.TUsers ON [@UserID_tmp].UserID = dbo.TUsers.UserID
							LEFT JOIN dbo.TWeiXinRegister ON dbo.TUsers.UserID = dbo.TWeiXinRegister.UserID
		
	
	RETURN 0
	
end