USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubRoomList]    Script Date: 05/15/2019 17:22:39 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[SP_ClubRoomList]
@UserID	INT ,
@ClubID INT ,
@GroupID int --楼层ID
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 3		--玩家未加入该工会
		
	if not exists (select * from TClubPlayInfo where @ClubID=ClubID and @GroupID=GroupID)
		return 4         --请求进入的楼层不存在
		
	--declare @CurrencyType int  --结算方式  0--金币  1--积分
	--declare @GameID int 
	--declare @RoomID int
	--select @GameID=GameID from TClubPlayInfo where @ClubID=ClubID and @GroupID=GroupID
	--select @RoomID=RoomID from TGameRoomInfo where @GameID=GameNameID
	--select @CurrencyType=CurrencyType from TClubPlayInfo where @ClubID=ClubID and @GroupID=GroupID
	--		if @CurrencyType=0  --金币结算  判断用户金币数量是否够 
	--		begin
	--			declare @Money bigint  --用户金币
	--			declare @LowestMoney bigint --最低限入
	--			select @Money=WalletMoney from TUserInfo where UserID=@UserID
	--			select @LowestMoney=LessPoint from TGameRoomBase where @RoomID=RoomID
	--			if @Money<=@LowestMoney
	--			return 5   --金币不够 不能进入俱乐部房间
	--		end
	--		if @CurrencyType=1 --积分结算
	--		begin
	--			declare @Point bigint
	--			select @Point=ClubPoints from TClubUserTable where @ClubID=ClubID and @UserID=UserID
	--			if @Point<=0
	--			return 6   --积分不够  不能进入俱乐部房间  
	--		end	
	--declare @Lotteries bigint  --玩家身上的碎钻数量
	--declare @LowestLotteries int  --打完抽水碎钻数量  
	--select @Lotteries=Lotteries from Web_Users where UserID=@UserID
	--select @LowestLotteries=CrushingDrillScale from Web_Config 	
	--if @Lotteries<@LowestLotteries
	--		return 7  --玩家身上碎钻数量不足	
	DECLARE @Temp TABLE(IsPlay int,BuyGameCount INT,ClubPayType INT,szDeskPassWord NVARCHAR(20),GameName NVARCHAR(30),GameID INT ,AllowEnter int,
	FinishCondition int,LeaveSeconds int ,BuyMinutes int, DeskXuHao int,DeskConfig nvarchar(512),RunCount int)

	INSERT INTO @Temp(IsPlay,BuyGameCount,ClubPayType,szDeskPassWord,GameID,AllowEnter,FinishCondition,LeaveSeconds,BuyMinutes,DeskXuHao,DeskConfig,RunCount) 
	SELECT IsPlay,BuyGameCount,ClubPayType,szDeskPassWord,GameID,MidEnter,FinishCondition,(BuyMinutes * 60 - datediff(s,BeginTime,GETDATE())),
	BuyMinutes, DeskXuHao ,DeskConfig,RunCount
	FROM dbo.TBuyGameDeskRecord 
	WHERE ClubID=@ClubID AND @GroupID=GroupID order by DeskXuHao asc
	

	UPDATE a SET a.GameName=TGameNameInfo.ComName FROM @Temp a, dbo.TGameNameInfo WHERE dbo.TGameNameInfo.NameID=a.GameID
	
	SELECT * FROM @Temp
	
	RETURN 0
END