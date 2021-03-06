
GO
/****** Object:  StoredProcedure [dbo].[SP_BankOpNormal]    Script Date: 03/21/2017 14:05:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[SP_ClubUpDownPoints]
    @UserID INT ,			
    @ClubID	int,
    @OperType tinyint,			 --1-上分   0-下分
    @TargetUserID int,
    @Points	bigint				 --变化分数
AS 
    BEGIN 
        SET NOCOUNT ON ; 
     
	if not exists(select * from TClubTable where MasterID=@UserID and @ClubID=ClubID)
		return 1    --不是该俱乐部群主   没有权限
	if not exists(select * from TClubUserTable where ClubID=@ClubID and UserID=@UserID)
		return 4    --目标用户ID错误   不是该俱乐部成员
	declare @UserPoint bigint
	select @UserPoint=ClubPoints from TClubUserTable where ClubID=@ClubID and UserID=@TargetUserID
	declare @IsOpen int
	select @IsOpen=UserScoreDisable FROM dbo.Web_Config   --俱乐部是否开启上下分功能
	if @IsOpen=0
	return 5  --------俱乐部上下分功能已经关闭  无法操作
	declare @RoomID int 
	declare @Isplay tinyint
	declare @AllPoint bigint--改变之后的分数
	set @RoomID=0
	set @Isplay=0
	set @AllPoint=0
	if exists (select * from TWLoginRecord where UserID=@TargetUserID)
	begin
		select @RoomID=WID from TWLoginRecord where UserID=@TargetUserID
		set @Isplay=1
	end
	declare @UserStartPoint bigint
	select @UserStartPoint=ClubPoints from TClubUserTable where @TargetUserID=UserID and ClubID=@ClubID
	if @OperType=1  --上分
		begin
			update TClubUserTable  set ClubPoints=ClubPoints+@Points where ClubID=@ClubID and UserID=@TargetUserID	
			insert into TClubPointChangeLog values(@ClubID,@UserID,@OperType,@TargetUserID,@UserStartPoint,@Points,GETDATE(),'上分')		
		end 
	ELSE if @OperType=0
		begin
			if @Isplay=1
				return 3   --游戏中不能下分
				
			if @UserPoint<@Points
			begin
				return 2   --分数不能为负
			end
			
			update TClubUserTable  set ClubPoints=ClubPoints-@Points where ClubID=@ClubID and UserID=@TargetUserID 
			insert into TClubPointChangeLog values(@ClubID,@UserID,@OperType,@TargetUserID,@UserStartPoint,@Points,GETDATE(),'下分')
			
		end
		
	
	if exists(select * from TClubUserTable where ClubID=@ClubID and UserID=@UserID)
		select @AllPoint=ClubPoints from TClubUserTable where ClubID=@ClubID and UserID=@TargetUserID
	
	select @RoomID as RoomID,@Isplay as IsPlay,@AllPoint as AllPoint
	
	return 0
    END 

