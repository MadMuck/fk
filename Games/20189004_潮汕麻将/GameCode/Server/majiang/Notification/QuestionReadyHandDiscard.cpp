#include "QuestionReadyHandDiscard.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

QuestionReadyHandDiscardImpl::QuestionReadyHandDiscardImpl(const QuestionReadyHandDiscardData &data,
    const RefCntObj<ReadyHandDiscardResponseDelegate> &delegate):
    m_Data(data),
    m_Delegate(delegate){}

void QuestionReadyHandDiscardImpl::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_READY_HAND_DISCARD).OnSend(context, (void*)&m_Data);
}

bool QuestionReadyHandDiscardImpl::IsInterestedIn(PlayerPos::Type playerPos, int tag) const
{
    return playerPos == m_Data.m_PlayerPos;
}

bool QuestionReadyHandDiscardImpl::OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
{
    bool ret = false;

	if(selectionIdx >= 0 && selectionIdx < m_Data.m_Cards.Count())
	{
		ret = true;
		m_Delegate->OnSendCard(playerPos, m_Data.m_Cards[selectionIdx]);
	}
    return ret;
}

QuestionReadyHandDiscard::QuestionReadyHandDiscard(const QuestionReadyHandDiscardData &data, const RefCntObj<ReadyHandDiscardResponseDelegate> &delegate):
    Question(new QuestionReadyHandDiscardImpl(data, delegate))
{}
