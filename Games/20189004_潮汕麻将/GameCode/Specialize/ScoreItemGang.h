#ifndef SCORE_ITEM_GANG_H
#define SCORE_ITEM_GANG_H

#include "GameContext.h"
#include "ScoreItem.h"
#include "Interception.h"
#include "PlayerPos.h"
#include "ExtensionLiquidation.h"

class ScoreItemGangImpl : public ScoreItemImpl
{
	GameContext *m_GameContext;
	Interception m_Interception; // 对应的杠动作
	WinnerList m_WinnerList;	// 杠牌时已胡牌的玩家
	int m_Idx;					// 对应的拦牌动作索引
	
public:
    ScoreItemGangImpl(GameContext *context, const Interception &gang, const WinnerList &winners, int idx);
	int Type() const;
    int Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const;
};

class ScoreItemZhuanYiImpl : public ScoreItemImpl
{
	int m_Score;
	PlayerPos::Type m_OriOwner;
public:
	ScoreItemZhuanYiImpl(PlayerPos::Type oriOwner, int score);
	int Type() const;
	int Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const;
};

class ScoreItemPengImpl : public ScoreItemImpl
{
	GameContext *m_GameContext;
	Interception m_Interception; // 对应的碰动作
	WinnerList m_WinnerList;	// 碰牌时已胡牌的玩家
	int m_Idx;					// 对应的拦牌动作索引

public:
	ScoreItemPengImpl(GameContext *context, const Interception &peng, const WinnerList &winners, int idx);
	int Type() const;
	int Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const;
};

#endif
