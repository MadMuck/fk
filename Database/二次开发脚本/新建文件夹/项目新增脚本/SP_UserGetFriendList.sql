
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-04-17
-- Description:	��һ�ȡ�����б�
-- =============================================
CREATE PROCEDURE [dbo].[SP_UserGetFriendList]
 @UserID	INT			--�û�ID
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select *from TUserInfo where UserID=@UserID)
	return 1  --û������û�
	DECLARE @Temp TABLE(FriendID INT,LogoID INT,Sex INT,HeadURL NVARCHAR(256),NickName NVARCHAR(64),GameName NVARCHAR(30),
	Fascination int,WalletMoney bigint,SignDescr NVARCHAR(128),OnlineFlag int,RoomID int)   --OnlineFlag����Ƿ�����  0--���� 1---����
	INSERT INTO @Temp(FriendID)SELECT(FriendID)from TUserFriendTable where UserID=@UserID
	
	UPDATE a SET a.LogoID=TUsers.LogoID,a.Sex=TUsers.Sex,a.NickName=TUsers.NickName,a.OnlineFlag=TUsers.OnlineFlag  
	FROM @Temp a, dbo.TUsers WHERE dbo.TUsers.UserID=a.FriendID
	UPDATE a SET a.HeadURL=TWeiXinRegister.HeadUrl FROM @Temp a, dbo.TWeiXinRegister WHERE dbo.TWeiXinRegister.UserID=a.FriendID
	update @Temp set GameName='' 
	
    update a set a.RoomID=TGameLock.RoomID from @Temp a,TGameLock where a.FriendID=TGameLock.UserID and a.OnlineFlag=1
    update a set a.GameName=TGameRoomInfo.RoomName from @Temp a,TGameRoomInfo where a.RoomID=TGameRoomInfo.RoomID and a.OnlineFlag=1
    
    update @Temp set GameName='����' where OnlineFlag=1 and GameName=''
    select * from @Temp order by Fascination desc 
END
GO
