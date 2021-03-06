

/****** Object:  StoredProcedure [dbo].[SP_CreateClub]    Script Date: 03/26/2019 16:18:58 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_CreateClub]
@UserID	INT ,							--用户ID
--@ClubName NVARCHAR(40),					--俱乐部昵称
@GameID   int,							--游戏ID
@BuyCount int,							--购买局数
@FinishCondition int,					-- 结束条件，0局数，1胡息，2圈数, 3-时效
@PayType	int,						--0开局之后扣除房费，1房主开房时付费，2AA制扣除房费
@PositionLimit	int,					--0不开启定位，1开启定位
@PlayerNum	int,						--游戏限制人数,0与游戏人数相同
@MidEnter	int,						--0中途不可以进入，1中途可进入
@BuyMinutes	int,						--购买时长
@DeskConfig	varbinary(max),				--具体游戏的玩法
@PreventCheating int,				--0不开启防作弊检查  1开启防作弊检查
@CurrencyType	int						--结算方式   0--金币结算    1---分数结算
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @ClubID INT 
	DECLARE @ClubLimit INT 
	DECLARE @ClubJoinLimit INT 
	DECLARE @CanUserCreate TINYINT
	
	SELECT TOP 1 @ClubLimit=ClubLimit,@ClubJoinLimit=ClubJoinLimit,@CanUserCreate=CanUserCreate FROM dbo.Web_Config
	
	IF @CanUserCreate=0 AND NOT EXISTS (SELECT * FROM dbo.Web_Agency WHERE AgencyID=@UserID)
		RETURN 3		--用户权限不足
	
	IF @ClubLimit<>0 AND (SELECT COUNT(*) FROM TClubTable WHERE MasterID=@UserID) >= @ClubLimit
		RETURN 4		--超过创建限制
		
	IF @ClubJoinLimit<>0 AND (SELECT COUNT(*) FROM TClubUserTable WHERE UserID=@UserID) >= @ClubJoinLimit
		RETURN 5		--超过加入俱乐部限制
		
	--IF EXISTS (SELECT * FROM TClubTable WHERE ClubName=@ClubName)
	--	RETURN 6		--俱乐部名称重复
	
	WHILE 1=1
	BEGIN 
		set @ClubID=RIGHT(100000000 + CONVERT(bigint, ABS(CHECKSUM(NEWID()))), 6)
		IF @ClubID<100000
			SET @ClubID=@ClubID+100000
		IF NOT EXISTS(SELECT * FROM TClubTable WHERE ClubID=@ClubID)
			BREAK
	END 
	
	declare @GroupName nvarchar(64)
	select @GroupName=ComName from TGameNameInfo where @GameID=NameID 
	
	INSERT INTO TClubTable(ClubID ,ClubName ,MasterID) VALUES(@ClubID,@GroupName,@UserID)
	INSERT INTO dbo.TClubUserTable(ClubID, UserID) VALUES(@ClubID,@UserID)
	
	
	insert into TClubPlayInfo(GroupID,GroupName,ClubID,GameID,PlayCount,bFinishCondition,bPayType,bPostionLimit,bPlayNumber,bMidEnter,bMinutes,
	DeskConfig,PreventCheating,CurrencyType) 
	values(1,@GroupName,@ClubID,@GameID,@BuyCount,@FinishCondition,@PayType,@PositionLimit,@PlayerNum,@MidEnter,@BuyMinutes,@DeskConfig,
	@PreventCheating,@CurrencyType)
	
	SELECT @GroupName AS ClubName,@ClubID AS ClubID
	
	RETURN 0
END