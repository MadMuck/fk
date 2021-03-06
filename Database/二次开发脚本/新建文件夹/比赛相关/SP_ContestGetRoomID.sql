
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestGetRoomID]    Script Date: 02/22/2017 09:25:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Description:	从报名表中获取客户端应该进入比赛房间ID
-- =============================================
ALTER PROCEDURE [dbo].[SP_ContestGetRoomID]
    @UserID INT ,	--用户ID
    @ContestID INT
AS 
    BEGIN
		SET NOCOUNT ON ;
    
        DECLARE @ContestRoomID INT
        DECLARE @ContestType INT 
		DECLARE @MatchID INT 
		DECLARE @RoomTemp INT 
		DECLARE @BeginTime DATETIME
		DECLARE @CircleTime INT 
		DECLARE @bNotifyG INT 
		SET @MatchID=0
        SET @ContestRoomID = 0
        SET @bNotifyG=0
		--先判断该用户是否为断线用户
        SELECT  @ContestRoomID = [WID]
        FROM    [TWLoginRecord]
        WHERE   userid = @UserID
        
        IF @ContestRoomID = 0 
        BEGIN 
			SELECT @MatchID=ISNULL(MatchID,0),@RoomTemp=RoomID FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID
			IF @MatchID <> 0
			BEGIN 
				IF EXISTS (SELECT * FROM dbo.TContestUserRecord WHERE UserID=@UserID AND MatchID=@MatchID AND IsKick=0)
					SET @ContestRoomID=@RoomTemp
			END 
        END 
        
        IF @ContestRoomID = 0 
            BEGIN
                SELECT  @ContestRoomID = RoomID
                FROM    dbo.TSignUp
                WHERE   (SignState = 0 OR SignState = 2)
                        AND ContestID = @ContestID
                        AND UserID = @UserID
            
				IF @ContestRoomID = 0
				BEGIN 
					SELECT TOP 1
							@ContestRoomID = t.roomid
					FROM    ( SELECT    roomid ,
										COUNT(roomid) AS countnum
							  FROM      TSignUp
							  WHERE     signstate = 0
										AND ContestID = @ContestID
							  GROUP BY  roomid
							) t
					ORDER BY t.countnum DESC ---查找未开赛并且报名人数最多的一个房间
				END 
                IF @ContestRoomID = 0 ---没有用户报名，则找一个可以报名的房间
                    BEGIN
				--先判断是否压根儿就没有用户报名所有游戏
						SELECT @ContestType=ContestType,@BeginTime=StartTime,@CircleTime=CircleTime FROM dbo.TContestInfo WHERE ContestID=@ContestID
						IF @ContestType=2		--循环赛
						BEGIN 
							--检查房间状态表
							DELETE FROM TContestCircleFreeRecord WHERE RoomID NOT IN (SELECT RoomID FROM dbo.TGameRoomInfo WHERE UpdateTime >= DATEADD(MI, - 3, GETDATE()))
							UPDATE TContestCircleFreeRecord SET FREE=0 WHERE FREE=1 AND BeginTime<GETDATE()
							IF EXISTS (SELECT * FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=1)
							BEGIN 
								IF EXISTS (SELECT * FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=2)
									RETURN 1
								ELSE
								BEGIN 
									SELECT @ContestRoomID=RoomID ,@MatchID=MatchID FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=1
								END  
							END 
							ELSE IF NOT EXISTS (SELECT * FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=0)
								RETURN 1
							ELSE 
							BEGIN 
								SELECT TOP 1 @ContestRoomID=RoomID FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=0
								--计算开始时间
								IF (@BeginTime<GETDATE())
								BEGIN 
									DECLARE @Totalmi INT 
									DECLARE @CountTime INT 
									SET @Totalmi = DATEDIFF(mi,@BeginTime,GETDATE())
									SET @CountTime= @CircleTime-(@Totalmi%@CircleTime)
									SET @BeginTime = DATEADD(mi,(@Totalmi+@CountTime),@BeginTime)
									
									UPDATE dbo.TContestInfo SET StartTime=@BeginTime WHERE ContestID=@ContestID
								END 
								
								INSERT INTO TContestLog(ContestID,RoomID,ContestType,ContestStat) VALUES(@ContestID,@ContestRoomID,@ContestType,0)
								Select @MatchID=ident_current('TContestLog')
								
								UPDATE TContestCircleFreeRecord SET FREE=1,BeginTime=@BeginTime,MatchID=@MatchID WHERE ContestID=@ContestID AND FREE=0 AND RoomID=@ContestRoomID 
								
								SET @bNotifyG=1
							END 
						END 
						ELSE 
						BEGIN 
							IF EXISTS ( SELECT DISTINCT (RoomID) FROM TSignUp WHERE ContestID = @ContestID ) 
							BEGIN
								SELECT TOP 1
										@ContestRoomID = a.RoomID
								FROM    ( SELECT    y.RoomID
										  FROM      TGameRoomInfo x
													INNER JOIN TGameRoomBase y ON x.RoomID = y.RoomID
										  WHERE     x.EnableRoom = 1
													AND y.ContestID = @ContestID
										) a
								WHERE   a.RoomID NOT IN (
										SELECT DISTINCT
												( RoomID )
										FROM    TSignUp
										WHERE   SignState <> 0
												OR ContestID <> @ContestID )
							END
							ELSE 
							BEGIN
								--找一个在线人数最多的房间进去
								SELECT TOP 1
										@ContestRoomID = a.RoomID
								FROM    TGameRoomInfo a
										INNER JOIN TGameRoomBase b ON a.RoomID = b.RoomID
								WHERE   b.ContestID = @ContestID
										AND a.EnableRoom = 1
								ORDER BY a.OnLineCount DESC
							END
						END 
                    END
            END

        IF @ContestRoomID = 0 
            BEGIN
                RETURN 1
            END
        ELSE 
            BEGIN
				DECLARE @ConstestNum INT   
				DECLARE @ChampionCount INT 
				DECLARE @BestRank INT     
				DECLARE @JoinCount INT
				DECLARE @ContestTime DATETIME
				SET @JoinCount=0
				SET @BestRank=0
				SET @ChampionCount=0
				
				SELECT @ContestType=ContestType,@ContestTime=StartTime FROM dbo.TContestInfo WHERE ContestID=@ContestID
				IF @ContestType=2
					SELECT @ContestTime=ISNULL(BeginTime,0) FROM dbo.TContestCircleFreeRecord WHERE ContestID=@ContestID AND RoomID=@ContestRoomID
				
				SELECT @ConstestNum=ISNULL(COUNT(*),0) FROM dbo.TSignUp WHERE ContestID=@ContestID AND RoomID=@ContestRoomID
				SELECT @JoinCount=ISNULL(COUNT(*),0) FROM dbo.TContestRecord_New WHERE ContestID=@ContestID AND UserID=@UserID
				SELECT @ChampionCount=ISNULL(COUNT(*),0) FROM dbo.TContestRecord_New WHERE ContestID=@ContestID AND UserID=@UserID AND RankNum=1
				SELECT TOP 1 @BestRank=ISNULL(RankNum,0) FROM dbo.TContestRecord_New WHERE ContestID=@ContestID AND UserID=@UserID ORDER BY RankNum ASC
				 
                SELECT TOP 3 @ContestRoomID AS RoomID,@ConstestNum AS ConstestNum,@ChampionCount AS ChampionCount,@BestRank AS BestRank,
                @JoinCount AS JoinCount,@ContestTime AS ContestTime,@ContestType AS ContestType,GiveNum,GiveType,@bNotifyG AS bNotifyG,@MatchID AS MatchID
                FROM dbo.Web_MatchAwardConfig a INNER JOIN TContestInfo b on a.TypeID=b.SendTypeID and b.ContestID=@ContestID ORDER BY Rank ASC
            END
        RETURN 0
    END
