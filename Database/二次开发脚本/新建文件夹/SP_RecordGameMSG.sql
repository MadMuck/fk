
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



-- =============================================
-- Author:		
-- Create date: 
-- Description:	
-- =============================================
ALTER PROCEDURE [dbo].[SP_RecordGameMSG]
	@UserID		INT,					--用户ID
	@DeskPass	NVARCHAR(20),			--桌子密码
	@GameMSG	VARBINARY(MAX)			--游戏信息
AS
BEGIN
	SET NOCOUNT ON;
	
	DELETE FROM dbo.TRecordGameMSG WHERE UserID=@UserID AND DeskPassword=@DeskPass
	
	INSERT INTO dbo.TRecordGameMSG( UserID, GameInfo, DeskPassword ) VALUES(@UserID,@GameMSG,@DeskPass)
	
END
	RETURN 0
















