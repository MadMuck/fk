
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestAbandon]    Script Date: 02/21/2017 17:05:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[SP_ContestExit]
    @UserID INT ,
    @RoomID INT ,
    @ContestID INT,
    @MatchID INT
AS 
    BEGIN
        SET NOCOUNT ON ;
		
		UPDATE dbo.TContestUserRecord SET IsKick =1 WHERE UserID=@UserID AND ContestID=@ContestID AND MatchID=@MatchID
		delete TSignUp where roomid=@roomID and userid=@UserID
    END
