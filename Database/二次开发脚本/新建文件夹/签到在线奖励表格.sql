--ǩ����������
CREATE TABLE TSignAward(
	SignDay INT,		--����ǩ������
	AwardMoney INT,		--ǩ�������������
	AwardJewels INT,	--ǩ��������ʯ����
	AwardLotteries INT  --ǩ��������ȯ����
	)

--���߽�������
CREATE TABLE TOnlineAward(
	OnlineID INT,		--���߽���ID��1��ʼ��������
	OnlineAward INT,	--���߽�������
	OnlineType INT,		--���߽������� 0-��� 1-��ʯ 2-��ȯ
	OnlineTime INT		--����ʱ�䣨�룩����С����
	)

--ǩ����¼
CREATE TABLE TSignAwardLog(
	UserID INT,				--�û�ID
	LastSignTime DATETIME,	--�ϴ�ǩ��ʱ��
	SignDay INT				--����ǩ������
	)

--���߽�����¼
CREATE TABLE TOnlineAwardLog(
	UserID INT,				--�û�ID
	LastGetTime DATETIME,	--�ϴ��콱ʱ��
	GetTime INT				--�콱����
	)