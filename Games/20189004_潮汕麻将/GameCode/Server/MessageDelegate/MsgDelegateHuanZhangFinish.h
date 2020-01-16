#ifndef MSG_DELEGATE_HUAN_ZHANG_FINISH_H
#define MSG_DELEGATE_HUAN_ZHANG_FINISH_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionPlayer.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHuanZhang.h"
#include "ExtensionLogger.h"
#include "NotificationHuanZhangFinish.h"

class MsgDelegateHuanZhangFinish : public MessageDelegateImpl
{

public:

	void OnSend(GameContext *context, void *data)
	{
		INIT_MSG(HuanZhangFinish, msg, 0, dataToSend, dataSendSize);

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionHuanZhang, extHuanZhang);

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			msg.Clear();
			PlayerPosList posList = ShownCardPlayerPosList(extPlayer);
			posList.insert(i);
			CopyAllCardsInHand((uint8 *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), extPlayer->GetPlayerList(), posList);
			CardList swapCards = extHuanZhang->GetswapCard(i);
			CardList selectCards = extHuanZhang->Get(i);
			for(int j = 0; j < swapCards.size() && j < 3; ++j)
			{
				msg.m_byHuanZhangData[i][j] = swapCards.at(j);
				msg.m_bySelectData[i][j] = selectCards.at(j);
			}
			extGameRunningData->SendDataToPlayer(i, dataToSend, dataSendSize, THING_CHANGE_SEND_CARD);

			LOGGER_FILE(context, "player "<< i <<" get card " << " with " << swapCards);
		}

		// 换张结束，清空换张选项
		extHuanZhang->Clear();
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		return true;
	}
};

#endif