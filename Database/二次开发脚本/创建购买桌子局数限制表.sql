go
if exists(select * from dbo.sysobjects where id = object_id('TVIPDeskToCount'))  
drop table TVIPDeskToCount  
go
CREATE TABLE TVIPDeskToCount(--VIP桌子对应的游戏局数
		BuyGameCount int not null,--游戏局数
		NeedJewels bigint not null--所消耗的钻石
		)
go
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(1,8)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(2,16)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(3,32)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(4,64)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(5,128)