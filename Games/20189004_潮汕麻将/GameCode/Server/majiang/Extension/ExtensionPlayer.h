#ifndef EXTENSION_PLAYER_H
#define EXTENSION_PLAYER_H

#include "GameContext.h"
#include "RefCntObj.h"

#include <map>
#include "Player.h"

class PlayerStatus
{
protected:
	std::map<int, int> m_Flag;

public:

	void Reset();

	void SetFlag(int type, int flag);

	int GetFlag(int type, int defaultVal = 0) const;

	void SetBool(int type, bool val);

	bool GetBool(int type, bool defaulVal = false) const;
};

typedef std::map<PlayerPos::Type, Player *> PlayerList;

class HandSortingDelegate
{
public:
	virtual ~HandSortingDelegate(){}

	virtual void Sort(PlayerPos::Type playerPos, CardList &hand)
	{
		hand.Sort();
	}
};

class ExtensionPlayer : public GameContextExtension
{
protected:
    PlayerPos::Type m_ActivePlayer;
	PlayerPos::Type m_LastActivePlayer;
    PlayerList m_Players;

	std::map<PlayerPos::Type, unsigned char> m_PlayerDes;

	std::map<PlayerPos::Type, PlayerStatus> m_PlayerStatus; // 听牌标记

	RefCntObj<HandSortingDelegate> m_HandSortingDelegate;

	std::map<PlayerPos::Type, PlayerPos::Type> m_GangBao; // 听牌标记

public:

	ExtensionPlayer();

	void Reset();

    void OnCreate();

    void OnDestroy();

    void SetActivePlayerPos(PlayerPos::Type player);

    PlayerPos::Type GetActivePlayerPos() const;

    Player *GetActivePlayer();

	PlayerPos::Type GetLastActivePlayerPos() const;

    Player *GetPlayer(PlayerPos::Type pos);

    PlayerList &GetPlayerList();

	PlayerStatus &GetPlayerStatus(PlayerPos::Type playerPos);

	void SetHandSortingDelegate( const RefCntObj<HandSortingDelegate> &delegate);

	void SortHand(PlayerPos::Type playerPos);

	void SetGangBaoPos(PlayerPos::Type playerPos, PlayerPos::Type srcPos);

	PlayerPos::Type GetGangBaoPos(PlayerPos::Type playerPos);

	void SetDes(int playcount, PlayerPos::Type bankpos);

	const unsigned char GetDes(PlayerPos::Type pos) const;

    void OnGameReset();

	void OnGameStart();

	void OnGameFinish();
};

#endif
