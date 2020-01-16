#pragma once
#include "GameDesk.h"
#include <afxmt.h>
#include <deque>
#include<list>

//��ʱ�� ID
#define IDT_USER_CUT				1L				//���߶�ʱ�� ID
//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SAFE						11				//��Ϸ��ȫ����
#define GF_NO_CALL_SCORE			13				//���˽з�
#define GF_TERMINATE				14				//����ֹͣ��Ϸ
#define GF_AHEAD_END				15				//��ǰ����

//������  
//��ʱ������
#define		TIME_ONE_SECOND			31				//һ���ʱ��
#define		TIME_FREE_TIME			32				//���ж�ʱ��
#define		TIME_XIAZHU_TIME		33				//��ע��ʱ��
#define     TIME_ROLL_TIME          34              //ҡ����ʱ��
#define		TIME_SEND_TIME			35				//���ƶ�ʱ��
#define		TIME_GAME_FINISH		36				//��Ϸ������ʱ��
#define		TIME_ROB_NT				37				//��ׯ��ʱ��


#define MAX_NAME_INFO                   256
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // ��ϷID

//*****************************************************************************************
//��Ϸ����
class CServerGameDesk : public CGameDesk
{
private:
	bool				m_bIsSuperControl;								//�Ƿ��ǳ����û����ƽ��
	bool                m_bShowChouMa;									//�Ƿ���ʾ����

	BYTE				m_byRobNTtime;									//��ׯ�׶�ʱ��
	BYTE				m_byFreeTime;									//���н׶�ʱ��
	BYTE				m_byBetTime;									//��ע�׶�ʱ��
	BYTE                m_byRolldiceTime;								//ҡ���׶�ʱ��
	BYTE				m_byOpenCardTime;								//�����׶�ʱ��
	BYTE				m_byWinPrizeStart;								//������ʼλ��
	BYTE        		m_byWinPrize[3];								//���ֿ���λ��
	BYTE				m_byBetWaitTime;								//����ע�׶η��Ƶȴ�ʱ��
	BYTE				m_byOpenAwardWaitTime;							//����ע�׶η��Ƶȴ�ʱ��
	BYTE        		m_byMaxNtPeople;								//�����ׯ����
	BYTE				m_byNtStation;									//ׯ��λ��
	BYTE				m_byNuoLimit;									//Ų����
	BYTE				m_byDiceNum;									//������
	int					m_iPlayingMethod;								//�淨
	
	bool				m_bIsRobotWinAndLostAutoCtrl;					//�Ƿ�����������Ӯ�Զ�����
	//�������ڸ�������ӮǮ���ʿ���
	int					m_iRobotWinLuckyAtA1;							/*<������������1ӮǮ�ĸ���*/
	int					m_iRobotWinLuckyAtA2;							/*<��������Ӯ���ƣ�������������2ӮǮ�ĸ���*/
	int					m_iRobotWinLuckyAtA3;							/*<��������Ӯ���ƣ�������������3ӮǮ�ĸ���*/
	int					m_iRobotWinLuckyAtA4;							/*<��������Ӯ���ƣ�������������4ӮǮ�ĸ���*/

	long long			m_iRobotWinMoneyA1;								//<������ӮǮ����1
	long long			m_iRobotWinMoneyA2;								//<������ӮǮ����2
	long long			m_iRobotWinMoneyA3;								//<������ӮǮ����3
	long long			m_iResetRobotHaveWinMoney;						//<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ

	int					m_iNtPlayCount;									//ׯ����ׯ����
	int					m_iNtPlayLimite;								//ׯ����ׯ��������
	long long           m_lSingleNoteLimit;                             //��������ע����
	long long           m_lRemainedNotes;                               //ʣ����ע��
	long long			m_iApplyNtLimite;								//��ׯ����
	long long			m_iUserBetSumLimite;							//��������ע����	
	long long			m_iBetAreaLimite[BET_ARES];						//��ע����������

	int					m_iRemaindTime;			//�׶�ʣ��ʱ��

