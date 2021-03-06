USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_DeleteAbnormalFlag]    Script Date: 04/13/2017 14:03:43 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:	    Longan
-- Create date: 5.14 2009
-- Description:	用于清除非正常断线用户在线标志
-- =============================================
ALTER PROCEDURE [dbo].[SP_DeleteAbnormalFlag]
    @UserID INT ,   ----玩家ID
    @MatchID INT
AS 
    BEGIN
        SET NOCOUNT ON ;
        IF @MatchID > 0 
            UPDATE  TContestUserRecord
            SET     IsKick = 1
            WHERE   UserID = @UserID
                    AND MatchID = @MatchID 
        --UPDATE  TUSERS
        --SET     onlineflag = 0
        --WHERE   userid = @UserID
        DELETE  FROM TWLoginRecord
        WHERE   userid = @UserID
    END
