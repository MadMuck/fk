#ifndef NOTIFICATION_DISTRIBUTE_CARD_H
#define NOTIFICATION_DISTRIBUTE_CARD_H

#include "Notification.h"

#define MSG_DELEGATE_DISTRIBUTE_CARD "DistributeCard"

class NotificationDistributeCard : public Notification
{
public:
    void OnSend(GameContext *context) const;
};

#endif
