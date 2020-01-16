

set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
alter PROCEDURE SP_GetAttachMent
@UserID	INT ,
@MailID INT 
AS  
BEGIN 
	SET NOCOUNT ON
	
	DECLARE @SendMoney BIGINT
	DECLARE @SendJewels INT
	DECLARE @SendLotteries INT
	DECLARE @IsGet INT
	SET @SendMoney=0
	SET @SendJewels=0
	SET @SendLotteries=0
	
	IF @MailID=0	--全部提取
	BEGIN 
		SELECT @SendMoney=ISNULL(SUM(SendMoney),0),@SendJewels=ISNULL(SUM(SendJewels),0),@SendLotteries=ISNULL(SUM(SendLotteries),0) 
		FROM TMailList WHERE UserID=@UserID AND (IsGet=2 OR IsGet=3) AND IsSend=1
		
		UPDATE TMailList SET IsGet=4 WHERE UserID=@UserID AND (IsGet=2 OR IsGet=3)
		
		SELECT @SendMoney=@SendMoney+ISNULL(SUM(SendMoney),0),@SendJewels=@SendJewels+ISNULL(SUM(SendJewels),0),@SendLotteries=@SendLotteries+ISNULL(SUM(SendLotteries),0) 
		FROM TMailList LEFT JOIN TMailtoAllUserLog ON TMailList.MailID=TMailtoAllUserLog.MailID AND TMailtoAllUserLog.UserID=@UserID 
		WHERE TMailList.UserID=0 AND TMailList.IsGet=2 AND ((TMailList.IsSend=0 AND (TMailtoAllUserLog.IsGet=2 OR TMailtoAllUserLog.IsGet=3)) OR 
		(TMailList.IsSend=1 AND (TMailtoAllUserLog.IsGet<>4 AND TMailtoAllUserLog.IsGet<>5)))
		
		
		UPDATE TMailtoAllUserLog SET IsGet=4 WHERE UserID=@UserID AND (IsGet=2 OR IsGet=3)
		
		INSERT INTO TMailtoAllUserLog(MailID,UserID,IsGet) SELECT MailID,@UserID,4 FROM TMailList
		WHERE UserID=0 AND IsGet=2 AND (MailID NOT IN (SELECT MailID FROM TMailtoAllUserLog WHERE UserID=@UserID)) AND IsSend=1
	END 
	ELSE 
	BEGIN 
		IF NOT EXISTS(SELECT * FROM TMailList WHERE MailID=@MailID)
		RETURN 2		--邮件不存在
		
		DECLARE @UserIDTemp INT 
		SELECT @UserIDTemp=UserID,@SendMoney=SendMoney,@SendJewels=SendJewels,@SendLotteries=SendLotteries,@IsGet=IsGet
		FROM TMailList WHERE MailID=@MailID
		IF @IsGet<>2 AND @IsGet<>3
			RETURN 4	--邮件附件已领取或无附件
		SET @IsGet=4
		IF @UserIDTemp=0
		BEGIN 
			DECLARE @IsGetTemp INT 
			SET @IsGetTemp=-1
			SELECT @IsGetTemp=ISNULL(IsGet,-1) FROM TMailtoAllUserLog WHERE MailID=@MailID AND UserID=@UserID
			
			IF @IsGetTemp = -1
			BEGIN 
				INSERT INTO TMailtoAllUserLog(MailID,UserID,IsGet) VALUES(@MailID,@UserID,@IsGet)
			END 
			ELSE 
			BEGIN
				IF @IsGetTemp<>2 AND @IsGetTemp<>3
					RETURN 4	--邮件附件已领取或无附件
					
				UPDATE TMailtoAllUserLog SET IsGet=@IsGet WHERE MailID=@MailID AND UserID=@UserID
			END 
				
		END 
		ELSE IF @UserIDTemp=@UserID
			UPDATE TMailList SET IsGet=@IsGet,IsSend=1 WHERE MailID=@MailID AND UserID=@UserID
		ELSE 
			RETURN 3		--不是该玩家的邮件
			
	END 
	
	---发奖
	DECLARE @UserName NVARCHAR(64)
	SELECT @UserName=UserName FROM dbo.TUsers WHERE UserID=@UserID
	IF @SendMoney <> 0		--金币
	BEGIN
		DECLARE @TotalMoney BIGINT
		SELECT @TotalMoney=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
		UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@SendMoney WHERE UserID=@UserID
		INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
                VALUES  ( @UserID ,@UserName ,@TotalMoney ,@SendMoney ,33 ,GETDATE() ,N'邮件提取奖励')
	END 
	IF @SendJewels <> 0	--房卡
	BEGIN
		DECLARE @JewelsBefore INT 
		SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
		update Web_Users set Jewels=Jewels+@SendJewels where UserID=@UserID 
		INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
		VALUES(@UserID,@JewelsBefore,@SendJewels,13,GETDATE(),N'邮件提取奖励')
	END 
	IF @SendLotteries <> 0	--奖券
	BEGIN
		UPDATE dbo.Web_Users SET Lotteries=Lotteries+@SendLotteries where UserID=@UserID
		DECLARE @StartLotteries INT 
		SELECT @StartLotteries=Lotteries FROM dbo.Web_Users WHERE userid = @userid
		INSERT INTO dbo.Web_LotteriesLog( UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
		VALUES  ( @UserID,@UserName,@StartLotteries,@SendLotteries,@StartLotteries+@SendLotteries,GETDATE(),N'邮件提取奖励' ,6 )
	END 
	
	SELECT @SendMoney AS SendMoney,@SendJewels AS SendJewels,@SendLotteries AS SendLotteries,@IsGet AS IsGet
		
	RETURN 0
	
END 