#include "NotificationNormalDrawCard.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

NotificationNormalDrawCard::NotificationNormalDrawCard(PlayerPos::Type playerPos, Card::Type card, bool fromHead)
{
	m_DrawCardData.playerPos = playerPos;
	m_DrawCardData.card = card;
	m_DrawCardData.fromHead = fromHead;
}

void NotificationNormalDrawCard::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_NORMAL_DRAW_CARD).OnSend(context, (void*)&m_DrawCardData);
}
