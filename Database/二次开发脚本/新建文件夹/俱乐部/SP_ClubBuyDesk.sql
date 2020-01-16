


ALTER PROCEDURE SP_ClubBuyDesk
@UserID INT,					--�û�ID
@ClubID INT ,
@BuyCount INT ,					--�������
@PayType INT,
@GameID varchar(15),			--��Ϸ����
@FinishCondition INT,			--���������ʽ
@PositionLimit INT ,			--�Ƿ�����λ
@PlayerNum INT ,				--��Ϸ��������
@MidEnter INT,
@DeskConfig VARBINARY(MAX)
AS 
begin
	SET NOCOUNT ON
	DECLARE @ReturnValue int
	DECLARE @DeskPassWord varchar(20)
	DECLARE @RoomID INT
	DECLARE @TempNameID INT 
	DECLARE @DeskID INT 
	DECLARE @AAJewels INT 
	DECLARE @Invoice INT 
	DECLARE @InvoiceNum INT 
	DECLARE @ClubMaster INT 
	DECLARE @GameName NVARCHAR(30)
	SET @TempNameID=0
	SET @DeskPassWord=''
	SET @RoomID=0
	SET @ReturnValue=0
	SET @AAJewels=0
	SET @Invoice=0
	SET @InvoiceNum=0
	SET @ClubMaster=0

	if NOT EXISTS(select * from TUsers where UserID=@UserID)--�û�������
	begin
		return 1
	end

	SELECT @ClubMaster=ISNULL(MasterID,0) FROM TClubTable WHERE ClubID=@ClubID
	
	IF @ClubMaster=0
		RETURN 2		--���ֲ�ID����
		
	IF NOT EXISTS (SELECT * FROM dbo.TClubUserTable WHERE ClubID=@ClubID AND UserID=@UserID)
		RETURN 3		--���δ����ù���
		

	SELECT TOP 1 @InvoiceNum=InvoiceNum,@Invoice=Invoice FROM dbo.Web_Config
	
	IF EXISTS (SELECT * FROM TBuyGameDeskRecord WHERE MasterID=@UserID) AND @Invoice=0 --���ɴ���ʱ�����ظ�����
		RETURN 4 
	ELSE IF EXISTS (SELECT * FROM TBuyGameDeskRecord WHERE MasterID=@UserID AND ClubID<>0)
		RETURN 5	--���ֲ�����ÿ��ֻ�ܴ���һ��
	
	declare @Jewels int--��ȡ������ʯ
	SET @Jewels=-1
	--IF @PayType=2
	--	select @Jewels=isnull(AANeedJewels,-1) from TVIPDeskToCount where BuyGameCount=@BuyCount AND GameID=@GameID
	--ELSE 
	--BEGIN 
		select @Jewels=isnull(NeedJewels,-1) from TVIPDeskToCount where BuyGameCount=@BuyCount AND GameID=@GameID
	--END 
	if @Jewels<0--��������
	begin
		return 6
	END
	
	SELECT @GameName=ComName FROM dbo.TGameNameInfo WHERE NameID=@GameID
	
	declare @TempJewels int
	select @TempJewels=Jewels from Web_Users where UserID=@ClubMaster
	if (@TempJewels-@Jewels)<0--��ʯ����
	begin
		return 7
	end

	SELECT @RoomID=RoomID ,@DeskID=DeskID FROM HNF_DistributeTable(@GameID)
	IF @RoomID<>255 AND @DeskID<>255
	BEGIN 
		--��̬��������
		WHILE 1=1
		BEGIN 
			set @DeskPassWord=RIGHT(100000000 + CONVERT(bigint, ABS(CHECKSUM(NEWID()))), 6)
			IF NOT EXISTS(SELECT * FROM TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPassWord)
				AND NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecordDelete WHERE DeskPassword=@DeskPassWord)
				BREAK
		END 
		
		INSERT INTO TBuyGameDeskRecord(RoomID,TableNumber,MasterID,BuyTime,BuyGameCount,szDeskPassWord,GameID,DeskConfig,PayType,Jewels,FinishCondition,PositionLimit,PlayerNum,MasterState,MidEnter,ClubPayType,ClubID) 
		values(@RoomID,@DeskID,@UserID,getdate(),@BuyCount,@DeskPassWord,@GameID,@DeskConfig,0,@Jewels,@FinishCondition,@PositionLimit,@PlayerNum,0,@MidEnter,@PayType,@ClubID)
		
		UPDATE Web_Users SET Jewels=Jewels-@Jewels WHERE UserID=@ClubMaster
		UPDATE dbo.Web_Users SET LockJewels=LockJewels+@Jewels WHERE UserID=@ClubMaster
		
		INSERT INTO TClubBuyDeskRecord(ClubID ,UserID ,BuyTime ,CostJewels ,DeskPass ,IsCost) VALUES(@ClubID,@UserID,getdate(),@Jewels,@DeskPassWord,0)
		
		DECLARE @NickName NVARCHAR(64) 
		DECLARE @HeadURL NVARCHAR(256) 
		DECLARE @LogonID INT 
		DECLARE @Sex INT 
		SET @HeadURL=''
		
		SELECT @NickName=NickName,@LogonID=LogoID,@Sex=Sex FROM dbo.TUsers WHERE UserID=@UserID
		SELECT @HeadURL=HeadUrl FROM dbo.TWeiXinRegister WHERE UserID=@UserID
		
		SELECT @DeskPassWord AS szPassWord,@NickName AS NickName,@HeadURL AS HeadURL,@LogonID AS LogonID,@Sex AS Sex,@RoomID AS iRoomID,@DeskID AS iDeskID,@Jewels AS Jewels,@GameName AS GameName
		
		return 0
	END 
	ELSE
	BEGIN --�����ѹ�����
		return 8
	END 

end