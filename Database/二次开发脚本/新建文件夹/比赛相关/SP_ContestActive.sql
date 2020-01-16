
GO
/****** Object:  StoredProcedure [dbo].[SP_ContestActive]    Script Date: 02/21/2017 17:05:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROC [dbo].[SP_ContestActive]
    @MatchID INT ,
    @RoomID  INT ,
    @ContestID INT
AS 
    BEGIN
        SET NOCOUNT ON ;
		
		IF NOT EXISTS (SELECT * FROM dbo.TContestCircleFreeRecord WHERE ContestID=@ContestID AND MatchID=@MatchID AND FREE=1 AND RoomID=@RoomID)
			RETURN 1		--û�ҵ������¼
       
       DECLARE @BeginTime DATETIME
       
       SELECT @BeginTime=BeginTime FROM dbo.TContestCircleFreeRecord WHERE ContestID=@ContestID AND MatchID=@MatchID AND FREE=1 AND RoomID=@RoomID
       
       IF @BeginTime<=GETDATE()
       BEGIN 
			RETURN 2		--����ʱ���ѹ�������ʧ��
	   END 
       ELSE 
       BEGIN 
			SELECT @BeginTime AS BeginTime
			RETURN 0		--����ɹ�
		END 
    END
