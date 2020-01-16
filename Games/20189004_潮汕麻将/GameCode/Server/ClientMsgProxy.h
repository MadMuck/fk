#ifndef CLIENT_MSG_PROXY_H
#define CLIENT_MSG_PROXY_H

#include "UpgradeMessageEx.h"

#include "ExtensionGameParam.h"
#include "ExtensionPlayer.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionMessageDelegate.h"
#include "ExtensionQueMen.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionLiquidation.h"
#include "ExtensionHuanZhang.h"
#include "ExtensionLogger.h"
#include "ExtensionGameDirection.h"
#include "ExtensionLiquidation.h"
#include "PlayerStatusDef.h"

#include "QuestionWaitDiscard.h"
#include "QuestionReadyHandDiscard.h"

#include "MsgDelegateAgreeToPlay.h"
#include "MsgDelegateEnableActionDelegate.h"

#include "MsgDelegateGameBegin.h"
#include "MsgDelegateWaitDiscard.h"
#include "MsgDelegateWaitDiscardReadyHand.h"
#include "MsgDelegateDistributeCard.h"
#include "MsgDelegateDrawCard.h"
#include "MsgDelegateFirstRound.h"
#include "MsgDelegateDiscardResult.h"
#include "MsgDelegateInterception.h"
#include "MsgDelegateInterceptionResult.h"
#include "MsgDelegateLiquidation.h"
#include "MsgDelegateDingQue.h"
#include "MsgDelegateHuanZhang.h"
#include "MsgDelegateHuanZhangFinish.h"


template<typename T>
class ClientMsgProxy
{
	T *m_GamePlatformProxy;

public:
	ClientMsgProxy(T *proxy):m_GamePlatformProxy(proxy){}

	void Init(GameContext *context)
	{
		OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsg);
		extMsg->SetDelegate(MSG_DELEGATE_GAME_BEGIN, MessageDelegate(new MsgDelegateGameBegin())); // 游戏开始通知
		extMsg->SetDelegate(MSG_DELEGATE_DISTRIBUTE_CARD, MessageDelegate(new MsgDelegateDistributeCard())); // 发牌通知
		extMsg->SetDelegate(MSG_DELEGATE_NORMAL_DRAW_CARD, MessageDelegate(new MsgDelegateDrawCard())); // 抓牌通知
		extMsg->SetDelegate(MSG_DELEGATE_FIRST_RUN, MessageDelegate(new MsgDelegateFirstRound())); // 庄家开始出牌通知
		extMsg->SetDelegate(MSG_DELEGATE_WAIT_DISCARD_CARD, MessageDelegate(new MsgDelegateWaitDiscard())); // 等待玩家出牌通知
		extMsg->SetDelegate(MSG_DELEGATE_DISCARD_CARD, MessageDelegate(new MsgDelegateDiscardResult())); // 出牌结果通知
		extMsg->SetDelegate(MSG_DELEGATE_INTERCEPTION, MessageDelegate(new MsgDelegateInterception()));  // 玩家拦牌通知
		extMsg->SetDelegate(MSG_DELEGATE_INTERCEPTION_RESULT, MessageDelegate(new MsgDelegateInterceptionResult())); // 玩家拦牌结果通知
		extMsg->SetDelegate(MSG_DELEGATE_LIQUIDATION, MessageDelegate(new MsgDelegateLiquidation())); // 结算通知

