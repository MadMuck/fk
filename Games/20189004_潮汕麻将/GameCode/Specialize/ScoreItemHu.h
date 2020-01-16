#ifndef SCORE_ITEM_HU_H
#define SCORE_ITEM_HU_H

#include "ScoreItem.h"
#include "PlayerPos.h"
#include "GameContext.h"
#include "Interception.h"
#include "ExtensionLiquidation.h"

class ScoreItemHuImpl : public ScoreItemImpl
{
	GameContext *m_GameContext;
	WinnerList m_Winners;
	PlayerPos::Type m_Trigger; // 如果有
	int m_Score;
	int m_Type;
	bool m_BaoPei;

public:
	ScoreItemHuImpl(GameContext *context, PlayerPos::Type trigger, const WinnerList &winners, int score, int type, bool baoPei = false);
	int Type() const;
    int Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const;
};

#endif
