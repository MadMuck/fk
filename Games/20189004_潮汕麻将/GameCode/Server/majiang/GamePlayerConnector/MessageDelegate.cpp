#include "MessageDelegate.h"

MessageDelegate::MessageDelegate(MessageDelegateImpl *impl):m_Impl(impl){}

void MessageDelegate::OnSend(GameContext *gameContext, void *data)
{
	if(m_Impl.GetObj())
		m_Impl->OnSend(gameContext, data);
}

bool MessageDelegate::OnResponse(GameContext *gameContext, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
{
	bool ret = false;
	if(m_Impl.GetObj())
		ret = m_Impl->OnResponse(gameContext, playerPos, data, dataSize, msgID);

	return ret;
}