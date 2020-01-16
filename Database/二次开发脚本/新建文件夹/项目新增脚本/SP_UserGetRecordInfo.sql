
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-10
-- Description:	�û�����Ϸ�л�ȡս����Ϣ(�齫)
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserGetRecordInfo] 
	@UserID int,
	@TargetID int
AS
BEGIN
	SET NOCOUNT ON;
	declare @iCount int				--Ŀ���û��ܾ���
	declare @WinnerNum int          --ʤ������
	declare @Multiple  int          --�����
	declare @IsFriend  int          --�ܷ񱻹�ע����Ӻ��ѣ�
	
	select @iCount=COUNT(DISTINCT DeskPassword) from TDeskTotleRecord where DeskUserID=@TargetID
	select @WinnerNum=SUM(Winner) from TDeskTotleRecord where DeskUserID=@TargetID
	select @Multiple=isnull(Multiple,0) from TUserInfo where UserID=@TargetID
	
	select @IsFriend=isnull(UserFollowPower,1) from TUsers where UserID=@TargetID
	if @UserID=@TargetID
	set @IsFriend=0   --�Լ��϶����޷���ע�Լ���
	if exists(select * from TUserFriendTable where UserID=@UserID and FriendID=@TargetID)
	set @IsFriend=0   --�Ѿ��Ǻ��� 
	
	select @iCount as iCount,
		   @WinnerNum as WinnerNum,
		   @Multiple as Multiple,
		   @IsFriend as IsFriend
	return 0
END
GO
