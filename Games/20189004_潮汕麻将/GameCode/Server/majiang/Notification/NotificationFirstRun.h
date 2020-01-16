#ifndef NOTIFICATION_FIRST_RUN_H
#define NOTIFICATION_FIRST_RUN_H

#include "Notification.h"

#define MSG_DELEGATE_FIRST_RUN "FirstRun"

class NotificationFirstRun : public Notification
{
public:
	void OnSend(GameContext *context) const;
};

#endif