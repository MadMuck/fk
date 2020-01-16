go
if exists(select * from dbo.sysobjects where id = object_id('Web_LuckDrawTypeInfo'))  
drop table Web_LuckDrawTypeInfo  
go

CREATE TABLE Web_LuckDrawTypeInfo
(
		LuckDrawType INT NOT NULL primary key,
		LuckDrawName Varchar(20) not null
)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�齱���0-���,1-����,2-����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_LuckDrawTypeInfo', @level2type=N'COLUMN',@level2name=N'LuckDrawType'
GO 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�齱�������' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_LuckDrawTypeInfo', @level2type=N'COLUMN',@level2name=N'LuckDrawName'
GO 

insert into Web_LuckDrawTypeInfo values(0,'���')
insert into Web_LuckDrawTypeInfo values(1,'����')
insert into Web_LuckDrawTypeInfo values(2,'����')

go
if exists(select * from dbo.sysobjects where id = object_id('Web_LuckDrawAwardInfo'))  
drop table Web_LuckDrawAwardInfo  
go
CREATE TABLE Web_LuckDrawAwardInfo
(
		ID int IDENTITY(1,1),
		LuckDrawType INT NOT NULL,--�齱���
		LuckDrawValue INT not null,--���
		LuckDrawprobability INT not null, -- �н�����
)

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�齱���0-���,1-����,2-����' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_LuckDrawAwardInfo', @level2type=N'COLUMN',@level2name=N'LuckDrawType'
GO 

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�齱���' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_LuckDrawAwardInfo', @level2type=N'COLUMN',@level2name=N'LuckDrawValue'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'�н�����(�ٷֱ�)' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Web_LuckDrawAwardInfo', @level2type=N'COLUMN',@level2name=N'LuckDrawprobability'
GO

insert into Web_LuckDrawAwardInfo values(0,100,30)
insert into Web_LuckDrawAwardInfo values(1,10,10)
insert into Web_LuckDrawAwardInfo values(2,1,10)
insert into Web_LuckDrawAwardInfo values(0,1000,20)
insert into Web_LuckDrawAwardInfo values(1,15,5)
insert into Web_LuckDrawAwardInfo values(2,2,5)
insert into Web_LuckDrawAwardInfo values(0,100,10)
insert into Web_LuckDrawAwardInfo values(1,10,5)
insert into Web_LuckDrawAwardInfo values(2,1,3)
insert into Web_LuckDrawAwardInfo values(2,0,2)