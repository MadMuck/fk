go
alter table TGameRoomInfo DROP COLUMN EnableBuy

ALTER TABLE dbo.TGameRoomInfo ADD LockUserType INT DEFAULT(1)
--1,������ң���;���ܼ���  2���������λ�ã���;�ɼ������λ�� 3����������ң���;��������������˳�����

ALTER TABLE dbo.TGameRoomInfo ADD LockMaster INT DEFAULT(1)
--�����Ƿ������Ϸ,1����,0������

ALTER TABLE dbo.TGameRoomInfo ADD IsConnected BIT DEFAULT(0)
--�����Ƿ���������������