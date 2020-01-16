#ifndef QUESTION_INTERCEPTION_H
#define QUESTION_INTERCEPTION_H

#include "Question.h"
#include "Interception.h"

#define MSG_DELEGATE_INTERCEPTION "Interception"

class InterceptionResponseDelegate
{
public:
	virtual void OnAction(PlayerPos::Type pos, const Interception &action, const InterceptionList &actions) = 0;
    virtual void OnIgnore(PlayerPos::Type pos, const InterceptionList &actions) = 0;
};

struct QuestionInterceptionData
{
	PlayerPos::Type playerPos;
	InterceptionList interceptions;
};

class QuestionInterceptionImpl : public QuestionImpl
{
    PlayerPos::Type m_PlayerPos;
	CardList m_CardList;
    InterceptionList m_Interceptions;
    RefCntObj<InterceptionResponseDelegate> m_Delegate;
public:

    QuestionInterceptionImpl(PlayerPos::Type playerPos, const InterceptionList &interceptions, const RefCntObj<InterceptionResponseDelegate> &delegate);

    void OnSend(GameContext *context) const;
    bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const;
    bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx);
	bool IsInterception();
};

class QuestionInterception : public Question
{
public:
    QuestionInterception(PlayerPos::Type playerPos, const InterceptionList &interceptions, const RefCntObj<InterceptionResponseDelegate> &delegate);
};

#endif
