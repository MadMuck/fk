#ifndef QUESTION_WAIT_DISCARD_H
#define QUESTION_WAIT_DISCARD_H

#include "Question.h"
#include "Card.h"
#include "PlayerPos.h"

#define MSG_DELEGATE_WAIT_DISCARD_CARD "WaitDiscardCard"

class DiscardResponseDelegate
{
public:
    virtual void OnSendCard(PlayerPos::Type playerPos, Card::Type card) const = 0;
};

struct QuestionWaitDiscardData
{
	PlayerPos::Type playerPos;
	CardList cards;
};

class QuestionWaitDiscardImpl : public QuestionImpl
{
    PlayerPos::Type m_PlayerPos;
    CardList m_Cards;
    RefCntObj<DiscardResponseDelegate> m_Delegate;
public:
    QuestionWaitDiscardImpl(PlayerPos::Type playerPos, const CardList &cards, RefCntObj<DiscardResponseDelegate> &delegate);

    void OnSend(GameContext *context) const;
    bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const;
    bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx);
};

class QuestionWaitDiscard : public Question
{
public:
    QuestionWaitDiscard(PlayerPos::Type playerPos, const CardList &cards, RefCntObj<DiscardResponseDelegate> &delegate);
};

#endif
