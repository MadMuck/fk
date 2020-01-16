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
* ���캯��	
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
* ��������
*/
CServerGameDesk::~CServerGameDesk(void)
{
	GameContextCreator::Destroy(m_GameContext);
}


/// ��ʼ��Ϸ
bool CServerGameDesk::InitDeskGameStation()
{
	LoadConfig(); // �ȼ���һ�����ã�֮��ÿ����Ϸ���¼���

	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	// ���÷�������
	m_GameContext->SetPlayerCount(extGameParam->GetParam(GameParam_PlayerCount));

	m_iRunGameCount = 0;

	return true;
}

// ��Ϸ����ִ��֮ǰ
void CServerGameDesk::OnBeforeProcedureStart(int latest, int current)
{
	LOGGER_FILE(m_GameContext, "before procedure start, from "<<(int)latest<<" to "<<current);
	switch(current)
	{
	case ProcedureType::Liquidation: 
		{
			// ����֮ǰ
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

// ��Ϸ����ִ��֮��
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
		extGameRunningData->AddBanker(extBankerSelector->GetBankerPos()); // ������ׯ����
	}
	else if(current == ProcedureType::Idle && latest == ProcedureType::Liquidation)
	{
		// ������̽�����һ����Ϸ������
		OnProcedureLiquidationFinish();
	}
}

void CServerGameDesk::GameInit()
{
	StartUpdateLoop(true);

	// ��ʼ�������
	InitRandom();

	GameContext *context = m_GameContext;

	m_ClientMsgProxy.Init(m_GameContext);

	//OBJ_GET_EXT(m_GameContext, ExtensionGameDirection, extGameDir);
	//extGameDir->SetGameDirection(RefCntObj<GameDirection>(new SpecGameDirection(m_GameContext)));

	if(m_GameOwnerNotSet && IsVipRoom())
	{
		m_GameOwnerNotSet = false;
		// ���÷���
		context->SetGameOwner(GetGameOwner());
	}

	//���������һ��������������
	if(IsVipRoom() && m_iRunGameCount == 0)
	{
		OBJ_GET_EXT(m_GameContext, ExtensionGameStatistic, extGameStat);
		extGameStat->OnGameReset();
	}

	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);

	extProcCtrl->SetProcedureChangedCallback(&m_ProcedureChangedDelegate);

	// ��������������Ϸ����
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

// ������Ϸ
void CServerGameDesk::GameReset()
{
	m_Dismissed = false;
	m_GameOwnerNotSet = true;
	m_BecomeBankerTimes.clear();
	m_GameContext->Reset();

	LoadConfig(); // ���¼���һ������(OnGetGameStation����Ҫ�����ò���)
	
	LOGGER_FILE(m_GameContext, "reset game");
}

// ����
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
			LOGGER_FILE(m_GameContext, "Player "<< (int)i <<"�����Ʊ���Ϊ->" << extLiquidation->GetMaxMulti());
		}
		LOGGER_FILE(m_GameContext, "Player "<< (int)i <<" beforechangeuserpoint money = " << (int)money[i]);
	}

	bool cut[PLAY_COUNT];
	memset(cut, 0, sizeof(cut));

	ChangeUserPointint64(money, cut);
	RecoderGameInfo(money);

	// ����������Ϣ
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
		// ��һ�����Ƶ�����ׯ
		PlayerPos::Type nextBanker = winners.Get(0);


		// �Ƿ���һ�ڶ���
		std::map<PlayerPos::Type, std::map<int, int> > lightTimes; // ��һ�������ĳһ�غϵ��ڴ���
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

		//��¼��ׯ
		if(oldBanker != nextBanker)
			m_BecomeBankerTimes[nextBanker]++;
	}
}

