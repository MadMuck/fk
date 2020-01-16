#include "NotificationInterceptionResult.h"
#include "ExtensionMessageDelegate.h"

NotificationInterceptionResult::NotificationInterceptionResult(PlayerPos::Type playerPos, const Interception &interception)
{
	m_Data.playerPos = playerPos;
	m_Data.interception = interception;
}

void NotificationInterceptionResult::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION_RESULT).OnSend(context, (void*)&m_Data);
}