

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_MailUpdate

AS  
BEGIN 
	SET NOCOUNT ON
	INSERT INTO TMailDeleteLog(MailID,UserID) SELECT MailID,UserID FROM TMailList WHERE DATEDIFF(mi,SendTime,GETDATE())>KeepTime*24*60 AND KeepTime<>0
	
	DECLARE @Mailtable TABLE(MailID INT)
	INSERT INTO @Mailtable(MailID) SELECT MailID FROM TMailList WHERE UserID=0 AND DATEDIFF(mi,SendTime,GETDATE())>KeepTime*24*60 AND KeepTime<>0
	DELETE FROM TMailList WHERE DATEDIFF(mi,SendTime,GETDATE())>KeepTime*24*60 AND KeepTime<>0
	DELETE FROM TMailtoAllUserLog WHERE MailID IN (SELECT MailID FROM @Mailtable)
	
	DECLARE @MailTemp TABLE(MailID INT,UserID INT,SenderName NVARCHAR(25),MailTitle	NVARCHAR(25),SendTime DATETIME,IsGet INT,IsNew INT,KeepTime INT)
	
	INSERT INTO @MailTemp(MailID ,UserID ,SenderName ,MailTitle,SendTime,IsGet,IsNew,KeepTime)
	SELECT MailID ,UserID ,SenderName ,MailTitle ,SendTime,IsGet,1,KeepTime FROM TMailList WHERE IsSend=0
	
	UPDATE TMailList SET IsSend=1
	
	INSERT INTO @MailTemp (MailID ,UserID,IsNew) SELECT MailID,UserID,0 FROM TMailDeleteLog
	
	DELETE FROM TMailDeleteLog
	
	SELECT * FROM @MailTemp
	
	RETURN 0
END 