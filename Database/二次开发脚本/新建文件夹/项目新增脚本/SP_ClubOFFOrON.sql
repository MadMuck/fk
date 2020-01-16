
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-29
-- Description:	���ֲ�������ر�
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubOFFOrON]
	@UserID int,
	@ClubID int,
	@ClubIsOpen int
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TClubTable where MasterID=@UserID and ClubID=@ClubID)
	return 1 ---�û�����Ⱥ��
	if exists(select * from TClubTable where MasterID=@UserID and ClubID=@ClubID and ClubIsOpen=@ClubIsOpen)
	return 2  ---���ֲ���ǰ״̬�Ѿ���Ҫ������״̬  �����ظ�����
	
	update TClubTable set ClubIsOpen=@ClubIsOpen where MasterID=@UserID and ClubID=@ClubID
	return 0
END
GO
