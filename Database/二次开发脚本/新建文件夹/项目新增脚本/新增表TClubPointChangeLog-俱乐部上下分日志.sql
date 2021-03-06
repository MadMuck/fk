
CREATE TABLE TClubPointChangeLog(
	ClubID INT ,            --俱乐部ID
	UserID int,			    --群主ID
	OperType int,			--1-上分   0-下分  2--游戏输赢
	TargetUserID int,       --目标用户ID
	TargetUserStartPoint bigint,   --目标用户变化之前的积分数量
	ChangePoint bigint,		--改变积分数量
	OperTime datetime,		--改变时间
	Remark nvarchar(50) not null  --说明
)

--俱乐部上下分 日志