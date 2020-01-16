#ifndef NOTIFICATION_LIQUIDATION_H
#define NOTIFICATION_LIQUIDATION_H

#include "Notification.h"
#include "Liquidation.h"

#define MSG_DELEGATE_LIQUIDATION "Liquidation"

class NotificationLiquidation : public Notification
{
    Liquidation m_Liquidation;
public:
    NotificationLiquidation(const Liquidation &liquidation);
    void OnSend(GameContext *context) const;
};

#endif
