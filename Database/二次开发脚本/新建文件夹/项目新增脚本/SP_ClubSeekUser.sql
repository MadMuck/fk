
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-12
-- Description:	���ֲ�Ⱥ��ͨ�����ID������ң���ѯ����Ƿ��ڸþ��ֲ���
-- =============================================
CREATE PROCEDURE [dbo].[SP_ClubSeekUser] 
@UserID	INT ,
@ClubID INT 
AS
BEGIN
	SET NOCOUNT ON;
	if exists(select * from TClubUserTable where ClubID=@ClubID and UserID=@UserID)
	return 1   --�ó�Ա�Ѿ��ڱ����ֲ�
	
	if not exists(select * from TUsers where UserID=@UserID)
	return 2   --û��������ID
	declare @LogoID INT,@Sex INT,@HeadURL NVARCHAR(256),@NickName NVARCHAR(64)
	
	select @LogoID=LogoID,@Sex=Sex,@NickName=NickName from TUsers where UserID=@UserID
	select @HeadURL=HeadUrl from TWeiXinRegister where UserID=@UserID
	
	select @LogoID as LogoID,@Sex as Sex,@NickName as NickName,@HeadURL as HeadURL
	return 0
END
GO
