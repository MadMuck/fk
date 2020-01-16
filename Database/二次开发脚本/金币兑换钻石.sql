set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
if exists(select * from dbo.sysobjects where id = object_id('SP_MoneyToJewel'))  
drop procedure SP_MoneyToJewel  
go
create procedure SP_MoneyToJewel
@UserID int,--用户ID
@Money bigint--所消耗金币
as
begin
set nocount on
declare @ReturnValue int
declare @Jewels int--获得钻石
declare @XiaohaoMoney bigint--消耗金币
set @ReturnValue=0
if not exists(select * from TUsers where UserID=@UserID)--用户不存在
begin
	set @ReturnValue=2
	return @ReturnValue
end
declare @TempMoney bigint
select @TempMoney=WalletMoney from TUserInfo where UserID=@UserID
if (@TempMoney-@Money)<0--金币不足
begin
	set @ReturnValue=3
	return @ReturnValue
end
set @Jewels=@Money/5000--5000金币兑换一个钻石
if @Jewels<=0
begin
	set @ReturnValue=3
	return @ReturnValue
end
set @XiaohaoMoney=@Jewels*5000
--扣取金币
set xact_abort on   
begin tran
update TUserInfo set WalletMoney=WalletMoney-@XiaohaoMoney where UserID=@UserID
update Web_Users set Jewels=Jewels+@Jewels where UserID=@UserID
commit tran 
select @XiaohaoMoney as UseMoney,@Jewels as Jewels
set @ReturnValue=1
return @ReturnValue
end