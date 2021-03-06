USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_UserExclusion]    Script Date: 05/10/2019 18:44:33 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		LiMu
-- Create date: 2019-04-04
-- Description:	俱乐部排斥功能  绑定或解除用户排斥关系
-- =============================================
ALTER PROCEDURE [dbo].[SP_UserExclusion]
@UserID INT,					--操作用户ID
@ClubID INT ,
@Type   int,					 --排斥功能   0---绑定排斥关系  1----解绑排斥关系
@RedUserID INT ,				 --红色方UserID		
@GreenUserID INT				 --绿色方UserID
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TClubTable where MasterID=@UserID)
	return 1   --用户不是俱乐部群主  无权操作
	if @Type=0  --绑定排斥关系
	begin
		if exists(select * from TClubUserExclusion 
		where (@RedUserID=RedUserID and @GreenUserID=GreenUserID and @ClubID=ClubID) or (@RedUserID=GreenUserID and @GreenUserID=RedUserID and @ClubID=ClubID))
		return 2   --这两用户已经是绑定关系  无法再次绑定
		insert into TClubUserExclusion(ClubID,RedUserID,GreenUserID)
		values(@ClubID,@RedUserID,@GreenUserID)
		return 0  --成功
	end	
	else if @Type=1
	begin
		delete TClubUserExclusion where @RedUserID=RedUserID and @GreenUserID=GreenUserID and @ClubID=ClubID
		return 0 --成功
	end
	else
	begin 
		return 3--操作类型错误
	end
END
