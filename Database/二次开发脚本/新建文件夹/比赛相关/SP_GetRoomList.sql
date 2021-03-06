USE [NEWEST]
GO
/****** Object:  StoredProcedure [dbo].[SP_GetRoomList]    Script Date: 03/03/2017 18:17:51 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		Fred Huang
-- Create date: 2008-12-03
-- Description:	取得房间列表信息
-- =============================================
ALTER PROCEDURE [dbo].[SP_GetRoomList]
	@ServiceName	NVARCHAR(30)	--服务名称
AS
BEGIN
	SET NOCOUNT ON;


	SELECT	dbo.TGameRoomInfo.RoomID, dbo.TGameRoomInfo.ServerInfoID, dbo.TGameRoomInfo.SocketPort, dbo.TGameRoomInfo.IDSort, 
                dbo.TGameNameInfo.KindID, dbo.TGameRoomInfo.EnableRoom, dbo.TGameNameInfo.NameID, dbo.TGameServerInfo.DeskPeople, 
                dbo.TGameServerInfo.SupportType, dbo.TGameServerInfo.ServiceVer, dbo.TGameServerInfo.GameName, dbo.TGameServerInfo.GameTable, 
                dbo.TGameServerInfo.DLLFileName, dbo.TGameRoomBase.RoomRule, dbo.TGameRoomBase.UserPower, dbo.TGameRoomInfo.GameTypeID, 
                dbo.TGameRoomBase.BasePoint, dbo.TGameRoomBase.LessPoint, dbo.TGameRoomBase.MaxPoint,dbo.TGameRoomBase.MaxLimite, dbo.TGameRoomBase.RunPublish, 
                dbo.TGameRoomBase.Tax, dbo.TGameRoomBase.MaxPeople, dbo.TGameRoomBase.DeskCount, dbo.TGameRoomInfo.StopLogon, 
                dbo.TGameRoomInfo.RoomName, dbo.TGameRoomInfo.ServerIP, dbo.TGameRoomTable.TableLock, dbo.TGameRoomTable.TableMatch, 
                dbo.TGameRoomTable.TableIPRule, dbo.TGameRoomTable.TableUserNameRule, dbo.TGameRoomInfo.ServiceName,TGameRoomBase.VipResever
	,dbo.TGameRoomInfo.VirtualUser,dbo.TGameRoomInfo.VirtualGameTime ,
	dbo.TGameRoomInfo.tenancyID,
	dbo.Web_ApplyVipRoom.OpenRoomDate,
	dbo.Web_ApplyVipRoom.AppleyDays,
	
	dbo.TGameRoomInfo.SendTimeLength,
	dbo.TGameRoomInfo.SendCount,
	
	dbo.TContestInfo.ContestID,
	dbo.TContestInfo.StartTime, 
	--dbo.TContestInfo.EndTime,
	dbo.TContestInfo.Chip,
	dbo.TContestInfo.LowChip,
	--dbo.TContestInfo.LowCount,
	dbo.TContestInfo.UpPeople,
	dbo.TContestInfo.LeastPeople,
	dbo.TContestInfo.ContestTime
--wushuqun 2009.6.5    
    ,dbo.TGameRoomInfo.BattleRoomID,dbo.TGameRoomInfo.BattleMatchTable 
	FROM	dbo.TGameNameInfo INNER JOIN
                dbo.TGameRoomInfo ON dbo.TGameNameInfo.NameID = dbo.TGameRoomInfo.GameNameID INNER JOIN
                dbo.TGameRoomBase ON dbo.TGameRoomInfo.RoomID = dbo.TGameRoomBase.RoomID INNER JOIN
                dbo.TGameRoomMsg ON dbo.TGameRoomInfo.RoomID = dbo.TGameRoomMsg.RoomID INNER JOIN
                dbo.TGameRoomTable ON dbo.TGameRoomInfo.RoomID = dbo.TGameRoomTable.RoomID INNER JOIN
                dbo.TGameServerInfo ON dbo.TGameNameInfo.NameID = dbo.TGameServerInfo.NameID
               LEFT JOIN dbo.TContestInfo ON dbo.TGameRoomBase.ContestID =dbo.TContestInfo.ContestID
               LEFT JOIN dbo.Web_ApplyVipRoom ON dbo.TGameRoomInfo.tenancyID = dbo.Web_ApplyVipRoom.ID
	WHERE	dbo.TGameRoomInfo.ServiceName = @ServiceName
END

--exec SP_GetRoomList ddz

