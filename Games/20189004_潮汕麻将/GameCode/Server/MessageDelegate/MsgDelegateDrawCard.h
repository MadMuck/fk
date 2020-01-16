#ifndef MSG_DELEGATE_DRAW_CARD_H
#define MSG_DELEGATE_DRAW_CARD_H

#include "UpgradeMessageEx.h"
#include "ExtensionPlayer.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionProcedureController.h"
#include "ExtensionLogger.h"

#include "MessageDelegate.h"
#include "NotificationNormalDrawCard.h"
#include "MessageSender.h"
#include "AdapterUtil.h"

#include "ProcedureType.h"

class MsgDelegateDrawCard : public MessageDelegateImpl
{

public:

	void OnSend(GameContext *context, void *data)
	{
		NormalDrawCardData &drawCardData = *(NormalDrawCardData *)data;

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);

		INIT_MSG(tagZhuaPaiEx, msg, 0, dataToSend, dataSendSize);

		msg.byUser = drawCardData.playerPos;
		msg.bHead = !extProcCtrl->IsInLatestHistory(ProcedureType::DrawCardAfterKong);

		msg.byRemainingCards = extCardBlocks->GetCardBlocks()->Count();

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			if(i == drawCardData.playerPos)
				msg.byPs = drawCardData.card;
			else
				msg.byPs = 0;

			extGameRunningData->SendDataToPlayer(i, dataToSend, dataSendSize, THING_ZHUA_PAI);
		}

		LOGGER_FILE(context, (int)drawCardData.playerPos<<" draw card "<<(int)drawCardData.card);
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID){return true;}
};

#endif