

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
alter PROCEDURE SP_ClubUserList
@UserID	INT ,
@ClubID INT 
AS
BEGIN
	SET NOCOUNT ON
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 3		--玩家未加入该工会
		
		
	DECLARE @Temp TABLE(UserID INT,LogoID INT,Sex INT,HeadURL NVARCHAR(256),NickName NVARCHAR(64),ClubPoints bigint,Winner int,OnlineFlag int,LoginTime datetime)

	INSERT INTO @Temp(UserID,ClubPoints) SELECT UserID,ClubPoints FROM dbo.TClubUserTable WHERE ClubID=@ClubID
	
	UPDATE a SET a.LogoID=TUsers.LogoID,a.Sex=TUsers.Sex,a.NickName=TUsers.NickName,a.OnlineFlag=TUsers.OnlineFlag FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.UserID
	UPDATE a SET a.LoginTime=TUserInfo.LastLoginTM FROM @Temp a, dbo.TUserInfo WHERE dbo.TUserInfo.UserID=a.UserID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.UserID
	
	declare @IsOpen int
	declare @RoomNumber int --在房间中的人数
	declare @OnlineFlag int --在线人数
	SELECT @RoomNumber=ISNULL(COUNT(*),0) from  TBuyGameDeskRecord INNER JOIN TGameCutNetRecord ON TBuyGameDeskRecord.szDeskPassWord=TGameCutNetRecord.DeskPassword 
	where TBuyGameDeskRecord.ClubID=@ClubID
	select @OnlineFlag=ISNULL(COUNT(*),0) from TUsers INNER JOIN TClubUserTable ON TUsers.UserID=TClubUserTable.UserID 
	where TUsers.OnlineFlag=1 and TClubUserTable.ClubID=@ClubID
	select @IsOpen=UserScoreDisable FROM dbo.Web_Config   --俱乐部是否开启上下分功能
	
	declare @UserIDTmp int
	declare @Winner int  --大赢家次数
	set @Winner=0
	--声明游标
	declare db_cursor CURSOR SCROLL for select UserID from @Temp
	--打开游标
	open db_cursor  
	--读取游标第一行
	fetch first from db_cursor into @UserIDTmp
	while @@fetch_status=0
	begin
		select @Winner=ISNULL(SUM(Winner),0) from TDeskTotleRecord 
				where @UserIDTmp=DeskUserID and @ClubID=ClubID 
		update @Temp set Winner=@Winner where UserID=@UserIDTmp
		
		fetch next from db_cursor into @UserIDTmp
	end	
	--关闭游标
	close db_cursor
	--释放游标
	deallocate db_cursor
	
	SELECT @MasterID AS MasterID,@IsOpen as IsOpen,@RoomNumber as RoomNumber,* FROM @Temp
	
	RETURN 0
END