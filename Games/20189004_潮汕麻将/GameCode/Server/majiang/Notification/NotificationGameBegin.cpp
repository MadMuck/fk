#include "NotificationGameBegin.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

void NotificationGameBegin::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_GAME_BEGIN).OnSend(context, NULL);
}
