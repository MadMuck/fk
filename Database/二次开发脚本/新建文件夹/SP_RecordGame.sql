USE [58gw]
GO
/****** Object:  StoredProcedure [dbo].[SP_RecordGame]    Script Date: 05/15/2019 15:13:06 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[SP_RecordGame]
@DeskPassWord varchar(20),   --桌子密码 
@DeskUserID NVARCHAR(1000),
@UserScore NVARCHAR(1000),
@GameCount INT,
@GameSN NVARCHAR(20),
@GameType INT ,
@GameID INT
as
begin
	set nocount ON
	
	DECLARE @ID INT 
	DECLARE @ClubID INT 
	declare @GroupID int
	DECLARE @MasterID INT 
	DECLARE @CreateTime DATETIME
	DECLARE @StartTime DATETIME
	SET @ID=0
	IF @GameType=1
	BEGIN 
		SELECT @ID=ISNULL(ID,0),@CreateTime=BuyTime,@ClubID=ClubID,@GroupID=isnull(GroupID,0),@MasterID=MasterID FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord=@DeskPassWord
		IF @ID=0
			RETURN 0
	END 
	
	IF OBJECT_ID('tempdb.dbo.#temp_TGameRecord') IS NOT NULL 
    BEGIN
        DROP TABLE #temp_TGameRecord
    END
    CREATE TABLE #temp_TGameRecord
	(
		ID INT ,
		UserID INT ,
		ScordNew INT
	)
    INSERT INTO #temp_TGameRecord(UserID,ScordNew,ID) SELECT a.RS,b.RS,a.ID FROM dbo.Split(@DeskUserID,',') a,dbo.Split(@UserScore,',') b WHERE a.ID=b.ID

	IF @DeskPassWord<>''
	BEGIN 
		UPDATE dbo.TBuyGameDeskRecord SET RunCount=RunCount+1 WHERE szDeskPassWord=@DeskPassWord
	END 

	IF NOT EXISTS (SELECT * FROM TDeskTotleRecord WHERE ID=@ID) 
	BEGIN 
		IF (@GameType=1)
		BEGIN 
			INSERT INTO TDeskTotleRecord(ID,DeskPassword,DeskUserID,UserScore,CreateTime,GameID,ClubID,MasterID,GroupID)
			SELECT @ID,@DeskPassWord,UserID,ScordNew,@CreateTime,@GameID,@ClubID,@MasterID,@GroupID FROM #temp_TGameRecord WHERE UserID<>0
			update TDeskTotleRecord set Winner=0 where DeskPassword=@DeskPassWord
			declare @MaxUserScore int
			select @MaxUserScore=MAX(UserScore) from TDeskTotleRecord where @DeskPassWord=DeskPassword
			update TDeskTotleRecord set Winner=1 where @MaxUserScore=UserScore and @DeskPassWord=DeskPassword and @MaxUserScore<>0 and Winner=0
			
		END 
		INSERT INTO TUserGameRecord(ID,GameID,DeskPassword,DeskUserID,UserScore,PlayCount,StartTime,RecordNum,GameType)
		SELECT @ID,@GameID,@DeskPassWord,UserID,ScordNew,@GameCount,GETDATE(),(@GameSN+REPLACE(STR(ID-1,2,0),' ','0')),@GameType FROM #temp_TGameRecord WHERE UserID<>0

	END 
	ELSE 
	BEGIN  
		UPDATE b SET b.UserScore=b.UserScore+a.ScordNew,b.PlayCount=b.PlayCount+1 FROM #temp_TGameRecord a ,TDeskTotleRecord b WHERE a.UserID=b.DeskUserID AND b.ID=@ID
		
		INSERT INTO TDeskTotleRecord(ID,DeskPassword,DeskUserID,UserScore,CreateTime,GameID,ClubID,MasterID)
		SELECT @ID,@DeskPassWord,UserID,ScordNew,@CreateTime,@GameID,@ClubID,@MasterID FROM #temp_TGameRecord 
		WHERE UserID<>0 AND UserID NOT IN (SELECT DeskUserID FROM TDeskTotleRecord WHERE ID=@ID)
		
		INSERT INTO TUserGameRecord(ID,GameID,DeskPassword,DeskUserID,UserScore,PlayCount,StartTime,RecordNum,GameType)
		SELECT @ID,@GameID,@DeskPassWord,UserID,ScordNew,@GameCount,GETDATE(),(@GameSN+REPLACE(STR(ID-1,2,0),' ','0')),@GameType FROM #temp_TGameRecord WHERE UserID<>0
	END 
	
end