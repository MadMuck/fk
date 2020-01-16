#ifndef MSG_DELEGATE_FIRST_ROUND_H
#define MSG_DELEGATE_FIRST_ROUND_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionTimer.h"
#include "ExtensionLiquidation.h"
#include "ExtensionLogger.h"
#include "ExtensionQueMen.h"
#include "NotificationFirstRun.h"
#include "Util.h"

#include "AdapterUtil.h"

class MsgDelegateFirstRound : public MessageDelegateImpl
{
	static void SendFirstRound(void **data, int dataCnt)
	{
		GameContext *context = (GameContext *)data[0];
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		extGameRunningData->SetGameState(GS_PLAY_GAME);
		return;	//THING_BEGIN_OUT此消息改为在msgdelegatewaitdiscard通知玩家出牌
		/*
		GameContext *context = (GameContext *)data[0];

		OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);

		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		INIT_MSG(tagBeginOutPaiEx, msg, 0, dataToSend, dataSendSize);
		msg.byNt = extBankerSelector->GetBankerPos();
		msg.byUser = extPlayer->GetActivePlayerPos();

		extGameRunningData->SendDataToAll(dataToSend, dataSendSize, THING_BEGIN_OUT);

		LOGGER_FILE(context, "first run");
		*/
	}

public:

	void OnSend(GameContext *context, void *data)
	{
		void *pointers[] = {context};
		int pointerCnt = ARRAY_LEN(pointers);
		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		extTimer->AddToQueue(0.2f, TimerData(pointers, pointerCnt, SendFirstRound));
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		return true;
	}
};

#endif