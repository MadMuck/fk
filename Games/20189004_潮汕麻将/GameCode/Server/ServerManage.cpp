#include "StdAfx.h"
#include "ServerManage.h"
#include "GameDef.h"
#include "UpgradeMessage.h"

#include "GameContextCreator.h"
#include "ExtensionProcedureController.h"
#include "ExtensionPlayer.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionMessageDelegate.h"
#include "ExtensionTimer.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionActionValidator.h"
#include "ExtensionLiquidation.h"
#include "ExtensionLogger.h"
#include "ExtensionGameStatistic.h"

#include "ProcedureGameStart.h"
#include "ProcedureDistributeCard.h"
#include "ProcedureAfterGetCard.h"
#include "ProcedureNormalDrawCard.h"
#include "ProcedureInterceptCard.h"
#include "ProcedureDetermineIntercept.h"
#include "ProcedureWaitSendCard.h"
#include "ProcedureReadyHandDiscard.h"
#include "ProcedureLiquidation.h"
#include "ProcedureFanMa.h"


#include "ProcedureQiangGang.h"
#include "ProcedureDingQue.h"
#include "ProcedureHuanZhang.h"

#include "UtilTemplate.h"

#include "UpgradeMessage.h"
#include "AdapterUtil.h"

#include "PlayerActionDelegate.h"

#include "DrawCardValidator.h"
#include "SpecProcedureJumpDelegate.h"
#include "SpecScoreItemListDelegate.h"
#include "SpecCardScoreDelegate.h"
#include "ExtensionGameParam.h"
#include "SpecInterceptionValidator.h"
#include "SpecInterceptCardQuestionDelegate.h"
#include "SpecGameDirection.h"
#include "SpecCardSortingDelegate.h"

#include "ClientMsgProxy.h"

#include "Logger.h"
#include "Util.h"

#include <algorithm>

#include <time.h>

#include "SpecBeforeLiquidation.h"
#include "GangHandler.h"


GameRunningData::GameRunningData(CServerGameDesk *desk):m_Desk(desk)
{
}

unsigned char GameRunningData::GetDeskStationByID(long userID) const
{
	unsigned char ret = -1;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(m_Desk->m_pUserInfo[i] && m_Desk->m_pUserInfo[i]->m_UserData.dwUserID == userID)
		{
			ret = i;
			break;
		}
	}
	return ret;
}

long GameRunningData::GetIDbyDeskStation(unsigned char deskStation) const
{
	long userID = 0;
	if(m_Desk->m_pUserInfo[deskStation])
	{
		userID = m_Desk->m_pUserInfo[deskStation]->m_UserData.dwUserID;
	}
	return userID;
}

int GameRunningData::GetRemainingGameCount() const
{
	return m_Desk->m_iVipGameCount - m_Desk->m_iRunGameCount;
}

void GameRunningData::SendDataToPlayer(PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
{
	m_Desk->SendDataToPlayer(playerPos, data, dataSize, msgID);
}

void GameRunningData::SendDataToAll( void *data, int dataSize, int msgID)
{
	m_Desk->SendDataToPlayers(data, dataSize, msgID);
}

bool GameRunningData::IsPlayerReadyToPlay(unsigned char deskStation) const
{
	bool ret = false;
	if(m_Desk->m_pUserInfo[deskStation])
	{
		ret = USER_ARGEE == m_Desk->m_pUserInfo[deskStation]->m_UserData.bUserState;
	}
	return ret;
}

void GameRunningData::PlayerGetReadyToPlay(unsigned char deskStation)
{
	if(m_Desk->m_pUserInfo[deskStation])
	{
		m_Desk->m_pUserInfo[deskStation]->m_UserData.bUserState = USER_ARGEE;
	}
}

int GameRunningData::GetGameState() const{
	return m_Desk->GetGameState();
}

void GameRunningData::SetGameState(unsigned char gameState)
{
	m_Desk->SetGameState(gameState);
}


/*------------------------------------------------------------------------------*/
/**
* 构造函数	
*/
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN),
	m_GameContext(GameContextCreator::Create()),
	m_TickInterval(100), m_GameStarted(false), m_Dismissed(false),
	m_ProcedureChangedDelegate(this), m_MessageSender(this), 
	m_TimerStarted(false), m_GameOwnerNotSet(true), m_UseLogFile(false),
	m_ClientMsgProxy(this), m_GameRunningData(this), m_WhatsGoingOn(THING_MAX)
{
	m_bGameStation = GS_WAIT_ARGEE;

	MessageSender *sender = &m_MessageSender;

	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
	extGameRunningData->SetDelegate(&m_GameRunningData);
}
/*------------------------------------------------------------------------------*/
/**
* 析构函数
*/
CServerGameDesk::~CServerGameDesk(void)
{
	GameContextCreator::Destroy(m_GameContext);
}


/// 初始游戏
bool CServerGameDesk::InitDeskGameStation()
{
	LoadConfig(); // 先加载一次配置，之后每局游戏重新加载

	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	// 设置房间人数
	m_GameContext->SetPlayerCount(extGameParam->GetParam(GameParam_PlayerCount));

	m_iRunGameCount = 0;

	return true;
}

// 游戏过程执行之前
void CServerGameDesk::OnBeforeProcedureStart(int latest, int current)
{
	LOGGER_FILE(m_GameContext, "before procedure start, from "<<(int)latest<<" to "<<current);
	switch(current)
	{
	case ProcedureType::Liquidation: 
		{
			// 结算之前
			DoBeforeLiquidation(m_GameContext);

			OBJ_GET_EXT(m_GameContext, ExtensionGamePlayerConnector, extGamePlayerConnector);
			extGamePlayerConnector->Clear();

			OBJ_GET_EXT(m_GameContext, ExtensionPlayerActionDelegate, extActDelegate);
			extActDelegate->RemoveAll();

			OBJ_GET_EXT(m_GameContext, ExtensionTimer, extTimer);
			extTimer->Clear();
		}
		
		break;
	}
}

// 游戏过程执行之后
void CServerGameDesk::OnProcedureChanged(int latest, int current)
{
	LOGGER_FILE(m_GameContext, "procedure change from "<<(int)latest<<" to "<<current);

	if(current == ProcedureType_HuanZhang)
	{
		m_WhatsGoingOn = THING_CHANGE_NOTIFY;
	}
	else if(current == ProcedureType_DingQue)
	{
		m_WhatsGoingOn = THING_DING_QUE;
	}
	else if(current == ProcedureType::Idle)
	{
		m_WhatsGoingOn = THING_MAX;
	}
	else
	{
		m_WhatsGoingOn = THING_OUT_PAI;
	}

	if(current == ProcedureType::GameStart)
	{
		OnProcedureGameStart();
	}
	else if(current == ProcedureType::DistributeCard)
	{
		OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
		OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
		extGameRunningData->AddBanker(extBankerSelector->GetBankerPos()); // 增加坐庄次数
	}
	else if(current == ProcedureType::Idle && latest == ProcedureType::Liquidation)
	{
		// 结算过程结束（一局游戏结束）
		OnProcedureLiquidationFinish();
	}
}

