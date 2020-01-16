#include "NotificationDistributeCard.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

void NotificationDistributeCard::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_DISTRIBUTE_CARD).OnSend(context, NULL);
}
