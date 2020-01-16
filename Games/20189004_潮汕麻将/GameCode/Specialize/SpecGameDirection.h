#ifndef SPEC_GAME_DIRECTION_H
#define SPEC_GAME_DIRECTION_H

#include "GameDirection.h"
#include "GameContext.h"
#include "ExtensionLiquidation.h"

class SpecGameDirection : public GameDirection
{
	GameContext *m_GameContext;
public:
	SpecGameDirection(GameContext *context):GameDirection(), m_GameContext(context){}

	PlayerPos::Type GetNextPos(PlayerPos::Type pos) const
	{
		OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
		const WinnerList &winners = extLiquidation->GetWinners();
		PlayerPos::Type ret = pos;
		if(m_PlayerCount - (int)winners.size() > 1)
		{
			do
			{
				ret = AntiClockWiseNext(ret, m_PlayerCount);
				// 跳过已经胡牌的玩家
			}
			while(winners.Contains(ret));
		}
		return ret;
	}
};

#endif