void CServerGameDesk::GameInit()
{
	StartUpdateLoop(true);

	// 初始化随机数
	InitRandom();

	GameContext *context = m_GameContext;

	m_ClientMsgProxy.Init(m_GameContext);

	//OBJ_GET_EXT(m_GameContext, ExtensionGameDirection, extGameDir);
	//extGameDir->SetGameDirection(RefCntObj<GameDirection>(new SpecGameDirection(m_GameContext)));

	if(m_GameOwnerNotSet && IsVipRoom())
	{
		m_GameOwnerNotSet = false;
		// 设置房主
		context->SetGameOwner(GetGameOwner());
	}

	//创建房间第一局清除大结算数据
	if(IsVipRoom() && m_iRunGameCount == 0)
	{
		OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);
		extGameStat->OnGameReset();
	}

	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);

	extProcCtrl->SetProcedureChangedCallback(&m_ProcedureChangedDelegate);

	// 根据需求配置游戏流程
	extProcCtrl->SetProcedure(ProcedureType::GameStart, ProcedureGameStart(context, ProcedureGameStartJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::DistributeCard, ProcedureDistributeCard(context, ProcedureDistributeCardJumpDelegate()));

	extProcCtrl->SetProcedure(ProcedureType_HuanZhang, ProcedureHuanZhang(context));
	extProcCtrl->SetProcedure(ProcedureType_DingQue, ProcedureDingQue(context));

	extProcCtrl->SetProcedure(ProcedureType::NormalDrawCard, ProcedureNormalDrawCard(context, SpecDrawCardDelegate::Create(), ProcedureNormalDrawJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::AfterDrawCard, ProcedureAfterGetCard(context, ProcedureAfterGetCardJumpDelegate()));

	ProcedureInterceptCard interceptCard = ProcedureInterceptCard(context, ProcedureInterceptCardJumpDelegate());
	interceptCard.SetQuestionDelegate(RefCntObj<InterceptCardQuestionDelegate>(new SpecInterceptCardQuestionDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::InterceptCard, interceptCard);

	extProcCtrl->SetProcedure(ProcedureType::DetermineIntercept, ProcedureDetermineIntercept(context, SpecProcedureDetermineInterceptJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::AfterIntercept, ProcedureAfterGetCard(context, ProcedureAfterGetCardJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::DrawCardAfterKong, ProcedureNormalDrawCard(context, SpecDrawCardDelegate::Create(), ProcedureNormalDrawJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::WaitSendCard, ProcedureWaitSendCard(context, ProcedureSendCardJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::ReadyHandDiscard, ProcedureReadyHandDiscard(context, ProcedureReadyHandDiscardJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType_QiangGang, ProcedureQiangGang(context));
	extProcCtrl->SetProcedure(ProcedureType_FanMa, ProcedureFanMa(context, ProcedureFanMaJumpDelegate()));
	extProcCtrl->SetProcedure(ProcedureType::Liquidation, ProcedureLiquidation(context, 
		RefCntObj<ScoreItemListDelegate>(new SpecScoreItemListDelegate(context)), 
		SpecLiquidationJumpDelegate()));
}

void CServerGameDesk::GameClear()
{
	m_GameContext->GameFinish();

	OBJ_GET_EXT(m_GameContext, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->Clear();

	OBJ_GET_EXT(m_GameContext, ExtensionTimer, extTimer);
	extTimer->Clear();

	m_bGameStation = GS_WAIT_ARGEE;

	LOGGER_FILE(m_GameContext, "clean game"<<std::endl<<std::endl<<std::endl<<std::endl<<std::endl);
}

// 重置游戏
void CServerGameDesk::GameReset()
{
	m_Dismissed = false;
	m_GameOwnerNotSet = true;
	m_BecomeBankerTimes.clear();
	m_GameContext->Reset();

	LoadConfig(); // 重新加载一次配置(OnGetGameStation里面要读配置参数)
	
	LOGGER_FILE(m_GameContext, "reset game");
}

// 结算
void CServerGameDesk::Liquidate()
{
	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	__int64 money[PLAY_COUNT];
	const WinnerList &winners = extLiquidation->GetWinners();
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		PlayerPos::Type playerPos = extGameRunningData->GetPlayerPos(i);
		money[i] = extLiquidation->GetLiquidation().GetScoreList().GetScore(playerPos);
		if(winners.Contains(i))
		{
			RecordUserMultiple(GetDeskID(i),extLiquidation->GetMaxMulti());
			LOGGER_FILE(m_GameContext, "Player "<< (int)i <<"最大胡牌倍数为->" << extLiquidation->GetMaxMulti());
		}
		LOGGER_FILE(m_GameContext, "Player "<< (int)i <<" beforechangeuserpoint money = " << (int)money[i]);
	}

	bool cut[PLAY_COUNT];
	memset(cut, 0, sizeof(cut));

	ChangeUserPointint64(money, cut);
	RecoderGameInfo(money);

	// 发出结算消息
	OBJ_GET_EXT(m_GameContext, ExtensionGamePlayerConnector, extGamePlayerConnector);
	Liquidation &liquidation = extLiquidation->GetLiquidation();
	NotificationLiquidation notification(liquidation);
	extGamePlayerConnector->SendNotification(notification);


	OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		LOGGER_FILE(m_GameContext, "Player "<< (int)i <<" afterchangeuserpoint money = " << (int)money[i]);
		extGameStat->Add(i, GameStat_TotalScore, money[i]);
	}
}

void CServerGameDesk::SelectNextBanker()
{
	OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
	OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
	OBJ_GET_EXT(m_GameContext, ExtensionGameDirection, extGameDirection);

	PlayerPos::Type bankerPos = extBankerSelector->GetBankerPos();
	WinnerList winners = extLiquidation->GetWinners();

	if(winners.size() > 0)
	{
		// 第一个胡牌的人坐庄
		PlayerPos::Type nextBanker = winners.Get(0);


		// 是否有一炮多响
		std::map<PlayerPos::Type, std::map<int, int> > lightTimes; // 这一局玩家在某一回合点炮次数
		for(WinnerList::iterator itr = winners.begin(); itr != winners.end(); ++itr)
		{
			if(extLiquidation->GetLoser(*itr) != PlayerPos::Max)
				lightTimes[extLiquidation->GetLoser(*itr)][extLiquidation->GetWinRound(*itr)]++;
		}

		for(std::map<PlayerPos::Type, std::map<int, int> >::iterator itr = lightTimes.begin();
			itr != lightTimes.end();
			++itr)
		{
			for(std::map<int, int>::iterator itr2 = itr->second.begin();
				itr2  != itr->second.end();
				++itr2)
			{
				if(itr2->second > 1)
				{
					nextBanker = extGameDirection->GetNextPos(itr->first);
					break;
				}
			}
		}

		PlayerPos::Type oldBanker = extBankerSelector->GetBankerPos();
		extBankerSelector->SetBankerPos(nextBanker);

		OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
		extPlayer->SetDes(m_GameContext->GetPlayerCount(), nextBanker);

		//记录换庄
		if(oldBanker != nextBanker)
			m_BecomeBankerTimes[nextBanker]++;
	}
}

/// 游戏开始
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (!CGameDesk::GameBegin(bBeginFlag)) 
	{
		return false;
	}

	LoadConfig();

	InitGameRule();

	if(m_UseLogFile)
	{
		InitLogFile();
	}

	GameInit();

	LOGGER_FILE(m_GameContext, "Game Start ("<<m_iRunGameCount<<"/"<<m_iVipGameCount<<")");

	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{//这个是日志
		LOGGER_FILE(m_GameContext, "Player "<<GetUserInfo(i)->m_UserData.nickName<<"("<<GetUserInfo(i)->m_UserData.szName<<") at position "<<i
			<<"/"<<(int)extGameRunningData->GetPlayerPos(i));
	}
	LOGGER_FILE(m_GameContext,"尝试输出日志"<<"一共有玩家人数为："<<m_GameContext->GetPlayerCount());
	// 开始一局游戏
	m_GameContext->GameStart();

	UpdateRemainingGameCount();

	CreateNewRecord();

	m_GameStarted = true;
	m_Dismissed = false;

	//m_bGameStation = GS_PLAY_GAME;

	return true;
}

/// 游戏结束
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//游戏正常结束
		{
			LOGGER_FILE(m_GameContext, "game finish normal");
			break;
		}
	case GFF_DISSMISS_FINISH:
		{
			m_Dismissed = true;
			DissMissRoomBefore();
			break;
		}
	case GFF_FORCE_FINISH:
		{
			//游戏强制结束
			OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSeletor);
			OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);

			tagCountFenEx msg;
			msg.Clear();
			msg.byNt = extBankerSeletor->GetBankerPos();

			PlayerPosList visiblePos;
			for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
			{
				visiblePos.insert(i);
			}
			CopyAllCardsInHand((uchar *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), 
				extPlayer->GetPlayerList(), visiblePos);

			for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
			{
				CopyInterceptionData(msg.m_UserGCPData[i], 
					sizeof(msg.m_UserGCPData[i])/sizeof(msg.m_UserGCPData[i][0]),
					i, extPlayer->GetPlayer(i)->GetInterceptions());
			}

			SendDataToPlayers(&msg,sizeof(msg),THING_ENG_HANDLE);

			GameClear();
			GameReset();
			break;
		}
	}

	if(m_Dismissed)
	{
		GameDismissClean();
	}

	if(m_GameStarted)
	{
		CloseRecord();
		SavePlayBackFile();
	}
	
	m_GameStarted = false;

	LOGGER_FILE(m_GameContext, "CGameDesk::GameFinish");
	return CGameDesk::GameFinish(bDeskStation,bCloseFlag);
}

/// 判断是否正在游戏
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{	
	return m_bGameStation >= GS_DICE && m_bGameStation <= GS_PLAY_GAME;
}

/// 框架消息处理函数
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}

