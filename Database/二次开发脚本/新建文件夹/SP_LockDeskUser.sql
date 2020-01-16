



set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
create procedure SP_LockDeskUser
@DeskPassWord varchar(20),   --×À×ÓÃÜÂë 
@DeskUserID NVARCHAR(1000),
@UserStation NVARCHAR(1000)
as
begin
	set nocount ON
	
	INSERT INTO TLockDeskUser(DeskPassword ,DeskUserID ,UserStation) 
	SELECT @DeskPassWord,a.RS,b.RS FROM  dbo.Split(@DeskUserID,',') a,dbo.Split(@UserStation,',') b WHERE a.ID=b.ID AND a.RS<>0
end