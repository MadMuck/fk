#include "Question.h"

bool QuestionImpl::IsInterception()
{
	return false;
}

Question::Question(QuestionImpl *impl):m_Impl(impl)
{}

// 通知发送
void Question::OnSend(GameContext *context) const
{
    m_Impl->OnSend(context);
}

// 是否要处理某个玩家的反馈
bool Question::IsInterestedIn(PlayerPos::Type playerPos, int tag) const
{
    return m_Impl->IsInterestedIn(playerPos, tag);
}

// 处理玩家的反馈
bool Question::OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
{
    return m_Impl->OnResponse(playerPos, tag, selectionIdx);
}

bool Question::IsInterception()
{
	return m_Impl->IsInterception();
}