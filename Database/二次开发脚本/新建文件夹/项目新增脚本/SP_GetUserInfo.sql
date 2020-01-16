
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		limu
-- Create date: 2019-05-07
-- Description:	��ȡ�û�������Ϣ
-- =============================================
alter PROCEDURE [dbo].[SP_GetUserInfo] 
@UserID INT					--�û�ID
AS
BEGIN
	SET NOCOUNT ON;
	if not exists(select * from TUserInfo where @UserID=UserID)
	return 1   ---UserID����ȷ
	
	declare @NickName NVARCHAR(30),@Lotteries INT,@LogoID INT,@Sex INT,@Fascination INT,@SignDescr NVARCHAR(128),
	@iMoney BIGINT,@Jewels INT,@HeadUrl NVARCHAR(256),@Phone VARCHAR(50),@WhetherShielding int
	
	SELECT @Sex=Sex,@LogoID=logoid,@NickName=NickName,@WhetherShielding=UserFollowPower FROM	TUsers WHERE @UserID=UserID
	SELECT @Lotteries=Lotteries,@SignDescr=isnull(SignDescr,''),@Jewels=Jewels,@Phone=isnull(Phone,'') FROM Web_Users WHERE @UserID=UserID
	SELECT @Fascination=Fascination,@iMoney=WalletMoney from TUserInfo WHERE @UserID=UserID
	SELECT @HeadUrl=HeadUrl from TWeiXinRegister WHERE @UserID=UserID
	
	select @NickName as NickName,
		   @Lotteries as Lotteries,
		   @LogoID as LogoID,
		   @Sex as Sex,
		   @Fascination as Fascination,
		   @SignDescr as SignDescr,
		   @iMoney as iMoney,
		   @Jewels as Jewels,
		   @HeadUrl as HeadUrl,
		   @Phone as Phone,
		   @WhetherShielding as WhetherShielding
	return 0
END
GO
