#include "QuestionWaitDiscard.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

QuestionWaitDiscardImpl::QuestionWaitDiscardImpl(PlayerPos::Type playerPos,
    const CardList &cards,
    RefCntObj<DiscardResponseDelegate> &delegate):
    m_PlayerPos(playerPos),
    m_Cards(cards),
    m_Delegate(delegate){}

void QuestionWaitDiscardImpl::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	QuestionWaitDiscardData data;
	data.playerPos = m_PlayerPos;
	data.cards = m_Cards;
	extMsgDelegate->GetDelegate(MSG_DELEGATE_WAIT_DISCARD_CARD).OnSend(context, &data);
}

bool QuestionWaitDiscardImpl::IsInterestedIn(PlayerPos::Type playerPos, int tag) const
{
    return playerPos == m_PlayerPos;
}

bool QuestionWaitDiscardImpl::OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
{
    bool ret = false;
	if(selectionIdx >= 0 && selectionIdx < m_Cards.Count())
	{
		ret = true;
		m_Delegate->OnSendCard(playerPos, m_Cards[selectionIdx]);
	}
    return ret;
}

QuestionWaitDiscard::QuestionWaitDiscard(PlayerPos::Type playerPos, const CardList &cards, RefCntObj<DiscardResponseDelegate> &delegate):
    Question(new QuestionWaitDiscardImpl(playerPos, cards, delegate))
{}
