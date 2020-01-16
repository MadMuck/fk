#pragma once

#include "stdafx.h"

#include "GameContext.h"
#include "ProcedureController.h"

#include "MessageSender.h"
#include "ProcedureChangedDelegate.h"

#include "ExtensionGameRunningData.h"
#include "SpecInterceptionValidator.h"

#include "ClientMsgProxy.h"

#include "msgbase.h"

#include <map>



#define TIME_UPDATE					 10				// ��Ϸ����ѭ����ʱ��ID
#define TIME_AUTO_DISMISS			 11				// �Զ���ɢ��Ϸ�Ķ�ʱ��

const int TIME_STEP = 31;           /// ʱ��
const int TIME_STEP_TIME = 500;     /// 

const int TIME_NETWORK = 1000;      /// �����ӳ�ʱ��

/** ��Ϸ�������� */

struct CreateRoomCfg
{
	bool			m_bHuanSanzhang;//�Ƿ�����
	bool			m_bDingQue;		//�Ƿ�ȱ
	unsigned int	m_ucDiFen;		//�׷�

	CreateRoomCfg()
	{
		m_bHuanSanzhang = false;
		m_bDingQue = false;
		m_ucDiFen = 0;
	}
};


class CServerGameDesk;

class GameRunningData : public GameRunningDataDelegate
{
	CServerGameDesk *m_Desk;
public:
	GameRunningData(CServerGameDesk *desk);

	unsigned char GetDeskStationByID(long userID) const;
	long GetIDbyDeskStation(unsigned char deskStation) const;
	int GetRemainingGameCount() const;
	void SendDataToPlayer(PlayerPos::Type playerPos, void *data, int dataSize, int msgID);
	void SendDataToAll( void *data, int dataSize, int msgID);
	bool IsPlayerReadyToPlay(unsigned char deskStation) const;
	void PlayerGetReadyToPlay(unsigned char deskStation);
	int GetGameState() const;
	void SetGameState(unsigned char gameState);
};

/// ��Ϸ������
class CServerGameDesk : public CGameDesk
{
public:
	/// ���캯��
	CServerGameDesk(); 
	/// ��������
	virtual ~CServerGameDesk();

public:
	/// ��ʼ��Ϸ
	virtual bool	InitDeskGameStation();
	/// ��Ϸ��ʼ
	virtual bool	GameBegin(BYTE bBeginFlag);
	/// ��Ϸ����
	virtual bool	GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	/// �ж��Ƿ�������Ϸ
	virtual bool	IsPlayGame(BYTE bDeskStation);
	/// �����Ϣ������
	virtual bool	HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// ��Ϸ���ݰ�������
	virtual bool	HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// �û��뿪��Ϸ��
	virtual BYTE	UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);	
	//�����ı��û����������ĺ���
	virtual bool	CanNetCut(BYTE bDeskStation);
	//��Ҷ��ߴ���
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	virtual bool RecordUserMultiple(int UserID,int Multiple);

	bool CanBeginGame();

	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool	OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// ������Ϸ״̬
	virtual bool	ReSetGameState(BYTE bLastStation);
	/// ��ʱ����Ϣ
	virtual bool	OnTimer(UINT uTimerID);

	/// ��;��ɢ
	virtual void DissMissRoomBefore();

	//�޸Ľ�������1(ƽ̨->��Ϸ)
	/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//�޸Ľ�������2(ƽ̨->��Ϸ)
	/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
	/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//�޸Ľ�������3(ƽ̨->��Ϸ)
	/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

	//����ʤ��
	/// �ж�
	virtual BOOL Judge();
	/// ʤ
	virtual BOOL JudgeWiner();
	/// ��
	virtual BOOL JudgeLoser();

	bool IsVipRoom() const;

	int GetGameOwner() const;

	const CGameUserInfo *GetUserInfo(int pos) const;

	long GetUID(int playerPos) const;

	void SendDataToPlayer(int playerPos, void *data, int dataSize, int msgID);

	void SendDataToPlayers(void *data, int dataSize, int msgID);

	void SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);

	void CopyAllPlayerHandPai(uchar	byArHandPai[][HAND_CARD_NUM]);

	void OnProcedureChanged(int latest, int current);

	void OnBeforeProcedureStart(int latest, int current);

	int GetRoomID() const
	{
		if(IsVipRoom())
			return atoi(m_szDeskPassWord);
		return m_pDataManage->m_InitData.uRoomID;
	}

	int GetDeskID(BYTE playerPos) const
	{
		int ret = 99999;
		if(m_pUserInfo[playerPos])
		{
			ret = m_pUserInfo[playerPos]->m_UserData.bDeskNO;
		}
		return ret;
	}

	bool IsUserReady(BYTE playerPos) const;

	int GetGameState() const{
		return m_bGameStation;
	}

	void SetGameState(unsigned char gameState)
	{
		m_bGameStation = gameState;
	}

private:

	void GameInit();

	void GameClear();

	void GameReset();

	void Liquidate();

	void DoGameStatistic();

	void SelectNextBanker();

	void LoadConfig();

	void StartUpdateLoop(bool b);

	void SendScoreBoard(bool dismissRoom);

	// ��Ϸ��ʼ����֮��
	void OnProcedureGameStart();

	// һ����Ϸ��������(����)֮��
	void OnProcedureLiquidationFinish();

	void InitLogFile();

	void InitGameRule();

	void OnPlayerQuitInGame(BYTE deskStation);

	void UpdateRemainingGameCount();

	void GameDismissClean();

	GameContext *m_GameContext;

	SYSTEMTIME m_LatestUpdateTime;

	float m_TickInterval;

	bool m_GameStarted;

	bool m_Dismissed;

	ProcedureChangedDelegate<CServerGameDesk> m_ProcedureChangedDelegate;

	GameMessageSender<CServerGameDesk> m_MessageSender;

	int m_CurrentGameProcedureState; // ��ǰ����Ϸ����״̬

	bool m_TimerStarted;

	bool m_GameOwnerNotSet;

	bool m_UseLogFile;

	ClientMsgProxy<CServerGameDesk> m_ClientMsgProxy;

	GameRunningData m_GameRunningData;

	std::map<PlayerPos::Type, int> m_BecomeBankerTimes; // ÿ�������ׯ����(����������ׯ����)

	SpecInterceptionValidator m_InterceptionValidator;

	int m_WhatsGoingOn;

	//�ͷŷ�����Դʱ��¼�����Ϣ��֪ͨ��Ϸ���������Ϸ����
	virtual void ReleaseRoomBefore() ;

	void _putRoomInfo();

	//�ָ����䷢����Ϸ���ݵ���Ϸ������
	//iCount����������
	virtual void SetReturnGameInfo(int UserID,void* szUserGameInfo,int iSize,int iCount);

	__int64 TimeDiff(SYSTEMTIME left, SYSTEMTIME right);

public:

	zNewFileOperator		m_FileOperator[PLAY_COUNT];

	CString					m_RecordFilePath;		//�û�Ŀ¼

	void CloseRecord();								//�����ط�
		
	bool CreateNewRecord();							//�����ط�

	CreateRoomCfg			m_RoomCfg;
};

/******************************************************************************************************/
