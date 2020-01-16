
CREATE TABLE TMailList(
	MailID		INT IDENTITY(1,1),
	UserID		INT ,		--0标识发送给所有人
	SenderName	NVARCHAR(25),
	MailTitle	NVARCHAR(25),
	MailMessage	NVARCHAR(650),
	SendMoney	BIGINT ,
	SendJewels	INT ,
	SendLotteries	INT ,
	SendTime	DATETIME,
	IsGet		INT	,		--0无附件未读，1无附件已读，2有附件未读，3有附件已读未领取，4有附件已读已领取
	IsSend		BIT	,		--0未通知，1-已通知
	KeepTime	INT			--保存天数 0标识无限期
	)

CREATE TABLE TMailtoAllUserLog(
	MailID		INT ,
	UserID		INT ,
	IsGet		INT			--0无附件未读，1无附件已读，2有附件未读，3有附件已读未领取，4有附件已读已领取，5已删除
	)
	
CREATE TABLE TMailDeleteLog(
	MailID		INT ,
	UserID		INT 
	)
	
--ALTER TABLE dbo.Web_Config ADD MailKeepTime	INT DEFAULT(7) NOT NULL 


CREATE TABLE TSysMessage(
	ID				INT IDENTITY(1,1),
	MContent		NVARCHAR(150),		--公告内容
	NextSendTime	DATETIME,			--下次广播时间，新增信息填当前时间
	KeepTime		DATETIME,			--保存时间，在此时间前用户可主动获取此消息
	IntervalTime	INT ,				--发送间隔，分钟
	IsSend			BIT					--是否已广播
	)