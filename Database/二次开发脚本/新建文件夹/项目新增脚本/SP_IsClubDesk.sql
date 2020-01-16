
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-16
-- Description:	根据桌子判断是否为俱乐部房间
-- =============================================
CREATE PROCEDURE [dbo].[SP_IsClubDesk] 
@UserID int,--用户ID
@InputPassWord varchar(20)--输入密码
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TBuyGameDeskRecord where @InputPassWord=szDeskPassWord)
	return 1   --没有这个桌子
	
	declare @ClubID int
	select @ClubID=ClubID from TBuyGameDeskRecord where @InputPassWord=szDeskPassWord
	select @ClubID as ClubID
	return 0
END
GO
