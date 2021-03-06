
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestApply]    Script Date: 03/01/2017 10:57:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROC [dbo].[SP_ContestApply]
    @gameID INT ,---游戏ID
    @UserID INT ,---用户ID
    @TypeID INT ,--0报名1退赛
    @roomID INT ,---房间ID
    @ContestID INT
AS 
    BEGIN
        SET NOCOUNT ON ;

        DECLARE @rs INT  ---操作状态 1参赛成功2退赛成功3报名失败(已报名了)4报名成功（比赛已开赛无法再）,5未报名退赛失败,6比赛已开赛无法退赛，7已达到最大人数，8钱不足,9房间配置错误
        DECLARE @yNum INT ---已报名人数
        DECLARE @MaxNum INT ---最大报名数
        DECLARE @tState INT  ---报名状态（当前用户）0已报名1已开赛
        DECLARE @UMoney BIGINT ---用户当前的积分
        DECLARE @Money INT  ---扣费
        DECLARE @isOK INT
        DECLARE @EnterFeeType INT 
        DECLARE @Jewels INT 
        DECLARE @MatchID INT 
        DECLARE @ContestType INT 
		DECLARE @UserName NVARCHAR(20)
		DECLARE @JewelsBefore INT 
		
        SELECT  @rs = -1 ,
                @tState = -1 ,
                @yNum = 0 ,
                @MaxNum = 0 ,
                @isOK = 0,
                @MatchID =0
        
        SELECT @MaxNum = ISNULL(t.UpPeople, 0), @Money = ISNULL(Entryfee, 0) ,@EnterFeeType=t.EntryFeeType,@ContestType=ContestType
        FROM TContestInfo AS t JOIN TGameRoomBase tg ON tg.contestid = t.contestid
        WHERE tg.roomid = @roomID AND t.contestID = @contestID

        IF @TypeID = 0 AND @UserID > 0---报名
        BEGIN
            SELECT TOP 1 @tState = ISNULL(signState, -1) FROM TSignUp
            WHERE gameNameid = @gameID AND roomid = @roomID AND ContestID = @ContestID ---是否开赛了
            IF @tState = 1---已开赛了(无法再报名)
            BEGIN
                SET @rs = 4
            END
            ELSE IF EXISTS (SELECT * FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID IN (SELECT ContestID FROM dbo.TContestInfo 
						WHERE DATEDIFF(mi,GETDATE(),StartTime)<10 AND DATEDIFF(mi,GETDATE(),StartTime)>0 AND ContestType<>0))
			BEGIN 
				SET @rs=12
			END 
            ELSE ---未开赛
            BEGIN
				---查询用户是否参与了报名 0:报名 1:未报名
                SET @tState = -1
                SELECT TOP 1 @tState = ISNULL(signState, -1) FROM TSignUp WHERE gameNameid = @gameID AND roomid = @roomID AND userid = @UserID
                IF @tState < 0 --未报名
                BEGIN
                    SELECT @yNum = ISNULL(COUNT(id), 0) FROM TSignUp WHERE gamenameid = @gameID AND roomid = @roomID ---已报名人数
                    
                    IF EXISTS (SELECT * FROM dbo.TSignUp WHERE gamenameid = @gameID AND roomid = @roomID)
						SELECT @MatchID=MatchID FROM dbo.TSignUp WHERE gamenameid = @gameID AND roomid = @roomID
					ELSE 
					BEGIN 
						IF EXISTS (SELECT * FROM dbo.TContestLog WHERE ContestID=@ContestID AND RoomID=@roomID AND ContestType=@ContestType AND ContestStat=0)
						BEGIN 
							SELECT @MatchID=MatchID FROM dbo.TContestLog WHERE ContestID=@ContestID AND RoomID=@roomID AND ContestType=@ContestType AND ContestStat=0
						END 
						ELSE 
						BEGIN 
							IF @ContestType=0
							BEGIN 
								INSERT INTO TContestLog(ContestID,RoomID,ContestType,ContestStat) VALUES(@ContestID,@RoomID,@ContestType,0)
								Select @MatchID=ident_current('TContestLog')
							END 
							ELSE
							BEGIN 
								SET @rs = 9
							END 
						END 
					END 
                    
                    IF @yNum < @MaxNum AND @MaxNum > 0 AND @rs=-1 --未达到最大报名人数
                    BEGIN
                        IF @Money > 0 --收入场费
                        BEGIN
							IF @EnterFeeType=0
							BEGIN 
								SELECT  @UMoney = ISNULL(WalletMoney,0) FROM tuserinfo WHERE userid = @UserID
								IF @UMoney >= @Money
								BEGIN	--扣除本次报名的费用
									SELECT @UMoney=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
									SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
									UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney-@Money WHERE UserID=@UserID
									INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
											VALUES  ( @UserID ,@UserName ,@UMoney ,-@Money ,21 ,GETDATE() ,'比赛报名') 
									INSERT INTO dbo.TSignUp( GameNameID ,ContestID ,RoomID ,UserID ,SignState ,SignTime ,EntryFee,MatchID)
											VALUES ( @gameID,@ContestID,@RoomID,@UserID,0,GETDATE(),@Money,@MatchID)
									SET @rs = 1
								END
								ELSE	---没钱入场
								BEGIN
									SET @rs = 8
								END
							END 
							ELSE IF @EnterFeeType=1
							BEGIN 
								SELECT  @Jewels = ISNULL(Jewels,0) FROM dbo.Web_Users WHERE userid = @UserID
								IF @Jewels >= @Money
								BEGIN	--扣除本次报名的费用
									SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
									update Web_Users set Jewels=Jewels-@Money where UserID=@UserID
									INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
									VALUES(@UserID,@JewelsBefore,-@Money,4,GETDATE(),'比赛报名消耗钻石')
									INSERT INTO dbo.TSignUp( GameNameID ,ContestID ,RoomID ,UserID ,SignState ,SignTime ,EntryFee,MatchID)
											VALUES ( @gameID,@ContestID,@RoomID,@UserID,0,GETDATE(),@Money,@MatchID)
									SET @rs = 1
								END
								ELSE	---没钱入场
								BEGIN
									SET @rs = 8
								END
							END 
                        END
                        ELSE --不需要入场费
						BEGIN
							INSERT INTO dbo.TSignUp( GameNameID ,ContestID ,RoomID ,UserID ,SignState ,SignTime ,EntryFee,MatchID)
											VALUES ( @gameID,@ContestID,@RoomID,@UserID,0,GETDATE(),0,@MatchID)
							SET @rs = 1
						END
                    END
                    ELSE IF @rs=-1 ----已达到最大报名人数
                    BEGIN
                        SET @rs = 7
                    END
                END
                IF @tState = 0 --已报名（无需再报名）
                BEGIN
                    SET @rs = 3
                END
            END
        END
        ELSE IF @TypeID = 1 AND @UserID > 0---退赛
        BEGIN
            SELECT TOP 1 @tState = ISNULL(signState, -1) , @money = ISNULL(entryfee, 0) FROM TSignUp
            WHERE gameNameid = @gameID AND roomid = @roomID AND userid = @UserID ---查询用户是否参与了报名
            IF @tState = 0 --成功退赛
            BEGIN
                SET @rs = 2
                IF @EnterFeeType=0
                BEGIN 
					SELECT @UMoney=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
					SELECT @UserName=UserName FROM dbo.TUsers WHERE UserID=@UserID
					UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@Money WHERE UserID=@UserID
					INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
							VALUES  ( @UserID ,@UserName ,@UMoney ,@Money ,22 ,GETDATE() ,'比赛退赛')
				END 
				ELSE IF @EnterFeeType=1
				BEGIN 
					SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
					update Web_Users set Jewels=Jewels+@Money where UserID=@UserID
					INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
					VALUES(@UserID,@JewelsBefore,@Money,5,GETDATE(),'比赛退赛返还钻石')
				END 
				delete TSignUp where gameNameid=@gameID and roomid=@roomID and userid=@UserID
            END
            IF @tState < 0 ---未报名，无法退赛
            BEGIN
                SET @rs = 5
            END
            IF @tState = 1 ---已开赛比赛，无法退赛
            BEGIN
                SET @rs = 6
            END
        END
        SELECT @yNum = ISNULL(COUNT(id), 0) FROM TSignUp WHERE gamenameid = @gameID AND roomid = @roomID ---已报名人数
		----查询该游戏的报名人数上限
        SELECT @MaxNum = ISNULL(t.UpPeople, 0) FROM TContestInfo AS t JOIN TGameRoomBase tg ON tg.contestid = t.contestid
        WHERE tg.roomid = @roomID AND t.contestID = @contestID
        IF @MaxNum > 0 
        BEGIN
            IF @MaxNum = @yNum 
            BEGIN
                SET @isOK = 1
            END
        END

		SELECT @UMoney=WalletMoney FROM dbo.TUserInfo WHERE UserID=@UserID
        SELECT @Jewels=Jewels FROM Web_Users where UserID=@UserID

        SELECT  @rs AS rs ,
                @yNum AS ynum ,
                @MaxNum - @yNum AS snum ,
                @maxnum AS maxnum ,
                @isOk AS isOk,
                @UMoney AS UserMoney,
                @Jewels AS Jewels
    END
