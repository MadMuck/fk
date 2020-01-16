#ifndef NOTIFICATION_DISCARD_CARD_H
#define NOTIFICATION_DISCARD_CARD_H

#include "Notification.h"
#include "PlayerPos.h"
#include "Card.h"

#define MSG_DELEGATE_DISCARD_CARD "DiscardCard"

struct DiscardCardData
{
	PlayerPos::Type playerPos;
	Card::Type card;
};

class NotificationDiscardCard : public Notification
{
	DiscardCardData m_DiscardCardData;
public:
	NotificationDiscardCard(PlayerPos::Type playerPos, Card::Type card);
	void OnSend(GameContext *context) const;
};

#endif
