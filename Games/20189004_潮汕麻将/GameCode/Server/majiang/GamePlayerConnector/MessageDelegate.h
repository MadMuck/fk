#ifndef MESSAGE_DELEGATE_H
#define MESSAGE_DELEGATE_H

#include "GameContext.h"
#include "RefCntObj.h"

class MessageDelegateImpl
{
public:
	virtual ~MessageDelegateImpl(){}

	virtual void OnSend(GameContext *context, void *data) = 0;

	virtual bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID) = 0;
};

class MessageDelegate
{
protected:
	RefCntObj<MessageDelegateImpl> m_Impl;
public:
	virtual ~MessageDelegate(){}
	MessageDelegate(MessageDelegateImpl *impl);

	void OnSend(GameContext *context, void *data);

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID);
};

#endif