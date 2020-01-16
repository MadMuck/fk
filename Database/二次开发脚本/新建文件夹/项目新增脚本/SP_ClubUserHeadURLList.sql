
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		LiMu
-- Create date: 2019-04-02
-- Description:	��ȡ�Ѿ���������ҵ�ͷ���ַ
-- =============================================
alter PROCEDURE [dbo].[SP_ClubUserHeadURLList] 
@UserID	INT ,
@ClubID INT ,
@GroupID int --¥��ID
AS
BEGIN
	SET NOCOUNT ON;
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--���ֲ�ID����
		
	IF NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 3		--���δ����ù���
	
	--1.��TBuyGameDeskRecord���л�ȡ   �þ��ֲ�¥�� �е����Ӻź����
	--2.������TBuyGameDeskRecord���л�ȡ������  ��TGameCutNetRecode���л�ȡUserID
	--3.������TGameCutNetRecode���л�ȡUserID   ��TWeiXinRegister���л�ȡ�û���ͷ���ַ
	

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
