go
if exists(select * from dbo.sysobjects where id = object_id('SP_GetLuckDrawInfo'))  
drop procedure SP_GetLuckDrawInfo  
go

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE procedure SP_GetLuckDrawInfo
@UserID int   --ÓÃ»§ID
as
begin
	set nocount ON
	
	declare @FreeLuckDrawLeaveCount int
	--select @FreeLuckDrawLeaveCount=ISNULL(LuckDrawFreeLeaveCount,0) from TUserInfo where UserID=@UserID
	set @FreeLuckDrawLeaveCount = 3
	if @FreeLuckDrawLeaveCount is null
	begin
		return 1
	end
	
	declare @FreeTotalCount int
	set @FreeTotalCount = 3
	declare @LuckDrawChargeMoney int
	--select @FreeTotalCount=LuckDrawFreeCount,@LuckDrawChargeMoney=LuckDrawChargeMoney from Web_Config
	set @LuckDrawChargeMoney=100
	select top 10 *,@FreeLuckDrawLeaveCount as FreeLuckDrawLeaveCount, @FreeTotalCount as FreeTotalCount,@LuckDrawChargeMoney as LuckDrawChargeMoney from Web_LuckDrawAwardInfo
	return 0
end