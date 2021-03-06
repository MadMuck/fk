USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_SetUserOnlineFlag]    Script Date: 04/13/2017 14:13:31 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		Fred Huang
-- Create date: 2008-12-01
-- Description:	设置用户在线状态标志
--				UserID=0时，所有人设置为下线
--				UserID!=0时，
--					FlagID=0 ：下线
--                  FlagID=1 ：上线
-- =============================================
ALTER PROCEDURE [dbo].[SP_SetUserOnlineFlag]
	@UserID		INT,		--用户ID号
	@FlagID		INT,		--上、下线标志，-上线，-下线
	@SocketID   INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
    --------------------------------
	--Kylin 20090319 添加新防止账户同时登陆

	if @FlagID = -1
	BEGIN 
		UPDATE dbo.TUsers SET OnlineFlag=0 WHERE UserID=@UserID
		IF @UserID<>0
			delete TZLoginRecord where UserID=@UserID
		ELSE IF @SocketID<>-1
			DELETE TZLoginRecord where SocketID=@SocketID
	END 
	if @FlagID = -2
		delete TWLoginRecord where UserID=@UserID
	--------------------------------
END

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
