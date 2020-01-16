go
if exists(select * from dbo.sysobjects where id = object_id('Web_LuckDrawRecordInfo'))  
drop table Web_LuckDrawRecordInfo  
go
CREATE TABLE Web_LuckDrawRecordInfo
(
		ID int IDENTITY(1,1),
		UserID int not null, --用户ID
		LuckDrawType INT NOT NULL,--抽奖类别
		LuckDrawValue INT not null,--金额
		LuckDrawTime datetime not null,--抽奖时间
)

go
if exists(select * from dbo.sysobjects where id = object_id('Web_LuckDrawRecordHistoryInfo'))  
drop table Web_LuckDrawRecordHistoryInfo  
go
CREATE TABLE Web_LuckDrawRecordHistoryInfo
(
		ID int IDENTITY(1,1),
		UserID int not null, --用户ID
		LuckDrawType INT NOT NULL,--抽奖类别
		LuckDrawValue INT not null,--金额
		LuckDrawTime datetime not null,--抽奖时间
)