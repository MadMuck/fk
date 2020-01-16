#ifndef MSG_DELEGATE_GAME_BEGIN_H
#define MSG_DELEGATE_GAME_BEGIN_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionTimer.h"
#include "ExtensionLiquidation.h"
#include "ExtensionLogger.h"
#include "Util.h"
#include "NotificationGameBegin.h"

#include "AdapterUtil.h"

class MsgDelegateGameBegin : public MessageDelegateImpl
{
	static void SendGameBegin(void **data, int dataCnt)
	{
		GameContext *context = (GameContext *)data[0];

		OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionGameDirection, extGameDirection);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

		PlayerPos::Type bankerPos = extBankerSelector->GetBankerSelector()->GetBanker();

		{
			// 游戏开始消息
			INIT_MSG(tagBeginEx, msg, 0, dataToSend, dataSendSize);
			msg.byNt = bankerPos;
			msg.byRemainingCards = extCardBlocks->GetCardBlocks()->Count();

			for(int i =0;i<context->GetPlayerCount();++i)
				msg.m_byWindDes[i] = extPlayer->GetDes(i);

			extGameRunningData->SendDataToAll(dataToSend, dataSendSize, THING_BEGIN);
		}
		

		{
			// 发送第一次掷色子结果
			INIT_MSG(tagTwoSeziDirAndGetPai, msg, 0, dataToSend, dataSendSize);
			msg.byUser = bankerPos;
			msg.bySezi0 = extGameDirection->GetDices1()[0];
			msg.bySezi1 = extGameDirection->GetDices1()[1];
			msg.byDirection = extGameDirection->GetStartCardBlockPos1();
			msg.byGetPai = extGameDirection->GetStartDrawPos();
			extGameRunningData->SendDataToAll(dataToSend, dataSendSize, THING_2SEZI_DIR_GP);

			extGameRunningData->SetGameState(GS_DICE);
		}
		

		LOGGER_FILE(context, "game begin");
	}

public:

	void OnSend(GameContext *context, void *data)
	{
		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		void *pointers[] = {context};
		int pointerCnt = ARRAY_LEN(pointers);

		// 让其他任务等当前任务结束一段时间之后才执行
		extTimer->AddToQueue(1.0f, TimerData(pointers, pointerCnt, SendGameBegin));
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		return true;
	}
};

#endif