/// ��Ϸ��ʼ
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
	{//�������־
		LOGGER_FILE(m_GameContext, "Player "<<GetUserInfo(i)->m_UserData.nickName<<"("<<GetUserInfo(i)->m_UserData.szName<<") at position "<<i
			<<"/"<<(int)extGameRunningData->GetPlayerPos(i));
	}
	LOGGER_FILE(m_GameContext,"���������־"<<"һ�����������Ϊ��"<<m_GameContext->GetPlayerCount());
	// ��ʼһ����Ϸ
	m_GameContext->GameStart();

	UpdateRemainingGameCount();

	CreateNewRecord();

	m_GameStarted = true;
	m_Dismissed = false;

	//m_bGameStation = GS_PLAY_GAME;

	return true;
}

/// ��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
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
			//��Ϸǿ�ƽ���
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

/// �ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{	
	return m_bGameStation >= GS_DICE && m_bGameStation <= GS_PLAY_GAME;
}

/// �����Ϣ������
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
// 			GameFinish(bDeskStation,GFF_FORCE_FINISH);//ǿ�˽�����Ϸ
// 			break;
// 		}
// 	default:
// 		{
// 			break;
// 		}
// 	}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

/// ��Ϸ���ݰ�������
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

/// �û��뿪��Ϸ��
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

//�����ı��û����������ĺ���
bool CServerGameDesk::CanNetCut(BYTE bDeskStation)
{	
	return CGameDesk::CanNetCut(bDeskStation);
}

//��Ҷ��ߴ���
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

/// ��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	// ��¼���λ��
	const CGameUserInfo *userInfo = GetUserInfo(bDeskStation);
	if(!userInfo) return false;

	InitGameRule();

	int curplayercount = 0;
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		if(GetUserInfo(i) != NULL)
			++curplayercount;
	}

	if(IsVipRoom() && 0 == m_iRunGameCount && curplayercount == 1)		//���������һ����Ϸδ��ʼʱ��ɾ������Ϸ�����йص���չ����ֹ�ϸ���������δ��������쳣
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
		// ����ʣ�����
		SC_RemainingGameCount msg;
		msg.remaingameCount = m_iVipGameCount - m_iRunGameCount;
		msg.total = m_iVipGameCount;
		SendDataToPlayer(bDeskStation, &msg, sizeof(msg), S_C_UPDATE_REMAIN_JUSHU_SIG);
	}
	
	return true;
}

/// ������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return true;
}

/// ��ʱ����Ϣ
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

//�޸Ľ�������1(ƽ̨->��Ϸ)
/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return true;
}

//�޸Ľ�������2(ƽ̨->��Ϸ)
/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return true;
}

//�޸Ľ�������3(ƽ̨->��Ϸ)
/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return true;
}

