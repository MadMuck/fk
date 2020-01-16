

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
ALTER PROCEDURE SP_DeleteRecord
@UserID	INT ,
@DeskPass varchar(20)   --×À×ÓÃÜÂë 
as
begin
	set nocount ON
	
	IF NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecordDelete WHERE DeskPassword=@DeskPass)
		RETURN 3
		
	IF (SELECT DeskMaster FROM dbo.TBuyGameDeskRecordDelete WHERE DeskPassword=@DeskPass) <> @UserID
		RETURN 2
		
	UPDATE TBuyGameDeskRecordDelete SET bNotice=0 WHERE DeskPassword=@DeskPass
		
	RETURN 0
end