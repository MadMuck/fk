GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROCEDURE [dbo].[SP_ClearGameData]
	@RoomID		INT		
AS
BEGIN
	SET NOCOUNT ON;
    
    --��������Ϣ
    DELETE FROM dbo.TSignUp WHERE RoomID=@RoomID
    --�����¼��־
    DELETE FROM dbo.TWLoginRecord WHERE WID=@RoomID
    --
    
END

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
