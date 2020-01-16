#ifndef MSG_DELEGATE_DISCARD_RESULT_H
#define MSG_DELEGATE_DISCARD_RESULT_H

#include "ExtensionPlayer.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionLogger.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionPlayerReadyHand.h"
#include "NotificationDiscardCard.h"

#include "MessageDelegate.h"
#include "NotificationDiscardCard.h"
#include "MessageSender.h"
#include "AdapterUtil.h"
#include "UpgradeMessageEx.h"

class MsgDelegateDiscardResult : public MessageDelegateImpl
{
public:
	void OnSend(GameContext *context, void *data)
	{
		DiscardCardData &discardCardData = *(DiscardCardData *)data;

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		INIT_MSG(tagOutPaiEx, msg, 0, dataToSend, dataSendSize);

		msg.byUser = discardCardData.playerPos;
		msg.byPs = discardCardData.card;
		bool bTing = extPlayer->GetPlayerStatus(discardCardData.playerPos).GetBool(PlayerStatus_ReadyHand);

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			CardList cards = extCardBlocks->GetDiscardedCards(i);
			CopyCards(msg.m_byArOutPai[i], sizeof(msg.m_byArOutPai[i]), cards, false);
		}

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			PlayerPosList posList = ShownCardPlayerPosList(extPlayer);
			posList.insert(i);
			CopyAllCardsInHand((BYTE *)msg.m_byArHandPai, PLAY_COUNT, sizeof(msg.m_byArHandPai), extPlayer->GetPlayerList(), posList);
			extGameRunningData->SendDataToPlayer(i, dataToSend, dataSendSize, THING_OUT_PAI);
		}

		LOGGER_FILE(context, "player "<<(int)msg.byUser<<" discard "<<(int)msg.byPs<<" "<<(int)bTing);
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID){return true;}
};

#endif