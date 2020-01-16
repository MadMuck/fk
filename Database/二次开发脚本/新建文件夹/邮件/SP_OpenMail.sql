

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE PROCEDURE SP_OpenMail
@UserID	INT ,
@MailID INT 
AS  
BEGIN 
	SET NOCOUNT ON
	
	IF NOT EXISTS(SELECT * FROM TMailList WHERE MailID=@MailID)
		RETURN 2		--邮件不存在
		
	DECLARE @UserIDTemp INT 
	DECLARE @MailMessage NVARCHAR(650)
	DECLARE @SendMoney BIGINT
	DECLARE @SendJewels INT
	DECLARE @SendLotteries INT
	DECLARE @IsGet INT
	SELECT @UserIDTemp=UserID,@MailMessage=MailMessage,@SendMoney=SendMoney,@SendJewels=SendJewels,@SendLotteries=SendLotteries,@IsGet=IsGet
	FROM TMailList WHERE MailID=@MailID
	IF @UserIDTemp=0
	BEGIN 
		DECLARE @IsGetTemp INT 
		SET @IsGetTemp=-1
		SELECT @IsGetTemp=ISNULL(IsGet,-1) FROM TMailtoAllUserLog WHERE MailID=@MailID AND UserID=@UserID
		IF @IsGetTemp=5
			RETURN 4	--邮件已删除
		ELSE IF @IsGetTemp = -1
		BEGIN 
			IF @IsGet=0
				SET @IsGet=1
			ELSE IF @IsGet=2
				SET @IsGet=3
				
			INSERT INTO TMailtoAllUserLog(MailID,UserID,IsGet) VALUES(@MailID,@UserID,@IsGet)
		END 
		ELSE 
		BEGIN
			IF @IsGetTemp=0
				SET @IsGetTemp=1
			ELSE IF @IsGetTemp=2
				SET @IsGetTemp=3
				
			SET @IsGet=@IsGetTemp
			UPDATE TMailtoAllUserLog SET IsGet=@IsGet WHERE MailID=@MailID AND UserID=@UserID
		END 
			
	END 
	ELSE IF @UserIDTemp=@UserID
	BEGIN 
		IF @IsGet=0
			SET @IsGet=1
		ELSE IF @IsGet=2
			SET @IsGet=3
			
		UPDATE TMailList SET IsGet=@IsGet WHERE MailID=@MailID AND UserID=@UserID
	END 
	ELSE
		RETURN 3		--不是该玩家的邮件
		
	SELECT @UserIDTemp AS UserID,@MailMessage AS MailMessage,@SendMoney AS SendMoney,@SendJewels AS SendJewels,@SendLotteries AS SendLotteries,@IsGet AS IsGet
		
	RETURN 0
	
END 