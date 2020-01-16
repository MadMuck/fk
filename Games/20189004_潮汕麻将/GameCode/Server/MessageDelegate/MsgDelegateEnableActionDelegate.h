#ifndef MSG_DELEGATE_ENABLE_ACTION_DELEGATE_H
#define MSG_DELEGATE_ENABLE_ACTION_DELEGATE_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionGameParam.h"
#include "ExtensionPlayer.h"
#include "PlayerStatusDef.h"
#include "PlayerActionDelegate.h"

class MsgDelegateEnableActionDelegate
{

public:

	void OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);

		if(extGameParam->IsTrue(GameParam_ActionDelegateEnabled))
		{
			OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

			if(extPlayer->GetPlayerList().size() < playerPos)
				return;

			bool enabled = extPlayer->GetPlayerStatus(playerPos).GetBool(PlayerStatus_ActionDelegate);

			Usertouguan req = *(Usertouguan *)data;
			if((req.enable && !enabled) ||
				(!req.enable && enabled))
			{
				extPlayer->GetPlayerStatus(playerPos).SetBool(PlayerStatus_ActionDelegate, req.enable);
				INIT_MSG(Usertouguan, msg, 0, dataToSend, dataSendSize);
				for(int i = 0; i < context->GetPlayerCount(); ++i)
				{
					msg.bTuoGuan[i] = extPlayer->GetPlayerStatus(i).GetBool(PlayerStatus_ActionDelegate);
				}
				msg.desk = playerPos;
				msg.enable = req.enable;

				extGameRunningData->SendDataToAll(dataToSend, dataSendSize, msgID);

				OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActionDelegate);
				extPlayerActionDelegate->OnEnableActionDelegate(playerPos, req.enable);

				extPlayer->GetPlayerStatus(playerPos).SetFlag(PlayerStatus_TimeOutCount, 0);
			}
		}
		
	}
};

#endif