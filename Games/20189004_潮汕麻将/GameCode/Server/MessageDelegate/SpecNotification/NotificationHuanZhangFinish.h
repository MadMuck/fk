#ifndef NOTIFICATION_HUAN_ZHANG_FINISH_H
#define NOTIFICATION_HUAN_ZHANG_FINISH_H

#include "Notification.h"
#include "ExtensionMessageDelegate.h"

#define MSG_DELEGATE_HUAN_ZHANG_FINISH "HuanZhangFinish"


class NotificationHuanZhangFinish : public Notification
{
public:
	void OnSend(GameContext *context) const
	{
		OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
		extMsgDelegate->GetDelegate(MSG_DELEGATE_HUAN_ZHANG_FINISH).OnSend(context, NULL);
	}
};

#endif