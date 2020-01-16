#ifndef MSG_DELEGATE_DING_QUE_H
#define MSG_DELEGATE_DING_QUE_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionGamePlayerConnector.h"
#include "PlayerActionDelegate.h"
#include "PlayerActionDelegateListener.h"
#include "ExtensionQueMen.h"
#include "ExtensionGameParam.h"
#include "ExtensionPlayer.h"

#include "QuestionDingQue.h"

class MsgDelegateDingQue : public MessageDelegateImpl
{

	static void AutoDingQue(GameContext *context, int type, PlayerPos::Type playerPos, void *data)
	{
		MsgDelegateDingQue *msgDelegate = (MsgDelegateDingQue *)data;

		std::map<int, int> countOfType;
		countOfType[0] = 0;
		countOfType[1] = 0;
		countOfType[2] = 0;
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		const CardList &cards = extPlayer->GetPlayer(playerPos)->GetCards();
		for(CardList::const_iterator itr = cards.begin();
			itr != cards.end();
			++itr)
		{
			if(*itr / 10 < 3)
				countOfType[*itr / 10] ++;
		}

		int queType = Random() % 3;

		// 选数量最少的花色
		int minNum = cards.Count();
		for(std::map<int, int>::iterator itr = countOfType.begin();
			itr != countOfType.end();
			++itr)
		{
			if(itr->second < minNum)
			{
				minNum = itr->second;
				queType = itr->first;
			}
		}

		OnDingQue(msgDelegate, context, playerPos, queType, THING_DING_QUE);
	}

	static void OnDingQue(MsgDelegateDingQue *msgDelegate, GameContext *context, PlayerPos::Type playerPos, int type, int msgID)
	{
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		if(type >= 0 && type < 3)
		{
			extGamePlayerConnector->OnResponse(playerPos, msgID, type);
		}

		{
			INIT_MSG(tagDingQueEx, msg, 0, dataToSend, dataSendSize);
			for(int i = 0; i < context->GetPlayerCount(); ++i)
			{
				msg.bFinish[i] = extQueMen->HasDetermined(i);
				msg.byQuePai[i] = extQueMen->Get(i);
				msg.byUser = playerPos;
				msg.bNotify = false;
			}

			extPlayer->SortHand(playerPos);

			PlayerPosList posList = ShownCardPlayerPosList(extPlayer);
			posList.insert(playerPos);
			CopyAllCardsInHand((BYTE *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), extPlayer->GetPlayerList(), posList);

			extGameRunningData->SendDataToAll(dataToSend, dataSendSize, msgID);
		}

		msgDelegate->m_WaitingPlayer.erase(playerPos);
	}

	std::set<PlayerPos::Type> m_WaitingPlayer;

public:

	void OnSend(GameContext *context, void *data)
	{
		PlayerPos::Type playerPos = *(PlayerPos::Type *)data;

		m_WaitingPlayer.insert(playerPos);

		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		INIT_MSG(tagDingQueEx, msg, 0, dataToSend, dataSendSize);
		msg.byUser = playerPos;
		msg.bNotify = true;
		extGameRunningData->SendDataToPlayer(playerPos, dataToSend, dataSendSize, THING_DING_QUE);

		LOGGER_FILE(context, "question ding que");

		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		if(extGameParam->GetParam(GameParam_ActionDelegateEnabled) > 0)
		{
			float oriDelay = extGameParam->GetParam(GameParam_DingQueTime) * 1.0f;
			float delay = oriDelay;

			if(extPlayer->GetPlayerStatus(playerPos).GetBool(PlayerStatus_ActionDelegate))
			{
				delay = 1;
			}

			// 设置超时自动定缺
			PlayerActionDelegateListener<MsgDelegateDingQue>::AddActionDelegate(context, playerPos, AutoDingQue, delay, THING_DING_QUE, this);

			if(!extPlayer->GetPlayerStatus(playerPos).GetBool(PlayerStatus_ActionDelegate))
			{
				// 玩家没有托管， 加个监听器
				OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
				extPlayerActDelegate->AddListener(playerPos, 
					PlayerActionDelegateListener<MsgDelegateDingQue>::Create(this, AutoDingQue, oriDelay, THING_DING_QUE));
			}
		}
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		if(m_WaitingPlayer.find(playerPos) == m_WaitingPlayer.end())
			return false;

		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);

		EXTRACT_REQUEST(tagDingQueEx, data, dataSize, req);
		if(!req)
			return false;

		if(extQueMen->HasDetermined(playerPos))
			return false;

		LOGGER_FILE(context, "Player "<<(int)playerPos<<" select que men "<<(int)req->byQue);

		OnDingQue(this, context, playerPos, req->byQue, msgID);

		return true;
	}
};

#endif