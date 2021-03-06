USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubGetStatisticData]    Script Date: 05/22/2019 16:16:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-19
-- Description:	统计功能：显示所有成员的每个人参与的场次，输赢的分数，数据会一直累加，可以按时间查询
-- =============================================
ALTER PROCEDURE [dbo].[SP_ClubGetStatisticData] 
@UserID INT ,   
@ClubID INT	,
@BeginTime BIGINT,    
@EndTime BIGINT  
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where UserID=@UserID)
	return 1    --UserID不正确
	if not exists(select *  from TClubTable where ClubID=@ClubID)
	return 2    --没有这个俱乐部ID
	
	declare @Temp table(UserID int,UserSession int,UserScore bigint,LogoID int,Sex int,HeadURL NVARCHAR(256),NickName NVARCHAR(64))
	insert into @Temp(UserID) select UserID from TClubUserTable where @ClubID=ClubID
	
	
	UPDATE a SET a.LogoID=TUsers.LogoID,a.Sex=TUsers.Sex,a.NickName=TUsers.NickName
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.UserID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.UserID
	declare @UserIDTmp int
	declare @UserSessionTmp int
	declare @UserScoreTmp bigint
	--声明游标
	declare db_cursor CURSOR SCROLL for select UserID from @Temp
	--打开游标
	open db_cursor  
	--读取游标第一行
	fetch first from db_cursor into @UserIDTmp
	while @@fetch_status=0
		begin
			select @UserScoreTmp=isnull(SUM(UserScore),0) from TDeskTotleRecord 
			where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00')
			
			select @UserSessionTmp=isnull(count(*),0) from TDeskTotleRecord 
			where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00')
			
			update @Temp set UserScore=@UserScoreTmp,UserSession=@UserSessionTmp where UserID=@UserIDTmp
			
		--读取下一行
			fetch next from db_cursor into @UserIDTmp
		end
	--关闭游标
	close db_cursor
	--释放游标
	deallocate db_cursor
	
	select * from @Temp order by UserScore desc
	return 0
END
