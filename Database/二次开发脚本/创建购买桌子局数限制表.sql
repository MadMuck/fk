go
if exists(select * from dbo.sysobjects where id = object_id('TVIPDeskToCount'))  
drop table TVIPDeskToCount  
go
CREATE TABLE TVIPDeskToCount(--VIP���Ӷ�Ӧ����Ϸ����
		BuyGameCount int not null,--��Ϸ����
		NeedJewels bigint not null--�����ĵ���ʯ
		)
go
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(1,8)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(2,16)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(3,32)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(4,64)
insert into TVIPDeskToCount(BuyGameCount,NeedJewels) values(5,128)