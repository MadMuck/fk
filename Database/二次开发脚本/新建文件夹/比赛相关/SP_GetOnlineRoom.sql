
GO
/****** Object:  StoredProcedure [dbo].[SP_GetOnlineRoom]    Script Date: 02/18/2017 13:41:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROC [dbo].[SP_GetOnlineRoom]
AS 
    BEGIN
        SET NOCOUNT ON ;
        SELECT  dbo.TGameServerInfo.ServerInfoID ,
                dbo.TGameServerInfo.GameName ,
                dbo.TGameServerInfo.GameTable ,
                dbo.TGameServerInfo.NameID ,
                dbo.TGameServerInfo.SupportType ,
                dbo.TGameServerInfo.ServiceVer ,
                dbo.TGameServerInfo.DeskPeople ,
                dbo.TGameServerInfo.DLLFileName ,
                dbo.TGameRoomInfo.id ,
                dbo.TGameRoomInfo.RoomID ,
                dbo.TGameRoomInfo.RoomName ,
                dbo.TGameRoomInfo.ServerIP ,
                dbo.TGameRoomInfo.ServerInfoID AS Expr1 ,
                dbo.TGameRoomInfo.GameTypeID ,
                dbo.TGameRoomInfo.GameKindID ,
                dbo.TGameRoomInfo.GameNameID ,
                dbo.TGameRoomInfo.SocketPort ,
                dbo.TGameRoomInfo.OnlineCount ,
                dbo.TGameRoomInfo.UpdateTime ,
                dbo.TGameRoomInfo.EnableRoom ,
                dbo.TGameRoomInfo.StopLogon ,
                dbo.TGameRoomInfo.IDSort ,
                dbo.TGameRoomInfo.Version ,
                dbo.TGameRoomInfo.ServiceName ,
                dbo.TGameRoomBase.DeskCount ,
                dbo.TGameRoomInfo.VirtualUser ,
                dbo.TGameRoomInfo.VirtualGameTime ,
                dbo.TGameRoomInfo.RoomPassword ,
                dbo.TGameRoomBase.RoomRule ,
                dbo.TContestInfo.UpPeople ,
                dbo.TContestInfo.StartTime ,
                --dbo.TContestInfo.EndTime ,
                TContestInfo.LowChip ,
                TContestInfo.LeastPeople ,
				--wushuqun 2009.6.5    
                dbo.TGameRoomInfo.BattleRoomID ,
                dbo.TGameRoomInfo.BattleMatchTable ,
                dbo.TGameRoomBase.ContestID ,
                dbo.TGameRoomBase.LessPoint ,
                dbo.TGameRoomBase.MaxPoint ,
                dbo.TGameRoomBase.BasePoint ,
                dbo.TGameRoomInfo.tenancyID ,
                dbo.Web_ApplyVipRoom.OpenRoomDate ,
                dbo.Web_ApplyVipRoom.AppleyDays
        FROM    dbo.TGameServerInfo WITH ( NOLOCK )
                INNER JOIN dbo.TGameRoomInfo WITH ( NOLOCK ) ON dbo.TGameServerInfo.ServerInfoID = dbo.TGameRoomInfo.ServerInfoID
                INNER JOIN dbo.TGameRoomBase ON dbo.TGameRoomInfo.RoomID = dbo.TGameRoomBase.RoomID
                LEFT JOIN dbo.TContestInfo ON dbo.TGameRoomBase.ContestID = dbo.TContestInfo.ContestID
                LEFT JOIN dbo.Web_ApplyVipRoom ON dbo.TGameRoomInfo.tenancyID = dbo.Web_ApplyVipRoom.ID
        WHERE   ( dbo.TGameRoomInfo.UpdateTime >= DATEADD(MI, -3, GETDATE()) )
                AND ( dbo.TGameRoomInfo.EnableRoom = 1 )
        ORDER BY dbo.TGameRoomInfo.IDSort
    END
