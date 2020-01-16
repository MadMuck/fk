set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go
if exists(select * from dbo.sysobjects where id = object_id('SP_BuyDesk'))  
drop procedure SP_BuyDesk  
go
CREATE PROCEDURE SP_BuyDesk
@UserID int,					--用户ID
@BuyCount INT ,					--购买局数
@PayType INT,
@GameID varchar(15),			--游戏名字
@FinishCondition INT,			--房间结束方式
@PositionLimit INT ,			--是否开启定位
@PlayerNum INT ,				--游戏限制人数
@MidEnter INT,
@BuyMinutes INT,
@DeskConfig VARBINARY(MAX),
@CurrencyType int
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
	DECLARE @EveryJewelForMinutes INT
	DECLARE @InvoiceNum INT 
	SET @TempNameID=0
	SET @DeskPassWord=''
	SET @RoomID=0
	SET @ReturnValue=0
	SET @AAJewels=0
	SET @Invoice=0
	SET @InvoiceNum=0

	if NOT EXISTS(select * from TUsers where UserID=@UserID)--用户不存在
	begin
		return 2
	end

	SELECT TOP 1 @InvoiceNum=ISNULL(InvoiceNum,0),@Invoice=ISNULL(Invoice,0),@EveryJewelForMinutes=ISNULL(EveryJewelForMinutes,0) FROM dbo.Web_Config
	
	if(@FinishCondition = 3 and @BuyMinutes < @EveryJewelForMinutes)
	begin
		return 7 --购买时长过短
	end
	if(@FinishCondition = 3 and @EveryJewelForMinutes = 0)
	begin
		return 8 --不支持时效模式
	end
	
	IF EXISTS (SELECT * FROM TBuyGameDeskRecord WHERE MasterID=@UserID) AND @Invoice=0 --不可代开时不能重复购买
	BEGIN 
		select @RoomID=RoomID,@DeskPassWord=szDeskPassWord,@TempNameID=GameID from TBuyGameDeskRecord where MasterID=@UserID
		--IF (SELECT UpdateTime FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID)>= DATEADD(MI, - 3, GETDATE())
		BEGIN 
			select @DeskPassWord as szPassWord,@TempNameID as NameID
			return 6
		END 
	END 
	ELSE IF @Invoice=1 AND (SELECT COUNT(*) FROM TBuyGameDeskRecord WHERE MasterID=@UserID) >= @InvoiceNum
		RETURN 9	--可代开但超过最大限制
	
	declare @Jewels int--获取消耗钻石
	SET @Jewels=-1
	if @FinishCondition = 3
	begin
		set @Jewels = @BuyMinutes / @EveryJewelForMinutes
		if @EveryJewelForMinutes * @Jewels < @BuyMinutes
		begin
			set @Jewels = @Jewels + 1
		end
	end
	else
	begin
		IF @PayType=2
			select @Jewels=isnull(AANeedJewels,-1) from TVIPDeskToCount where BuyGameCount=@BuyCount AND GameID=@GameID
		ELSE 
		BEGIN 
			select @Jewels=isnull(NeedJewels,-1) from TVIPDeskToCount where BuyGameCount=@BuyCount AND GameID=@GameID
		END 
		if @Jewels<0--局数错误
		begin
			return 4
		END
	end
	
	declare @TempJewels int
	select @TempJewels=Jewels from Web_Users where UserID=@UserID
	if (@TempJewels-@Jewels)<0--钻石不足
	begin
		return 3
	end

	SELECT @RoomID=RoomID ,@DeskID=DeskID FROM HNF_DistributeTable(@GameID)
	IF @RoomID<>255 AND @DeskID<>255
	BEGIN 
		--动态生成密码
		WHILE 1=1
		BEGIN 
			set @DeskPassWord=RIGHT(100000000 + CONVERT(bigint, ABS(CHECKSUM(NEWID()))), 6)
			IF NOT EXISTS(SELECT * FROM TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPassWord)
				AND NOT EXISTS (SELECT * FROM dbo.TBuyGameDeskRecordDelete WHERE DeskPassword=@DeskPassWord)
				BREAK
		END 
		
		IF @Invoice=0	--不可代开时房主默认在房间，可代开时房主默认离开
			INSERT INTO TBuyGameDeskRecord(RoomID,TableNumber,MasterID,BuyTime,BuyGameCount,szDeskPassWord,GameID,DeskConfig,PayType,Jewels,FinishCondition,PositionLimit,PlayerNum,MasterState,MidEnter,BuyMinutes,BeginTime,CurrencyType) 
			values(@RoomID,@DeskID,@UserID,getdate(),@BuyCount,@DeskPassWord,@GameID,@DeskConfig,@PayType,@Jewels,@FinishCondition,@PositionLimit,@PlayerNum,0,@MidEnter,@BuyMinutes,GETDATE(),CurrencyType)
		ELSE IF @Invoice=1
			INSERT INTO TBuyGameDeskRecord(RoomID,TableNumber,MasterID,BuyTime,BuyGameCount,szDeskPassWord,GameID,DeskConfig,PayType,Jewels,FinishCondition,PositionLimit,PlayerNum,MasterState,MidEnter,BuyMinutes,BeginTime,CurrencyType) 
			values(@RoomID,@DeskID,@UserID,getdate(),@BuyCount,@DeskPassWord,@GameID,@DeskConfig,@PayType,@Jewels,@FinishCondition,@PositionLimit,@PlayerNum,1,@MidEnter,@BuyMinutes,GETDATE(),CurrencyType)

		IF @PayType=1   --选择房主创建房间时扣费会在这里扣费，其他情况会再其他地方扣费
		BEGIN 
			DECLARE @JewelsBefore INT 
			set xact_abort on   
			begin TRAN
			SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
			update Web_Users set Jewels=Jewels-@Jewels where UserID=@UserID
			INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
			VALUES(@UserID,@JewelsBefore,@Jewels,1,GETDATE(),'创建房间消耗钻石')
			commit tran 
		END 
		ELSE IF @PayType=0
		BEGIN 
			UPDATE Web_Users SET Jewels=Jewels-@Jewels WHERE UserID=@UserID
			UPDATE dbo.Web_Users SET LockJewels=LockJewels+@Jewels WHERE UserID=@UserID
			SET @Jewels=0
		END 
		ELSE IF @PayType=2
		BEGIN 
			SET @AAJewels=@Jewels
			SET @Jewels=0
		END
		
		SELECT @RoomID AS iRoomID,@DeskID AS iDeskID,@DeskPassWord AS szPassWord,len(@DeskPassWord) AS iPassWordLen,@Jewels AS Jewels,@AAJewels AS AAJewels
		
		return 1
	END 
	ELSE
	BEGIN --桌子已购买完
		return 5
	END 

	return @ReturnValue

end