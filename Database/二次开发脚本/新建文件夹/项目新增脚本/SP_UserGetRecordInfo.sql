
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-10
-- Description:	用户在游戏中获取战绩信息(麻将)
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserGetRecordInfo] 
	@UserID int,
	@TargetID int
AS
BEGIN
	SET NOCOUNT ON;
	declare @iCount int				--目标用户总局数
	declare @WinnerNum int          --胜利次数
	declare @Multiple  int          --最大倍数
	declare @IsFriend  int          --能否被关注（添加好友）
	
	select @iCount=COUNT(DISTINCT DeskPassword) from TDeskTotleRecord where DeskUserID=@TargetID
	select @WinnerNum=SUM(Winner) from TDeskTotleRecord where DeskUserID=@TargetID
	select @Multiple=isnull(Multiple,0) from TUserInfo where UserID=@TargetID
	
	select @IsFriend=isnull(UserFollowPower,1) from TUsers where UserID=@TargetID
	if @UserID=@TargetID
	set @IsFriend=0   --自己肯定是无法关注自己的
	if exists(select * from TUserFriendTable where UserID=@UserID and FriendID=@TargetID)
	set @IsFriend=0   --已经是好友 
	
	select @iCount as iCount,
		   @WinnerNum as WinnerNum,
		   @Multiple as Multiple,
		   @IsFriend as IsFriend
	return 0
END
GO
