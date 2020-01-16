go
if exists(select * from dbo.sysobjects where id = object_id('TWeiXinRegister'))  
drop table TWeiXinRegister  
go
CREATE TABLE TWeiXinRegister(
UserID int,			   --用户ID
UionID  nvarchar(64),  --微信,QQ标识码
NickName nvarchar(50), --微信登录昵称
Sex tinyint,		   --性别
HeadUrl nvarchar(256), --图像地址
)
