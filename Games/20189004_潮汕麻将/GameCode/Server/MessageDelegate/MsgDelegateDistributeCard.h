#ifndef MSG_DELEGATE_DISTRIBUTE_CARD_H
#define MSG_DELEGATE_DISTRIBUTE_CARD_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionTimer.h"
#include "ExtensionLogger.h"
#include "ExtensionTimer.h"
#include "Util.h"
#include "NotificationDistributeCard.h"

#include "AdapterUtil.h"

class MsgDelegateDistributeCard : public MessageDelegateImpl
{

public:

	void OnSend(GameContext *context, void *data)
	{

		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);

		INIT_MSG(tagSendPaiEx, msg, 0, dataToSend, dataSendSize);

		CardList jokerList = extHuPattern->GetJokers();

		msg.byAllPaiNum = extCardBlocks->GetCardBlocks()->Count();

		msg.byRemainPai = extCardBlocks->GetCardBlocks()->Count();

		if(!jokerList.empty())
		{
			msg.m_byJoker = jokerList.at(0);
			msg.m_byFanCard = Card::GetLast(msg.m_byJoker);
		}

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			PlayerPosList posList = ShownCardPlayerPosList(extPlayer);
			posList.insert(i);
			CopyAllCardsInHand((uint8 *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), extPlayer->GetPlayerList(), posList);
			extGameRunningData->SendDataToPlayer(i, dataToSend, dataSendSize, THING_SEND_PAI);
		}

		extGameRunningData->SetGameState(GS_SEND_CARD);

		LOGGER_FILE(context, "dispatch cards");
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		return true;
	}
};

#endif