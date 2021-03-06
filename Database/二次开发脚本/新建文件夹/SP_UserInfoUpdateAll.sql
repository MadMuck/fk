USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UserInfoUpdateAll]    Script Date: 05/16/2019 15:33:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		lifuquan
-- Create date: 2012-12-04
-- Description:	批量更新一局游戏结束后所有玩家的信息
-- =============================================
ALTER PROCEDURE [dbo].[SP_UserInfoUpdateAll]
    (
      @UserCount INT ,			-- 玩家总数
      @NameID INT ,			--游戏ID
      @GameTable NVARCHAR(30) ,	--游戏表
      @PlayTimeCount INT ,			-- 游戏时间
      @IsExperciseRoom INT ,			--是否是体验房间
      @IsMoneyGame INT ,			--是否金币场
      @Param NVARCHAR(MAX) ,	-- 特殊格式的参数，由服务器与本存储过程协同解释协议
      @RoomID INT,				-- PengJiLin, 2011-4-14, 增加房间ID 
      @Type int	,--类型 --.0.金币结算 1-俱乐部积分结算  2-(练习房)积分结算(这个不会进来结算) .3.碎钻结算
      @PassWord varchar(20) 
    )
AS 
    BEGIN
        SET NOCOUNT ON ;
	--SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;--设置事物的隔离基本

        IF OBJECT_ID('tempdb.dbo.#temp_TMatchTable') IS NOT NULL 
            BEGIN
                DROP TABLE #temp_TMatchTable
            END
        CREATE TABLE #temp_TMatchTable
            (
              UserID INT ,				-- 1用户ID
              OnlineTime INT ,				-- 2用户在线时间
              Point BIGINT ,			-- 3积分
              [Money] BIGINT ,			-- 4金币
              Money_tiyan BIGINT ,    -- 用来存放 4金币 的中间变量，如果是体验房间,将清零
              TaxCom BIGINT ,			-- 5税收
              Win INT ,				-- 6胜局数
              Lost INT ,				-- 7输局数
              Mid INT ,				-- 8平局数
              Cut INT ,				-- 9断线数
              BankVer INT ,				--银行版本
              StartMoney BIGINT DEFAULT ( 0 ) ,
              UserBWalletMoney BIGINT DEFAULT ( 0 ) ,
              IsMoneyGame INT ,				--是否金币场
              IsExperciseRoom INT ,				--是否是体验房间
              IsUserMoney BIGINT DEFAULT ( 0 ) ,	--玩家身上的金币是否小于0
              PlayTimeCount INT ,				--游戏时间
              PlayTimes_V INT ,
              PlayRounds_V INT
            )
	-- 真正的参数，由N个用户组成
        DECLARE @UserID INT				-- 1用户ID
        DECLARE @OnlineTime INT				-- 2用户在线时间
        DECLARE @Point BIGINT			-- 3积分
        DECLARE @Money BIGINT			-- 4金币
        DECLARE @TaxCom BIGINT			-- 5税收
        DECLARE @Win INT				-- 6胜局数
        DECLARE @Lost INT				-- 7输局数
        DECLARE @Mid INT				-- 8平局数
        DECLARE @Cut INT				-- 9断线数

        DECLARE @iUser INT	-- 循环变量

        DECLARE @pos0 INT	-- 字符串位置
        DECLARE @pos1 INT	-- 字符串位置
        DECLARE @pos2 INT	-- 字符串位置
        DECLARE @pos3 INT	-- 字符串位置
        DECLARE @pos4 INT	-- 字符串位置
        DECLARE @pos5 INT	-- 字符串位置
        DECLARE @pos6 INT	-- 字符串位置
        DECLARE @pos7 INT	-- 字符串位置
        DECLARE @pos8 INT	-- 字符串位置
        DECLARE @pos9 INT	-- 字符串位置

		declare @ClubID int 
		declare @MasterID int
		select @ClubID=isnull(ClubID,0),@MasterID=isnull(MasterID,0) from TBuyGameDeskRecord where @PassWord=szDeskPassWord
		
	-- PengJiLin, 2011-3-29, 增加在线时长和游戏局数送金币功能
        DECLARE @PlayRounds INT--第一次送金币的局数
        DECLARE @PlayTimes INT--在线送金币的游戏时长
        DECLARE @PlayRounds_m INT--达到第一次局数送的金币
        DECLARE @PlayTimes_m INT--达到时长送的金币
	
        SELECT  @PlayTimes = PlayTimes ,
                @PlayTimes_m = PlayTimesMoney ,
                @PlayRounds = PlayCounts_1 ,
                @PlayRounds_m = PlayCountsMoney_1
        FROM    TGameRoomBase
        WHERE   RoomID = @RoomID

	-- 如果没有设置送金币，则数据置0
        DECLARE @PlayRounds_V INT
        DECLARE @PlayTimes_V INT
        SET @PlayTimes_V = @PlayTimeCount
        SET @PlayRounds_V = 1
        IF @PlayTimes <= 0
            OR @PlayTimes_m <= 0 
            BEGIN
                SET @PlayTimes_V = 0
            END
        IF @PlayRounds <= 0
            OR @PlayRounds_m <= 0 
            BEGIN
                SET @PlayRounds_V = 0
            END

	-- 如果有设置送金币，则返回值为11，服务端则使用送金币功能
        DECLARE @SendMoneyFlag INT
        SET @SendMoneyFlag = 0
        IF @PlayTimes_V > 0
            OR @PlayRounds_V > 0 
            BEGIN
                SET @SendMoneyFlag = 1
            END

        DECLARE @BankVer INT			--银行版本
        SELECT TOP 1
                @BankVer = BankVersion
        FROM    Web_Config

    -- 赋初值后，进入循环
        SET @iUser = 0
        SET @pos0 = 0
		
		DECLARE @TaxComSum INT --网站同事彭清特殊要求传给她所有人的抽税总和。
		DECLARE @IsRobot INT --网站同事彭清特殊要求，不给他机器人的抽税
		DECLARE @IsTax INT    --房间选择台费还是税率。0代表台费，1代表税率 
		declare @GameTypeID int
		DECLARE @RoomRule INT
		DECLARE @IsTaiFei INT
		SET @TaxComSum = 0;
		set @GameTypeID=0
		SET @IsRobot = 0;
		select @IsTax=TaxType from TGameRoomBase where RoomID=@RoomID
		select @GameTypeID=GameTypeID from TGameRoomInfo where RoomID=@RoomID
		if @GameTypeID<>2
		set @GameTypeID=1
		select @RoomRule=RoomRule from TGameRoomBase where RoomID=@RoomID
		SET @IsTaiFei=@RoomRule&(0x00020000)
		
        WHILE @iUser < @UserCount 
            BEGIN
                SET @pos1 = CHARINDEX(',', @param, @pos0 + 1)
                SET @pos2 = CHARINDEX(',', @param, @pos1 + 1)
                SET @pos3 = CHARINDEX(',', @param, @pos2 + 1)
                SET @pos4 = CHARINDEX(',', @param, @pos3 + 1)
                SET @pos5 = CHARINDEX(',', @param, @pos4 + 1)
                SET @pos6 = CHARINDEX(',', @param, @pos5 + 1)
                SET @pos7 = CHARINDEX(',', @param, @pos6 + 1)
                SET @pos8 = CHARINDEX(',', @param, @pos7 + 1)
                SET @pos9 = CHARINDEX(',', @param, @pos8 + 1)

		-- 如果没有找到后面的元素，则直接返回
                IF @pos9 = 0 
                    BEGIN
                        CONTINUE ;
                    END
		-- 显式给一个临时变量赋值，以便出问题时调试
		-- 直接在下面插入记录语句里写convert函数也是可以的，但那样代码不易读
                SET @UserID = CONVERT(INT, SUBSTRING(@param, @pos0,
                                                     @pos1 - @pos0))
                SET @OnlineTime = CONVERT(INT, SUBSTRING(@param, @pos1 + 1,
                                                         @pos2 - @pos1 - 1))
                SET @Point = CONVERT(BIGINT, SUBSTRING(@param, @pos2 + 1,
                                                       @pos3 - @pos2 - 1))
                SET @Money = CONVERT(BIGINT, SUBSTRING(@param, @pos3 + 1,
                                                       @pos4 - @pos3 - 1))
                SET @TaxCom = CONVERT(BIGINT, SUBSTRING(@param, @pos4 + 1,
                                                        @pos5 - @pos4 - 1))
                SET @Win = CONVERT(INT, SUBSTRING(@param, @pos5 + 1,
                                                  @pos6 - @pos5 - 1))
                SET @Lost = CONVERT(INT, SUBSTRING(@param, @pos6 + 1,
                                                   @pos7 - @pos6 - 1))
                SET @Mid = CONVERT(INT, SUBSTRING(@param, @pos7 + 1,
                                                  @pos8 - @pos7 - 1))
                SET @Cut = CONVERT(INT, SUBSTRING(@param, @pos8 + 1,
                                                  @pos9 - @pos8 - 1))

                INSERT  INTO #temp_TMatchTable
                        ( UserID ,
                          OnlineTime ,
                          Point ,
                          [Money] ,
                          Money_tiyan ,
                          TaxCom ,
                          Win ,
                          Lost ,
                          Mid ,
                          Cut ,
                          BankVer ,
                          StartMoney ,
                          UserBWalletMoney ,
                          IsMoneyGame ,
                          IsExperciseRoom ,
                          PlayTimeCount ,
                          PlayTimes_V ,
                          PlayRounds_V
                        )
                VALUES  ( @UserID ,
                          @OnlineTime ,
                          @Point ,
                          @Money ,
                          @Money ,
                          @TaxCom ,
                          @Win ,
                          @Lost ,
                          @Mid ,
                          @Cut ,
                          @BankVer ,
                          0 ,
                          0 ,
                          @IsMoneyGame ,
                          @IsExperciseRoom ,
                          @PlayTimeCount ,
                          @PlayTimes_V ,
                          @PlayRounds_V
                        )
			if @IsTax=0 and @Type=0 --台费
			begin
				if 	@IsRobot=0
				begin
					if @Money > 0
						exec Web_pGameDoneRebate @UserID,@TaxCom,@NameID,@GameTypeID,1,1
					if @Money < 0
						exec Web_pGameDoneRebate @UserID,@TaxCom,@NameID,@GameTypeID,2,1
				end
			end
			
			if @IsTax=1 and @Type=0--税率
			begin
				if 	@IsRobot=0
				begin
					if @Money > 0
						exec Web_pGameDoneRebate @UserID,@TaxCom,@NameID,@GameTypeID,1,2
				end
			end
		-------------------------------------------------金币变化日志------------------------------------
			   DECLARE @UserName NVARCHAR(50)
               DECLARE @UserStartMoney BIGINT
               
                SET @IsRobot=0
                SELECT  @IsRobot = IsRobot
                FROM    TUsers WITH ( NOLOCK )
                WHERE   UserID = @UserID
                --.@Type  .0.金币结算 1-俱乐部积分结算  2-(练习房)积分结算(这个不会进来结算) .3.碎钻结算.
                IF ( ISNULL(@isRobot, 1) != 1 and @Type=0 )
                BEGIN
					SELECT  @UserName = UserName FROM  TUsers WHERE UserID = @UserID
                    SELECT  @UserStartMoney = ( WalletMoney + BankMoney )
                    FROM    TUserInfo WITH ( NOLOCK )
                    WHERE   UserID = @UserID
					if @Type=0
                    EXEC proc_AddWeb_MoneyChangeLog @UserID, @UserName, @UserStartMoney, @Money, 1, '游戏输赢'  ---金币变化日志
                END
                    
                IF ISNULL(@isRobot, 1) != 1 and @Type=3   --碎钻（奖券）变化日志
                begin
					SELECT  @UserName = UserName FROM  TUsers WHERE UserID = @UserID
					select @UserStartMoney=Lotteries from Web_Users where UserID=@UserID
					INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
					VALUES  (@UserID, @UserName ,@UserStartMoney ,@Money ,@UserStartMoney+@Money,GETDATE(),'游戏输赢',7)	
				end	
				
			    IF ISNULL(@isRobot, 1) != 1 and @Type=1   --积分（俱乐部上下分）变化日志
                begin
				
					SELECT  @UserName = UserName FROM  TUsers WHERE UserID = @UserID
					select @UserStartMoney=ClubPoints from TClubUserTable where UserID=@UserID
					insert into TClubPointChangeLog values(@ClubID,@MasterID,2,@UserID,@UserStartMoney,@Money,GETDATE(),'俱乐部游戏输赢')	
				end	
											
				
		-- 循环变量增加，且字符串位置增加
                SET @iUser = @iUser + 1
                SET @pos0 = @pos9 + 1
            END

     if @Type=3   --更改用户碎钻数量
          begin
				UPDATE  b
						SET   	b.Lotteries = b.Lotteries+a.Money
						FROM    #temp_TMatchTable a ,
								Web_Users b
						WHERE   a.UserID = b.UserID
				update Web_Users set Lotteries=0 where Lotteries<0					
          end  
	if @Type=0  --更改用户金币数量
		  begin
			UPDATE  b
						SET   	b.WalletMoney = b.WalletMoney+a.Money
						FROM    #temp_TMatchTable a ,
								TUserInfo b
						WHERE   a.UserID = b.UserID
			
		  end
	if @Type=1				----更改用户俱乐部积分数量
		begin
			UPDATE  b
						SET   	b.ClubPoints = b.ClubPoints+a.Money
						FROM    #temp_TMatchTable a ,
								TClubUserTable b
						WHERE   a.UserID = b.UserID and @ClubID=b.ClubID
		end
	--更新 是否金币场  @StartMoney,@UserBWalletMoney的两个参数
	/*@IsMoneyGame = 1,@BankVer=1 更新金币场，银行版本号为一个的参数*/
		if @Type=0
		begin
			UPDATE  a
			SET     a.StartMoney = b.WalletMoney + b.BankMoney ,
					a.UserBWalletMoney = b.WalletMoney
			FROM    #temp_TMatchTable a ,
					TUserInfo b WITH ( NOLOCK )
			WHERE   a.UserID = b.UserID
					AND a.IsMoneyGame = 1
					AND a.BankVer = 1
		/*@IsMoneyGame = 1,@BankVer=2 更新金币场，银行版本号为一个的参数*/	
			UPDATE  a
			SET     a.StartMoney = 0 ,
					a.UserBWalletMoney = b.WalletMoney
			FROM    #temp_TMatchTable a ,
					TBankWallet b WITH ( NOLOCK )
			WHERE   a.UserID = b.UserID
					AND b.GameID = @NameID
					AND a.IsMoneyGame = 1
					AND a.BankVer = 2

		--更新金币场，扣除金币小于零的玩家
			UPDATE  #temp_TMatchTable
			SET     IsUserMoney = UserBWalletMoney + [Money] ,
					[Money] = -UserBWalletMoney ,
					Money_tiyan = -UserBWalletMoney
			WHERE   IsMoneyGame = 1
					AND UserBWalletMoney + [Money] < 0
					and IsExperciseRoom <> 1

		--如果是体验房，@IsExperciseRoom = 1,更新money字段时相当于加零,所以将Money_tinyan=0
			--UPDATE  #temp_TMatchTable
			--SET     Money_tiyan = 0
			--WHERE   IsExperciseRoom = 1

			SET XACT_ABORT ON ;
			BEGIN TRAN
			BEGIN TRY
			--更新玩家身上的金币
				IF ( @BankVer = 1 ) and @IsExperciseRoom <> 1
					BEGIN
				/*========更新 金币场=1，银行版本号为1，玩家身上的金币小于0===============*/
						UPDATE  b --with(rowlock)
						SET     b.TaxCom = b.TaxCom + a.TaxCom ,
								b.WalletMoney = 0
						FROM    #temp_TMatchTable a ,
								TUserInfo b
						WHERE   a.UserID = b.UserID
								AND a.IsMoneyGame = 1
								AND a.IsUserMoney < 0
								AND a.BankVer = 1
				
				/*========更新 金币场=1，银行版本号为1，玩家身上的金币大于等于0===============*/		
						UPDATE  b --with(rowlock)
						SET     b.TaxCom = b.TaxCom + a.TaxCom ,
								b.WalletMoney = b.WalletMoney + a.Money
						FROM    #temp_TMatchTable a ,
								TUserInfo b
						WHERE   a.UserID = b.UserID
								AND a.IsMoneyGame = 1
								AND a.IsUserMoney >= 0
								AND a.BankVer = 1
					END
				ELSE 
						IF ( @BankVer = 2 ) and @IsExperciseRoom <> 1
						BEGIN
				/*========更新 金币场=1，银行版本号为2，玩家身上的金币小于0===============*/
							UPDATE  b --with(rowlock)
							SET     b.WalletMoney = 0
							FROM    #temp_TMatchTable a ,
									TBankWallet b
							WHERE   a.UserID = b.UserID
									AND b.GameID = @NameID
									AND a.IsMoneyGame = 1
									AND a.IsUserMoney < 0
									AND a.BankVer = 2
				
							UPDATE  b --with(rowlock)
							SET     b.TaxCom = b.TaxCom + a.TaxCom
							FROM    #temp_TMatchTable a ,
									TUserInfo b
							WHERE   a.UserID = b.UserID
									AND a.IsMoneyGame = 1
									AND a.IsUserMoney < 0
									AND a.BankVer = 2

				/*========更新 金币场=1，银行版本号为2，玩家身上的金币大于等于0===============*/
							UPDATE  b --with(rowlock)
							SET     b.WalletMoney = b.WalletMoney + a.Money
							FROM    #temp_TMatchTable a ,
									TBankWallet b
							WHERE   a.UserID = b.UserID
									AND b.GameID = @NameID
									AND a.IsMoneyGame = 1
									AND a.IsUserMoney >= 0
									AND a.BankVer = 2
				
							UPDATE  b --with(rowlock)
							SET     b.TaxCom = b.TaxCom + a.TaxCom
							FROM    #temp_TMatchTable a ,
									TUserInfo b
							WHERE   a.UserID = b.UserID
									AND a.IsMoneyGame = 1
									AND a.IsUserMoney >= 0
									AND a.BankVer = 2
						END
                            
                       
		--更新dbo.TMatchTable表
            DECLARE @SQL VARCHAR(MAX)
            SET @SQL = 'UPDATE b
				 SET b.OnLineTimeCount=b.OnLineTimeCount+a.OnlineTime
				,b.Point=b.Point+a.Point
				,b.Money=b.Money+a.Money_tiyan
				,b.TaxCom=b.TaxCom+a.TaxCom
				,b.Win=b.Win+a.Win
				,b.Lost=b.Lost+a.Lost
				,b.Mid=b.Mid+a.Mid
				,b.Cut=b.Cut+a.Cut
				,b.PlayTimeCount=b.PlayTimeCount+a.PlayTimeCount
				,b.PlayTimes_T=b.PlayTimes_T+a.PlayTimes_V
				,b.PlayCounts_T=b.PlayCounts_T+a.PlayRounds_V
				from #temp_TMatchTable a,' + @GameTable
                + ' b WHERE a.UserID=b.UserID
				AND NameID=' + LTRIM(STR(@NameID))
				--+'   AND RoomID='+STR(@RoomID) -- PengJiLin, 2011-4-14, 增加房间ID
		--print @SQL
            EXEC(@SQL)
            COMMIT TRAN
        END TRY

        BEGIN CATCH
            ROLLBACK  
        END CATCH
		end
        SELECT  @SendMoneyFlag AS SendMoneyFlag

        RETURN 0
    END
