#ifndef EXTENSION_GAME_RUNNING_DATA_H
#define EXTENSION_GAME_RUNNING_DATA_H

#include "GameContext.h"

#include <map>

class GameRunningDataDelegate
{
public:
	virtual ~GameRunningDataDelegate(){}

	virtual unsigned char GetDeskStationByID(long userID) const = 0;
	virtual long GetIDbyDeskStation(unsigned char deskStation) const = 0;
	virtual int GetRemainingGameCount() const = 0;
	virtual void SendDataToPlayer(PlayerPos::Type playerPos, void *data, int dataSize, int msgID) = 0;
	virtual void SendDataToAll( void *data, int dataSize, int msgID) = 0;
	virtual bool IsPlayerReadyToPlay(unsigned char deskStation) const = 0;
	virtual void PlayerGetReadyToPlay(unsigned char deskStation) = 0;
	virtual int GetGameState() const = 0;
	virtual void SetGameState(unsigned char gameState) = 0;
};


class ExtensionGameRunningData : public GameContextExtension
{
	std::map<PlayerPos::Type, int> m_BankerTimes; // ×ø×¯¾ÖÊý

	GameRunningDataDelegate *m_Delegate;                                  

public:

	void SetDelegate(GameRunningDataDelegate *delegate)
	{
		m_Delegate = delegate;
	}
	
	unsigned char GetDeskStation(PlayerPos::Type playerPos) const
	{
		return playerPos;
	}

	PlayerPos::Type GetPlayerPos(unsigned char deskStation) const
	{
		return deskStation;
	}

	void OnPlayerLeft(unsigned char deskStation)
	{
	}

	void AddBanker(PlayerPos::Type playerPos)
	{
		++m_BankerTimes[playerPos];
	}

	int GetBankerTimes(PlayerPos::Type playerPos) const
	{
		int ret = 0;
		if(m_BankerTimes.find(playerPos) != m_BankerTimes.end())
		{
			ret = m_BankerTimes.find(playerPos)->second;
		}
		return ret;
	}

	int GetRemainingGameCount() const
	{
		return m_Delegate->GetRemainingGameCount();
	}

	void SendDataToPlayer(PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		m_Delegate->SendDataToPlayer(playerPos, data, dataSize, msgID);
	}

	void SendDataToAll( void *data, int dataSize, int msgID)
	{
		m_Delegate->SendDataToAll(data, dataSize, msgID);
	}

	bool IsPlayerReadyToPlay(unsigned char deskStation) const
	{
		return m_Delegate->IsPlayerReadyToPlay(deskStation);
	}

	void PlayerGetReadyToPlay(unsigned char deskStation)
	{
		m_Delegate->PlayerGetReadyToPlay(deskStation);
	}

	int GetGameState() const
	{
		return m_Delegate->GetGameState();
	}

	void SetGameState(unsigned char gameState)
	{
		return m_Delegate->SetGameState(gameState);
	}

	void Clear()
	{
		m_BankerTimes.clear();
	}

	void OnGameReset()
	{
		Clear();
	}
};

#endif