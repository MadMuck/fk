
CREATE PROCEDURE SP_MiddleCostJewels
@UserID		INT,
@AAJewels	INT  
AS 
BEGIN 
	SET NOCOUNT ON 
	DECLARE @Jewels INT 
	DECLARE @LockJewels INT
	DECLARE @JewelsBefore INT 
	
	IF @AAJewels=0
		RETURN 1
	
	DECLARE @NoticeTable TABLE(UserID INT ,Jewels INT ,LockJewels INT,PayType INT)
	
	IF NOT EXISTS (SELECT * FROM TGameCutNetRecord WHERE UserID=@UserID)
	BEGIN
		SELECT @JewelsBefore=Jewels+LockJewels,@Jewels=Jewels,@LockJewels=LockJewels FROM Web_Users WHERE UserID=@UserID
		UPDATE Web_Users SET Jewels=Jewels-@AAJewels WHERE UserID=@UserID
		INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
		VALUES(@UserID,@JewelsBefore,-@AAJewels,1,GETDATE(),'创建房间消耗钻石')
		INSERT INTO @NoticeTable(UserID,Jewels,LockJewels,PayType) VALUES(@UserID,@Jewels-@AAJewels,@LockJewels,2)
	END
	ELSE
		RETURN 1
	
	SELECT * FROM @NoticeTable
	RETURN 0

END 