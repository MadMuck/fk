

--Ψһ��ʶ
ALTER TABLE TBuyGameDeskRecord ADD ID INT IDENTITY(1,1)
--���о���
ALTER TABLE TBuyGameDeskRecord ADD RunCount INT DEFAULT(0)
--��ϷID
ALTER TABLE TBuyGameDeskRecord ADD GameID INT
--��Ϸ����
ALTER TABLE TBuyGameDeskRecord ADD DeskConfig NVARCHAR(1000)
--���ѷ�ʽ
ALTER TABLE TBuyGameDeskRecord ADD PayType INT
--������ʯ
ALTER TABLE TBuyGameDeskRecord ADD Jewels INT
--������������λ���أ���������
ALTER TABLE dbo.TBuyGameDeskRecord ADD FinishCondition INT DEFAULT(0),PositionLimit INT DEFAULT(0),PlayerNum INT DEFAULT(0)
--��;����
ALTER TABLE dbo.TBuyGameDeskRecord ADD MidEnter INT DEFAULT(0)