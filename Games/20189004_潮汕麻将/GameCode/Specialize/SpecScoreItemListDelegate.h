#ifndef SPEC_SCORE_ITEM_LIST_DELEGATE_H
#define SPEC_SCORE_ITEM_LIST_DELEGATE_H

#include "GameContext.h"
#include "ExtensionPlayer.h"
#include "ExtensionLiquidation.h"
#include "Liquidation.h"
#include "ScoreItemGang.h"
#include "ScoreItemHu.h"

#include "ExtensionInvalidInterception.h"
#include "ExtensionScoreItemList.h"


class SpecScoreItemListDelegate : public ScoreItemListDelegate
{
	GameContext *m_Context;
public:
	SpecScoreItemListDelegate(GameContext *context):m_Context(context)
	{
	}

	ScoreItemList GetScoreItemList(PlayerPos::Type playerPos) const
	{
		OBJ_GET_EXT(m_Context, ExtensionScoreItemList, extScoreItemlist);
		return extScoreItemlist->GetItemList(playerPos);
	}
};


#endif