// 	switch (pNetHead->bAssistantID)
// 	{
// 	case ASS_GM_FORCE_QUIT:
// 		{
// 			GameFinish(bDeskStation,GFF_FORCE_FINISH);//强退结束游戏
// 			break;
// 		}
// 	default:
// 		{
// 			break;
// 		}
// 	}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

/// 游戏数据包处理函数
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if(bWatchUser)
	{
		return false;
	}

	if(pNetHead->bMainID != MDM_GM_GAME_NOTIFY) return false;

	const CGameUserInfo *userInfo = GetUserInfo(bDeskStation);
	if(userInfo == NULL) return false;

	LOGGER_FILE(m_GameContext, "recvive player "<< (int)bDeskStation << " message with thingID : " << pNetHead->bAssistantID);

	StartUpdateLoop(true);
	bool ret = m_ClientMsgProxy.OnReceive(m_GameContext, bDeskStation, pData, uSize, pNetHead->bAssistantID);

	if(!ret || ASS_GM_AGREE_GAME == pNetHead->bAssistantID)
	{
		ret = __super::HandleNotifyMessage(bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser);
	}
	return ret;
}

/// 用户离开游戏桌
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{	
	if(!IsVipRoom())
	{
		OnPlayerQuitInGame(bDeskStation);
		OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);	
		extBankerSelector->SetBankerPos(PlayerPos::Max);
	}

	return CGameDesk::UserLeftDesk(bDeskStation, pUserInfo);
}

//用来改变用户断线条件的函数
bool CServerGameDesk::CanNetCut(BYTE bDeskStation)
{	
	return CGameDesk::CanNetCut(bDeskStation);
}

//玩家断线处理
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	OnPlayerQuitInGame(bDeskStation);

	return CGameDesk::UserNetCut(bDeskStation, pLostUserInfo);
}

bool CServerGameDesk::CanBeginGame()
{
	InitGameRule();
	int readyCnt = 0;
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		if(m_GameRunningData.IsPlayerReadyToPlay(i))
		{
			++readyCnt;
		}
	}
	return readyCnt >= m_GameContext->GetPlayerCount();
}

/// 获取游戏状态信息
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	// 记录玩家位置
	const CGameUserInfo *userInfo = GetUserInfo(bDeskStation);
	if(!userInfo) return false;

	InitGameRule();

	int curplayercount = 0;
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		if(GetUserInfo(i) != NULL)
			++curplayercount;
	}

	if(IsVipRoom() && 0 == m_iRunGameCount && curplayercount == 1)		//创建房间第一局游戏未开始时先删除与游戏人数有关的拓展，防止上个房间数据未清理出现异常
	{
		ObjRmExt(m_GameContext,ExtensionPlayer);
	}

	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
	PlayerPos::Type playerPos = extGameRunningData->GetPlayerPos(bDeskStation);
	if(playerPos != PlayerPos::Max)
	{
		OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
		extPlayer->GetPlayerStatus(bDeskStation).SetBool(PlayerStatus_IsLeft, false);
	}

	m_ClientMsgProxy.OnGetGameStationEx(m_GameContext, m_bGameStation, bDeskStation, uSocketID, bWatchUser);


	if(IsVipRoom())
	{
		// 发送剩余局数
		SC_RemainingGameCount msg;
		msg.remaingameCount = m_iVipGameCount - m_iRunGameCount;
		msg.total = m_iVipGameCount;
		SendDataToPlayer(bDeskStation, &msg, sizeof(msg), S_C_UPDATE_REMAIN_JUSHU_SIG);
	}
	
	return true;
}

