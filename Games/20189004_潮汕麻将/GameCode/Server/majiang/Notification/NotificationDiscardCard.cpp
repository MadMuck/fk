#include "NotificationDiscardCard.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

NotificationDiscardCard::NotificationDiscardCard(PlayerPos::Type playerPos, Card::Type card)
{
	m_DiscardCardData.playerPos = playerPos;
	m_DiscardCardData.card = card;
}

void NotificationDiscardCard::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_DISCARD_CARD).OnSend(context, (void*)&m_DiscardCardData);
}
