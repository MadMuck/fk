#include "GamePlayerConnector.h"

void GamePlayerConnector::SendQuestion(GameContext *context, const Question &question)
{
    m_QuestionList.push_back(question);
    question.OnSend(context);
}

void GamePlayerConnector::SendNotification(GameContext *context, const Notification &notification)
{
    notification.OnSend(context);
}

void GamePlayerConnector::ResendQuestion(GameContext *context, PlayerPos::Type playerPos, int tag)
{
	for(QuestionList::iterator itr = m_QuestionList.begin();
		itr != m_QuestionList.end();
		++itr)
	{
		if((*itr).IsInterestedIn(playerPos, tag))
		{
			(*itr).OnSend(context);
		}
	}
}

void GamePlayerConnector::ResendQuestion(GameContext *context)
{
	for(QuestionList::iterator itr = m_QuestionList.begin();
		itr != m_QuestionList.end();
		++itr)
	{
		(*itr).OnSend(context);
	}
}

bool GamePlayerConnector::IsPlayerBlocking(PlayerPos::Type playerPos, int tag)
{
	bool ret = false;
	for(QuestionList::iterator itr = m_QuestionList.begin();
		itr != m_QuestionList.end();
		++itr)
	{
		if((*itr).IsInterestedIn(playerPos, tag) && (*itr).IsInterception())
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void GamePlayerConnector::Clear()
{
    m_QuestionList.clear();
}

void GamePlayerConnector::OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
{
    for(QuestionList::iterator itr = m_QuestionList.begin();
        itr != m_QuestionList.end();
        ++itr)
    {
        if((*itr).IsInterestedIn(playerPos, tag))
        {
            if((*itr).OnResponse(playerPos, tag, selectionIdx))
            {
                m_QuestionList.erase(itr);
                break;
            }
        }
    }
}