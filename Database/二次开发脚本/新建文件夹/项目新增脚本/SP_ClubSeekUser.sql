
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-12
-- Description:	俱乐部群主通过玩家ID搜索玩家（查询玩家是否在该俱乐部）
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubSeekUser] 
@UserID	INT ,
@ClubID INT 
AS
BEGIN
	SET NOCOUNT ON;
	if exists(select * from TClubUserTable where ClubID=@ClubID and UserID=@UserID)
	return 1   --该成员已经在本俱乐部
	
	if not exists(select * from TUsers where UserID=@UserID)
	return 2   --没有这个玩家ID
	declare @LogoID INT,@Sex INT,@HeadURL NVARCHAR(256),@NickName NVARCHAR(64)
	
	select @LogoID=LogoID,@Sex=Sex,@NickName=NickName from TUsers where UserID=@UserID
	select @HeadURL=HeadUrl from TWeiXinRegister where UserID=@UserID
	
	select @LogoID as LogoID,@Sex as Sex,@NickName as NickName,@HeadURL as HeadURL
	return 0
END
GO
