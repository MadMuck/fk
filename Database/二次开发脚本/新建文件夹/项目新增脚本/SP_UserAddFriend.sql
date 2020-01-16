GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-17
-- Description:	玩家添加好友
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserAddFriend]
	@UserID		INT,			--用户ID
	@TargetUserID	INT	,	--目标用户ID
	@Type int				--0.添加好友   1.删除好友
AS
BEGIN
	SET NOCOUNT ON;
	if @Type=0     --0.添加好友
	begin
		if exists(select * from TUserFriendTable where UserID=@UserID and FriendID=@TargetUserID)
		return 1  --目标用户已经是该玩家好友 无法再次添加
		
		declare @MaxFriendCount int --最大好友数量 （现在默认为100名 后期再说）
		set @MaxFriendCount=100;
		
		if @MaxFriendCount<=(select COUNT(*) from TUserFriendTable where UserID=@TargetUserID)
		return 2    --目标用户 好友数量已经达到上限
		
		if @MaxFriendCount<=(select COUNT(*) from TUserFriendTable where UserID=@UserID)
		return 3    --用户 好友数量已经达到上限
		declare @Shield int ---0-目标用户屏蔽自己了   1----没有
		select @Shield=UserFollowPower from TUsers where @TargetUserID=UserID
		if @Shield=0
		return 6    --目标用户屏蔽自己   无法添加好友
		
		insert into TUserFriendTable(UserID,FriendID)
		values(@UserID,@TargetUserID)
		
		return 0;
	end
	else if @Type=1  --1.删除好友
	begin
		if not exists(select * from TUserFriendTable where UserID=@UserID and FriendID=@TargetUserID)
		return 4  --目标用户不是该玩家好友 无法删除
		delete from TUserFriendTable where UserID=@UserID and FriendID=@TargetUserID
		return 0
	end
	else
	begin
		return 5  --未知错误
	end
END
