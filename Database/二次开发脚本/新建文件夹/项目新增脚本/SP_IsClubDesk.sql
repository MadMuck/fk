
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-16
-- Description:	���������ж��Ƿ�Ϊ���ֲ�����
-- =============================================
CREATE PROCEDURE [dbo].[SP_IsClubDesk] 
@UserID int,--�û�ID
@InputPassWord varchar(20)--��������
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TBuyGameDeskRecord where @InputPassWord=szDeskPassWord)
	return 1   --û���������
	
	declare @ClubID int
	select @ClubID=ClubID from TBuyGameDeskRecord where @InputPassWord=szDeskPassWord
	select @ClubID as ClubID
	return 0
END
GO