/// 重置游戏状态
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return true;
}

/// 定时器消息
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	bool ret = true;
	if(TIME_UPDATE == uTimerID)
	{
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);

		__int64 timedif = TimeDiff(curTime, m_LatestUpdateTime);
		float passtime;												

		if(timedif <= 0)
			passtime = m_TickInterval;
		else
			passtime = timedif * 1.0f / 1000;

		if(passtime <= 0)
			passtime = m_TickInterval;

		OBJ_GET_EXT(m_GameContext, ExtensionProcedureController, extProcCtrl);
		extProcCtrl->Update(passtime);

		OBJ_GET_EXT(m_GameContext, ExtensionTimer, extTimer);
		extTimer->Update(passtime);

		m_LatestUpdateTime = curTime;
	}
	else
	{
		ret = __super::OnTimer(uTimerID);
	}
	return ret;
}

void CServerGameDesk::DissMissRoomBefore()
{
	OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
 	if(m_iRunGameCount > 0 || IsPlayGame(0))
 	{
		SendScoreBoard(true);
	}
	GameDismissClean();
}

//修改奖池数据1(平台->游戏)
/// @param bAIWinAndLostAutoCtrl 机器人控制输赢开关
/// @return 是否成功
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return true;
}

//修改奖池数据2(平台->游戏)
/// @param m_iAIWantWinMoney 机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
/// @param m_iAIWinLuckyAt 机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
/// @return 是否成功
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return true;
}

//修改奖池数据3(平台->游戏)
/// @param iReSetAIHaveWinMoney 机器人输赢控制：重置机器人已经赢钱的数目
/// @return 是否成功
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return true;
}

//控制胜负
/// 判断
BOOL CServerGameDesk::Judge()
{
	return 0;
}
/// 胜
BOOL CServerGameDesk::JudgeWiner()
{
	return 0;
}

BOOL CServerGameDesk::JudgeLoser()
{
	return 0;
}

bool CServerGameDesk::IsVipRoom() const
{
	return m_bIsBuy;
}

int CServerGameDesk::GetGameOwner() const
{
	int playerPos = 0;
	if(IsVipRoom())
	{
		for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
		{
			if(m_iDeskMaster == GetUID(i))
			{
				playerPos = i;
				break;
			}
		}
	}
	return playerPos;
}

const CGameUserInfo *CServerGameDesk::GetUserInfo(int pos) const
{
	return m_pUserInfo[pos];
}

long CServerGameDesk::GetUID(int playerPos) const
{
	long ret = INVALID_UID;
	const CGameUserInfo *userInfo = GetUserInfo(playerPos);
	if(userInfo)
	{
		ret = userInfo->m_UserData.dwUserID;
	}
	return ret;
}

void CServerGameDesk::SendDataToPlayer(int playerPos, void *data, int dataSize, int msgID)
{
	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);

	BYTE bDeskStation = extGameRunningData->GetDeskStation(playerPos);
	if(bDeskStation != 255)
	{
		SendGameDataEx(bDeskStation, data, dataSize, MDM_GM_GAME_NOTIFY, msgID, 0);
	}
}

void CServerGameDesk::SendDataToPlayers(void *data, int dataSize, int msgID)
{
	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
	const PlayerList &players = extPlayer->GetPlayerList();
	for(PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
	{
		SendDataToPlayer(itr->second->GetPos(), data, dataSize, msgID);
	}
}

void CServerGameDesk::LoadConfig()
{
	CString s = CINIFile::GetAppPath ();/////本地路径    
	CINIFile f(s +_T("\\")+SKIN_FOLDER  + _T("_s.ini"));

	m_UseLogFile = f.GetKeyVal("game", "LogFile", 0) > 0;

	//int tickMSec = f.GetKeyVal("game","TickInterval", 100); //游戏更新循环间隔(毫秒)
	int tickMSec = 50; //游戏更新循环间隔(毫秒)
	if(tickMSec <= 1)
	{
		tickMSec = 50;
	}
	m_TickInterval = tickMSec * 1.0f / 1000;

	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	GameParamLoader<CINIFile>::Load(f, extGameParam, NAME_ID, m_pDataManage->m_InitData.uRoomID);
}

void CServerGameDesk::StartUpdateLoop(bool b)
{
	if(b && !m_TimerStarted)
	{
		SetTimer(TIME_UPDATE, (int)(m_TickInterval * 1000));
		m_TimerStarted = true;
		GetLocalTime(&m_LatestUpdateTime);
	}
	else if(!b && m_TimerStarted)
	{
		KillTimer(TIME_UPDATE);
		m_TimerStarted = false;
	}
}

void CServerGameDesk::SendScoreBoard(bool dismissRoom)
{
	if(dismissRoom && IsVipRoom())
	{
		CalculateBoard data[PLAY_COUNT];
		OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);

		std::vector<PlayerPos::Type> playerPosList;
		for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
		{
			playerPosList.push_back(i);
		}

		std::vector<PlayerPos::Type> totalWinner = extGameStat->FindPlayerWithMost(GameStat_TotalScore, playerPosList);
		std::vector<PlayerPos::Type> totalLighter = extGameStat->FindPlayerWithMost(GameStat_LightTimes, playerPosList);

		OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);

		for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
		{
			data[i].bWinner = extLiquidation->GetGameCount() > 0 && extGameStat->Get(i, GameStat_TotalScore) > 0 &&
				std::find(totalWinner.begin(), totalWinner.end(), i) != totalWinner.end();

			data[i].bBestFire = extLiquidation->GetGameCount() > 0 && extGameStat->Get(i, GameStat_LightTimes) > 0 &&
				std::find(totalLighter.begin(), totalLighter.end(), i) != totalLighter.end();

			data[i].iZimo = extGameStat->Get(i, GameStat_SelfFeedTimes);
			data[i].iJiePao = extGameStat->Get(i, GameStat_CatchFireTimes);
			data[i].iDianPao = extGameStat->Get(i, GameStat_LightTimes);
			data[i].iAnGang = extGameStat->Get(i, GameStat_AnGangTimes);
			data[i].iMingGang = extGameStat->Get(i, GameStat_MingGangTimes);
			data[i].iBuGang = extGameStat->Get(i, GameStat_BuGangTimes);
			data[i].iPaiTou = extGameStat->Get(i, GameStat_PaiTouTimes);
			data[i].i64WinMoney = extGameStat->Get(i, GameStat_TotalScore);

			LOGGER_FILE(m_GameContext, "zimo:"<<data[i].iZimo<<" jiepao:"<<data[i].iJiePao<<" dianpao:"<<data[i].iDianPao<<
				" angang:"<<data[i].iAnGang<<" minggang:"<<data[i].iMingGang<<" bugang:"<<data[i].iBuGang<<" paitou:"<<data[i].iPaiTou<<" money:"<<data[i].i64WinMoney);
		}

		SendDataToPlayers(data, sizeof(data), S_C_UPDATE_CALCULATE_BOARD_SIG);

		LOGGER_FILE(m_GameContext, "Send Score Board");
	}
}

