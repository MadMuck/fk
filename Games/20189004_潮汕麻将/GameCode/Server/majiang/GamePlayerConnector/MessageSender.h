#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

#include "PlayerPos.h"

class MessageSender
{
public:
	virtual ~MessageSender(){}
	virtual void Send(PlayerPos::Type playerPos, void *data, int dataSize, int msgID) = 0;
	virtual void SendToAll(void *data, int dataSize, int msgID) = 0;
};

template<typename T>
class GameMessageSender : public MessageSender
{
	T *m_Delegate;
public:
	GameMessageSender(T *delegate):m_Delegate(delegate){}

	void Send(PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		m_Delegate->SendDataToPlayer(playerPos, data, dataSize, msgID);
	}

	void SendToAll(void *data, int dataSize, int msgID)
	{
		m_Delegate->SendDataToPlayers(data, dataSize, msgID);
	}
};

#endif