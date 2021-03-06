USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UsePropBuyAndUseBackpack]    Script Date: 06/26/2019 18:35:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-06-13
-- Description:	玩家背包赠送道具
-- =============================================
ALTER PROCEDURE [dbo].[SP_UsePropBuyAndUseBackpack] 
	@UserID int, 
	@TargetID int,
	@PropID int,
	@Number int
AS
BEGIN
	SET NOCOUNT ON;
	declare @UserBeibao int  ---玩家背包道具数量
	set @UserBeibao=0
	declare @ChangeFascination int  --道具可改变的魅力值
	select @UserBeibao=isnull(PropNum,0) from Web_UserBackpack where @UserID=UserID and @PropID=PropID
	select @ChangeFascination=Fascination from Web_PropConfig where @PropID=id
	
	 declare @StartFascination int  --改变之前的魅力值
     declare @UserName nvarchar(50)
	
	
	
	if @UserBeibao<@Number and @ChangeFascination>0
		return 2  --玩家背包道具数量不足
	if not exists(select * from TUserInfo where UserID=@TargetID)
		return 1  --使用对象不存在
	 if @ChangeFascination>0   --改变的魅力值 大于0  就代表是飞机或者跑车
    begin
		if exists(select * from Web_UserBackpack where @TargetID=UserID and @PropID=PropID)
		update Web_UserBackpack set GiftNum=GiftNum+@Number where @TargetID=UserID and @PropID=PropID
		else
		insert into Web_UserBackpack(UserID,PropID,PropNum,GiftNum,PropGetTime)values(@TargetID,@PropID,0,@Number,GETDATE())
	end
	select @StartFascination=Fascination from TUserInfo where UserID=@TargetID  --目标用户之前的魅力值
    select @UserName = UserName from TUsers where UserID=@TargetID              --目标用户的昵称
	
	
	 update TUserInfo set Fascination=Fascination+@ChangeFascination*@Number where UserID=@TargetID   --改变目标用户魅力值
	 update Web_UserBackpack set PropNum=PropNum-@Number where @UserID=UserID and @PropID=PropID  --改变赠送用户背包的道具数量
	 delete Web_UserBackpack where GiftNum=0 and PropNum=0
	 
	 INSERT INTO dbo.Web_FascinationChangeLog(SendUserID,ReceiveUserID,UserName ,StartFascination ,ChangeFascination ,ChangeType,DateTime,Remark,PropID,PropNum)
		VALUES  (@UserID,@TargetID, @UserName ,@StartFascination ,@ChangeFascination ,1,GETDATE(),'道具赠送',@PropID,@Number)
		
	declare @Surplus int --赠送人赠送道具后背包剩余的道具数量
	set @Surplus=0
	select @Surplus=isnull(PropNum,0)+ISNULL(GiftNum,0) from Web_UserBackpack where @UserID=UserID and @PropID=PropID
	select @Surplus as Surplus
	return 0
   
END
