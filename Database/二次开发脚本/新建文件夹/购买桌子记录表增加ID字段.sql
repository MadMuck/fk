

--唯一标识
ALTER TABLE TBuyGameDeskRecord ADD ID INT IDENTITY(1,1)
--进行局数
ALTER TABLE TBuyGameDeskRecord ADD RunCount INT DEFAULT(0)
--游戏ID
ALTER TABLE TBuyGameDeskRecord ADD GameID INT
--游戏配置
ALTER TABLE TBuyGameDeskRecord ADD DeskConfig NVARCHAR(1000)
--付费方式
ALTER TABLE TBuyGameDeskRecord ADD PayType INT
--消费钻石
ALTER TABLE TBuyGameDeskRecord ADD Jewels INT
--结束条件，定位开关，限制人数
ALTER TABLE dbo.TBuyGameDeskRecord ADD FinishCondition INT DEFAULT(0),PositionLimit INT DEFAULT(0),PlayerNum INT DEFAULT(0)
--中途加入
ALTER TABLE dbo.TBuyGameDeskRecord ADD MidEnter INT DEFAULT(0)