	//��Ϸ�ܵ���ע(������ʵ��Һͻ�����)
	long long			m_i64UserBetCount[PLAY_COUNT][BET_ARES];		//�������ÿ���������ע���
	long long			m_i64LastBet[PLAY_COUNT][BET_ARES];				//��������ϰ���Ϸ�е���ע���
	long long           m_i64AreaGain[PLAY_COUNT][BET_ARES];			//�������ÿ���������Ӯ���
	long long			m_i64AreaBetAllSum[BET_ARES];					//�����������ע
	long long			m_i64UserBetSum[PLAY_COUNT];					//����ҵ�����ע	
	long long			m_i64SumAllBet;									//����ע��
	long long			m_iAreaBetLimite;								//������ע����

	//��ʵ��ҵ���ע�������������ˣ�
	long long			m_i64PeopleBetCount[PLAY_COUNT][BET_ARES];		//����ʵ�����ÿ���������ע���
	long long			m_i64PeopleBetSum[PLAY_COUNT];					//����ʵ��ҵ�����ע	
	long long			m_i64PeopleAreaBetAllSum[BET_ARES];				//���������ʵ��ҵ�����ע
	long long			m_i64PeopleSumAllBet;							//��ʵ�������ע��

	//�Y����Ϣ
	long long     		m_i64PlayerGrade[PLAY_COUNT];					//��ҳɼ�
	long long     		m_i64PlayCoin[PLAY_COUNT];						//�����Ӯ���
	long long     		m_i64ReturnMoney[PLAY_COUNT];					//�����Ľ����
	long long			m_i64ChangePlayCoin[PLAY_COUNT];				//��˰����ҵĵ÷�

	deque<STruOpenAward> m_dstAward;									//������¼
	vector<long>		 m_vlSuperID;									//���泬����ҵ�����
	vector<BYTE>      	 m_blNtWaitList;								//��ׯ�б�

	//���˿�����ʾ
	BYTE                m_bSuperDeskStation;							//�����û���λ��
	bool				m_bShowRobotBet;								//�����û����ƻ�������ע��ʾ

	int					m_iRunTime[100];								//�ܵ�ʱ������

	bool				m_bXiaZhuang;
	CString				dwjlog;

	//��������ֵ
	int					G_iChouMaValue[BETSTYLE];

	UserNuoData			m_sUserNuoData[PLAY_COUNT][NUO_COUNT];			//���Ų���

	long long			m_i64UserBetCountEx[PLAY_COUNT][BET_ARES];		//�������ÿ���������ע���ex(Ųע֮��ģ�����Ųע�൱����ע��ֻ�ǲ����ӵ�������עֵ�Ų֮������ׯ����Ǯ��������Ǯ��ׯ�ң���������û��ϵ)

	BYTE				m_byNuoCount[PLAY_COUNT];						//���Ų����

	int					m_iChouMaCount[BET_ARES][BETSTYLE];				//�����¼��ÿ������ÿ���������˼���

	long long			m_i64WinMoney[PLAY_COUNT];						//�����Ӯ��¼

	//��������
public:
	//���캯��
	CServerGameDesk(); //����������ʱ��һ�ε���
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:

	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����(�ͷ����˳�ʱ����)
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ���ͷ����˳�ʱ�ڶ��ε��ã�
	virtual bool IsPlayGame(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	///���ش�����Ϣ���ͷ�������ʱ�ڶ��ε��ã�
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);

	//�û��뿪��Ϸ��(�ͷ����˳�ʱ��һ�ε���)
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	/// �������
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	//�û������뿪
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

	//��ȡ��Ϸ״̬��Ϣ���ͷ�������ʱ�����ε��ã�
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬���ͷ����˳�ʱ���ã�
	virtual bool ReSetGameState(BYTE bLastStation);

	//����INI�ļ�������������ʱ�ڶ��ε��ã�
	virtual bool InitDeskGameStation();

	//������
public:
	//������Զ���ʼ���ɷ���˿���
	BOOL StartGame();
	//��ȡ��̬����
	BOOL LoadIni();
	//������ע
	BOOL LoadExtIni(int iRoomID);
	//������������
	void ResetGameData();
	//ɾ�����ж�ʱ��
	void KillAllTimer();

	//������Ϸ��ʱ��
	void	SetGameTimer(UINT uTimerID, BYTE byTime);
private:
	//��ȡ���ʣ�����Ǯ
	long long GetUserRemaindMoney(BYTE byDeskStation);

