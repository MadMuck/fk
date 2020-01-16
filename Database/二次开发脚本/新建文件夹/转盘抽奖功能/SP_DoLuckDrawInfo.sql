go
if exists(select * from dbo.sysobjects where id = object_id('SP_DoLuckDrawInfo'))  
drop procedure SP_DoLuckDrawInfo  
go

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
CREATE procedure SP_DoLuckDrawInfo
@UserID int   --用户ID
as
begin
	set nocount ON
	declare @index int
	select @index = cast(floor(RAND() * 100) as int)
	set @index = @index + 1
	
	declare @awardindex int
	declare @type int
	declare @value int
	declare @awardprobility int 
	set @awardindex=1
	while @index > 0
	begin
		select @type=LuckDrawType,@value=LuckDrawValue,@awardprobility=LuckDrawprobability from Web_LuckDrawAwardInfo where ID=@awardindex
		
		if @awardprobility is null
		begin
			set @awardprobility = 100
		end
		
		set @index = @index - @awardprobility
		if @index > 0
		begin
			set @awardindex = @awardindex + 1
		end	
		
		if @awardindex >= 10
		begin
			break
		end
		
	end
	
	if @type is null or @value is null or @value < 0
	begin
		return 1
	end
	
	declare @chargemoney int
	--select top 1 @chargemoney=LuckDrawChargeMoney from Web_Config
	set @chargemoney = 100
	declare @UserName varchar(20)
	select @UserName=UserName from TUsers where UserID=@UserID
	
	declare @FreeLuckDrawLeaveCount int
	--select @FreeLuckDrawLeaveCount=ISNULL(LuckDrawFreeLeaveCount,0) from TUserInfo where UserID=@UserID
	set @FreeLuckDrawLeaveCount = 3
	if @FreeLuckDrawLeaveCount is null
	begin
		return 2
	end
	
	declare @StartMoney bigint
	declare @StartJewels int
	declare @StartLotteris int
	
	SET XACT_ABORT ON;
	BEGIN TRAN
	
	if @FreeLuckDrawLeaveCount <= 0
	begin
		if @chargemoney is null or @chargemoney < 0
		begin
			return 3
		end
		--消耗金币抽奖
			select @StartMoney=WalletMoney+BankMoney from TUserInfo where UserID=@UserID
			update TUserInfo set WalletMoney = WalletMoney - @chargemoney where UserID=@UserID
			EXEC proc_AddWeb_MoneyChangeLog @UserID, @UserName,
						@StartMoney, @chargemoney, 21, '转盘抽奖付费'  ---金币变化日志
	end
	--else
	--begin
	--	-- 免费抽奖
	--		update TUserInfo set LuckDrawFreeLeaveCount = LuckDrawFreeLeaveCount - 1 where UserID=@UserID
	--end
	
	if @type = 0 and @value > 0
	begin
		select @StartMoney=WalletMoney+BankMoney from TUserInfo where UserID=@UserID
		update TUserInfo set WalletMoney = WalletMoney + @value where UserID=@UserID
		EXEC proc_AddWeb_MoneyChangeLog @UserID, @UserName,
						@StartMoney, @value, 21, '转盘抽奖奖励'  ---金币变化日志
	end
	else if @type = 1 and @value > 0
	begin
		select @StartJewels=Jewels from Web_Users where UserID=@UserID
		update Web_Users set Jewels = Jewels + @value where UserID=@UserID
		insert into TJewelChangeLog values(@UserID,@StartJewels,@value,21,GETDATE(),'转盘抽奖奖励')
		
	end
	else if @type = 2 and @value > 0
	begin
		select @StartLotteris=Lotteries from Web_Users where UserID=@UserID
		update Web_Users set Lotteries = Lotteries + @value where UserID=@UserID
		insert into Web_LotteriesLog values(@UserID,@UserName,@StartLotteris,@value,@StartLotteris+@value,GETDATE(),'转盘抽奖奖励',21)
	end
	
	insert into Web_LuckDrawRecordInfo values(@UserID,@type,@value,GETDATE())
	
	COMMIT TRAN
	
	set @awardindex = @awardindex - 1
	
	select @awardindex as awardindex
	
	return 0
end