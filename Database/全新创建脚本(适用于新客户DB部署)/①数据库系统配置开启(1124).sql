
/***
* 老客户确保此脚本已经运行过
* 新客户确保运行此脚本后，手动将SQL SERVER重启
***/
/**************************开启SQL SERVER2005系统配置项*****************************/
EXEC sp_configure 'show advanced options', '1' ;
GO
RECONFIGURE ;
GO
EXEC sp_configure 'clr enabled', '1'
GO
RECONFIGURE ;
GO

----- 手动重启服务器，手动重启服务器，手动重启服务器，手动重启服务器
----- 不要启用 xp_cmdshell 
