
GO
/****** Object:  StoredProcedure [dbo].[SP_GetRecordURL]    Script Date: 10/28/2016 16:49:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[SP_GetRecordURL]
@RecordID NVARCHAR(20)
AS 
BEGIN 
	DECLARE @GameID INT 
	
	IF NOT EXISTS (SELECT * FROM dbo.TUserGameRecord WHERE RecordNum=@RecordID)
		RETURN 1
	
	SELECT @GameID=GameID FROM dbo.TUserGameRecord WHERE RecordNum=@RecordID
	
	SELECT @GameID AS GameID
	
	return 0
end