
/***
* �Ͽͻ�ȷ���˽ű��Ѿ����й�
* �¿ͻ�ȷ�����д˽ű����ֶ���SQL SERVER����
***/
/**************************����SQL SERVER2005ϵͳ������*****************************/
EXEC sp_configure 'show advanced options', '1' ;
GO
RECONFIGURE ;
GO
EXEC sp_configure 'clr enabled', '1'
GO
RECONFIGURE ;
GO

----- �ֶ��������������ֶ��������������ֶ��������������ֶ�����������
----- ��Ҫ���� xp_cmdshell 
