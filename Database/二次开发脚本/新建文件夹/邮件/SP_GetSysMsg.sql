

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_GetSysMsg
@IsAllUpdate	INT 
AS  
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @MContent NVARCHAR(150)
	DECLARE @NextSendTime DATETIME
	DECLARE @KeepTime DATETIME
	DECLARE @IntervalTime INT 
	SET @MContent=''
	
	IF @IsAllUpdate<>0
	BEGIN 
		DELETE FROM dbo.TSysMessage WHERE KeepTime<GETDATE()
		SELECT TOP 1 @MContent=ISNULL(MContent,'') FROM dbo.TSysMessage WHERE IsSend=0 ORDER BY NextSendTime ASC
		IF @MContent=''
			SELECT TOP 1 @MContent=ISNULL(MContent,'') FROM dbo.TSysMessage WHERE IsSend=1 AND NextSendTime<=GETDATE() AND KeepTime>GETDATE() ORDER BY NextSendTime ASC
		
		IF @MContent<>''
		BEGIN 
			SELECT @NextSendTime=NextSendTime,@KeepTime=KeepTime,@IntervalTime=IntervalTime FROM dbo.TSysMessage WHERE MContent=@MContent
			SET @NextSendTime=DATEADD(mi,@IntervalTime,GETDATE())
			IF @NextSendTime>@KeepTime
				DELETE FROM dbo.TSysMessage WHERE MContent=@MContent
			else
				UPDATE dbo.TSysMessage SET IsSend=1,NextSendTime=@NextSendTime WHERE MContent=@MContent
		END 
		
	END 
	ELSE 
	BEGIN 
		SELECT TOP 1 @MContent=ISNULL(MContent,'') FROM dbo.TSysMessage WHERE IsSend=1 AND KeepTime>GETDATE() ORDER BY NextSendTime ASC
	END 
	
	IF @MContent=''
		RETURN 1
		
	SELECT @MContent AS MContent
	
	RETURN 0
	
END 