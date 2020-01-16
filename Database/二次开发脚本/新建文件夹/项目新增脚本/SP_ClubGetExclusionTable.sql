
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-08
-- Description:	��ȡ���ֲ��ų��
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubGetExclusionTable]
@UserID int,
@ClubID int 
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TClubTable where MasterID=@UserID)
	return 1---���û����Ǿ��ֲ�����  û��Ȩ��
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--���ֲ�ID����
	
	DECLARE @Temp TABLE(RedUserID INT,RedLogoID INT,RedSex INT,RedHeadURL NVARCHAR(256),RedNickName NVARCHAR(64),			--��ɫ��ID
						GreenUserID INT,GreenLogoID INT,GreenSex INT,GreenHeadURL NVARCHAR(256),GreenNickName NVARCHAR(64)) --��ɫ��ID

	INSERT INTO @Temp(RedUserID,GreenUserID) SELECT RedUserID,GreenUserID 
	FROM dbo.TClubUserExclusion WHERE ClubID=@ClubID 
	
	UPDATE a SET a.RedLogoID=TUsers.LogoID,a.RedSex=TUsers.Sex,a.RedNickName=TUsers.NickName 
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.RedUserID     --��ɫ��ID
	
	UPDATE a SET a.GreenLogoID=TUsers.LogoID,a.GreenSex=TUsers.Sex,a.GreenNickName=TUsers.NickName 
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.GreenUserID   --��ɫ��ID
	
	UPDATE a SET a.RedHeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.RedUserID
	
	UPDATE a SET a.GreenHeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.GreenUserID
	
	SELECT * FROM @Temp
	
	RETURN 0
	
END
GO
