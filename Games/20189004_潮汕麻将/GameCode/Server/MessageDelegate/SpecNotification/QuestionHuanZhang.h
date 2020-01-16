#ifndef QUESTION_HUAN_ZHANG_H
#define QUESTION_HUAN_ZHANG_H

#include "Question.h"
#include "PlayerPos.h"
#include "Card.h"
#include "ExtensionMessageDelegate.h"

#define MSG_DELEGATE_HUAN_ZHANG "HuanZhang"

class HuanZhangQuestionData
{
public:
	PlayerPos::Type playerPos;
	CardList cards;
};

class HuanZhangResponseDelegate
{
public:
	virtual ~HuanZhangResponseDelegate(){}

	virtual bool OnSelect(PlayerPos::Type playerPos, Card::Type card) = 0;
};

class QuestionHuanZhangImpl : public QuestionImpl
{
	HuanZhangQuestionData m_Data;

	RefCntObj<HuanZhangResponseDelegate> m_Delegate;
public:

	QuestionHuanZhangImpl(PlayerPos::Type playerPos, const CardList &cards, const RefCntObj<HuanZhangResponseDelegate> &delegate):m_Delegate(delegate)
	{
		m_Data.playerPos = playerPos;
		m_Data.cards = cards;
	}

	void OnSend(GameContext *context) const
	{
		OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
		extMsgDelegate->GetDelegate(MSG_DELEGATE_HUAN_ZHANG).OnSend(context, (void*)&m_Data);
	}

	bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const
	{
		return m_Data.playerPos == playerPos;
	}

	bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
	{
		return m_Delegate->OnSelect(playerPos, m_Data.cards[selectionIdx]);
	}
};

class QuestionHuanZhang : public Question
{
public:
	QuestionHuanZhang(PlayerPos::Type playerPos, const CardList &cards, const RefCntObj<HuanZhangResponseDelegate> &delegate):
	  Question(new QuestionHuanZhangImpl(playerPos, cards, delegate)){}
};

#endif