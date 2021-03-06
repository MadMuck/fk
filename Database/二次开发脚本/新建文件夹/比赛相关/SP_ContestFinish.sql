
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestFinish]    Script Date: 03/06/2017 10:03:34 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- ======================================================
-- Description:	比赛结束
-- ======================================================
ALTER PROCEDURE [dbo].[SP_ContestFinish]
    @ContestID INT ,
    @GameID INT ,
    @RoomID INT ,
    @MatchID INT ,
    @LeastPeople INT
AS 
    BEGIN
        SET NOCOUNT ON ;
	
		--清空房间登录信息
        DELETE  FROM TWLoginRecord
        WHERE   WID = @RoomID
		--清空该房间的在线人数
        UPDATE  TGameRoomInfo
        SET     OnLineCount = 0
        WHERE   RoomID = @RoomID
                AND GameNameID = @GameID
		--获取奖励方案
        DECLARE @SendTypeID INT
        SET @SendTypeID = 0
        SELECT  @SendTypeID = SendTypeID
        FROM    TContestInfo
        WHERE   ContestID = @ContestID
		--循环赛重置房间信息
		UPDATE dbo.TContestCircleFreeRecord SET FREE=0 WHERE ContestID=@ContestID AND RoomID=@roomID
		-------------------------------------------------------------------------------------------------------
        SELECT  *
        INTO    #temp
        FROM    ( SELECT    ROW_NUMBER() OVER ( ORDER BY M.USERID ) AS orders ,
                            M.UserID ,
                            M.walletmoney ,
                            M.bankmoney ,
                            givenum,
                            GiveType
                  FROM      TUserInfo M
                            INNER JOIN ( SELECT B.UserID ,
                                                C.GiveNum,
												C.GiveType
                                         FROM   TContestRecord_New B
                                                INNER JOIN Web_MatchAwardConfig C ON C.TypeID = @SendTypeID
                                                              AND B.MatchID = @MatchID
                                                              AND B.RankNum = C.Rank
                                       ) AS N ON M.UserID = N.UserID
                ) a
        DECLARE @looper INT
        SET @looper = 0
        SELECT  @looper = MAX(orders)
        FROM    #temp
		
        DECLARE @userid INT
        DECLARE @UserName NVARCHAR(50)---用户名
        DECLARE @StartMoney BIGINT---用户改变之前总金币（银行+钱包）
        DECLARE @ChangeMoney BIGINT----当前更改数量
        DECLARE @ChangeType INT
        DECLARE @GiveType INT
        DECLARE @StartLotteries BIGINT
        DECLARE @JewelsBefore INT 

        WHILE @looper > 0 
            BEGIN
                SELECT  @userid = userid ,
                        @StartMoney = ( walletmoney + bankmoney ) ,
                        @ChangeMoney = givenum,
                        @GiveType = GiveType
                FROM    #temp
                WHERE   orders = @looper		
                SELECT  @UserName = userName
                FROM    TUsers
                WHERE   userid = @userid
				
				IF @GiveType=0
					EXEC proc_AddWeb_MoneyChangeLog @userid, @UserName,
						@StartMoney, @ChangeMoney, 34, '比赛奖励'  ---金币变化日志
				IF @GiveType=1
				BEGIN 
					SELECT @StartLotteries=Lotteries FROM dbo.Web_Users WHERE userid = @userid
					INSERT INTO dbo.Web_LotteriesLog( UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
					VALUES  ( @userid,@UserName,@StartLotteries,@ChangeMoney,@StartLotteries+@ChangeMoney,GETDATE(),N'比赛奖励' ,1 )
				END 
				IF @GiveType=2
				BEGIN 
					SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
					update Web_Users set Jewels=Jewels+@ChangeMoney where UserID=@UserID
					INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
					VALUES(@UserID,@JewelsBefore,@ChangeMoney,7,GETDATE(),'比赛奖励')
				END 

                SET @looper = @looper - 1
            END

        DROP TABLE #temp
		---------------------------------------------------------------------------------------------------------
		--更新奖励到个人钱包
        UPDATE  A
        SET     A.WalletMoney = A.WalletMoney + T.GiveNum
        FROM    TUserInfo A ,
                ( SELECT    B.UserID ,
                            C.GiveNum,
                            C.GiveType
                  FROM      TContestRecord_New B
                            INNER JOIN Web_MatchAwardConfig C ON C.TypeID = @SendTypeID
                                                              AND B.MatchID = @MatchID
                                                              AND B.RankNum = C.Rank
                ) AS T
        WHERE   A.UserID = T.UserID AND T.GiveType=0

		update A 
		set A.Lotteries=A.Lotteries+T.GiveNum
		from dbo.Web_Users A,
			(select B.UserID,C.GiveNum,C.GiveType 
			from TContestRecord_New B inner join Web_MatchAwardConfig C 
			on C.TypeID=@SendTypeID and B.MatchID=@MatchID and B.RankNum=C.Rank) as T
		where A.UserID=T.UserID AND T.GiveType=1

		IF NOT EXISTS (SELECT * FROM dbo.TContestRecord_New WHERE GameNameID = @GameID AND ContestID = @ContestID AND MatchID = @MatchID AND RoomID = @RoomID)
		BEGIN 
			RETURN 1
		END 
		
		--清空该比赛的报名信息(游戏未开始不删除报名信息,在返还报名费时处理)
		DELETE  FROM TSignUp
		WHERE   GameNameID = @GameID
				AND RoomID = @RoomID
				AND ContestID = @ContestID
		
		--返回获得名次的用户ID
        SELECT  UserID,RankNum
        FROM    TContestRecord_New
        WHERE   GameNameID = @GameID
                AND ContestID = @ContestID
                AND MatchID = @MatchID
                AND RoomID = @RoomID
        
        RETURN 0
    END
