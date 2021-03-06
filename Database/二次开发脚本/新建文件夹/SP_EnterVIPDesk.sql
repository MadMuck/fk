USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_EnterVIPDesk]    Script Date: 05/21/2019 17:59:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[SP_EnterVIPDesk]
@UserID int,--用户ID
@InputPassWord varchar(20)--输入密码
as
BEGIN 
	set nocount on
	DECLARE @RoomID INT 
	DECLARE @TableNumber INT 
	DECLARE @GameID INT 
	DECLARE @PayType INT 
	DECLARE @Jewels INT 
	DECLARE @IsPaly INT
	DECLARE @PositionLimit INT 
	DECLARE @ClubID INT 
	DECLARE @DeskPassWord varchar(20)
	declare @GroupID int --楼层ID
	if not exists(select * from TUsers where UserID=@UserID)
	BEGIN 
		return 2	--用户不存在
	END 
	
	IF EXISTS (SELECT * FROM TGameCutNetRecord WHERE UserID=@UserID AND DeskPassword<>@InputPassWord) 
	BEGIN 
		SELECT @DeskPassWord=DeskPassword FROM TGameCutNetRecord WHERE UserID=@UserID
		select @RoomID=RoomID,@GameID=GameID from TBuyGameDeskRecord where szDeskPassWord=@DeskPassWord
		select @DeskPassWord as szPassWord,@GameID as NameID
		return 6
	END 
	--IF EXISTS (SELECT * FROM TBuyGameDeskRecord WHERE MasterID=@UserID AND MasterState=0 AND szDeskPassWord<>@InputPassWord) 
	--BEGIN 
	--	select @RoomID=RoomID,@DeskPassWord=szDeskPassWord,@GameID=GameID from TBuyGameDeskRecord where MasterID=@UserID AND MasterState=0
	--	select @DeskPassWord as szPassWord,@GameID as NameID
	--	return 6
	--END 
	
	if exists(select * from TBuyGameDeskRecord where szDeskPassWord=@InputPassWord)--存在对应的桌子
	BEGIN 
		select @RoomID = RoomID,@TableNumber = TableNumber,@GameID=GameID,@PayType=PayType,@Jewels=Jewels,@IsPaly=IsPlay,@PositionLimit=PositionLimit,@ClubID=ClubID,
			@GroupID=GroupID from TBuyGameDeskRecord where szDeskPassWord=@InputPassWord
		IF @PayType=2 AND @IsPaly=0
		BEGIN 
			IF @Jewels>(SELECT Jewels FROM dbo.Web_Users WHERE UserID=@UserID)
				RETURN 5
		END 
		
		IF @ClubID<>0 AND NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
			RETURN 7	--俱乐部房间但玩家不是俱乐部成员
		if @ClubID<>0
		begin
			--------------
			declare @ExclusionID int  --排斥ID
			--declare @szDeskPassWord nvarchar(20)  --桌子号
			--select @szDeskPassWord=szDeskPassWord from TBuyGameDeskRecord where ClubID=@ClubID and GroupID=@GroupID and DeskXuHao=@DeskXuHao
			set @ExclusionID=0
			select @ExclusionID=isnull(GreenUserID,0)  from TClubUserExclusion where @UserID=RedUserID
			if @ExclusionID<>0   --有排斥ID  有桌子号 去TGameCutNetRecord表里查看一下  这个排斥ID是否在这个桌子
			begin
			   if exists(select * from TGameCutNetRecord where @InputPassWord=DeskPassword and @ExclusionID=UserID)
			   return 11   --存在排斥ID  不让坐桌
			end
			
			set @ExclusionID=0
			select @ExclusionID=isnull(RedUserID,0)  from TClubUserExclusion where @UserID=GreenUserID
			if @ExclusionID<>0   --有排斥ID  有桌子号 去TGameCutNetRecord表里查看一下  这个排斥ID是否在这个桌子
			begin
			   if exists(select * from TGameCutNetRecord where @InputPassWord=DeskPassword and @ExclusionID=UserID)
			   return 11   --存在排斥ID  不让坐桌
			end
				declare @Limiter int 
			select @Limiter=ClubIsOpen from TClubTable where ClubID=@ClubID
			if @Limiter=0
				return 12   ---俱乐部功能已经关闭  无法坐桌
			--------------------
			declare @Lotteries bigint  --玩家身上的碎钻数量
			declare @LowestLotteries int  --打完抽水碎钻数量  
			select @Lotteries=isnull(Lotteries,0) from Web_Users where UserID=@UserID
			select @LowestLotteries=isnull(UserScoreDisable,0) from Web_Config 
			if @Lotteries<@LowestLotteries
			return 10  --玩家身上碎钻数量不足
			
			--根据俱乐部ID和楼层ID  查找该楼层的结算方式
			declare @CurrencyType int  --结算方式  0--金币  1--积分
			select @CurrencyType=CurrencyType from TClubPlayInfo where @ClubID=ClubID and @GroupID=GroupID
			if @CurrencyType=0  --金币结算  判断用户金币数量是否够 
			begin
				declare @Money bigint  --用户金币
				declare @LowestMoney bigint --最低限入
				select @Money=WalletMoney from TUserInfo where UserID=@UserID
				select @LowestMoney=LessPoint from TGameRoomBase where @RoomID=RoomID
				if @Money<@LowestMoney
				return 8   --金币不够 不能进入俱乐部房间
			end
			if @CurrencyType=1 --积分结算
			begin
				declare @Point bigint
				select @Point=ClubPoints from TClubUserTable where @ClubID=ClubID and @UserID=UserID
				if @Point<=0
				return 9   --积分不够  不能进入俱乐部房间  
			end
	
		end
		IF @RoomID<>255 AND @TableNumber<>255 --AND EXISTS (SELECT * FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID AND UpdateTime >= DATEADD(MI, - 3, GETDATE()) AND IsConnected=1)
		BEGIN 
			SELECT @RoomID AS RoomID,@TableNumber AS TableNumber,@PositionLimit AS PositionLimit
			return 1	-- 已有对应的桌子资源
		END 
		ELSE  --重新分配桌子资源
		BEGIN 
			SELECT @RoomID=RoomID ,@TableNumber=DeskID FROM HNF_DistributeTable(@GameID)
			IF @RoomID<>255 AND @TableNumber<>255
			BEGIN 
				UPDATE TBuyGameDeskRecord SET RoomID=@RoomID ,TableNumber=@TableNumber WHERE szDeskPassWord=@InputPassWord
				DECLARE @UserStation INT 
				SELECT @UserStation=UserStation FROM TGameCutNetRecord WHERE DeskPassword=@InputPassWord
				SELECT RoomID,TableNumber,RunCount,@UserStation AS UserStation,@PositionLimit AS PositionLimit from TBuyGameDeskRecord where szDeskPassWord=@InputPassWord
				RETURN 3
			END 
			ELSE 
			BEGIN 
				RETURN 4  --没有空闲桌子
			END 	
		END 
	END
	RETURN 0
END 