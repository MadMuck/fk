USE [msdb]
GO

/****** Object:  Job [定期清理战绩]    Script Date: 08/11/2017 10:24:40 ******/
BEGIN TRANSACTION
DECLARE @ReturnCode INT
SELECT @ReturnCode = 0
/****** Object:  JobCategory [[Uncategorized (Local)]]]    Script Date: 08/11/2017 10:24:40 ******/
IF NOT EXISTS (SELECT name FROM msdb.dbo.syscategories WHERE name=N'[Uncategorized (Local)]' AND category_class=1)
BEGIN
EXEC @ReturnCode = msdb.dbo.sp_add_category @class=N'JOB', @type=N'LOCAL', @name=N'[Uncategorized (Local)]'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

END

DECLARE @jobId BINARY(16)
EXEC @ReturnCode =  msdb.dbo.sp_add_job @job_name=N'定期清理战绩', 
		@enabled=1, 
		@notify_level_eventlog=0, 
		@notify_level_email=0, 
		@notify_level_netsend=0, 
		@notify_level_page=0, 
		@delete_level=0, 
		@description=N'无描述。', 
		@category_name=N'[Uncategorized (Local)]', 
		@owner_login_name=N'SC-201612201802\Administrator', @job_id = @jobId OUTPUT
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
/****** Object:  Step [定期整理战绩]    Script Date: 08/11/2017 10:24:41 ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'定期整理战绩', 
		@step_id=1, 
		@cmdexec_success_code=0, 
		@on_success_action=1, 
		@on_success_step_id=0, 
		@on_fail_action=2, 
		@on_fail_step_id=0, 
		@retry_attempts=0, 
		@retry_interval=0, 
		@os_run_priority=0, @subsystem=N'TSQL', 
		@command=N'	DECLARE @Temp TABLE(CreateTime DATETIME,ID INT,MasterID INT,GameID INT,ToTalCount INT,RoomID INT,DeskID INT,Pass NVARCHAR(20),ClubID INT)

	INSERT INTO @Temp(ID,Pass,GameID,MasterID,ToTalCount,CreateTime,RoomID,DeskID,ClubID)
	SELECT ID,szDeskPassWord,GameID,MasterID,BuyGameCount,BuyTime,RoomID,TableNumber,ClubID FROM dbo.TBuyGameDeskRecord
		WHERE DATEDIFF(dd,BuyGameCount,GETDATE())>=2 AND RoomID=255 AND TableNumber=255

	DECLARE @DeskPass NVARCHAR(20)
	DECLARE db_cursor CURSOR SCROLL FOR select Pass from @Temp
	OPEN db_cursor
	FETCH FIRST FROM db_cursor INTO @DeskPass
	WHILE @@fetch_status=0
	BEGIN
		EXEC dbo.SP_CostJewels @DeskPass = @DeskPass,@Type = 1
		FETCH NEXT FROM db_cursor INTO @DeskPass
	END
	CLOSE db_cursor
	DEALLOCATE db_cursor

	DELETE FROM dbo.TBuyGameDeskRecord WHERE szDeskPassWord IN (SELECT Pass FROM @Temp)
	INSERT INTO dbo.TBuyGameDeskRecordDelete(ID ,DeskPassword ,GameID ,DeskMaster ,ToTalCount ,DeleteTime ,CreateTime ,RoomID ,DeskID,ClubID)
	SELECT ID,Pass,GameID,MasterID,ToTalCount,GETDATE(),CreateTime,RoomID,DeskID,ClubID FROM @Temp
	DELETE FROM dbo.TGameCutNetRecord WHERE DeskPassWord IN (SELECT Pass FROM @Temp)
	DELETE FROM dbo.TRecordGameMSG WHERE DeskPassWord IN (SELECT Pass FROM @Temp)

	DELETE FROM TDeskTotleRecord WHERE DATEDIFF(dd,CreateTime,GETDATE())>=2 AND DeskPassword NOT IN (SELECT szDeskPassWord FROM dbo.TBuyGameDeskRecord)

	DELETE FROM TUserGameRecord WHERE DATEDIFF(dd,StartTime,GETDATE())>=2 AND DeskPassword NOT IN (SELECT szDeskPassWord FROM dbo.TBuyGameDeskRecord)

', 
		@database_name=N'NEWEST', 
		@flags=0
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_update_job @job_id = @jobId, @start_step_id = 1
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobschedule @job_id=@jobId, @name=N'清理战绩', 
		@enabled=1, 
		@freq_type=4, 
		@freq_interval=1, 
		@freq_subday_type=1, 
		@freq_subday_interval=0, 
		@freq_relative_interval=0, 
		@freq_recurrence_factor=0, 
		@active_start_date=20170427, 
		@active_end_date=99991231, 
		@active_start_time=0, 
		@active_end_time=235959, 
		@schedule_uid=N'26278665-6ade-4611-8d17-95b219f36dfb'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @jobId, @server_name = N'(local)'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
COMMIT TRANSACTION
GOTO EndSave
QuitWithRollback:
    IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION
EndSave:

GO


