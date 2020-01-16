#include "GameDirection.h"
#include <iostream>

int GameDirection::Distance(PlayerPos::Type playerPos, PlayerPos::Type origin) const
{
	int length = 0;

	if(playerPos >= PlayerPos::Max)
	{
		length = PlayerPos::Max;
	}
	else
	{
		PlayerPos::Type idx = origin;
		while(idx != playerPos)
		{
			idx = GetNextPos(idx);
			++length;
		}
	}

	return length;
}

PlayerPos::Type GameDirection::ClockWiseNext(PlayerPos::Type playerPos, int playerCount)
{
	return (playerPos + 1) % playerCount; 
}

PlayerPos::Type GameDirection::AntiClockWiseNext(PlayerPos::Type playerPos, int playerCount)
{
	return (playerPos + playerCount - 1) % playerCount;
}

GameDirection::GameDirection():m_PlayerCount(4)
{
}

GameDirection::~GameDirection()
{}

void GameDirection::SetPlayerCount(int playerCount)
{
	m_PlayerCount = playerCount;
}

PlayerPos::Type GameDirection::GetNextPos(PlayerPos::Type pos) const
{
	// 0 -> (m_PlayerCount - 1) -> (m_PlayerCount - 2) -> ... -> 2 -> 1 -> 0
    return AntiClockWiseNext(pos, m_PlayerCount);
}

bool GameDirection::IsPreceding(PlayerPos::Type playerPos1, PlayerPos::Type playerPos2, PlayerPos::Type origin) const
{
	int dist1 = Distance(playerPos1, origin);
	int dist2 = Distance(playerPos2, origin);
	return dist1 < dist2; 
}
