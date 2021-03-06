USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_ClubOperation]    Script Date: 05/22/2019 13:51:03 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-040-26
-- Description:	俱乐部经营状况
-- =============================================
ALTER PROCEDURE [dbo].[SP_ClubOperation] 
	@UserID int,
	@ClubID int
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where UserID=@UserID)
	return 1    --UserID不正确
	if not exists(select *  from TClubTable where ClubID=@ClubID)
	return 2    --没有这个俱乐部ID
	
	declare @Temp table(CreateTime DATETIME,OneNumber int,TwoNumber int,ThreeNumber int,FourNumber int,FiveNumber int,CountNumber int)
 
	declare @i int
	declare @datetiem datetime
	declare @OneNumber int
	declare @TwoNumber int
	declare @ThreeNumber int
	declare @FourNumber int
	declare @FiveNumber int
	declare @CountNumber int
	set @i=0
	while(@i<7)
	begin
	 --select @datetiem=CreateTime from TDeskTotleRecord where datediff(d,CreateTime,getdate()) =@i --and ClubID=@ClubID
	 select @datetiem=getdate()-@i
	 select @datetiem=convert(datetime,convert(varchar(10),@datetiem,120))
	 
	 select @OneNumber=ISNULL(COUNT(distinct DeskPassword),0) from TDeskTotleRecord where ClubID=@ClubID and GroupID=1
				and  CreateTime>@datetiem and CreateTime<DATEADD(dd,1,@datetiem)
	 select @TwoNumber=ISNULL(COUNT(distinct DeskPassword),0) from TDeskTotleRecord where ClubID=@ClubID and GroupID=2
				and  CreateTime>@datetiem and CreateTime<DATEADD(dd,1,@datetiem)
	 select @ThreeNumber=ISNULL(COUNT(distinct DeskPassword),0) from TDeskTotleRecord where ClubID=@ClubID and GroupID=3
				and  CreateTime>@datetiem and CreateTime<DATEADD(dd,1,@datetiem)
	 select @FourNumber=ISNULL(COUNT(distinct DeskPassword),0) from TDeskTotleRecord where ClubID=@ClubID and GroupID=4
				and  CreateTime>@datetiem and CreateTime<DATEADD(dd,1,@datetiem)
	 select @FiveNumber=ISNULL(COUNT(distinct DeskPassword),0) from TDeskTotleRecord where ClubID=@ClubID and GroupID=5
				and  CreateTime>@datetiem and CreateTime<DATEADD(dd,1,@datetiem)
	 select @CountNumber=ISNULL(COUNT(distinct DeskPassword),0) from TDeskTotleRecord where ClubID=@ClubID 
				and  CreateTime>@datetiem and CreateTime<DATEADD(dd,1,@datetiem)
	 insert into @Temp(CreateTime,OneNumber,TwoNumber,ThreeNumber,FourNumber,FiveNumber,CountNumber) 
				values(@datetiem,@OneNumber,@TwoNumber,@ThreeNumber,@FourNumber,@FiveNumber,@CountNumber)
	 set @i=@i+1
	end

    select * from @Temp order by CreateTime desc
	return 0
END
