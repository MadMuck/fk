

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_GetClubBuyDeskRecord
@UserID	INT ,
@ClubID INT ,
@StartTime BIGINT,
@EndTime BIGINT
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--æ„¿÷≤øID¥ÌŒÛ
		
	IF @MasterID<>@UserID
	BEGIN 
		SELECT TClubBuyDeskRecord.UserID,BuyTime,CostJewels,DeskPass,TUsers.NickName FROM dbo.TClubBuyDeskRecord,dbo.TUsers 
		WHERE ClubID=@ClubID AND IsCost=1 AND 
		BuyTime>DATEADD(S,@StartTime + 8 * 3600,'1970-01-01 00:00:00')AND BuyTime<DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00')
		AND TClubBuyDeskRecord.UserID = TUsers.UserID AND TClubBuyDeskRecord.UserID=@UserID
	END 
	ELSE 
	BEGIN 
		SELECT TClubBuyDeskRecord.UserID,BuyTime,CostJewels,DeskPass,TUsers.NickName FROM dbo.TClubBuyDeskRecord,dbo.TUsers 
		WHERE ClubID=@ClubID AND IsCost=1 AND 
		BuyTime>DATEADD(S,@StartTime + 8 * 3600,'1970-01-01 00:00:00')AND BuyTime<DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00')
		AND TClubBuyDeskRecord.UserID = TUsers.UserID
	END 
		
	
	
	RETURN 0
END