// 给客户端的通知
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "GameContext.h"

class Notification
{
public:
    virtual ~Notification(){}

    // 通知发送
    virtual void OnSend(GameContext *context) const = 0;
};

#endif
