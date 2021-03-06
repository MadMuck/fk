USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UsePropBuyAndUse]    Script Date: 06/13/2019 16:26:33 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-09
-- Description:	用户在游戏中使用道具  根据道具编号改变目标用户的魅力值
-- =============================================

ALTER PROCEDURE [dbo].[SP_UsePropBuyAndUse] 
	@UserID int, 
	@TargetID int,
	@PropID int,
	@SpendType int,
	@RoomID int,
	@DeskNo int	
AS
BEGIN
	SET NOCOUNT ON;

	declare @Tmp bigint  --道具价格
	set @Tmp=0
		declare @Money bigint  --身上的金币
		
	declare @UserBeibao int  ---玩家背包道具数量
	set @UserBeibao=0
	declare @ChangeFascination int  --道具可改变的魅力值
	select @UserBeibao=isnull(PropNum,0) from Web_UserBackpack where @UserID=UserID and @PropID=PropID
	select @ChangeFascination=Fascination from Web_PropConfig where @PropID=id
	
	 declare @StartFascination int  --改变之前的魅力值
     declare @UserName nvarchar(50)
	
	
	
	if @UserBeibao<1 and @ChangeFascination>0
		return 2  --玩家背包道具数量不足
	if not exists(select * from TUserInfo where UserID=@TargetID)
		return 1  --使用对象不存在

    if @ChangeFascination>0   --改变的魅力值 大于0  就代表是飞机或者跑车
    begin
		if exists(select * from Web_UserBackpack where @TargetID=UserID and @PropID=PropID)
		update Web_UserBackpack set GiftNum=GiftNum+1 where @TargetID=UserID and @PropID=PropID
		else
		insert into Web_UserBackpack(UserID,PropID,PropNum,GiftNum,PropGetTime)values(@TargetID,@PropID,0,1,GETDATE())
	end
	if @ChangeFascination<=0
		begin
		
		select @Tmp=isnull(GoldNum,0) from Web_PropConfig where @PropID=id
		select @Money=WalletMoney from TUserInfo where UserID=@UserID
		if @Money<@Tmp
		return 3  --金币不足
		update TUserInfo set WalletMoney=WalletMoney-@Tmp where @UserID=UserID  --改变用户的金币数量
		select @UserName = UserName from TUsers where UserID=@UserID
		--添加金币变化日志
		--insert into Web_MoneyChangeLog values(@UserID,@UserName,@Money,@Tmp,29,getdate(),'使用道具')
		 INSERT  INTO dbo.Web_MoneyChangeLog
                                    ( UserID ,
                                      UserName ,
                                      StartMoney ,
                                      ChangeMoney ,
                                      ChangeType ,
                                      DateTime ,
                                      Remark
                                    )
                            VALUES  ( @UserID ,
                                      @UserName ,
                                      @Money ,
									  -@Tmp,
                                      29 ,
                                      GETDATE() ,
                                      '使用道具'
                                    )
                                  
	end
	select @StartFascination=Fascination from TUserInfo where UserID=@TargetID
    select @UserName = UserName from TUsers where UserID=@TargetID
    update TUserInfo set Fascination=Fascination+@ChangeFascination where UserID=@TargetID   --改变目标用户魅力值
    update Web_UserBackpack set PropNum=PropNum-1 where @UserID=UserID and @PropID=PropID
    delete Web_UserBackpack where GiftNum=0 and PropNum=0
    	---魅力变化日志
	INSERT INTO dbo.Web_FascinationChangeLog(SendUserID,ReceiveUserID,UserName ,StartFascination ,ChangeFascination ,ChangeType,DateTime,Remark,PropID,PropNum)
		VALUES  (@UserID,@TargetID, @UserName ,@StartFascination ,@ChangeFascination ,1,GETDATE(),'道具赠送',@PropID,1)
		select @Tmp as Money
	return 0
	
END
