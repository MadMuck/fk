#ifndef NOTIFICATION_INTERCEPTION_RESULT_H
#define NOTIFICATION_INTERCEPTION_RESULT_H

#include "Notification.h"
#include "PlayerPos.h"
#include "Interception.h"

#define MSG_DELEGATE_INTERCEPTION_RESULT "InterceptionResult"

struct InterceptionResultData
{
	PlayerPos::Type playerPos;
	Interception interception;
};

class NotificationInterceptionResult : public Notification
{
	InterceptionResultData m_Data;

public:
	NotificationInterceptionResult(PlayerPos::Type playerPos, const Interception &interception);

	void OnSend(GameContext *context) const;
};

#endif