--签到奖励规则
CREATE TABLE TSignAward(
	SignDay INT,		--连续签到天数
	AwardMoney INT,		--签到奖励金币数量
	AwardJewels INT,	--签到奖励钻石数量
	AwardLotteries INT  --签到奖励奖券数量
	)

--在线奖励规则
CREATE TABLE TOnlineAward(
	OnlineID INT,		--在线奖励ID，1开始连续数字
	OnlineAward INT,	--在线奖励数量
	OnlineType INT,		--在线奖励类型 0-金币 1-钻石 2-奖券
	OnlineTime INT		--在线时间（秒），从小到大
	)

--签到记录
CREATE TABLE TSignAwardLog(
	UserID INT,				--用户ID
	LastSignTime DATETIME,	--上次签到时间
	SignDay INT				--连续签到次数
	)

--在线奖励记录
CREATE TABLE TOnlineAwardLog(
	UserID INT,				--用户ID
	LastGetTime DATETIME,	--上次领奖时间
	GetTime INT				--领奖次数
	)