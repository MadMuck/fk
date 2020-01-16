
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-29
-- Description:	俱乐部开启或关闭
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubOFFOrON]
	@UserID int,
	@ClubID int,
	@ClubIsOpen int
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TClubTable where MasterID=@UserID and ClubID=@ClubID)
	return 1 ---用户不是群主
	if exists(select * from TClubTable where MasterID=@UserID and ClubID=@ClubID and ClubIsOpen=@ClubIsOpen)
	return 2  ---俱乐部当前状态已经是要操作的状态  无需重复操作
	
	update TClubTable set ClubIsOpen=@ClubIsOpen where MasterID=@UserID and ClubID=@ClubID
	return 0
END
GO
