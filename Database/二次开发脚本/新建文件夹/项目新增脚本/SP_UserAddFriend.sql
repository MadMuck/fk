GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-17
-- Description:	�����Ӻ���
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserAddFriend]
	@UserID		INT,			--�û�ID
	@TargetUserID	INT	,	--Ŀ���û�ID
	@Type int				--0.��Ӻ���   1.ɾ������
AS
BEGIN
	SET NOCOUNT ON;
	if @Type=0     --0.��Ӻ���
	begin
		if exists(select * from TUserFriendTable where UserID=@UserID and FriendID=@TargetUserID)
		return 1  --Ŀ���û��Ѿ��Ǹ���Һ��� �޷��ٴ����
		
		declare @MaxFriendCount int --���������� ������Ĭ��Ϊ100�� ������˵��
		set @MaxFriendCount=100;
		
		if @MaxFriendCount<=(select COUNT(*) from TUserFriendTable where UserID=@TargetUserID)
		return 2    --Ŀ���û� ���������Ѿ��ﵽ����
		
		if @MaxFriendCount<=(select COUNT(*) from TUserFriendTable where UserID=@UserID)
		return 3    --�û� ���������Ѿ��ﵽ����
		declare @Shield int ---0-Ŀ���û������Լ���   1----û��
		select @Shield=UserFollowPower from TUsers where @TargetUserID=UserID
		if @Shield=0
		return 6    --Ŀ���û������Լ�   �޷���Ӻ���
		
		insert into TUserFriendTable(UserID,FriendID)
		values(@UserID,@TargetUserID)
		
		return 0;
	end
	else if @Type=1  --1.ɾ������
	begin
		if not exists(select * from TUserFriendTable where UserID=@UserID and FriendID=@TargetUserID)
		return 4  --Ŀ���û����Ǹ���Һ��� �޷�ɾ��
		delete from TUserFriendTable where UserID=@UserID and FriendID=@TargetUserID
		return 0
	end
	else
	begin
		return 5  --δ֪����
	end
END
