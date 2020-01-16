#ifndef GAME_DIRECTION_H
#define GAME_DIRECTION_H

#include "PlayerPos.h"

class GameDirection
{
protected:
	int m_PlayerCount;

	int Distance(PlayerPos::Type playerPos, PlayerPos::Type origin) const;
public:
	static PlayerPos::Type ClockWiseNext(PlayerPos::Type playerPos, int playerCount);
	static PlayerPos::Type AntiClockWiseNext(PlayerPos::Type playerPos, int playerCount);

	GameDirection();
    virtual ~GameDirection();
    virtual void SetPlayerCount(int playerCount);
	virtual PlayerPos::Type GetNextPos(PlayerPos::Type pos) const;

	//以origin为基准， playerPos1 是否在 playerPos2 之前
	virtual bool IsPreceding(PlayerPos::Type playerPos1, PlayerPos::Type playerPos2, PlayerPos::Type origin) const;
};

#endif
