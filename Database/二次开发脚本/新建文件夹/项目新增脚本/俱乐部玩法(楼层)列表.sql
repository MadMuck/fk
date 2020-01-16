go
if exists(select * from dbo.sysobjects where id = object_id('TClubPlayInfo'))  
drop table TClubPlayInfo  
go

CREATE TABLE TClubPlayInfo
(
	ID INT IDENTITY(1,1),
	GroupID int,    --楼层ID
	GroupName nvarchar(64),  --楼层名称
	GroupNotice nvarchar(MAX), --楼层公告
	ClubID int,
	GameID int,
	PlayCount int, -- 购买局数
	bFinishCondition int, -- 结束条件
	bPayType int, --支付类别
	bPostionLimit int, --是否开启定位
	bPlayNumber int, --游戏人数
	bMidEnter int, --是否允许中途加入
	bMinutes int, --购买时长
	DeskConfig varbinary(MAX), -- 游戏玩法
	PreventCheating int,				--0不开启防作弊检查  1开启防作弊检查
	CurrencyType	int			--结算方式   0--金币结算    1---分数结算
)