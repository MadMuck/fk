go
if exists(select * from dbo.sysobjects where id = object_id('TWeiXinRegister'))  
drop table TWeiXinRegister  
go
CREATE TABLE TWeiXinRegister(
UserID int,			   --�û�ID
UionID  nvarchar(64),  --΢��,QQ��ʶ��
NickName nvarchar(50), --΢�ŵ�¼�ǳ�
Sex tinyint,		   --�Ա�
HeadUrl nvarchar(256), --ͼ���ַ
)
