#ifndef QUESTION_DING_QUE_H
#define QUESTION_DING_QUE_H

#include "Question.h"
#include "PlayerPos.h"
#include "ExtensionMessageDelegate.h"

#define MSG_DELEGATE_DING_QUE "DingQue"

class DingQueResponseDelegate
{
public:
	virtual ~DingQueResponseDelegate(){}

	virtual void OnSelect(PlayerPos::Type playerPos, int type) = 0;
};

class QuestionDingQueImpl : public QuestionImpl
{
	PlayerPos::Type m_PlayerPos;

	RefCntObj<DingQueResponseDelegate> m_Delegate;
public:

	QuestionDingQueImpl(PlayerPos::Type playerPos, const RefCntObj<DingQueResponseDelegate> &delegate):
	  m_PlayerPos(playerPos), m_Delegate(delegate){}

	void OnSend(GameContext *context) const
	{
		OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
		extMsgDelegate->GetDelegate(MSG_DELEGATE_DING_QUE).OnSend(context, (void *)&m_PlayerPos);
	}

	bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const
	{
		return playerPos == m_PlayerPos;
	}

	bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
	{
		bool ret = false;
		if(selectionIdx >= 0 && selectionIdx < 3)
		{
			ret = true;
			m_Delegate->OnSelect(playerPos, selectionIdx);
		}
		return ret;
	}
};

class QuestionDingQue : public Question
{
public:
	QuestionDingQue(PlayerPos::Type playerPos, const RefCntObj<DingQueResponseDelegate> &delegate):
	  Question(new QuestionDingQueImpl(playerPos, delegate)){}
};

#endif