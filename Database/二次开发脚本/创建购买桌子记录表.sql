go
if exists(select * from dbo.sysobjects where id = object_id('TBuyGameDeskRecord'))  
drop table TBuyGameDeskRecord  
go
CREATE TABLE TBuyGameDeskRecord(
		RoomID INT NOT NULL,--����ID
		TableNumber INT NOT NULL,--���Ӻ�
		MasterID INT DEFAULT(0) NOT NULL,--������
		BuyTime DATETIME,--����ʱ��
		BuyGameCount int default 0 not null,--�������
		szDeskPassWord varchar(20) not null,  --��������
		IsPlay tinyint default(0)
		)