//����ʤ��
/// �ж�
BOOL CServerGameDesk::Judge()
{
	return 0;
}
/// ʤ
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
	CString s = CINIFile::GetAppPath ();/////����·��    
	CINIFile f(s +_T("\\")+SKIN_FOLDER  + _T("_s.ini"));

	m_UseLogFile = f.GetKeyVal("game", "LogFile", 0) > 0;

	//int tickMSec = f.GetKeyVal("game","TickInterval", 100); //��Ϸ����ѭ�����(����)
	int tickMSec = 50; //��Ϸ����ѭ�����(����)
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
	// ���ó��Ʊ�������(����������)
	std::map<int, int> playerPosToID;
	for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
	{
		playerPosToID[i] = m_pUserInfo[i]->m_UserData.dwUserID;
	}
	playerPosToID[INVALID_PLAYER_POS] = 0;
	extCardBlocks->SetDrawValidator(DrawCardValidator(new SpecDrawCardValidator(playerPosToID, 
		extGameParam->GetParam(GameParam_ReservedCardCount))));

	// ����������Ϊ����
	OBJ_GET_EXT(m_GameContext, ExtensionActionValidator, extActValidator);
	extActValidator->SetInterceptionValidator(m_InterceptionValidator);

	// ���ó鿨֮���������Ϊ�����
	InterceptionCheckType afterDrawCardCheckType;
	afterDrawCardCheckType.Add(Interception::BuGang);
	afterDrawCardCheckType.Add(Interception::AnGang);
	afterDrawCardCheckType.Add(Interception::Hu);
	//afterDrawCardCheckType.Add(Interception::Ting);
	extActValidator->SetCheckType(ProcedureType::AfterDrawCard, afterDrawCardCheckType);

	// ���ó���֮���������Ϊ�����
	InterceptionCheckType discardCardCheckType;
	discardCardCheckType.Add(Interception::Peng);
	discardCardCheckType.Add(Interception::MingGang);
	discardCardCheckType.Add(Interception::Hu);
	extActValidator->SetCheckType(ProcedureType::InterceptCard, discardCardCheckType);

	// �������ƺ����ǰ��������Ϊ�����
	InterceptionCheckType afterInterceptCheckType;
	//afterInterceptCheckType.Add(Interception::AnGang);
	//afterInterceptCheckType.Add(Interception::Ting);
	extActValidator->SetCheckType(ProcedureType::AfterIntercept, afterInterceptCheckType);

	// ���ø���ʶ����
	extActValidator->SetInterceptionHandler(Interception::AnGang, GangHandler());

	// �������ͷ���������
	OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
	extLiquidation->SetCardScoreDelegate(RefCntObj<CardScoreDelegate>(new SpecCardScoreDelegate(m_GameContext)));

	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
	extPlayer->SetHandSortingDelegate(RefCntObj<HandSortingDelegate>(new SpecCardSortingDelegate(m_GameContext)));
}

void CServerGameDesk::OnProcedureLiquidationFinish()
{
	// һ����Ϸ����
	Liquidate();

	// ͳ����Ϸ����
	DoGameStatistic();	

	// ѡ��һ��ׯ��
	SelectNextBanker();

	// �������״̬
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
		// ����������
		LOGGER_FILE(m_GameContext, "all rounds finished , send total score board ");
		SendScoreBoard(true);
		m_Dismissed = true;
	}

	// һ����Ϸ����, ����һ��
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
		//����������Чֵ�����ǽ�ҳ���������վ�����ķ��䣬ʹ�������ļ�Ĭ���淨,�������
		playercount = m_bMaxPeople;
	}
	else if(IsVipRoom())
	{
		{
			extGameParam->SetParam(GameParam_PlayMethod, playmode->iPlayingMethod );
			extGameParam->SetParam(GameParam_bFanMa, playmode->bFanMa );//�Ƿ���
			extGameParam->SetParam(GameParam_FanMaNum, playmode->FanMaNum );//�������
			extGameParam->SetParam(GameParam_ReservedCardCount,playmode->bFanMa ? playmode->FanMaNum : 0);
		//	extGameParam->SetParam(GameParam_FanMaMulti, playmode->FanCardMulti );//������
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
		// ����ʣ�����
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
		// �ر���־�ļ�
		OBJ_GET_EXT(m_GameContext, ExtensionLogger, extLogger);
		extLogger->Close();
	}
	//����Ϸ�����ڴ�������ʱȷ������˽�ɢ����ʱ��Ҫɾ���������йص���չ���ȴ��¸����䴴�������������´�����Ӧ����չ
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
					// ���ڴ���
					extGameStat->Add(playerPos, GameStat_CatchFireTimes, 1);

					//���ڴ���
					PlayerPos::Type loser = extLiquidation->GetLoser(playerPos);
					if(loser != PlayerPos::Max)
						extGameStat->Add(loser, GameStat_LightTimes, 1);
				}
				if(WinMode_Self == winMode)
				{
					// ��������
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
		//�ļ��� ʱ��-�û�ID.dat
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
				str.Format("�û�ID��%d,�ط��룺%d", m_pUserInfo[i]->m_UserData.dwUserID, backCode);
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

	return  (__int64)sp.GetDays()*86400000 + (MillisecondsL - MillisecondsR);//�˴����غ���  
}

