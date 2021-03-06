USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UserEnterClubDesk]    Script Date: 06/25/2019 15:57:48 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		LiMu
-- Create date: 2019-04-02
-- Description:	玩家点击桌子  如果这个桌子已经存在 返回给玩家桌号 不存在 就读取配置 创建桌子
-- =============================================
ALTER PROCEDURE [dbo].[SP_UserEnterClubDesk]
@UserID	INT ,
@ClubID INT ,
@GroupID int, --楼层ID
@DeskXuHao int
AS
BEGIN
	SET NOCOUNT ON;
	DECLARE @ClubMaster INT   --俱乐部房主ID
	set @ClubMaster=0
	SELECT @ClubMaster=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	declare @GameID int 
	declare @RoomID int
	select @GameID=GameID from TClubPlayInfo where @ClubID=ClubID and @GroupID=GroupID
	select @RoomID=RoomID from TGameRoomInfo where @GameID=GameNameID
	if not exists (select * from TClubUserTable where @UserID=UserID and @ClubID=ClubID)
		return 1   --不是这个俱乐部的成员
	if not exists (select * from TClubPlayInfo where @GroupID=GroupID and @ClubID=ClubID) or @ClubMaster=0
		return 2   --没有这个楼层玩法
	declare @Limiter int 
	select @Limiter=ClubIsOpen from TClubTable where ClubID=@ClubID
	if @Limiter=0
		return 5   ---俱乐部功能已经关闭  无法坐桌
	declare @UserPoin bigint
	set @UserPoin=0
	declare @Type int 
	select @Type=CurrencyType from TClubPlayInfo where @GroupID=GroupID and @ClubID=ClubID
	if @Type=0  --金币
	begin
		select @UserPoin=WalletMoney from TUserInfo where @UserID=UserID
		declare @LowestMoney bigint --最低限入
		select @LowestMoney=LessPoint from TGameRoomBase where @RoomID=RoomID
		if @UserPoin<=@LowestMoney
		return 9  --用户金币不够
	end
	
	if @Type=1  --俱乐部积分
	begin
		select @UserPoin=ClubPoints from TClubUserTable where @UserID=UserID and @ClubID=ClubID
		if @UserPoin<=0
		return 10  --用户俱乐部积分不够
	end
	declare @Lotteries bigint  --玩家身上的碎钻数量
	declare @LowestLotteries int  --打完抽水碎钻数量  
	select @Lotteries=isnull(Lotteries,0) from Web_Users where UserID=@UserID
	
	select @LowestLotteries=isnull(UserScoreDisable,0) from Web_Config 	
	if @Lotteries<@LowestLotteries
			return 11  --玩家身上碎钻数量不足
			
	if exists (select * from TBuyGameDeskRecord where ClubID=@ClubID and GroupID=@GroupID and DeskXuHao=@DeskXuHao)--有这个桌子
	begin
		declare @ExclusionID int  --排斥ID
		declare @szDeskPassWord nvarchar(20)  --桌子号
		select @szDeskPassWord=szDeskPassWord from TBuyGameDeskRecord where ClubID=@ClubID and GroupID=@GroupID and DeskXuHao=@DeskXuHao
		set @ExclusionID=0
		select @ExclusionID=isnull(GreenUserID,0)  from TClubUserExclusion where @UserID=RedUserID
		if @ExclusionID<>0   --有排斥ID  有桌子号 去TGameCutNetRecord表里查看一下  这个排斥ID是否在这个桌子
		begin
		   if exists(select * from TGameCutNetRecord where @szDeskPassWord=DeskPassword and @ExclusionID=UserID)
		   return 4   --存在排斥ID  不让坐桌
		end
		
		set @ExclusionID=0
		select @ExclusionID=isnull(RedUserID,0)  from TClubUserExclusion where @UserID=GreenUserID
		if @ExclusionID<>0   --有排斥ID  有桌子号 去TGameCutNetRecord表里查看一下  这个排斥ID是否在这个桌子
		begin
		   if exists(select * from TGameCutNetRecord where @szDeskPassWord=DeskPassword and @ExclusionID=UserID)
		   return 4   --存在排斥ID  不让坐桌
		end
		
		
		select szDeskPassWord from TBuyGameDeskRecord where ClubID=@ClubID and GroupID=@GroupID and DeskXuHao=@DeskXuHao
		return 3
	end
	else       --没有桌子  拿到配置  去创建
	begin
		
		--declare @GameID int
		declare @PlayCount int
		declare @bFinishCondition int
		declare @bPayType int
		declare @bPostionLimit int
		declare @bPlayNumber int
		declare @bMidEnter int
		declare @bMinutes int
		declare @PreventCheating int
		declare @CurrencyType int
		declare @DeskConfig varbinary(max)
		declare @GroupName nvarchar(64)
		
		select @GameID=GameID,@PlayCount=PlayCount,@bFinishCondition=bFinishCondition,@bPayType=bPayType,@bPostionLimit=bPostionLimit,
		@bPlayNumber=bPlayNumber,@bMidEnter=bMidEnter,@bMinutes=bMinutes,@DeskConfig=DeskConfig,@PreventCheating=PreventCheating,
		@CurrencyType=CurrencyType from TClubPlayInfo where ClubID=@ClubID and GroupID=@GroupID
		
		declare @Jewels int--获取消耗钻石
		SET @Jewels=-1
		select @Jewels=isnull(NeedJewels,-1) from TVIPDeskToCount where BuyGameCount=@PlayCount AND GameID=@GameID
		if @Jewels<0
		begin
			return 6 --局数错误
		END
		
		declare @TempJewels int
		select @TempJewels=Jewels from Web_Users where UserID=@ClubMaster
		if (@TempJewels-@Jewels)<0
		begin
			return 7--俱乐部房主钻石不足  无法创建房间 坐桌失败
		end
		DECLARE @GameName NVARCHAR(30)
		SELECT @GameName=ComName FROM dbo.TGameNameInfo WHERE NameID=@GameID
	--	DECLARE @RoomID INT
		DECLARE @DeskID INT 
		DECLARE @DeskPassWord varchar(20)
		declare @MasterState int 
		set @MasterState=0   --房主状态：0在房间，1离开状态
		if @UserID<>@ClubMaster
		set @MasterState=1
		
		SET @RoomID=0
		set @DeskID=0
		SET @DeskPassWord=''
		SELECT @RoomID=RoomID ,@DeskID=DeskID FROM HNF_DistributeTable(@GameID)
		IF @RoomID<>255 AND @DeskID<>255
		BEGIN 
			--动态生成密码
			WHILE 1=1
			BEGIN 
				set @DeskPassWord=RIGHT(100000000 + CONVERT(bigint, ABS(CHECKSUM(NEWID()))), 6)
				IF NOT EXISTS(SELECT * FROM TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPassWord)
					AND NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecordDelete WHERE DeskPassword=@DeskPassWord)
					BREAK
			END 
			--根据楼层ID和俱乐部ID 在楼层表中找到该俱乐部楼层的结算方式  插入TBuyGameDeskRecord表
			INSERT INTO TBuyGameDeskRecord(RoomID,TableNumber,MasterID,BuyTime,BuyGameCount,szDeskPassWord,GameID,
			DeskConfig,PayType,Jewels,FinishCondition,PositionLimit,PlayerNum,MasterState,MidEnter,ClubPayType,ClubID,GroupID,DeskXuHao,CurrencyTypeint) 
			values(@RoomID,@DeskID,@UserID,getdate(),@PlayCount,@DeskPassWord,@GameID,
			@DeskConfig,0,@Jewels,@bFinishCondition,@bPostionLimit,@bPlayNumber,@MasterState,@bMidEnter,0,@ClubID,@GroupID,@DeskXuHao,@CurrencyType)
			
			UPDATE Web_Users SET Jewels=Jewels-@Jewels WHERE UserID=@ClubMaster
			UPDATE dbo.Web_Users SET LockJewels=LockJewels+@Jewels WHERE UserID=@ClubMaster
			
			INSERT INTO TClubBuyDeskRecord(ClubID ,UserID ,BuyTime ,CostJewels ,DeskPass ,IsCost) VALUES(@ClubID,@UserID,getdate(),@Jewels,@DeskPassWord,0)
			
			
			SELECT @DeskPassWord AS szPassWord,@RoomID AS iRoomID,@DeskID AS iDeskID,@Jewels AS Jewels,@GameName AS GameName,
				   @bPayType as bPayType,@PlayCount as PlayCount,@GameID as GameID,@CurrencyType as CurrencyType
					
			return 0
		END 
		ELSE
		BEGIN --桌子已购买完
			return 8
		END 	
	end
END
