USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubGetPlayInfoList]    Script Date: 04/26/2019 14:34:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROCEDURE [dbo].[SP_ClubGetPlayInfoList]
@UserID int,
@ClubID INT 					
AS 
begin
	SET NOCOUNT ON
	if not exists (select * from TClubTable where ClubID=@ClubID)
	return 1   --没有这个俱乐部
	--declare @PlayInfoTable TABLE
	--(
	--	ID INT,
	--	GroupID int,
	--	GroupName nvarchar(64),   --楼层名字
		
	--	ClubID int,
	--	GameID int,
	--	PlayCount int, -- 购买局数
	--	bFinishCondition int, -- 结束条件
	--	bPayType int, --支付类别
	--	bPostionLimit int, --是否开启定位
	--	bPlayNumber int, --游戏人数
	--	bMidEnter int, --是否允许中途加入
	--	bMinutes int, --购买时长
	--	DeskConfig varbinary(MAX),-- 游戏玩法
	--	PreventCheating int,    --0不开启防作弊 1开启
	--	CurrencyType int       --结算类型   0--金币   1--分数
		
	--)
	
	--insert into @PlayInfoTable select * from TClubPlayInfo where ClubID=@ClubID
	
	select * from TClubPlayInfo where ClubID=@ClubID ORDER BY GroupID ASC
	return 0

end