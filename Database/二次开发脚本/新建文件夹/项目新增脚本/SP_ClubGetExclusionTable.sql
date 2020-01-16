
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-08
-- Description:	获取俱乐部排斥表
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubGetExclusionTable]
@UserID int,
@ClubID int 
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TClubTable where MasterID=@UserID)
	return 1---该用户不是俱乐部房主  没有权限
	
	DECLARE @MasterID INT 
	SET @MasterID=0
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--俱乐部ID错误
	
	DECLARE @Temp TABLE(RedUserID INT,RedLogoID INT,RedSex INT,RedHeadURL NVARCHAR(256),RedNickName NVARCHAR(64),			--红色方ID
						GreenUserID INT,GreenLogoID INT,GreenSex INT,GreenHeadURL NVARCHAR(256),GreenNickName NVARCHAR(64)) --绿色方ID

	INSERT INTO @Temp(RedUserID,GreenUserID) SELECT RedUserID,GreenUserID 
	FROM dbo.TClubUserExclusion WHERE ClubID=@ClubID 
	
	UPDATE a SET a.RedLogoID=TUsers.LogoID,a.RedSex=TUsers.Sex,a.RedNickName=TUsers.NickName 
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.RedUserID     --红色方ID
	
	UPDATE a SET a.GreenLogoID=TUsers.LogoID,a.GreenSex=TUsers.Sex,a.GreenNickName=TUsers.NickName 
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.GreenUserID   --绿色方ID
	
	UPDATE a SET a.RedHeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.RedUserID
	
	UPDATE a SET a.GreenHeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.GreenUserID
	
	SELECT * FROM @Temp
	
	RETURN 0
	
END
GO
