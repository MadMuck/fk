#ifndef LIQUIDATION_H
#define LIQUIDATION_H

#include "PlayerPos.h"
#include "ScoreItem.h"
#include <map>
#include <list>

class Score
{
    PlayerPos::Type m_PlayerPos;
    int m_Score;
public:
    Score();
    Score(PlayerPos::Type playerPos);
    PlayerPos::Type GetPlayerPos() const;
    int GetScore() const;
    void Add(int score);
    void Clear();
};

class ScoreList : public std::map<PlayerPos::Type, Score>
{
public:
    int GetScore(PlayerPos::Type playerPos) const;
};

class ScoreOfType : public std::map<int, Score>
{
public:
	int GetScore(int type) const;
};

class Liquidation
{
	int m_PlayerCount;
    PlayerPos::Type m_BankerPos;
    ScoreList m_TotalScoreList;
	std::map<PlayerPos::Type, ScoreOfType> m_ScoreOfType;
	
public:
    Liquidation();
    void Prepare(PlayerPos::Type bankerPos, int playerCount);
    void Add(PlayerPos::Type player, const ScoreItem &scoreItem);
    const ScoreList &GetScoreList() const;
	ScoreOfType GetScoreOfType(PlayerPos::Type playerPos) const;
    void Clear();
};

class ScoreItemListDelegate
{
public:
	virtual ~ScoreItemListDelegate(){}
	virtual ScoreItemList GetScoreItemList(PlayerPos::Type playerPos) const = 0;
};

#endif
