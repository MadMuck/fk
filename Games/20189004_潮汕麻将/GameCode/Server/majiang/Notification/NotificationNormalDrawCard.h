#ifndef NOTIFICATION_NORMAL_DRAW_CARD_H
#define NOTIFICATION_NORMAL_DRAW_CARD_H

#include "Notification.h"
#include "PlayerPos.h"
#include "Card.h"

#define MSG_DELEGATE_NORMAL_DRAW_CARD "NormalDrawCard"

struct NormalDrawCardData
{
	PlayerPos::Type playerPos;
	Card::Type card;
	bool fromHead;
};

class NotificationNormalDrawCard : public Notification
{
	NormalDrawCardData m_DrawCardData;
public:
	NotificationNormalDrawCard(PlayerPos::Type playerPos, Card::Type card, bool fromHead);
    void OnSend(GameContext *context) const;
};

#endif
