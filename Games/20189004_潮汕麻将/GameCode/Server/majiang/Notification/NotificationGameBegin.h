#ifndef NOTIFICATION_GAME_BEGIN_H
#define NOTIFICATION_GAME_BEGIN_H

#include "Notification.h"

#define MSG_DELEGATE_GAME_BEGIN "GameBegin"

class NotificationGameBegin : public Notification
{
public:
    void OnSend(GameContext *context) const;
};

#endif
