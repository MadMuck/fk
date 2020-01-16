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



#define TIME_UPDATE					 10				// 游戏更新循环定时器ID
#define TIME_AUTO_DISMISS			 11				// 自动解散游戏的定时器

const int TIME_STEP = 31;           /// 时间
const int TIME_STEP_TIME = 500;     /// 

const int TIME_NETWORK = 1000;      /// 网络延迟时间

/** 游戏正常结束 */

struct CreateRoomCfg
{
	bool			m_bHuanSanzhang;//是否换三张
	bool			m_bDingQue;		//是否定缺
	unsigned int	m_ucDiFen;		//底分

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

/// 游戏桌子类
class CServerGameDesk : public CGameDesk
{
public:
	/// 构造函数
	CServerGameDesk(); 
	/// 析构函数
	virtual ~CServerGameDesk();

public:
	/// 初始游戏
	virtual bool	InitDeskGameStation();
	/// 游戏开始
	virtual bool	GameBegin(BYTE bBeginFlag);
	/// 游戏结束
	virtual bool	GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	/// 判断是否正在游戏
	virtual bool	IsPlayGame(BYTE bDeskStation);
	/// 框架消息处理函数
	virtual bool	HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// 游戏数据包处理函数
	virtual bool	HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// 用户离开游戏桌
	virtual BYTE	UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);	
	//用来改变用户断线条件的函数
	virtual bool	CanNetCut(BYTE bDeskStation);
	//玩家断线处理
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	virtual bool RecordUserMultiple(int UserID,int Multiple);

	bool CanBeginGame();

	/// 获取游戏状态信息
	virtual bool	OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// 重置游戏状态
	virtual bool	ReSetGameState(BYTE bLastStation);
	/// 定时器消息
	virtual bool	OnTimer(UINT uTimerID);

	/// 中途解散
	virtual void DissMissRoomBefore();

	//修改奖池数据1(平台->游戏)
	/// @param bAIWinAndLostAutoCtrl 机器人控制输赢开关
	/// @return 是否成功
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//修改奖池数据2(平台->游戏)
	/// @param m_iAIWantWinMoney 机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
	/// @param m_iAIWinLuckyAt 机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
	/// @return 是否成功
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//修改奖池数据3(平台->游戏)
	/// @param iReSetAIHaveWinMoney 机器人输赢控制：重置机器人已经赢钱的数目
	/// @return 是否成功
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

	//控制胜负
	/// 判断
	virtual BOOL Judge();
	/// 胜
	virtual BOOL JudgeWiner();
	/// 输
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

	// 游戏开始流程之后
	void OnProcedureGameStart();

	// 一局游戏正常结束(结算)之后
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

	int m_CurrentGameProcedureState; // 当前的游戏过程状态

	bool m_TimerStarted;

	bool m_GameOwnerNotSet;

	bool m_UseLogFile;

	ClientMsgProxy<CServerGameDesk> m_ClientMsgProxy;

	GameRunningData m_GameRunningData;

	std::map<PlayerPos::Type, int> m_BecomeBankerTimes; // 每个玩家上庄次数(房主初次上庄不算)

	SpecInterceptionValidator m_InterceptionValidator;

	int m_WhatsGoingOn;

	//释放房间资源时绩录玩家信息，通知游戏保存玩家游戏数据
	virtual void ReleaseRoomBefore() ;

	void _putRoomInfo();

	//恢复房间发送游戏数据到游戏服务器
	//iCount数据总数量
	virtual void SetReturnGameInfo(int UserID,void* szUserGameInfo,int iSize,int iCount);

	__int64 TimeDiff(SYSTEMTIME left, SYSTEMTIME right);

public:

	zNewFileOperator		m_FileOperator[PLAY_COUNT];

	CString					m_RecordFilePath;		//用户目录

	void CloseRecord();								//结束回放
		
	bool CreateNewRecord();							//创建回放

	CreateRoomCfg			m_RoomCfg;
};

/******************************************************************************************************/
