
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestUpdateRecord]    Script Date: 04/20/2018 14:39:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<rongqiufen>
-- Create date: <2011.1.25>
-- Description:	<>
-- =============================================
ALTER PROCEDURE [dbo].[SP_ContestUpdateRecord]
    @UserID INT ,			--用户ID
    @ContestID INT ,			--比赛ID
    @LowChip INT ,			--比赛需要的最低金币数
    @CheckedScore INT ,			--平台已经结算后的金币数
    @ContestScore INT ,			--当前这场比赛输赢的金币数
    @ContestPoint INT ,			--比赛ID
    @DeskNo INT ,
    @SitNO INT ,
    @RoomID INT ,
    @MatchID BIGINT ,
    @GameID INT
AS 
    BEGIN
        IF @MatchID = -1 
            BEGIN
                RETURN
            END
	
        IF ( @CheckedScore < @LowChip ) 
            BEGIN
                UPDATE  TContestUserRecord
                SET     ContestScore = @CheckedScore ,
                        ContestCount = ContestCount + 1 ,
                        IsKick = 1 ,--被淘汰
                        LeftTime = GETDATE()
                WHERE   UserID = @UserID
                        AND GameID = @GameID
                        AND RoomID = @RoomID
                        AND ContestID = @ContestID
                        AND MatchID = @MatchID
		--用户被淘汰，则清除该用户的房间登录记录
                DELETE  FROM TWLoginRecord
                WHERE   UserID = @UserID
                        AND WID = @RoomID
                        
                DELETE FROM dbo.TSignUp WHERE UserID=@UserID AND MatchID=@MatchID
            END
        ELSE 
            BEGIN
                UPDATE  TContestUserRecord
                SET     ContestScore = ContestScore + @ContestScore ,
                        ContestCount = ContestCount + 1 ,
                        LeftTime = GETDATE()
                WHERE   UserID = @UserID
                        AND GameID = @GameID
                        AND RoomID = @RoomID
                        AND ContestID = @ContestID
                        AND MatchID = @MatchID
            END
				
        DECLARE @ContestIndex INT
        DECLARE @Chip INT
        SELECT  @Chip = ContestScore ,
                @ContestIndex = ContestCount
        FROM    TContestUserRecord
        WHERE   UserID = @UserID
                AND GameID = @GameID
                AND RoomID = @RoomID
                AND ContestID = @ContestID
                AND MatchID = @MatchID

        SET @ContestIndex = @ContestIndex - 1

        INSERT  INTO dbo.TContestRecord
                ( ContestID ,
                  ContestIndex ,
                  UserID ,
                  ChangeChip ,
                  ChangePoint ,
                  Chip ,
                  DeskNo ,
                  SitNo ,
                  RoomID ,
                  [Time]
                )
        VALUES  ( @ContestID ,
                  @ContestIndex ,
                  @UserID ,
                  @ContestScore ,
                  @ContestPoint ,
                  @Chip ,
                  @DeskNo ,
                  @SitNO ,
                  @RoomID ,
                  GETDATE()
                )
	
    END
	--exec SP_ContestUpdateRecord 1001, 1, 100, 10, 1, 1, 1
