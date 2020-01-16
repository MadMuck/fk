
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-10
-- Description:	��¼�û�����Ϸ�к��Ƶ������  �齫ר��
-- =============================================
CREATE PROCEDURE [dbo].[SP_RecordUserMultiple]
	@UserID int,
	@Multiple int
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where UserID=@UserID)
	return 1
	declare @MultipleTmp int
	select @MultipleTmp=isnull(Multiple,0) from TUserInfo where UserID=@UserID
	if @MultipleTmp=0 or @MultipleTmp<@Multiple
	update TUserInfo set Multiple=@Multiple where UserID=@UserID
	return 0 
END
GO
