#ifndef MSG_DELEGATE_INTERCEPTION_RESULT_H
#define MSG_DELEGATE_INTERCEPTION_RESULT_H

#include "ExtensionPlayer.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionLiquidation.h"
#include "ExtensionLogger.h"
#include "ExtensionLiquidation.h"
#include "ExtensionScoreItemList.h"

#include "MessageDelegate.h"
#include "NotificationInterceptionResult.h"
#include "MessageSender.h"
#include "AdapterUtil.h"

#include "PlayerStatusDef.h"
#include "ScoreTypeDef.h"


class MsgDelegateInterceptionResult : public MessageDelegateImpl
{
public:

	void OnSend(GameContext *context, void *data)
	{
		InterceptionResultData &resultData = *(InterceptionResultData *)data;

		LOGGER_FILE(context, "player "<<(int)resultData.playerPos<<" do with type "<<(int)resultData.interception.type);

		switch(resultData.interception.type)
		{
		case Interception::Chi:
			Chi(context, resultData.playerPos, resultData.interception);
			break;
		case Interception::Peng:
			Peng(context, resultData.playerPos, resultData.interception);
			break;
		case Interception::MingGang:
		case Interception::AnGang:
		case Interception::BuGang:
		case Interception::MingGangTing:
		case Interception::AnGangTing:
		case Interception::BuGangTing:
		/*case Interception::YangMa:*/
			Gang(context, resultData.playerPos, resultData.interception);
			break;
		case Interception::Hu:
			Win(context, resultData.playerPos, resultData.interception);
			break;
		case Interception::Invalid:
			{

			}
			break;
		}		
	}

	void Chi(GameContext *context, PlayerPos::Type playerPos, const Interception &interception)
	{
		tagChiPaiEx msg;
		msg.Clear();
		msg.byPs = interception.src.card;
		msg.byUser = playerPos;
		msg.byBeChi = interception.src.playerPos;
		CopyCards(msg.byChiPs, sizeof(msg.byChiPs), interception.cards, false);

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			CopyInterceptionData(msg.m_UserGCPData[i], 
				sizeof(msg.m_UserGCPData[i])/ sizeof(msg.m_UserGCPData[i][0]),
				i, extPlayer->GetPlayer(i)->GetInterceptions());

			CardList discardedCards = extCardBlocks->GetDiscardedCards(i);
			CopyCards(msg.m_byArOutPai[i], sizeof(msg.m_byArOutPai[i]), discardedCards, false);
		}

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			PlayerPosList posList = ShownCardPlayerPosList(extPlayer);
			posList.insert(i);
			CopyAllCardsInHand((unsigned char *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), extPlayer->GetPlayerList(), posList);
			extGameRunningData->SendDataToPlayer(i, &msg, sizeof(msg), THING_CHI_PAI);
		}

	}

	void Peng(GameContext *context, PlayerPos::Type playerPos, const Interception &interception)
	{
		tagPengPaiEx msg;
		msg.Clear();
		msg.byPs = interception.src.card;
		msg.byUser = playerPos;
		msg.byBePeng = interception.src.playerPos;

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		extGameRunningData->SendDataToAll(&msg, sizeof(msg), THING_PENG_PAI);
	}

	void Gang(GameContext *context, PlayerPos::Type playerPos, const Interception &interception)
	{
		tagGangPaiEx msg;
		msg.Clear();
		msg.byPs = interception.src.card;
		msg.byUser = playerPos;
		msg.byBeGang = interception.src.playerPos;
		msg.byType = GetCpgType(interception.type);

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionScoreItemList, extScoreItemList);
		OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
		OBJ_GET_EXT(context,ExtensionGameParam,extGamePar);

		if(Interception::BuGang == interception.type /*|| Interception::YangMa == interception.type*/)
		{
			//修正补杠来源
			InterceptionList interceptionList = extPlayer->GetPlayer(playerPos)->GetInterceptions();
			if(!interceptionList.empty())
			{
				Interception gang = extPlayer->GetPlayer(playerPos)->GetInterceptions().back();
				msg.byBeGang = gang.src.playerPos;
			}
		}

		// 计算最近一次杠的分数
		ScoreItemList gangScoreItemList = extScoreItemList->GetItemOfType(playerPos, ScoreType_Gang);
		Liquidation liquidation;
		liquidation.Prepare(extBankerSelector->GetBankerPos(), context->GetPlayerCount());

		if(!gangScoreItemList.empty())
			liquidation.Add(playerPos, *gangScoreItemList.rbegin());

		int iBaseFen = extGamePar->GetParam(GameParam_GameScorePoint);

		if(msg.byType != ACTION_YANG_MA)
		{
			for(int i = 0; i < context->GetPlayerCount(); ++i)
			{
				msg.iGangFen[i] = liquidation.GetScoreList().GetScore(i) * iBaseFen;
			}
		}
		
		extGameRunningData->SendDataToAll(&msg, sizeof(msg), THING_GANG_PAI);
	}

	void Win(GameContext *context, PlayerPos::Type playerPos, const Interception &interception)
	{
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		tagHuPaiEx msg;
		msg.Clear();
		msg.byPs = interception.src.card;
		msg.byUser = playerPos;
		msg.byDianPao = interception.src.playerPos != playerPos ? interception.src.playerPos : 255;
		msg.bZimo = interception.src.playerPos == playerPos;
		msg.byUserNum = (unsigned char)extLiquidation->GetWinners().size();

		PlayerPosList posList;

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			CardList discardedCards = extCardBlocks->GetDiscardedCards(i);
			CopyCards(msg.m_byArOutPai[i], sizeof(msg.m_byArOutPai[i]), discardedCards, false);

			posList.insert(i);
		}

		const WinTypeList &winTypeList = extLiquidation->GetWinType().GetWinType(playerPos);
		WinTypeList outerWinTypeList;
//		outerWinTypeList = TranslateWinTypeList(winTypeList);//天胡，地胡，抢杠不需要发给客户端
		outerWinTypeList = winTypeList;
		int winTypeCnt = 0;
		for(WinTypeList::const_iterator itr = outerWinTypeList.begin(); itr != outerWinTypeList.end(); ++itr)
		{
			msg.byHuType[winTypeCnt++] = *itr;

			if(winTypeCnt >= MAX_HUPAI_TYPE)
				break;
		}

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			CopyAllCardsInHand((unsigned char *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), extPlayer->GetPlayerList(), posList);
			extGameRunningData->SendDataToPlayer(i, &msg, sizeof(msg), THING_HU_PAI);
		}
	}

	void Ting(GameContext *context, PlayerPos::Type playerPos, const Interception &interception)
	{
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		tagTingPaiEx msg;
		msg.Clear();
		msg.byUser = playerPos;
		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			msg.bHitTing[i] = extPlayer->GetPlayerStatus(i).GetBool(PlayerStatus_ReadyHand);
		}
		extGameRunningData->SendDataToAll(&msg, sizeof(msg), THING_TING_PAI);
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID){return true;}
};


#endif