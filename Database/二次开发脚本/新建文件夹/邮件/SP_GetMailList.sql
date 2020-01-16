

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_GetMailList
@UserID	INT 
AS  
BEGIN 
	SET NOCOUNT ON
	INSERT INTO TMailDeleteLog(MailID,UserID) SELECT MailID,UserID FROM TMailList WHERE DATEDIFF(dd,SendTime,GETDATE())>KeepTime*24*60 AND UserID<>@UserID AND KeepTime<>0
	
	DECLARE @Mailtable TABLE(MailID INT)
	INSERT INTO @Mailtable(MailID) SELECT MailID FROM TMailList WHERE UserID=0 AND DATEDIFF(dd,SendTime,GETDATE())>KeepTime*24*60 AND KeepTime<>0
	DELETE FROM TMailList WHERE DATEDIFF(dd,SendTime,GETDATE())>KeepTime*24*60 AND KeepTime<>0
	DELETE FROM TMailtoAllUserLog WHERE MailID IN (SELECT MailID FROM @Mailtable)
	
	DECLARE @MailTemp TABLE(MailID INT,UserID INT,SenderName NVARCHAR(25),MailTitle	NVARCHAR(25),SendTime DATETIME,IsGet INT,KeepTime INT)
	
	INSERT INTO @MailTemp(MailID ,UserID ,SenderName ,MailTitle,SendTime,IsGet,KeepTime)
	SELECT MailID ,UserID ,SenderName ,MailTitle ,SendTime,IsGet,KeepTime FROM TMailList WHERE UserID=@UserID OR UserID=0
	
	UPDATE a SET a.IsGet=TMailtoAllUserLog.IsGet FROM @MailTemp a, dbo.TMailtoAllUserLog WHERE dbo.TMailtoAllUserLog.MailID=a.MailID AND dbo.TMailtoAllUserLog.UserID=@UserID
	DELETE FROM @MailTemp WHERE IsGet=5		--已删除
	
	--增加已获取记录
	UPDATE TMailList SET IsSend=1 WHERE UserID=@UserID
	INSERT INTO TMailtoAllUserLog(MailID,UserID,IsGet) SELECT MailID,@UserID,IsGet FROM TMailList WHERE UserID=0 
	AND MailID NOT IN (SELECT MailID FROM TMailtoAllUserLog WHERE UserID=@UserID)
	
	SELECT * FROM @MailTemp
	
	RETURN 0
END 