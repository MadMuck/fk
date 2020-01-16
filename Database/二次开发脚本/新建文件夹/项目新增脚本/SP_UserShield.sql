
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-07
-- Description:	用户屏蔽自己 让其他玩家无法添加自己为好友
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserShield] 
@UserID INT,					--用户ID
@WhetherShielding int           --操作类型   0---屏蔽自己  1---取消屏蔽
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where @UserID=UserID)
	return 1   ---UserID不正确
	
	update TUsers set UserFollowPower=@WhetherShielding where UserID=@UserID
	return 0
END
GO
