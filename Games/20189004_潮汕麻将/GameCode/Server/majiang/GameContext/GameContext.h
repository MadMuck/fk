#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "ExtensionObject.h"
#include "PlayerPos.h"

class GameContext : public ExtensionObject
{
protected:

    PlayerPos::Type m_GameOwner;

	int m_PlayerCount;

public:

    GameContext();

    virtual ~GameContext();

    virtual int GetPlayerCount() const;

	virtual void SetPlayerCount(int cnt);

    virtual void OnAddExtension(Extension *ext);

    virtual void OnRemoveExtension(Extension *ext);

    virtual void SetGameOwner(PlayerPos::Type owner);

    virtual PlayerPos::Type GetGameOwner() const;

    virtual void GameStart();

    virtual void GameFinish();

    virtual void Reset();
};

class GameContextExtension : public Extension
{
protected:
    GameContext *m_Context;
public:
    void SetContext(GameContext *context){m_Context = context;}
    virtual void OnCreate(){}
    virtual void OnGameOwnerChanged(PlayerPos::Type owner){}
    virtual void OnGameStart(){}
    virtual void OnGameFinish(){}
    virtual void OnGameReset(){}
    virtual void OnDestroy(){}
};

#endif
