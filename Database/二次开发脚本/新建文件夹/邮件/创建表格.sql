
CREATE TABLE TMailList(
	MailID		INT IDENTITY(1,1),
	UserID		INT ,		--0��ʶ���͸�������
	SenderName	NVARCHAR(25),
	MailTitle	NVARCHAR(25),
	MailMessage	NVARCHAR(650),
	SendMoney	BIGINT ,
	SendJewels	INT ,
	SendLotteries	INT ,
	SendTime	DATETIME,
	IsGet		INT	,		--0�޸���δ����1�޸����Ѷ���2�и���δ����3�и����Ѷ�δ��ȡ��4�и����Ѷ�����ȡ
	IsSend		BIT	,		--0δ֪ͨ��1-��֪ͨ
	KeepTime	INT			--�������� 0��ʶ������
	)

CREATE TABLE TMailtoAllUserLog(
	MailID		INT ,
	UserID		INT ,
	IsGet		INT			--0�޸���δ����1�޸����Ѷ���2�и���δ����3�и����Ѷ�δ��ȡ��4�и����Ѷ�����ȡ��5��ɾ��
	)
	
CREATE TABLE TMailDeleteLog(
	MailID		INT ,
	UserID		INT 
	)
	
--ALTER TABLE dbo.Web_Config ADD MailKeepTime	INT DEFAULT(7) NOT NULL 


CREATE TABLE TSysMessage(
	ID				INT IDENTITY(1,1),
	MContent		NVARCHAR(150),		--��������
	NextSendTime	DATETIME,			--�´ι㲥ʱ�䣬������Ϣ�ǰʱ��
	KeepTime		DATETIME,			--����ʱ�䣬�ڴ�ʱ��ǰ�û���������ȡ����Ϣ
	IntervalTime	INT ,				--���ͼ��������
	IsSend			BIT					--�Ƿ��ѹ㲥
	)