		extMsg->SetDelegate(MSG_DELEGATE_DING_QUE, MessageDelegate(new MsgDelegateDingQue())); // 定缺询问
		extMsg->SetDelegate(MSG_DELEGATE_HUAN_ZHANG, MessageDelegate(new MsgDelegateHuanZhang())); // 换张询问
		extMsg->SetDelegate(MSG_DELEGATE_HUAN_ZHANG_FINISH, MessageDelegate(new MsgDelegateHuanZhangFinish()));
	}

	bool OnReceive(GameContext *context, unsigned char deskPos, void *data, unsigned int dataSize, int msgID)
	{
		OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		PlayerPos::Type playerPos = extGameRunningData->GetPlayerPos(deskPos);

		OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);

		bool res = true;
		switch(msgID)
		{
		case ASS_GM_AGREE_GAME:
			MsgDelegateAgreeToPlay().OnResponse(context, deskPos, msgID);
			break;
		case ASS_AUTOCARD:
			MsgDelegateEnableActionDelegate().OnResponse(context, deskPos, data, dataSize, msgID);
			break;
		case THING_OUT_PAI:
			{
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_WAIT_DISCARD_CARD).OnResponse(context, playerPos, data, dataSize, msgID);
				if(!res) res = extMsgDelegate->GetDelegate(MSG_DELEGATE_READY_HAND_DISCARD).OnResponse(context, playerPos, data, dataSize, msgID);
				if(!res)
					LOGGER_FILE(context, "player "<< (int)deskPos << " Out Card error");
			}
			break;
		case THING_PENG_PAI:
			{
				InterceptionResponseData intcData(InterceptionResponseData::Peng, data, dataSize);
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnResponse(context, playerPos, &intcData, sizeof(intcData), msgID);
				if(!res)
					LOGGER_FILE(context, "player "<< (int)deskPos << " Peng error");
			}
			break;
		case THING_GANG_PAI:
			{
				InterceptionResponseData intcData(InterceptionResponseData::Gang, data, dataSize);
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnResponse(context, playerPos, &intcData, sizeof(intcData), msgID);
				if(!res)
					LOGGER_FILE(context, "player "<< (int)deskPos << " Gang error");
			}
			break;
		case THING_TING_PAI:
			{
				InterceptionResponseData intcData(InterceptionResponseData::Ting, data, dataSize);
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnResponse(context, playerPos, &intcData, sizeof(intcData), msgID);
			}
			break;
		case THING_HU_PAI:
			{
				InterceptionResponseData intcData(InterceptionResponseData::Win, data, dataSize);
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnResponse(context, playerPos, &intcData, sizeof(intcData), msgID);
				if(!res)
					LOGGER_FILE(context, "player "<< (int)deskPos << " Hu error");
			}
			break;
		case ASS_PASS_CHECK:
			{
				InterceptionResponseData intcData(InterceptionResponseData::Ignore, data, dataSize);
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnResponse(context, playerPos, &intcData, sizeof(intcData), msgID);
			}
			break;
		case THING_CHANGE_NOTIFY:
			{
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_HUAN_ZHANG).OnResponse(context, playerPos, data, dataSize, msgID);
			}
			break;
		case THING_DING_QUE:
			{
				res = extMsgDelegate->GetDelegate(MSG_DELEGATE_DING_QUE).OnResponse(context, playerPos, data, dataSize, msgID);
			}
			break;
		}

		//当操作成功时才取消玩家行为代理
		if(res)
			extPlayerActDelegate->OnPlayerResponse(playerPos, msgID);

		return true;
	}

	void OnGetGameStation(GameContext *context, int gameState, unsigned char bDeskStation, UINT uSocketID, bool bWatchUser)
	{
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
		OBJ_GET_EXT(context, ExtensionHuanZhang, extHuanZhang);
		OBJ_GET_EXT(context, ExtensionGameDirection, extGameDirection);

		GameStationDataEx gsData;
		gsData.Init();

		gsData.byAutoOutTime = extGameParam->GetParam(GameParam_DecisionTimeLimit);//自动出牌时间

		gsData.byOutTime = extGameParam->GetParam(GameParam_DecisionTimeLimit);				//出牌时间
		gsData.byThinkTime = extGameParam->GetParam(GameParam_DecisionTimeLimit);			//思考时间
		gsData.byBeginTime = extGameParam->GetParam(GameParam_BeginWait);			//开始等待时间
		gsData.bAutoBegin =  extGameParam->GetParam(GameParam_AutoBegin);			//时间到了是否自动开始


		gsData.m_byFengDing = extGameParam->GetParam(GameParam_FanLimit); //原清远麻将封顶设置
		gsData.m_bShowTax = extGameParam->IsTrue(GameParam_ShowTax);
		gsData.m_iBaseFen = extGameParam->GetParam(GameParam_DiFen);
		gsData.m_bForceFinish = false;
		gsData.m_bChengBao = false;

		gsData.iVersion	=  MAIN_VERSION;
		gsData.iVersion2	= ASS_VERSION;

		gsData.m_tagPM.nPlayerCount = context->GetPlayerCount();


		//获取房间倍率逃跑罚分
		gsData.bShowUserId	= extGameParam->IsTrue(GameParam_ShowUserID);
		gsData.Stationpara = extGameRunningData->GetGameState();
		int agreeToPlayFlag = 0;
		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			if(extGameRunningData->IsPlayerReadyToPlay(i))
			{
				agreeToPlayFlag |= (1 << i);
			}
		}

		gsData.ucAgree = agreeToPlayFlag;
	

		if(gameState == GS_WAIT_ARGEE || gameState == GS_WAIT_NEXT || gameState == GS_SEND_CARD)
		{
			m_GamePlatformProxy->SendGameStation(bDeskStation, uSocketID, bWatchUser, &gsData, sizeof(gsData));
		}
		else if(gameState == GS_PLAY_GAME)
		{
			OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
			for(int i = 0; i < context->GetPlayerCount(); ++i)
			{
				Player *player = extPlayer->GetPlayer(i);
				CopyInterceptionData(gsData.m_UserGCPData[i], sizeof(gsData.m_UserGCPData[i])/ sizeof(gsData.m_UserGCPData[i][0]),
					i, player->GetInterceptions());
				CopyCards((uchar *)gsData.m_byArOutPai[i], sizeof(gsData.m_byArOutPai[i]), extCardBlocks->GetDiscardedCards(i), false);

				gsData.m_bTing[i] = extPlayer->GetPlayerStatus(i).GetBool(PlayerStatus_ReadyHand);
				gsData.m_bistuoguan[i] = extPlayer->GetPlayerStatus(i).GetBool(PlayerStatus_ActionDelegate);
			}

			gsData.m_RemainingCards = extCardBlocks->GetCardBlocks()->Count();

			gsData.byGetPai = extGameDirection->GetStartDrawPos();
			gsData.byDirection = extGameDirection->GetStartCardBlockPos1();
			gsData.bySezi0 = extGameDirection->GetDices1()[0]; 
			gsData.bySezi1 = extGameDirection->GetDices1()[1];

			CopyCardBlocks((uchar *)gsData.m_iArMenPai, PLAY_COUNT, sizeof(gsData.m_iArMenPai), *extCardBlocks->GetCardBlocks(), true);

			{
				PlayerPosList visiblePos;
				visiblePos.insert(bDeskStation);
				CopyAllCardsInHand((uchar *)gsData.m_byArHandPai, PLAY_COUNT, sizeof(gsData.m_byArHandPai), extPlayer->GetPlayerList(), visiblePos);
			}

			gsData.byNtStation = extBankerSelector->GetBankerPos();
			gsData.m_byNowOutStation = extPlayer->GetActivePlayerPos();

			const WinnerList &winners = extLiquidation->GetWinners();

			PlayerPosList visiblePos;
			for(WinnerList::const_iterator itr = winners.begin(); itr != winners.end(); ++itr)
			{
				visiblePos.insert(*itr);
			}
			visiblePos.insert(bDeskStation);

			CopyAllCardsInHand((uchar *)gsData.m_byArHandPai, PLAY_COUNT, sizeof(gsData.m_byArHandPai),
				extPlayer->GetPlayerList(), visiblePos);

			m_GamePlatformProxy->SendGameStation(bDeskStation, uSocketID, bWatchUser, &gsData, sizeof(gsData));

			// 重发拦牌选项
			OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayConnector);
			extGamePlayConnector->ResendQuestion(bDeskStation, 0);
		}	
	}

	void OnGetGameStationEx(GameContext *context, int gameState, unsigned char bDeskStation, UINT uSocketID, bool bWatchUser)
	{
		switch(gameState)
		{
		case GS_WAIT_ARGEE:
		case GS_WAIT_NEXT:
			{
				GameStationData_Base gsData;
				FillBaseData(context, gsData);
				m_GamePlatformProxy->SendGameStation(bDeskStation, uSocketID, bWatchUser, &gsData, sizeof(gsData));
				break;
			}
		case GS_DICE:
			{
				GameStationData_Dice gsData;
				FillDiceData(context, gsData);
				m_GamePlatformProxy->SendGameStation(bDeskStation, uSocketID, bWatchUser, &gsData, sizeof(gsData));
				break;
			}
		case GS_SEND_CARD:
			{
				GameStationData_SendCard gsData;
				FillSendCardData(context, gsData, bDeskStation);
				m_GamePlatformProxy->SendGameStation(bDeskStation, uSocketID, bWatchUser, &gsData, sizeof(gsData));
				break;
			}
		case GS_PLAY_GAME:
			{
				GameStationData_Playing gsData;
				FillPlayingData(context, gsData, bDeskStation);
				m_GamePlatformProxy->SendGameStation(bDeskStation, uSocketID, bWatchUser, &gsData, sizeof(gsData));

				{
					CardList sendCard;
					bool bError = false;
					for(int i = 0; i < MAX_HAND_PAI; ++i)
					{
						Card::Type card = gsData.m_byArHandPai[bDeskStation][i];
						sendCard.Add(card);
						if(0 == card)
							bError = true;
					}

					LOGGER_FILE(context,"OnGetGameStation playing, send to player " << (int)bDeskStation << ", handcard error = " << (int)bError << ", hand card = " << sendCard);

					if(bError)
					{
						OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
						LOGGER_FILE(context,"server hand card =  " << extPlayer->GetPlayer(bDeskStation)->GetCards());
					}
				}
				
				// 重发拦牌选项
				OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayConnector);
				extGamePlayConnector->ResendQuestion(bDeskStation, 0);

				break;
			}
		}
	}

	void FillBaseData(GameContext *context, GameStationData_Base &gsData)
	{
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

		gsData.Stationpara	= extGameRunningData->GetGameState();
		gsData.iVersion		= MAIN_VERSION;
		gsData.iVersion2	= ASS_VERSION;

		int agreeToPlayFlag = 0;
		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			if(extGameRunningData->IsPlayerReadyToPlay(i))
			{
				agreeToPlayFlag |= (1 << i);
			}
		}

		gsData.ucAgree		 = agreeToPlayFlag;
		gsData.byAutoOutTime = extGameParam->GetParam(GameParam_DecisionTimeLimit);//自动出牌时间
		gsData.byOutTime	 = extGameParam->GetParam(GameParam_DecisionTimeLimit);				//出牌时间
		gsData.byThinkTime	 = extGameParam->GetParam(GameParam_DecisionTimeLimit);			//思考时间
		gsData.byBeginTime	 = extGameParam->GetParam(GameParam_BeginWait);			//开始等待时间

		gsData.m_BasePoint  = extGameParam->GetParam(GameParam_DiFen);
		gsData.m_tagPM.nPlayerCount = context->GetPlayerCount();
		gsData.m_tagPM.iPlayingMethod =  extGameParam->GetParam(GameParam_PlayMethod);
		gsData.m_tagPM.bFanMa = extGameParam->GetParam(GameParam_bFanMa); //是否翻马
		gsData.m_tagPM.bFengDing = extGameParam->GetParam(GameParam_bFengDing);//是否封顶
		gsData.m_tagPM.FanMaNum = extGameParam->GetParam(GameParam_FanMaNum);//翻马个数
		gsData.m_tagPM.FengDingMulti = extGameParam->GetParam(GameParam_FengDingMulti);//封顶倍数

		for(int i = 0; i < context->GetPlayerCount(); ++i)
			gsData.m_byWindDes[i] = extPlayer->GetDes(i);

		//if(extGameParam->IsTrue(GameParam_YaoOrJiu))
		//	gsData.m_tagPM.iPlayingMethod |= PM_YaoJiu_One_Or_Nine;
		//if(extGameParam->IsTrue(GameParam_HaveJoker))
		//	gsData.m_tagPM.iPlayingMethod |= PM_HaveJoker;
		//else
		//	gsData.m_tagPM.iPlayingMethod |= PM_NoJoker;
		//if(extGameParam->IsTrue(GameParam_DanDiaoJoker12))
		//	gsData.m_tagPM.iPlayingMethod |= PM_DanDiao_Joker;
		//if(extGameParam->IsTrue(GameParam_TianDiHuDouble))
		//	gsData.m_tagPM.iPlayingMethod |= PM_TianDiHu_Double;
		//if(extGameParam->IsTrue(GameParam_FourJoker))
		//	gsData.m_tagPM.iPlayingMethod |= PM_ForeJoker_ZiMo;
		//if(extGameParam->IsTrue(GameParam_QiangGangDouble))
		//	gsData.m_tagPM.iPlayingMethod |= PM_QiangGang_Double;
		//if(extGameParam->IsTrue(GameParam_TimeOutAuto))
		//	gsData.m_tagPM.iPlayingMethod |= PM_TimeOut_Auto;
	}

	void FillDiceData(GameContext *context, GameStationData_Dice &gsData)
	{
		FillBaseData(context, gsData);

		OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
		OBJ_GET_EXT(context, ExtensionGameDirection, extGameDirection);

		gsData.byNtStation = extBankerSelector->GetBankerPos();
		gsData.byGetPai = extGameDirection->GetStartDrawPos();
		gsData.byDirection = extGameDirection->GetStartCardBlockPos1();
		gsData.bySezi0 = extGameDirection->GetDices1()[0]; 
		gsData.bySezi1 = extGameDirection->GetDices1()[1];
	}

	void FillSendCardData(GameContext *context, GameStationData_SendCard &gsData, unsigned char bDeskStation)
	{
		FillDiceData(context, gsData);

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);

		PlayerPosList visiblePos;
		visiblePos.insert(bDeskStation);
		CopyAllCardsInHand((uchar *)gsData.m_byArHandPai, PLAY_COUNT, sizeof(gsData.m_byArHandPai), extPlayer->GetPlayerList(), visiblePos);

		gsData.m_RemainingCards = extCardBlocks->GetCardBlocks()->Count();

		CardList jokerList = extHuPattern->GetJokers();

		if(!jokerList.empty())
		{
			gsData.m_byJoker = jokerList.at(0);
			gsData.m_byFanCard = Card::GetLast(gsData.m_byJoker);
		}
	}

	void FillPlayingData(GameContext *context, GameStationData_Playing &gsData, unsigned char bDeskStation)
	{
		FillSendCardData(context, gsData,bDeskStation);

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			Player *player = extPlayer->GetPlayer(i);
			CopyInterceptionData(gsData.m_UserGCPData[i], sizeof(gsData.m_UserGCPData[i])/ sizeof(gsData.m_UserGCPData[i][0]),
				i, player->GetInterceptions());
			CopyCards((uchar *)gsData.m_byArOutPai[i], sizeof(gsData.m_byArOutPai[i]), extCardBlocks->GetDiscardedCards(i), false);

			gsData.m_bistuoguan[i] = extPlayer->GetPlayerStatus(i).GetBool(PlayerStatus_ActionDelegate);
		}

		gsData.m_byNowOutStation = extPlayer->GetActivePlayerPos();


		const WinnerList &winners = extLiquidation->GetWinners();

		PlayerPosList visiblePos;
		for(WinnerList::const_iterator itr = winners.begin(); itr != winners.end(); ++itr)
		{
			visiblePos.insert(*itr);
			gsData.m_bIsHu[*itr] = true;

			if(gsData.m_byHuCard == 255)
			{
				Player *player = extPlayer->GetPlayer(*itr);
				for(auto itr2 = player->GetInterceptions().begin(); itr2 != player->GetInterceptions().end(); ++itr2)
				{
					if(Interception::Hu == itr2->type)
					{
						gsData.m_byHuCard = itr2->src.card;
						break;
					}
				}
			}
		}

		visiblePos.insert(bDeskStation);

		CopyAllCardsInHand((uchar *)gsData.m_byArHandPai, PLAY_COUNT, sizeof(gsData.m_byArHandPai),extPlayer->GetPlayerList(), visiblePos);
	}
};

#endif