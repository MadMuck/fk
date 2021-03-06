USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubGetPaihangbang]    Script Date: 05/17/2019 15:32:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-24
-- Description:	俱乐部排行榜
-- =============================================
ALTER PROCEDURE [dbo].[SP_ClubGetPaihangbang]
	@UserID			INT,			--用户ID
	@ClubID			INT,			
	@Type			INT,			--0:大赢家  1-场次  2--局数  3--分数
	@BeginTime		bigint,
	@EndTime        bigint				
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where UserID=@UserID)
	return 1    --UserID不正确
	if not exists(select *  from TClubTable where ClubID=@ClubID)
	return 2    --没有这个俱乐部ID
	
	declare @Temp table(UserID int,Number bigint,OneNumber int,TwoNumber int,ThreeNumber int,FourNumber int,FiveNumber int,
	LogoID int,Sex int,HeadURL NVARCHAR(256),NickName NVARCHAR(64))
	insert into @Temp(UserID) select UserID from TClubUserTable where @ClubID=ClubID
	
	
	UPDATE a SET a.LogoID=TUsers.LogoID,a.Sex=TUsers.Sex,a.NickName=TUsers.NickName
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.UserID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.UserID
	
	declare @UserIDTmp int
	declare @Number bigint
	declare @OneNumber int
	declare @TwoNumber int
	declare @ThreeNumber int
	declare @FourNumber int
	declare @FiveNumber int
	set @Number=0
	set @OneNumber=0
	set @TwoNumber=0
	set @ThreeNumber=0
	set @FourNumber=0
	set @FiveNumber=0
	--声明游标
	declare db_cursor CURSOR SCROLL for select UserID from @Temp
	--打开游标
	open db_cursor  
	--读取游标第一行
	fetch first from db_cursor into @UserIDTmp
	while @@fetch_status=0
		begin
			if @Type=0  --大赢家
			begin
				select @Number=isnull(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') 
				
				select @OneNumber=isnull(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=1
				
				select @TwoNumber=isnull(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=2
				
				select @ThreeNumber=isnull(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=3
				
				select @FourNumber=isnull(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=4
				
				select @FiveNumber=isnull(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=5
			end
			if @Type=1 --场次
			begin
				select @Number=isnull(COUNT(*),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00')
				
				select @OneNumber=isnull(COUNT(*),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=1
				
				select @TwoNumber=isnull(COUNT(*),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=2
				
				select @ThreeNumber=isnull(COUNT(*),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=3
				
				select @FourNumber=isnull(COUNT(*),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=4
				
				select @FiveNumber=isnull(COUNT(*),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=5
				
			end
			if @Type=2 --局数
			begin
				select @Number=isnull(SUM(PlayCount),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00')
				
				select @OneNumber=isnull(SUM(PlayCount),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=1
				
				select @TwoNumber=isnull(SUM(PlayCount),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=2
				
				select @ThreeNumber=isnull(SUM(PlayCount),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=3
				
				select @FourNumber=isnull(SUM(PlayCount),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=4
				
				select @FiveNumber=isnull(SUM(PlayCount),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID and CreateTime <DATEADD(S,@EndTime + 8 * 3600,'1970-01-01 00:00:00') 
				AND CreateTime > DATEADD(S,@BeginTime + 8 * 3600,'1970-01-01 00:00:00') and GroupID=5
			end
			if @Type=3 --分数
			begin
				select @Number=isnull(ClubPoints,0) from TClubUserTable where @UserIDTmp=UserID and @ClubID=ClubID
			end
			
			
			update @Temp set Number=@Number,OneNumber=@OneNumber,TwoNumber=@TwoNumber,ThreeNumber=@ThreeNumber,FourNumber=@FourNumber,FiveNumber=@FiveNumber
			 where UserID=@UserIDTmp
			
		--读取下一行
			fetch next from db_cursor into @UserIDTmp
		end
	--关闭游标
	close db_cursor
	--释放游标
	deallocate db_cursor
	
	select * from @Temp order by Number desc
	return 0
 
END
