USE [HN_zh]
GO
/****** Object:  StoredProcedure [dbo].[SP_GetTransferRecord]    Script Date: 05/29/2018 19:02:21 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



-- =============================================
-- Author:		zxd
-- Create date: 2010-01-14
-- Description:	取得转帐历史记录
-- =============================================
-- 增加显示网站上的转账日志 zhengguisheng,2010-06-02
-- =============================================
ALTER PROCEDURE [dbo].[SP_GetTransferRecord]
	@UserID	INT		--玩家ID
AS
BEGIN
	SET NOCOUNT ON;
	Select top 50 UserID, DestUserID, Money AS TransferMoney, ActualTransfer, TransTime,UserName,UserNameZZ
	from Web_vTransferLog--TTransferRecord,amended by zhengguisheng on 100602
	where UserID = @UserID or DestUserID = @UserID
	order by TransTime desc
	return 0
END

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON

