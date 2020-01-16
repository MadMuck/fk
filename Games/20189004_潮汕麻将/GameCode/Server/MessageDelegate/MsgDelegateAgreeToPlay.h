#ifndef MSG_DELEGATE_AGREE_TO_PLAY_H
#define MSG_DELEGATE_AGREE_TO_PLAY_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"

class MsgDelegateAgreeToPlay
{

public:
	MsgDelegateAgreeToPlay(){}

	void OnResponse(GameContext *context, PlayerPos::Type playerPos, int msgID)
	{
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		extGameRunningData->PlayerGetReadyToPlay(playerPos);
		extGameRunningData->SendDataToAll(NULL, 0, msgID);
	}
};

#endif