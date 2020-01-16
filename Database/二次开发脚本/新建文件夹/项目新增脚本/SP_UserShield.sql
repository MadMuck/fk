
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-07
-- Description:	�û������Լ� ����������޷�����Լ�Ϊ����
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserShield] 
@UserID INT,					--�û�ID
@WhetherShielding int           --��������   0---�����Լ�  1---ȡ������
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where @UserID=UserID)
	return 1   ---UserID����ȷ
	
	update TUsers set UserFollowPower=@WhetherShielding where UserID=@UserID
	return 0
END
GO