void CServerGameDesk::OnProcedureGameStart()
{
	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(m_GameContext, ExtensionCardBlocks, extCardBlocks);
	// 设置抽牌保留牌数(还有配牌器)
	std::map<int, int> playerPosToID;
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		playerPosToID[i] = m_pUserInfo[i]->m_UserData.dwUserID;
	}
	playerPosToID[INVALID_PLAYER_POS] = 0;
	extCardBlocks->SetDrawValidator(DrawCardValidator(new SpecDrawCardValidator(playerPosToID, 
		extGameParam->GetParam(GameParam_ReservedCardCount))));

	// 设置拦牌行为开关
	OBJ_GET_EXT(m_GameContext, ExtensionActionValidator, extActValidator);
	extActValidator->SetInterceptionValidator(m_InterceptionValidator);

	// 设置抽卡之后的拦牌行为检测项
	InterceptionCheckType afterDrawCardCheckType;
	afterDrawCardCheckType.Add(Interception::BuGang);
	afterDrawCardCheckType.Add(Interception::AnGang);
	afterDrawCardCheckType.Add(Interception::Hu);
	//afterDrawCardCheckType.Add(Interception::Ting);
	extActValidator->SetCheckType(ProcedureType::AfterDrawCard, afterDrawCardCheckType);

	// 设置出卡之后的拦牌行为检测项
	InterceptionCheckType discardCardCheckType;
	discardCardCheckType.Add(Interception::Peng);
	discardCardCheckType.Add(Interception::MingGang);
	discardCardCheckType.Add(Interception::Hu);
	extActValidator->SetCheckType(ProcedureType::InterceptCard, discardCardCheckType);

	// 设置拦牌后出牌前的拦牌行为检测项
	InterceptionCheckType afterInterceptCheckType;
	//afterInterceptCheckType.Add(Interception::AnGang);
	//afterInterceptCheckType.Add(Interception::Ting);
	extActValidator->SetCheckType(ProcedureType::AfterIntercept, afterInterceptCheckType);

	// 设置杠牌识别器
	extActValidator->SetInterceptionHandler(Interception::AnGang, GangHandler());

	// 设置牌型分数计算器
	OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
	extLiquidation->SetCardScoreDelegate(RefCntObj<CardScoreDelegate>(new SpecCardScoreDelegate(m_GameContext)));

	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
	extPlayer->SetHandSortingDelegate(RefCntObj<HandSortingDelegate>(new SpecCardSortingDelegate(m_GameContext)));
}

void CServerGameDesk::OnProcedureLiquidationFinish()
{
	// 一局游戏结束
	Liquidate();

	// 统计游戏数据
	DoGameStatistic();	

	// 选下一个庄家
	SelectNextBanker();

	// 重置玩家状态
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		if(m_pUserInfo[i])
		{
			m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
		}
	}

	if(m_iRunGameCount >= m_iVipGameCount)
	{
		m_Dismissed = true;
		// 局数打满了
		LOGGER_FILE(m_GameContext, "all rounds finished , send total score board ");
		SendScoreBoard(true);
		m_Dismissed = true;
	}

	// 一局游戏结束, 清理一下
	GameClear();

	StartUpdateLoop(false);

	GameFinish(0, GF_NORMAL);
}

void CServerGameDesk::InitLogFile()
{
	time_t nowTs = time(NULL);
	tm time_data;
	localtime_s(&time_data, &nowTs);
	char timeStrBuf[256];
	sprintf_s(timeStrBuf, "%d%02d%02d_%02d", 1900 + time_data.tm_year,
		time_data.tm_mon + 1,
		time_data.tm_mday,
		time_data.tm_hour);

	OBJ_GET_EXT(m_GameContext, ExtensionLogger, extLogger);
	char buf[256];
	sprintf(buf, "./Log/%s-%d_%d_%d.log", timeStrBuf, NAME_ID, GetRoomID(), GetDeskID(0));
	mkdir("./Log");
	extLogger->Open(buf);
}

