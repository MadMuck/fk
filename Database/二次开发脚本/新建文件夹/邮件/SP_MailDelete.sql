

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE PROCEDURE SP_MailDelete
@UserID	INT ,
@MailID INT 
AS  
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @IsGet INT
	
	IF @MailID=0	--全部删除
	BEGIN 
		DELETE FROM TMailList WHERE UserID=@UserID AND IsGet<>2 AND IsGet<>3 AND IsSend=1
		UPDATE TMailtoAllUserLog SET IsGet=5 WHERE UserID=@UserID AND IsGet<>2 AND IsGet<>3
		INSERT INTO TMailtoAllUserLog(MailID,UserID,IsGet) SELECT MailID,UserID,5 FROM TMailList
		WHERE UserID=0 AND MailID NOT IN (SELECT MailID FROM TMailtoAllUserLog WHERE UserID=@UserID) AND IsSend=1 AND IsGet<>2
	END 
	ELSE 
	BEGIN 
		IF NOT EXISTS(SELECT * FROM TMailList WHERE MailID=@MailID)
			RETURN 2		--邮件不存在
		
		DECLARE @UserIDTemp INT 
		SELECT @UserIDTemp=UserID,@IsGet=IsGet FROM TMailList WHERE MailID=@MailID
		
		IF @UserIDTemp=0
		BEGIN 
			DECLARE @IsGetTemp INT 
			SET @IsGetTemp=-1
			SELECT @IsGetTemp=ISNULL(IsGet,-1) FROM TMailtoAllUserLog WHERE MailID=@MailID AND UserID=@UserID
			
			IF @IsGetTemp = -1
			BEGIN 
				IF @IsGet=2
					RETURN 4 --附件未领取
				INSERT INTO TMailtoAllUserLog(MailID,UserID,IsGet) VALUES(@MailID,@UserID,5)
			END 
			ELSE 
			BEGIN
				IF @IsGetTemp=2 OR @IsGetTemp=3
					RETURN 4	--附件未领取
					
				UPDATE TMailtoAllUserLog SET IsGet=5 WHERE UserID=@UserID AND MailID=@MailID
			END 
				
		END 
		ELSE IF @UserIDTemp=@UserID
		BEGIN 
			IF @IsGet=2 OR @IsGet=3
				RETURN 4	--附件未领取
			DELETE FROM dbo.TMailList WHERE UserID=@UserID AND MailID=@MailID
		END 
		ELSE 
			RETURN 3		--不是该玩家的邮件
			
	END 
	
	SELECT @IsGet AS IsGet
		
	RETURN 0
	
END 