	//��ȡ�����ע�ܶ�
	long long GetUserBetCount(BYTE byDeskStation);

	//�������������ׯ��Ϣ
	void	DealUserApplyNt(void *pBuffer);

	//�������������ׯ��Ϣ
	void	DealUserApplyLeaveNt(void *pBuffer);

	//���������ע
	void	DealUserBet(void *pBuffer);

	//���������ѹ
	void DealContinueBet(void *pBuffer);

	//��������ע
	void DealClearBet(void *pBuffer);
	//�Ƿ�����ׯ�б���
	bool	IsInWaitNtList(BYTE byDeskStation);

	//����ҡ����Ϣ
	void SendRolldice();
	//���Ϳ�����Ϣ
	void SendFree();

	//���Ϳ�����Ϣ
	void SendAllCard();

	//���Ϳ�����¼
	void SendAwardRecord(BYTE );

	//�ж���ѡ��������ֵΪ����
	int shuangxx(BYTE winArea[],BYTE a, BYTE b);

	////�ж���ѡ��
	int sanxx(BYTE winArea[],BYTE a,BYTE b,BYTE c);

	//�жϱ���
	int isBaozi(BYTE winArea[],BYTE a);
	//ͳ����ҽ��
	void accountPlayCoin();

	//����
	void openAward();

	//��ס·�ӣ�72��
	void rememberLuzi();

	//�õ�����ʣ����ע��
	long long RemainingBettingNumber(BYTE);

	//�����û���֤
	void	SuperUserExamine(BYTE bDeskStation);

	//�������û����ƽ��
	void	DealSuperUserMessage(void *pBuffer);

	//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
	bool	IsSuperUser(BYTE byDeskStation);

	//�������Զ�����
	void RobotAutoControl();

	//��ͷ��˷�����ע����(���˿���)
	void SendBetDataToClient(BYTE bDeskStation);
	//��ׯ��
	bool ChangeNt(BYTE byLastNTStation);
	////����ׯ��
	//void ClearNt();
	//������ׯ�б�
	void ClearNtList();
	//��ׯ�ȴ�
	void NoNtWait();
	//��¼��������Ӯ���
	void RecordRobotWinMoney();

	//�ܷ񵽸��֣���ͨ��ҳ������ֲ���ҳ������ֲ����ֳ������������ҳ��ȶ����ܵ����֣�ֻ�д���������ֳ����ԣ�
	bool CanMinusPoint();

	//���������
	int GetDiceNum();

	//��ׯ���б��л�ȡ��ׯ��(��ׯ)
	bool OnGetZhuang();

	//���������ע��Ϣ����
	void DealUserBetInfo(BYTE byDeskStation, void *pData);

	//�������Ų����
	void DealUserNuo(BYTE byDeskStation, void *pData);

	//���������ע����
	long long GetAreaBetLimite(int iArea);

	//��ע�����Ƿ���Ч
	bool GetAreaBetVaild(int iArea);

	//֪ͨ��Ϸ����
	void NotiGameCount(BYTE byDeskStation = 255);

	//���������������
	void RandDice();

	//���ĳ������Ľ��,���˷��ر�����û�з���-1
	//�˴����ؽ����������10�����������ʱ��˴˽�����10
	int GetAresResult(int iArea);

	//����AI��Ӯ
	__int64	CountAIWinMoney();

	//���ô��������淨
	void SetPlayMode();

	//�Ƿ���type�淨
	bool IsPlayModeVaild(int type);

	//���ʹ����
	void SendScoreBoard();

private:
	//�޸Ľ�������1(ƽ̨->��Ϸ)
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//�޸Ľ�������2(ƽ̨->��Ϸ)
	virtual bool SetRoomPondEx(long long iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//�޸Ľ�������3(ƽ̨->��Ϸ)
	virtual bool SetRoomPondEx(long long iReSetAIHaveWinMoney);
	//����ʤ��
	/// �ж�
	virtual BOOL Judge();
	/// ʤ
	virtual BOOL JudgeWiner();
	/// ��
	virtual BOOL JudgeLoser();
	//GRM����
	//���½�������(��Ϸ->ƽ̨)
	/// @return ������ӮǮ��
	virtual void UpDataRoomPond(long long iAIHaveWinMoney){};
};

/******************************************************************************************************/
