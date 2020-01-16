/*
游戏服务器与客户端的通讯接口
*/

#ifndef GAME_PLAYER_CONNECTOR_H
#define GAME_PLAYER_CONNECTOR_H

#include <list>
#include "Question.h"
#include "Notification.h"

class GamePlayerConnector
{
    typedef std::list<Question> QuestionList;
    QuestionList m_QuestionList;
public:
    void SendQuestion(GameContext *context, const Question &question);

    void SendNotification(GameContext *context, const Notification &notification);

	void ResendQuestion(GameContext *context, PlayerPos::Type playerPos, int tag);

	void ResendQuestion(GameContext *context);

	bool IsPlayerBlocking(PlayerPos::Type playerPos, int tag);

    void Clear();

    void OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx);
};

#endif
