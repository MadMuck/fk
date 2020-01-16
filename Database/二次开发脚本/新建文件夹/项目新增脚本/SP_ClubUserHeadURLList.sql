
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		LiMu
-- Create date: 2019-04-02
-- Description:	获取已经在桌上玩家的头像地址
-- =============================================
alter PROCEDURE [dbo].[SP_ClubUserHeadURLList] 
@UserID	INT ,
@ClubID INT ,
@GroupID int --楼层ID
AS
BEGIN
	SET NOCOUNT ON;
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
		
	IF NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 3		--玩家未加入该工会
	
	--1.在TBuyGameDeskRecord表中获取   该俱乐部楼层 中的桌子号和序号
	--2.根据在TBuyGameDeskRecord表中获取的桌号  在TGameCutNetRecode表中获取UserID
	--3.根据在TGameCutNetRecode表中获取UserID   在TWeiXinRegister表中获取用户的头像地址
	

	declare @Temp table(szDeskPass nvarchar(20),szUserNickName nvarchar(30),DeskXuHao int,UserID int,LogonID int,Sex int,HeadURL NVARCHAR(256))
	insert into @Temp (szDeskPass,UserID) select b.DeskPassWord,b.UserID 
	from TBuyGameDeskRecord a,TGameCutNetRecord b where a.ClubID=@ClubID and a.GroupID=@GroupID and a.szDeskPassWord=b.DeskPassword
	
	update a set a.DeskXuHao=TBuyGameDeskRecord.DeskXuHao from @Temp a , TBuyGameDeskRecord where a.szDeskPass=TBuyGameDeskRecord.szDeskPassWord
	UPDATE a SET a.LogonID=TUsers.LogoID,a.Sex=TUsers.Sex,a.szUserNickName=TUsers.NickName FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.UserID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.UserID
	
	select * from @Temp
	return 0
END
GO
