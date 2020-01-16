go
if exists(select * from dbo.sysobjects where id = object_id('TClubPlayInfo'))  
drop table TClubPlayInfo  
go

CREATE TABLE TClubPlayInfo
(
	ID INT IDENTITY(1,1),
	GroupID int,    --¥��ID
	GroupName nvarchar(64),  --¥������
	GroupNotice nvarchar(MAX), --¥�㹫��
	ClubID int,
	GameID int,
	PlayCount int, -- �������
	bFinishCondition int, -- ��������
	bPayType int, --֧�����
	bPostionLimit int, --�Ƿ�����λ
	bPlayNumber int, --��Ϸ����
	bMidEnter int, --�Ƿ�������;����
	bMinutes int, --����ʱ��
	DeskConfig varbinary(MAX), -- ��Ϸ�淨
	PreventCheating int,				--0�����������׼��  1���������׼��
	CurrencyType	int			--���㷽ʽ   0--��ҽ���    1---��������
)