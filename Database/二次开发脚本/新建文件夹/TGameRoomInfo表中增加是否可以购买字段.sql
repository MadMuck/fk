go
alter table TGameRoomInfo DROP COLUMN EnableBuy

ALTER TABLE dbo.TGameRoomInfo ADD LockUserType INT DEFAULT(1)
--1,锁定玩家，中途不能加入  2，锁定玩家位置，中途可加入空闲位置 3，不锁定玩家，中途随意进出，申请退出结算

ALTER TABLE dbo.TGameRoomInfo ADD LockMaster INT DEFAULT(1)
--房主是否必须游戏,1必须,0不必须

ALTER TABLE dbo.TGameRoomInfo ADD IsConnected BIT DEFAULT(0)
--房间是否已连接主服务器