void CServerGameDesk::InitGameRule()
{
	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(m_GameContext, ExtensionCardBlocks, extCardBlocks);
	tagPM *playmode = (tagPM *)m_szDeskConfig;
	int playercount = playmode->nPlayerCount;
	if(playercount < 2 || playercount > 4 || !IsVipRoom())
	{
		//人数不是有效值或者是金币场或者是网站创建的房间，使用配置文件默认玩法,最大人数
		playercount = m_bMaxPeople;
	}
	else if(IsVipRoom())
	{
		{
			extGameParam->SetParam(GameParam_PlayMethod, playmode->iPlayingMethod );
			extGameParam->SetParam(GameParam_bFanMa, playmode->bFanMa );//是否翻马
			extGameParam->SetParam(GameParam_FanMaNum, playmode->FanMaNum );//翻马个数
			extGameParam->SetParam(GameParam_ReservedCardCount,playmode->bFanMa ? playmode->FanMaNum : 0);
		//	extGameParam->SetParam(GameParam_FanMaMulti, playmode->FanCardMulti );//翻马倍数
			extGameParam->SetParam(GameParam_bFengDing, playmode->bFengDing );
			extGameParam->SetParam(GameParam_FengDingMulti, playmode->FengDingMulti );
		//	extGameParam->SetParam(GameParam_bMaGenGang, playmode->bMaGenGang );
		//	extGameParam->SetParam(GameParam_bMaGenDiFen, playmode->bMaGenDiFen);

			extGameParam->SetParam(GameParam_bMaGenGang, (playmode->iPlayingMethod & PM_MaGenGang) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_bMaGenDiFen, (playmode->iPlayingMethod & PM_MaGenDiFen) > 0 ? 1 : 0);

			extGameParam->SetParam(GameParam_PM_KeJiePaoHu, (playmode->iPlayingMethod & PM_KeJiePaoHu) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_GangBao, (playmode->iPlayingMethod & PM_GangBao)>0 ? 1: 0);
			extGameParam->SetParam(GameParam_PM_BiHu, (playmode->iPlayingMethod & PM_BiHu)>0?1:0);
			extGameParam->SetParam(GameParam_PM_QiangGangHu, (playmode->iPlayingMethod & PM_QiangGangHu) > 0?1:0);
			extGameParam->SetParam(GameParam_PM_QiXiaoDui, (playmode->iPlayingMethod & PM_QiXiaoDui) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_HunYiSe, (playmode->iPlayingMethod & PM_HunYiSe) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_QingYiSe, (playmode->iPlayingMethod & PM_QingYiSe) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_GangKai, (playmode->iPlayingMethod & PM_GangKai) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_PengPengHU, (playmode->iPlayingMethod & PM_PengPengHU) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_ShiSanYao, (playmode->iPlayingMethod & PM_ShiSanYao) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_TianDiHu, (playmode->iPlayingMethod & PM_TianDiHu) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_18LuoHan, (playmode->iPlayingMethod & PM_18LuoHan) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_SuperQiDui,( playmode->iPlayingMethod & PM_SuperQiDui) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_DoubleSpQiDui,(playmode->iPlayingMethod & PM_DoubleSpQiDui) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_TripleSpQiDui, (playmode->iPlayingMethod & PM_TripleSpQiDui) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_YaoJiu, (playmode->iPlayingMethod & PM_YaoJiu) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_SmalSiXi,(playmode->iPlayingMethod & PM_SmalSiXi) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_BigSiXi, (playmode->iPlayingMethod & PM_BigSiXi) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_SmallSanYuan, (playmode->iPlayingMethod & PM_SmallSanYuan) > 0 ? 1 : 0);
			extGameParam->SetParam(GameParam_PM_BigSanYuan, (playmode->iPlayingMethod & PM_BigSanYuan) > 0 ? 1 : 0);
		}

		if(extGameParam->GetParam(GameParam_DiFen) < 1)
			extGameParam->SetParam(GameParam_DiFen, 1);
	}

	m_GameContext->SetPlayerCount(playercount);	
}

void CServerGameDesk::OnPlayerQuitInGame(BYTE deskStation)
{
	if(GS_PLAY_GAME == GetGameState())
	{
		OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
		PlayerPos::Type playerPos = extGameRunningData->GetPlayerPos(deskStation);
		if(playerPos != PlayerPos::Max)
		{
			OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
			extPlayer->GetPlayerStatus(deskStation).SetBool(PlayerStatus_IsLeft, true);
		}

 		extGameRunningData->OnPlayerLeft(deskStation);
		if(!IsVipRoom())
		{
			OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
			Usertouguan data;
			data.enable = true;
			data.desk = deskStation;
			for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
			{
				data.bTuoGuan[i] = extPlayer->GetPlayerStatus(deskStation).GetBool(PlayerStatus_ActionDelegate);
			}
			MsgDelegateEnableActionDelegate().OnResponse(m_GameContext, deskStation, &data, sizeof(Usertouguan), ASS_AUTOCARD);
		}
	}
}

void CServerGameDesk::UpdateRemainingGameCount()
{
	if(IsVipRoom())
	{
		SC_RemainingGameCount msg;
		// 发送剩余局数
		msg.remaingameCount = m_iVipGameCount - m_iRunGameCount;
		msg.total = m_iVipGameCount;
		SendDataToPlayers(&msg, sizeof(msg), S_C_UPDATE_REMAIN_JUSHU_SIG);
	}
}

void CServerGameDesk::GameDismissClean()
{
	GameClear();
	GameReset();
	if(m_UseLogFile)
	{
		// 关闭日志文件
		OBJ_GET_EXT(m_GameContext, ExtensionLogger, extLogger);
		extLogger->Close();
	}
	//本游戏人数在创建房间时确定，因此解散房间时需要删除跟人数有关的扩展，等待下个房间创建根据人数重新创建相应的扩展
	ObjRmExt(m_GameContext,ExtensionPlayer);
}

void CServerGameDesk::DoGameStatistic()
{
	LOGGER_FILE(m_GameContext, "Do Statistic");

	OBJ_GET_EXT(m_GameContext, ExtensionGameRunningData, extGameRunningData);
	OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);
	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);

	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		PlayerPos::Type playerPos = extGameRunningData->GetPlayerPos(i);
		extGameStat->Set(playerPos, GameStat_BankerTimes, extGameRunningData->GetBankerTimes(playerPos));

		OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
		WinnerList winners = extLiquidation->GetWinners();

		LOGGER_FILE(m_GameContext, ">> zimo:"<<extGameStat->Get(playerPos, GameStat_SelfFeedTimes)<<" "<<
			"jiepao:"<<extGameStat->Get(playerPos, GameStat_CatchFireTimes)<<" "<<
			"dianpao:"<<extGameStat->Get(playerPos, GameStat_LightTimes)<<" "<<
			"totalScore"<<extGameStat->Get(playerPos, GameStat_TotalScore));

		if(winners.size() > 0)
		{
			if(winners.Contains(playerPos))
			{
				extGameStat->Add(playerPos, GameStat_WinTimes, 1);

				int winMode = extLiquidation->GetWinMode(playerPos);

				if(WinMode_Fire == winMode)
				{
					// 接炮次数
					extGameStat->Add(playerPos, GameStat_CatchFireTimes, 1);

					//点炮次数
					PlayerPos::Type loser = extLiquidation->GetLoser(playerPos);
					if(loser != PlayerPos::Max)
						extGameStat->Add(loser, GameStat_LightTimes, 1);
				}
				if(WinMode_Self == winMode)
				{
					// 自摸次数
					extGameStat->Add(playerPos, GameStat_SelfFeedTimes, 1);
				}

				OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
			}
		}

		Player *player = extPlayer->GetPlayer(playerPos);
		const InterceptionList &interceptions = player->GetInterceptions();
		OBJ_GET_EXT(m_GameContext, ExtensionInvalidInterception, extInvalidIntc);
		for(InterceptionList::const_iterator itr = interceptions.begin();
			itr != interceptions.end();
			++itr)
		{
			int idx = itr - interceptions.begin();
			if(extInvalidIntc->Contains(playerPos, idx))
				continue;

			switch(itr->type)
			{
			case Interception::MingGang:
				extGameStat->Add(playerPos, GameStat_MingGangTimes, 1);
				break;
			case Interception::BuGang:
		//	case Interception::YangMa:
				extGameStat->Add(playerPos, GameStat_BuGangTimes, 1);
				break;
			case Interception::AnGang:
				extGameStat->Add(playerPos, GameStat_AnGangTimes, 1);
				break;
			}
		}

		OBJ_GET_EXT(m_GameContext, ExtensionFanMa, extFanMa);
		int paiTouTimes = extFanMa->GetZhongMaCount(i);
		if(paiTouTimes > 0)
			extGameStat->Add(playerPos, GameStat_PaiTouTimes, paiTouTimes);

		LOGGER_FILE(m_GameContext, "<< zimo:"<<extGameStat->Get(playerPos, GameStat_SelfFeedTimes)<<" "<<
			"jiepao:"<<extGameStat->Get(playerPos, GameStat_CatchFireTimes)<<" "<<
			"dianpao:"<<extGameStat->Get(playerPos, GameStat_LightTimes)<<" "<<
			"totalScore"<<extGameStat->Get(playerPos, GameStat_TotalScore));
	}
}

