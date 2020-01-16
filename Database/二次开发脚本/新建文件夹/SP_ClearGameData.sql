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
    
    --清理报名信息
    DELETE FROM dbo.TSignUp WHERE RoomID=@RoomID
    --清理登录标志
    DELETE FROM dbo.TWLoginRecord WHERE WID=@RoomID
    --
    
END

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
