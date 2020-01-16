
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestApplyHALL]    Script Date: 02/21/2017 17:05:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROC [dbo].[SP_ContestApplyHALL]
    @UserID INT ,---�û�ID
    @TypeID INT ,--0����1����
    @ContestID INT
AS 
    BEGIN
        SET NOCOUNT ON ;

        DECLARE @rs INT  ---����״̬ 0�����������ڴ�������1�����ɹ�2�����ɹ�3����ʧ��(�ѱ�����)4����ʧ�ܣ������ѿ����޷��٣�,
							--5δ��������ʧ��,6�����ѿ����޷�������7�Ѵﵽ���������8Ǯ����,9����δ����,10�û�������,11����ID����,12�ѱ����ٽ�����
        DECLARE @yNum INT ---�ѱ�������
        SELECT  @rs = -1 ,
                @yNum = 0    
                
        DECLARE @EnterFee INT 
		DECLARE @EnterFeeType INT
		DECLARE @ContestType INT 
		DECLARE @BeginTime DATETIME
		DECLARE @CircleTime INT 
		DECLARE @UpPeople INT  

		IF EXISTS (SELECT * FROM dbo.TGameRoomBase WHERE ContestID=@ContestID AND RoomRule&1048576>0)
			RETURN 0		--�����������ڴ�������
			
		IF NOT EXISTS (SELECT * FROM dbo.TContestInfo WHERE ContestID=@ContestID)
			RETURN 11
			
		IF NOT EXISTS (SELECT * FROM dbo.TUsers WHERE UserID=@UserID)
			RETURN 10	--�û�������

		SELECT @EnterFee=EntryFee,@EnterFeeType=EntryFeeType,@ContestType=ContestType,@BeginTime=StartTime,@CircleTime=CircleTime,@UpPeople=UpPeople
			FROM dbo.TContestInfo WHERE ContestID=@ContestID

        IF @TypeID = 0 AND @rs=-1---����
        BEGIN	
			DECLARE @MatchID INT 
			
			DECLARE @RoomID INT
			DECLARE @GameNameID INT
			DECLARE @bNotifyG INT 
			DECLARE @Money BIGINT
			DECLARE @Jewels BIGINT
			DECLARE @UserName NVARCHAR(20)
			DECLARE @JewelsBefore INT 
			
			SET @RoomID=0
			SET @bNotifyG=1
        
			IF EXISTS (SELECT * FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID AND SignState<>1)		--�ѱ���
				RETURN 3
			ELSE IF (SELECT WalletMoney FROM dbo.TUserInfo WHERE UserID=@UserID)<@EnterFee AND @EnterFeeType=0
				RETURN 8
			ELSE IF (SELECT Jewels FROM dbo.Web_Users WHERE UserID=@UserID)<@EnterFee AND @EnterFeeType=1
				RETURN 8
			--����ѭ�����쳣����	
			IF @ContestType=2 AND EXISTS (SELECT * FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0)
			BEGIN 
				SELECT TOP 1 @MatchID=MatchID FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0
				IF NOT EXISTS (SELECT * FROM dbo.TContestCircleFreeRecord WHERE FREE=1 AND BeginTime>GETDATE() AND MatchID=@MatchID)
					DELETE FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0
			END 
			
			---�жϱ���ʱ��
			IF EXISTS (SELECT * FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID IN (SELECT ContestID FROM dbo.TContestInfo 
						WHERE DATEDIFF(mi,GETDATE(),StartTime)<10 AND DATEDIFF(mi,GETDATE(),StartTime)>0 AND ContestType<>0))
					RETURN 12
			
			ELSE IF EXISTS (SELECT * FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0)	--���˱�����ͬ����
			BEGIN 
				SELECT @MatchID=MatchID,@RoomID=RoomID,@GameNameID=GameNameID FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0
				IF (SELECT UpdateTime FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID ) >= DATEADD(MI, - 3, GETDATE())	--�ѿ����ķ���
				BEGIN 
					
					SET @rs = 1
				END 
				ELSE --�����ѹر�
				BEGIN 
					DELETE FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0
					SET @rs = -1
				END 
			END 
			IF @rs=-1--���˱�����ͬ����
			BEGIN
				IF @ContestType=0
					SET @rs=0
				ELSE IF @ContestType=1		--��ʱ����ѡ������δ���ķ��䱨��
				BEGIN 
					IF EXISTS (SELECT * FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=2)
					BEGIN 
						RETURN 7	--������������
					END 
					ELSE IF EXISTS (SELECT * FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=1)
					BEGIN 
						RETURN 4	--�����ѿ�ʼ
					END 
					ELSE
					BEGIN 
						SELECT TOP 1 @RoomID =ISNULL(a.RoomID,0) from TGameRoomBase a inner join TGameRoomInfo b on a.RoomID=b.RoomID AND b.UpdateTime >= DATEADD(MI, - 3, GETDATE())
						WHERE a.ContestID=@ContestID
						IF @RoomID<>0
						BEGIN 
							SELECT @GameNameID=GameNameID FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID
							INSERT INTO TContestLog(ContestID,RoomID,ContestType,ContestStat) VALUES(@ContestID,@RoomID,@ContestType,0)
							Select @MatchID=ident_current('TContestLog')
							SET @bNotifyG=1
							SET @rs = 1
						END 
						ELSE 
							RETURN 9
					END 
				END 
				ELSE IF @ContestType=2		--ѭ����
				BEGIN 
					--��鷿��״̬��
					DELETE FROM TContestCircleFreeRecord WHERE RoomID NOT IN (SELECT RoomID FROM dbo.TGameRoomInfo WHERE UpdateTime >= DATEADD(MI, - 3, GETDATE()))
					UPDATE TContestCircleFreeRecord SET FREE=0 WHERE FREE=1 AND BeginTime<GETDATE()
					IF EXISTS (SELECT * FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=1)
					BEGIN 
						IF EXISTS (SELECT * FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=2)
							RETURN 7
						ELSE
						BEGIN 
							SELECT @RoomID=RoomID ,@MatchID=MatchID FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=1
							SET @rs = 1
						END  
					END 
					ELSE IF NOT EXISTS (SELECT * FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=0)
						RETURN 9
					ELSE 
					BEGIN 
						SELECT TOP 1 @RoomID=RoomID FROM TContestCircleFreeRecord WHERE ContestID=@ContestID AND FREE=0
						--���㿪ʼʱ��
						IF (@BeginTime<GETDATE())
						BEGIN 
							DECLARE @Totalmi INT 
							DECLARE @CountTime INT 
							SET @Totalmi = DATEDIFF(mi,@BeginTime,GETDATE())
							SET @CountTime= @CircleTime-(@Totalmi%@CircleTime)
							SET @BeginTime = DATEADD(mi,(@Totalmi+@CountTime),@BeginTime)
							
							UPDATE dbo.TContestInfo SET StartTime=@BeginTime WHERE ContestID=@ContestID
						END 
						
						INSERT INTO TContestLog(ContestID,RoomID,ContestType,ContestStat) VALUES(@ContestID,@RoomID,@ContestType,0)
						Select @MatchID=ident_current('TContestLog')
						
						UPDATE TContestCircleFreeRecord SET FREE=1,BeginTime=@BeginTime,MatchID=@MatchID WHERE ContestID=@ContestID AND FREE=0 AND RoomID=@RoomID 
						
						SET @bNotifyG=1
						
						SET @rs = 1
					END 
				END 
			END 
			
			IF @rs=1	--�����ɹ���ʼ��Ǯ
			BEGIN 
				--��Ǯ
				SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
				SELECT @GameNameID=GameNameID FROM dbo.TGameRoomInfo WHERE RoomID=@RoomID
				IF @EnterFeeType=0
				BEGIN 
					SELECT @Money=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
					UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney-@EnterFee WHERE UserID=@UserID
					INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
                            VALUES  ( @UserID ,@UserName ,@Money ,-@EnterFee ,21 ,GETDATE() ,'��������')
				END 
				ELSE IF @EnterFeeType=1
				BEGIN
					SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
					update Web_Users set Jewels=Jewels-@EnterFee where UserID=@UserID
					INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
					VALUES(@UserID,@JewelsBefore,-@EnterFee,4,GETDATE(),'��������������ʯ')
				END 
				INSERT INTO dbo.TSignUp( GameNameID ,ContestID ,RoomID ,UserID ,SignState ,SignTime ,EntryFee,MatchID)
					VALUES ( @GameNameID,@ContestID,@RoomID,@UserID,0,GETDATE(),@EnterFee,@MatchID)
				IF (SELECT COUNT(*) FROM dbo.TSignUp WHERE ContestID=@ContestID AND SignState=0 )>=@UpPeople
					UPDATE dbo.TSignUp SET SignState=2 WHERE ContestID=@ContestID AND SignState=0
			END 
        END 
        ELSE IF @TypeID = 1 AND @rs=-1
        BEGIN 
			IF NOT EXISTS (SELECT * FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID AND (SignState=1 OR SignState=2 OR SignState=0))
				RETURN 5
			ELSE IF EXISTS (SELECT * FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID AND SignState=1)
				RETURN 6
			ELSE 
			BEGIN 
				IF (SELECT SignState FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID)=2
					UPDATE dbo.TSignUp SET SignState=0 WHERE ContestID=@ContestID AND SignState=2
				DELETE FROM dbo.TSignUp WHERE UserID=@UserID AND ContestID=@ContestID
				IF @EnterFeeType=0
				BEGIN 
					SELECT @Money=WalletMoney+BankMoney FROM dbo.TUserInfo WHERE UserID=@UserID
					SELECT @UserName=UserName FROM dbo.TUsers WHERE  UserID=@UserID
					UPDATE dbo.TUserInfo SET WalletMoney=WalletMoney+@EnterFee WHERE UserID=@UserID
					INSERT  INTO dbo.Web_MoneyChangeLog( UserID ,UserName ,StartMoney ,ChangeMoney ,ChangeType ,DateTime ,Remark)
							VALUES  ( @UserID ,@UserName ,@Money ,@EnterFee ,22 ,GETDATE() ,'��������')
				END 
				ELSE IF @EnterFeeType=1
				BEGIN
					SELECT @JewelsBefore=Jewels+LockJewels FROM Web_Users where UserID=@UserID
					update Web_Users set Jewels=Jewels+@EnterFee where UserID=@UserID
					INSERT INTO TJewelChangeLog(UserID,StartJewel,ChangeJewel,ChangeType,DateTime,Remark) 
					VALUES(@UserID,@JewelsBefore,@EnterFee,5,GETDATE(),'��������������ʯ')
				END 
                SET @rs=2
			END 
        END 
        
        SELECT @Money=WalletMoney FROM dbo.TUserInfo WHERE UserID=@UserID
        SELECT @Jewels=Jewels FROM Web_Users where UserID=@UserID
        
        SELECT @yNum=COUNT(*) FROM dbo.TSignUp WHERE MatchID=@MatchID
        
        SELECT @Money AS UserMoney,@RoomID AS RoomID ,@BeginTime AS BeginTime,@bNotifyG AS bNotifyG,@yNum AS yNum,@MatchID AS MatchID,@Jewels AS Jewels
        
        RETURN @rs
    END
