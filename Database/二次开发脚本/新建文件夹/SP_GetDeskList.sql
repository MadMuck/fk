

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
alter PROCEDURE SP_GetDeskList
@UserID  INT,
@Type	 INT		--0-开启房间，1-已关闭房间
AS 
BEGIN 
	SET NOCOUNT ON
	declare @FinishCondition int
	declare @LeaveSeconds int
	set @FinishCondition=0
	set @LeaveSeconds=0
	
	IF @Type=0
	BEGIN 
		SELECT IsPlay,BuyTime,szDeskPassWord,ComName AS GameName ,FinishCondition, (BuyMinutes * 60 - datediff(s,BeginTime,GETDATE())) as LeaveSeconds, BuyMinutes
			FROM dbo.TBuyGameDeskRecord,dbo.TGameNameInfo WHERE MasterID=@UserID AND NameID=GameID AND ClubID=0 ORDER BY BuyTime DESC
	END
	ELSE IF @Type=1
	BEGIN 
		SELECT TOP 20 IsPlay,CreateTime AS BuyTime,DeskPassword AS szDeskPassWord,ComName AS GameName,FinishCondition,@LeaveSeconds as LeaveSeconds
		 FROM dbo.TBuyGameDeskRecordDelete,dbo.TGameNameInfo WHERE DeskMaster=@UserID AND NameID=GameID AND bNotice=1 AND ClubID=0 ORDER BY CreateTime DESC
	END 
END 