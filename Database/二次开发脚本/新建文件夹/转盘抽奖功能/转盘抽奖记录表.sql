go
if exists(select * from dbo.sysobjects where id = object_id('Web_LuckDrawRecordInfo'))  
drop table Web_LuckDrawRecordInfo  
go
CREATE TABLE Web_LuckDrawRecordInfo
(
		ID int IDENTITY(1,1),
		UserID int not null, --�û�ID
		LuckDrawType INT NOT NULL,--�齱���
		LuckDrawValue INT not null,--���
		LuckDrawTime datetime not null,--�齱ʱ��
)

go
if exists(select * from dbo.sysobjects where id = object_id('Web_LuckDrawRecordHistoryInfo'))  
drop table Web_LuckDrawRecordHistoryInfo  
go
CREATE TABLE Web_LuckDrawRecordHistoryInfo
(
		ID int IDENTITY(1,1),
		UserID int not null, --�û�ID
		LuckDrawType INT NOT NULL,--�齱���
		LuckDrawValue INT not null,--���
		LuckDrawTime datetime not null,--�齱ʱ��
)