bool CServerGameDesk::IsUserReady(BYTE playerPos) const
{
	bool ret = false;
	if(m_pUserInfo[playerPos])
	{
		BYTE userState = m_pUserInfo[playerPos]->m_UserData.bUserState;
		ret = (userState == USER_ARGEE || userState == USER_PLAY_GAME);
	}
	return ret;
}

bool CServerGameDesk::CreateNewRecord()
{
// 	DWORD cfgHandle=cfgOpenFile("BZGameLogon.BCF");
// 	if(cfgHandle < 0x10)
// 		return false;
// 
// 	m_RecordFilePath=cfgGetValue(cfgHandle,"PLAYBACK","FILEPATH","");
// 	if(m_RecordFilePath.Right(1)!="\\")
// 		m_RecordFilePath+"\\";
// 	CreateDirectory(m_RecordFilePath,NULL);

	CreateDirectory(m_pDataManage->m_InitData.szPlayBackTemp, NULL);

	RecordUserInfo TRecordUserInfo;
	memcpy(TRecordUserInfo.szDeskPassWord, m_szDeskPassWord, sizeof(m_szDeskPassWord));
	TRecordUserInfo.nPlayCount				= PLAY_COUNT;
	TRecordUserInfo.iVipGameCount			= m_iVipGameCount;
	TRecordUserInfo.iRunGameCount			= m_iRunGameCount + 1;
	TRecordUserInfo.nBasePoint				= m_DeskBasePoint;
	TRecordUserInfo.bBuyRoom				= m_bIsBuy;
	TRecordUserInfo.bVIPRoom				= m_pDataManage->m_InitData.bVIPRoom;
	TRecordUserInfo.nComType				= m_pDataManage->m_InitData.uComType;
	TRecordUserInfo.nKindID					= m_pDataManage->m_InitData.uComType;
	TRecordUserInfo.nNameID					= m_pDataManage->m_InitData.uNameID;
	TRecordUserInfo.dwRoomRule				= m_pDataManage->m_InitData.dwRoomRule;
	TRecordUserInfo.iPayType				= m_iPayType;
	TRecordUserInfo.iClubID					= m_iClubID;
//	TRecordUserInfo.bfree					= m_VipDeskFreeTimeInfo.bFree;
	for(int i = 0; i < m_GameContext->GetPlayerCount(); i ++)
	{
		TRecordUserInfo.userinfo[i].i64UserMoney		= m_pUserInfo[i]->m_UserData.i64Money;
		TRecordUserInfo.userinfo[i].nLogoID			= m_pUserInfo[i]->m_UserData.bLogoID;
		memcpy(TRecordUserInfo.userinfo[i].szNickName,m_pUserInfo[i]->m_UserData.nickName,sizeof(char) * 61);
		memcpy(TRecordUserInfo.userinfo[i].szheadUrl,m_pUserInfo[i]->m_UserData.szHeadUrl,sizeof(char) * 256);
		TRecordUserInfo.userinfo[i].bBoy		= m_pUserInfo[i]->m_UserData.bBoy;
		TRecordUserInfo.userinfo[i].uId = m_pUserInfo[i]->m_UserData.dwUserID ;
	}

	char fileName[200];
	for(int i = 0; i < m_GameContext->GetPlayerCount(); i ++)
	{
		memset(fileName,0,sizeof(fileName));
		//文件名 时间-用户ID.dat
		sprintf(fileName,"%s%d-%d.txt", m_pDataManage->m_InitData.szPlayBackTemp, GetTickCount(), m_pUserInfo[i]->m_UserData.dwUserID);
		//m_CachedUserID[i] = m_pUserInfo[i]->m_UserData.dwUserID;
		fileName[strlen(fileName)] = '\0';
		m_FileOperator[i].setFileName(fileName);
		m_FileOperator[i].createFile();
		TRecordUserInfo.bWatchDeskView = i;
		m_FileOperator[i].appendProperty(0,&TRecordUserInfo,sizeof(TRecordUserInfo),NULL,0);
	}

	return true;
}

