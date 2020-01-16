#ifndef QUESTION_READY_HAND_DISCARD_H
#define QUESTION_READY_HAND_DISCARD_H

#include "Question.h"
#include "Card.h"
#include "PlayerPos.h"

#define MSG_DELEGATE_READY_HAND_DISCARD "ReadyHandDiscard"

class ReadyHandDiscardResponseDelegate
{
public:
    virtual void OnSendCard(PlayerPos::Type playerPos, Card::Type card) const = 0;
};

struct QuestionReadyHandDiscardData
{
	PlayerPos::Type m_PlayerPos;
	CardList m_Cards;
	std::map<Card::Type, CardStat> m_CardsToWin;
	std::map<Card::Type, std::map<Card::Type, int> > m_CardScore;
};

class QuestionReadyHandDiscardImpl : public QuestionImpl
{
    QuestionReadyHandDiscardData m_Data;
    RefCntObj<ReadyHandDiscardResponseDelegate> m_Delegate;
public:
    QuestionReadyHandDiscardImpl(const QuestionReadyHandDiscardData &data, const RefCntObj<ReadyHandDiscardResponseDelegate> &delegate);

    void OnSend(GameContext *context) const;
    bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const;
    bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx);
};

class QuestionReadyHandDiscard : public Question
{
public:
    QuestionReadyHandDiscard(const QuestionReadyHandDiscardData &data, const RefCntObj<ReadyHandDiscardResponseDelegate> &delegate);
};

#endif
