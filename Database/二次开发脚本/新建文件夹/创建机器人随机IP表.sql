go
if exists(select * from dbo.sysobjects where id = object_id('TRobotRandIPList'))  
drop table TRobotRandIPList  
go
CREATE TABLE TRobotRandIPList(--VIP���Ӷ�Ӧ����Ϸ����
		id int identity(1,1) not null,
		RandIP NVARCHAR(16)
		)
go
insert into TRobotRandIPList(RandIP) values('14.17.32.255') --�㶫ʡ������
insert into TRobotRandIPList(RandIP) values('123.125.71.38') --������
insert into TRobotRandIPList(RandIP) values('114.80.166.240') --�Ϻ�
insert into TRobotRandIPList(RandIP) values('119.39.23.134') --��ɳ
insert into TRobotRandIPList(RandIP) values('123.150.187.130') --���