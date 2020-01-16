set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
if exists(select * from dbo.sysobjects where id = object_id('SP_FindBuyDeskStation'))  
drop procedure SP_FindBuyDeskStation  
go
create procedure SP_FindBuyDeskStation
@RoomID int--∑øº‰ID
as
begin
set nocount on
declare @tempDeskCount int
declare @index int
declare @VipCount int
declare @IsBuy tinyint
declare @UserID int
declare @DeskPassWord varchar(20)
DECLARE @DeskConfig VARBINARY(MAX)
DECLARE @PayType INT 

end