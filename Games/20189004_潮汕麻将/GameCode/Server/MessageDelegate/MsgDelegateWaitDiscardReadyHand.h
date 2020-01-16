#ifndef MSG_DELEGATE_WAIT_DISCARD_READY_HAND_H
#define MSG_DELEGATE_WAIT_DISCARD_READY_HAND_H

#include "MsgDelegateWaitDiscard.h"

class MsgDelegateWaitDiscardReadyHand : public MsgDelegateWaitDiscard
{
public:
	CardList GetCandidateCards(GameContext *context, PlayerPos::Type playerPos, const CardList &oriCards)
	{
		return oriCards;
	}
};

#endif