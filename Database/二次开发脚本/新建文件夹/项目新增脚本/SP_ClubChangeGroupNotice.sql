
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-10
-- Description:	�޸�¥�����ֺ͹���
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubChangeGroupNotice] 
	@UserID int,    
	@ClubID int,
	@GroupID int,
	@GroupName nvarchar(30),
	@GroupNotice nvarchar(100)
AS
BEGIN
	SET NOCOUNT ON;
	DECLARE @MasterID INT 
	SET @MasterID=0
	
	SELECT @MasterID=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @MasterID=0
		RETURN 2		--���ֲ�ID����
		
	IF @MasterID<>@UserID
		RETURN 3		--��Ҳ��ǻ᳤
	
	update TClubPlayInfo set GroupName=@GroupName,GroupNotice=@GroupNotice where ClubID=@ClubID and GroupID=@GroupID
	 return 0
END
GO
