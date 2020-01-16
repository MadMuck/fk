#include "QuestionInterception.h"
#include "Logger.h"
#include <memory.h>
#include "ExtensionMessageDelegate.h"

QuestionInterceptionImpl::QuestionInterceptionImpl(PlayerPos::Type playerPos,
    const InterceptionList &interceptions,
    const RefCntObj<InterceptionResponseDelegate> &delegate):
    m_PlayerPos(playerPos),
    m_Interceptions(interceptions),
    m_Delegate(delegate)
{
}

void QuestionInterceptionImpl::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	QuestionInterceptionData data;
	data.playerPos = m_PlayerPos;
	data.interceptions = m_Interceptions;
	extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnSend(context, (void*)&data);
}

bool QuestionInterceptionImpl::IsInterestedIn(PlayerPos::Type playerPos, int tag) const
{
    return playerPos == m_PlayerPos;
}

bool QuestionInterceptionImpl::OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
{
	if(selectionIdx >= 0 && selectionIdx < m_Interceptions.Count())
	{
		const Interception &selection = m_Interceptions[selectionIdx];
		m_Delegate->OnAction(playerPos, selection, m_Interceptions);
	}
	else
	{
		m_Delegate->OnIgnore(playerPos, m_Interceptions);
	}

    return true;
}

bool QuestionInterceptionImpl::IsInterception()
{
	return true;
}

QuestionInterception::QuestionInterception(PlayerPos::Type playerPos,
    const InterceptionList &interceptions,
    const RefCntObj<InterceptionResponseDelegate> &delegate):
    Question(new QuestionInterceptionImpl(playerPos, interceptions, delegate))
{
}