void CServerGameDesk::CopyAllPlayerHandPai(uchar byArHandPai[][HAND_CARD_NUM])
{
	memset(byArHandPai,255,sizeof(byArHandPai));
	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
	PlayerList players = extPlayer->GetPlayerList();
	
	for(PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
	{
		int i = 0;
		CardList cardlist = extPlayer->GetPlayer(itr->first)->GetCards();
		for(CardList::const_iterator itr2 = cardlist.begin(); itr2 != cardlist.end(); ++itr2)
		{
				byArHandPai[itr->first][i] = *itr2;
				++i;
		}
	}		
}

void CServerGameDesk::SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendGameData(bDeskStation, pData, uSize, MDM_GM_GAME_NOTIFY, bAssID, 0);

	if(bAssID == THING_SEND_PAI)
	{
		tagSendPaiEx &data = *((tagSendPaiEx *)pData);
		CopyAllPlayerHandPai(data.m_byArHandPai);
	}
	else if(bAssID == THING_DING_QUE)
	{
		tagDingQueEx &data = *((tagDingQueEx *)pData);
	}
	else if(bAssID == THING_CHANGE_NOTIFY)
	{
		tagChangePsEx &data = *((tagChangePsEx *)pData);
	}
	else if(bAssID == THING_CHANGE_SEND_CARD)
	{
		HuanZhangFinish &data = *((HuanZhangFinish *)pData);
		CopyAllPlayerHandPai(data.m_byArHandPai);
	}
	else if(bAssID == THING_BEGIN_OUT)
	{
		tagBeginOutPaiEx &data = *((tagBeginOutPaiEx *)pData);
	}
	else if(bAssID == THING_OUT_PAI)
	{
		tagOutPaiEx &data = *((tagOutPaiEx *)pData);
		CopyAllPlayerHandPai(data.m_byArHandPai);
	}
	else if(bAssID == THING_HU_PAI)
	{
		tagHuPaiEx &data = *((tagHuPaiEx *)pData);
		CopyAllPlayerHandPai(data.m_byArHandPai);
	}
	else if(bAssID == THING_ZHUA_PAI)
	{
		tagZhuaPaiEx &data = *((tagZhuaPaiEx *)pData);
		if(data.byPs == 0 || data.byPs == 255)
		{
			OBJ_GET_EXT(m_GameContext, ExtensionInterceptableCards, extIntcCards);
			data.byPs = extIntcCards->GetLatestDrawnCard();
		}
	}

	if (bMainID == MDM_GM_GAME_NOTIFY)
	{
		int iSize							= 0;
		void * pstation						= NULL;

		switch(m_bGameStation)
		{
		case GS_WAIT_ARGEE:
		case GS_WAIT_NEXT:
			{
				GameStationData_Base gsData;
				m_ClientMsgProxy.FillBaseData(m_GameContext, gsData);

				pstation = &gsData;
				iSize = sizeof(gsData);

				break;
			}
		case GS_DICE:
			{
				GameStationData_Dice gsData;
				m_ClientMsgProxy.FillDiceData(m_GameContext, gsData);
				
				pstation = &gsData;
				iSize = sizeof(gsData);

				break;
			}
		case GS_SEND_CARD:
			{
				GameStationData_SendCard gsData;
				m_ClientMsgProxy.FillSendCardData(m_GameContext, gsData, bDeskStation);

				CopyAllPlayerHandPai(gsData.m_byArHandPai);
				
				pstation = &gsData;
				iSize = sizeof(gsData);

				break;
			}
		case GS_PLAY_GAME:
			{
				GameStationData_Playing gsData;
				m_ClientMsgProxy.FillPlayingData(m_GameContext, gsData, bDeskStation);

				CopyAllPlayerHandPai(gsData.m_byArHandPai);
				
				pstation = &gsData;
				iSize = sizeof(gsData);

				break;
			}
		}

		m_FileOperator[bDeskStation].appendProperty(bAssID,pData,uSize,pstation,iSize);

		if(bAssID == THING_CPG_NOTIFY)
		{
			LOGGER_FILE(m_GameContext,"test::record THING_CPG_NOTIFY, bDeskStation = " << bDeskStation);
		}
	}
}

void CServerGameDesk::CloseRecord()
{
	char fileName[200];
	for(int i = 0; i < m_GameContext->GetPlayerCount(); i ++)
	{
		m_FileOperator[i].CloseFile();

		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}

		///unsigned int backCode = GetSmallStatementPlaybackCode(m_pUserInfo[i]->m_UserData.dwUserID);
		/*
				CString str;
				str.Format("用户ID：%d,回放码：%d", m_pUserInfo[i]->m_UserData.dwUserID, backCode);
				DebugPrintf(str);
		*/
		memset(fileName,0,sizeof(fileName));
		sprintf(fileName,"%s%s0%d.txt",m_pDataManage->m_InitData.szPlayBackTemp, m_GameSN, i);
		fileName[strlen(fileName)] = '\0';

		m_FileOperator[i].CutAndReName(fileName);
	}
}

bool CServerGameDesk::RecordUserMultiple(int UserID,int iMultiple)
{
	return __super::RecordUserMultiple(UserID,iMultiple);
}

void CServerGameDesk::ReleaseRoomBefore()
{
	_putRoomInfo();	
}

void CServerGameDesk::_putRoomInfo()
{
	GameInfForReplay data[PLAY_COUNT];
	OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);
	OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);

	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (!m_pUserInfo[i])
		{
			continue;
		}

		data[i].iZimo = extGameStat->Get(i, GameStat_SelfFeedTimes);
		data[i].iJiePao = extGameStat->Get(i, GameStat_CatchFireTimes);
		data[i].iDianPao = extGameStat->Get(i, GameStat_LightTimes);
		data[i].iAnGan = extGameStat->Get(i, GameStat_AnGangTimes);
		data[i].iMingGan = extGameStat->Get(i, GameStat_MingGangTimes);
		data[i].i64WinMoney = extGameStat->Get(i, GameStat_TotalScore);

		data[i].isBanker = (i == extBankerSelector->GetBankerPos());

		char szUserInf[512] = {0};
		memcpy_s(szUserInf,sizeof(szUserInf),&data[i],sizeof(GameInfForReplay));

		__super::ReleaseRoom(m_pUserInfo[i]->m_UserData.dwUserID,szUserInf,sizeof(GameInfForReplay));
	}
}

void CServerGameDesk::SetReturnGameInfo(int UserID,void* szUserGameInfo,int iSize,int iCount)
{
	if (nullptr == szUserGameInfo)
	{
		return;
	}

	GameInfForReplay *pGameInf =  (GameInfForReplay*)szUserGameInfo;
	OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);

	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (m_pReturnInfo[i].iUserID == UserID)
		{
			extGameStat->Set(i,GameStat_SelfFeedTimes,pGameInf->iZimo);
			extGameStat->Set(i,GameStat_CatchFireTimes,pGameInf->iJiePao);
			extGameStat->Set(i,GameStat_LightTimes,pGameInf->iDianPao);
			extGameStat->Set(i,GameStat_AnGangTimes,pGameInf->iAnGan);
			extGameStat->Set(i,GameStat_MingGangTimes,pGameInf->iMingGan);
			extGameStat->Set(i,GameStat_TotalScore,pGameInf->i64WinMoney);

			if(pGameInf->isBanker)
			{
				OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
				extBankerSelector->SetBankerPos(i);
			}

		}	
	}
}

__int64 CServerGameDesk::TimeDiff(SYSTEMTIME left, SYSTEMTIME right)  
{  

	CTime tmLeft(left.wYear, left.wMonth, left.wDay, 0, 0, 0);  
	CTime tmRight(right.wYear, right.wMonth, right.wDay, 0, 0, 0);  
	CTimeSpan sp = tmLeft - tmRight;  
	long MillisecondsL = (left.wHour*3600 + left.wMinute*60 + left.wSecond)*1000 + left.wMilliseconds;    
	long MillisecondsR = (right.wHour*3600 + right.wMinute*60 + right.wSecond)*1000 + right.wMilliseconds;    

	return  (__int64)sp.GetDays()*86400000 + (MillisecondsL - MillisecondsR);//此处返回毫秒  
}

