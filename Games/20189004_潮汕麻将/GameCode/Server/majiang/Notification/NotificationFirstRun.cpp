#include "NotificationFirstRun.h"
#include "ExtensionMessageDelegate.h"

void NotificationFirstRun::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_FIRST_RUN).OnSend(context, NULL);
}