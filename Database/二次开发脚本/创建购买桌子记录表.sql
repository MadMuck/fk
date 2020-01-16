go
if exists(select * from dbo.sysobjects where id = object_id('TBuyGameDeskRecord'))  
drop table TBuyGameDeskRecord  
go
CREATE TABLE TBuyGameDeskRecord(
		RoomID INT NOT NULL,--房间ID
		TableNumber INT NOT NULL,--桌子号
		MasterID INT DEFAULT(0) NOT NULL,--购买人
		BuyTime DATETIME,--购买时间
		BuyGameCount int default 0 not null,--购买局数
		szDeskPassWord varchar(20) not null,  --桌子密码
		IsPlay tinyint default(0)
		)
