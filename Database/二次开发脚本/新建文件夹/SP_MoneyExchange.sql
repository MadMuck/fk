
-- ================================================
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		LiMu
-- Create date: 2019-03-20
-- Description:	玩家使用金币兑换碎钻（碎钻复用奖券字段）
-- 关于比例配置问题  问过策划 不会出现小数的情况  例如（1个金币兑换1.5个碎钻 或者 1.5个金币兑换1个碎钻） 所以小数没做判断
-- =============================================
CREATE PROCEDURE [dbo].[SP_MoneyExchange]
(
	@UserID INT ,			-- 玩家ID
    @ExchangeNum INT		-- 需要兑换碎钻数量
)	
AS
BEGIN
	SET NOCOUNT ON;
	declare @Moneybegin bigint			--玩家兑换之前身上金币数量
	declare @CrushingDrillBegin bigint	--玩家兑换之前身上碎钻（奖券）数量
	declare @Money bigint				--玩家兑换之后身上金币数量
	declare @CrushingDrill bigint		--玩家兑换之后身上碎钻（奖券）数量
	
	DECLARE @UserName NVARCHAR(64)
	SELECT @UserName=UserName from dbo.TUsers WHERE UserID=@UserID
	
    select @Moneybegin=WalletMoney from TUserInfo where UserID=@UserID
    select @CrushingDrillBegin=Lotteries from Web_Users where UserID=@UserID
    declare @CrushingDrillForGold int    --碎钻（奖券）兑换金币
    declare @GoldForCrushingDrill int    --金币兑换碎钻（奖券）
    set @CrushingDrillForGold=0
    set @GoldForCrushingDrill=0
    
    select @CrushingDrillForGold=CrushingDrillForGold,@GoldForCrushingDrill=GoldForCrushingDrill
    from Web_Config
    if @CrushingDrillForGold<1 or @GoldForCrushingDrill<1
    begin
		return 1         --兑换比例配置错误
	end	
	
	
	declare @ConsumptionOfGold bigint   --兑换碎钻需要的消耗的金币数量
	
    if @CrushingDrillForGold=1 --表示多个金币（或者1换1）兑换1个碎钻（奖券）（和网站肖磊商议）
    begin
		
		set @ConsumptionOfGold=@GoldForCrushingDrill*@ExchangeNum --兑换碎钻（奖券）需要的消耗的金币数量
		if @ConsumptionOfGold>@Moneybegin
		begin
			return 2     --玩家身上金币不足
		end
		--兑换   更改玩家身上的金币数量和碎钻数量
		update TUserInfo set WalletMoney=WalletMoney-@ConsumptionOfGold where UserID=@UserID
		update Web_Users set Lotteries=Lotteries+@ExchangeNum where UserID=@UserID
		select @Money=WalletMoney from TUserInfo where UserID=@UserID
		select @CrushingDrill=Lotteries from Web_Users where UserID=@UserID
		
		---插入变化日志表
		INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
			VALUES  (@UserID, @UserName ,@CrushingDrillBegin ,@ExchangeNum ,@CrushingDrill,GETDATE(),'玩家金币兑换碎钻',6)
		
		select @Money as Money,@CrushingDrill as CrushingDrill
		return 0         --成功
    end
    else    --就是1个金币兑换多个碎钻的情况
    begin
		if(cast(@ExchangeNum as float)/@CrushingDrillForGold)<>(cast(@ExchangeNum/@CrushingDrillForGold as int))--需要兑换碎钻的数量不是后台配置兑换碎钻数量的整数倍
		return 1         --兑换比例配置错误																		--  例如我要换42个  配置是1个金币换10个碎钻的情况
																													
	    set @ConsumptionOfGold=cast(@ExchangeNum/@CrushingDrillForGold as bigint)--兑换碎钻需要的消耗的金币数量
		if @ConsumptionOfGold>@Moneybegin
		begin
			return 2     --玩家身上金币不足
		end
		--兑换   更改玩家身上的金币数量和碎钻数量
		update TUserInfo set WalletMoney=WalletMoney-@ConsumptionOfGold where UserID=@UserID
		update Web_Users set Lotteries=Lotteries+@ExchangeNum where UserID=@UserID
		select @Money=WalletMoney from TUserInfo where UserID=@UserID
		select @CrushingDrill=Lotteries from Web_Users where UserID=@UserID
		
		---插入变化日志表
		INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
			VALUES  (@UserID, @UserName ,@CrushingDrillBegin ,@ExchangeNum ,@CrushingDrill,GETDATE(),'玩家金币兑换碎钻',6)
		
		select @Money as Money,@CrushingDrill as CrushingDrill
		return 0 
    end
END
GO
