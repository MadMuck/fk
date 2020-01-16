
GO
/****** Object:  StoredProcedure [dbo].[SP_DeleteAbnormalFlag]    Script Date: 05/09/2017 10:02:14 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:	    
-- Create date: 
-- Description:	用于清除非正常断线用户在线标志
-- =============================================
ALTER PROCEDURE [dbo].[SP_DeleteAbnormalFlag]
    @UserID INT ,   ----玩家ID
    @MatchID INT 
AS 
BEGIN
    SET NOCOUNT ON ;

    DELETE  FROM TWLoginRecord
    WHERE   userid = @UserID
END
