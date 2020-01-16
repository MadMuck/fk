#ifndef MSG_DELEGATE_LIQUIDATION_H
#define MSG_DELEGATE_LIQUIDATION_H

#include "ExtensionLiquidation.h"
#include "ExtensionGameParam.h"
#include "ExtensionTimer.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionPlayer.h"
#include "ExtensionLogger.h"
#include "NotificationLiquidation.h"
#include "ExtensionScoreItemList.h"
#include "ExtensionFanMa.h"

#include "SpecCardScoreDelegate.h"
#include "ScoreTypeDef.h"
#include "MessageDelegate.h"
#include "WinType.h"
#include "UpgradeMessageEx.h"
#include "Util.h"

class MsgDelegateLiquidation : public MessageDelegateImpl
{
	Liquidation m_Liquidation;
	WinType *m_WinTypes;

	static void DelaySendMsg(void **data, int dataCnt)
	{
		GameContext *context = (GameContext *)data[0];
		MsgDelegateLiquidation *msgDelegate = (MsgDelegateLiquidation *)data[1];
		Liquidation &liquidation = msgDelegate->m_Liquidation;
		WinType *winTypes = msgDelegate->m_WinTypes;

		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
		OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSeletor);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
		OBJ_GET_EXT(context, ExtensionScoreItemList, extScoreItemList);
		OBJ_GET_EXT(context, ExtensionFanMa, extFanMa);
		

		const WinnerList &winners = extLiquidation->GetWinners();

		INIT_MSG(tagCountFenEx, msg, 0, dataToSend, dataSendSize);

		msg.byNt = extBankerSeletor->GetBankerPos();

		PlayerPosList visiblePos;
		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			visiblePos.insert(i);
		}
		CopyAllCardsInHand((uchar *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), 
			extPlayer->GetPlayerList(), visiblePos);

		std::set<PlayerPos::Type> playerReadyHand; // 能听牌的玩家
		std::set<PlayerPos::Type> playerNotReadyHand; // 未能听牌的玩家
		std::set<PlayerPos::Type> playerNotQueMen; // 定缺玩法下未缺一门玩家
		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			CopyInterceptionData(msg.m_UserGCPData[i], 
				sizeof(msg.m_UserGCPData[i])/sizeof(msg.m_UserGCPData[i][0]),
				i, extPlayer->GetPlayer(i)->GetInterceptions());

			msg.bHu[i] = winners.Contains(i);
			msg.m_iZongFan[i] = extScoreItemList->GetWinningFan(i);	
			if(msg.bHu[i])
			{
				msg.m_bIsZiMo[i] = extLiquidation->GetWinMode(i) == WinMode_Self;
				if(extLiquidation->GetLoser(i) < PlayerPos::Max)
					msg.m_bDianPao[extLiquidation->GetLoser(i)] = true;
				const WinTypeList &winTypeList = winTypes->GetWinType(i);

				msg.m_iPaiTouCount[i] = extFanMa->GetZhongMaCount(i);
				
				Player *player = extPlayer->GetPlayer(i);

				int winTypeCnt = 0;
				WinTypeList outerWinTypeList;
				//总分
				msg.m_iZongFan[i] = SpecCardScoreDelegate::GetFanWithWinTypes(context, winTypeList);
			//	outerWinTypeList = TranslateWinTypeList(winTypeList);
				outerWinTypeList = winTypeList;
				for(WinTypeList::const_iterator itr = outerWinTypeList.begin(); itr != outerWinTypeList.end(); ++itr)
				{
					msg.byHuType[i][winTypeCnt++] = *itr;
				}

				// 胡牌牌值
				const InterceptionList &interceptions = player->GetInterceptions();
				for(InterceptionList::const_iterator itr = interceptions.begin();
					itr != interceptions.end();
					++itr)
				{
					if(itr->type == Interception::Hu)
					{
						msg.m_byPs[i] = itr->src.card;
						break;
					}
				}

				LOGGER_FILE(context, "Player "<<(int)i<<" has inner win type :"<<winTypeList);
				LOGGER_FILE(context, "Player "<<(int)i<<" has outter win type :"<<outerWinTypeList);
			}

			msg.iZongFen[i] = liquidation.GetScoreList().GetScore(i);
			msg.iHuFen[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_Hu);
			msg.iGangFen[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_Gang);

			msg.iDianGangFen[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_MingGang);//接杠3分，放杠一家扣3分
			msg.iMingGangFen[i] =  liquidation.GetScoreOfType(i).GetScore(ScoreType_BuGang);//明杠3分，三家扣1分
			msg.iAnGangFen[i] =  liquidation.GetScoreOfType(i).GetScore(ScoreType_AnGang);//暗杠6分，三家扣2分
			msg.m_iPaiTouFen[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_PaiTou);

			LOGGER_FILE(context, "Player "<< (int)i <<" ZongFen = " << (int)msg.iZongFen[i]<<"马分"<<(int)msg.m_iPaiTouFen[i]
												<<"点杠"<<(int)msg.iDianGangFen[i]<<"明杠"<<(int)msg.iMingGangFen[i]<<"暗杠"<<(int)msg.iAnGangFen[i]);
		}


		extGameRunningData->SendDataToAll(dataToSend, dataSendSize, THING_ENG_HANDLE);

		LOGGER_FILE(context, "Liquidation");
		
	}

public:

	void OnSend(GameContext *context, void *data)
	{
		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);

		m_Liquidation =  *(Liquidation *)data;
		m_WinTypes = &extLiquidation->GetWinType();

// 		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
// 		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
// 		float delay = extGameParam->GetParam(GameParam_LiquidationDelay) * 1.0f / 1000;
// 
 		void *pointers[] = {context, this};
// 		extTimer->AddToQueue(delay, TimerData(pointers, ARRAY_LEN(pointers), DelaySendMsg));
		DelaySendMsg(pointers,ARRAY_LEN(pointers));
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		return true;
	}
};

#endif