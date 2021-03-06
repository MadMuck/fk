USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_InsertAbnormalRecord]    Script Date: 09/13/2017 14:10:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



-- =============================================
-- Author:		Longan
-- Create date: 5.14 2009 
-- Description:	修改写Z登录记录的时机，确保网络卡的时候，不至于导致用户登录不上
-- 修改:	ZXD
-- 日期:	20100309
-- 原因：	普通玩家才写记录，机器人不写记录
-- =============================================
ALTER PROCEDURE [dbo].[SP_InsertAbnormalRecord]
    @UserID     INT,  --玩家ID
    @ZID        INT, --ZID
    @SocketID   INT  
AS
BEGIN
	SET NOCOUNT ON;
	-- 
	Declare @IsRobot smallint
	select @IsRobot = IsRobot from TUsers where UserID = @UserID
	if @IsRobot = 0
		INSERT INTO [TZLoginRecord] ([UserID], [ZID], [SocketID])  VALUES(@UserID, @ZID, @SocketID)
END


set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON


