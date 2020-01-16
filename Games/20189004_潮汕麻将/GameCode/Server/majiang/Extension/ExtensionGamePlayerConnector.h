/*
游戏服务器与客户端的通讯接口
*/

#ifndef EXTENSION_GAME_PLAYER_CONNECTOR_H
#define EXTENSION_GAME_PLAYER_CONNECTOR_H

#include "GameContext.h"
#include "GamePlayerConnector.h"

class ExtensionGamePlayerConnector : public GameContextExtension
{
protected:
    GamePlayerConnector *m_GamePlayerConnector;

public:
    ExtensionGamePlayerConnector();
    ~ExtensionGamePlayerConnector();

    void OnGameFinish();

    virtual GamePlayerConnector *GetGamePlayerConnector();

    virtual void SendQuestion(const Question &question);

    virtual void SendNotification(const Notification &notification);

	virtual void ResendQuestion(PlayerPos::Type playerPos, int tag);

	virtual void ResendQuestion();

	virtual bool IsPlayerBlocking(PlayerPos::Type playerPos, int tag);

    virtual void Clear();

    virtual void OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx);
};

#endif
