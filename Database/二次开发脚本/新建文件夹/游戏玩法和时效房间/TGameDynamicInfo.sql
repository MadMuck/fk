go
if exists(select * from dbo.sysobjects where id = object_id('TGameDynamicInfo'))  
drop table TGameDynamicInfo  
go
CREATE TABLE TGameDynamicInfo
(
		GameID INT NOT NULL ,
		DynamicInfo1 VARBINARY(128) NOT NULL,--游戏信息,由pc进行设置
)