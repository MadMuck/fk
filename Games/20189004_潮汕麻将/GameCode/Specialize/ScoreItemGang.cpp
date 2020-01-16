#include "ScoreItemGang.h"
#include "ExtensionLiquidation.h"
#include "ExtensionGameParam.h"
#include "ExtensionPlayer.h"
#include "ExtensionInvalidInterception.h"
#include "ScoreTypeDef.h"
#include "PlayerStatusDef.h"
#include "ExtensionQueMen.h"
#include "WinType.h"
#include "ExtensionLogger.h"

ScoreItemGangImpl::ScoreItemGangImpl(GameContext *context, const Interception &gang, const WinnerList &winners, int idx):
m_GameContext(context),m_Interception(gang), m_WinnerList(winners), m_Idx(idx)
{
}

int ScoreItemGangImpl::Type() const
{
	int type = ScoreType_Gang;
	switch(m_Interception.type)
	{
		case Interception::MingGang://明杠对放杠一家收
			type = ScoreType_MingGang;
			break;
		case Interception::BuGang:
			type = ScoreType_BuGang;
			break;
		case Interception::AnGang:
			type = ScoreType_AnGang;
			break;
	}
	return type;
}


int ScoreItemGangImpl::Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const
{
	realSrc = src;

	int score = 0;

	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(m_GameContext, ExtensionInvalidInterception, extInvalidIntc);
	OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);

	if(!extInvalidIntc->Contains(dest, m_Idx))
	{
		switch(m_Interception.type)
		{
		case Interception::MingGang://明杠对放杠一家收
			{
				if(src == m_Interception.src.playerPos)
				{
					score = extGameParam->GetParam(GameParam_MingGangScore);
				}
				break;
			}
		case Interception::BuGang: //补杠向其余玩家收分
			{
				score = extGameParam->GetParam(GameParam_BuGangScore);
				break;
			}
		case Interception::AnGang://暗杠向其余玩家收分
			{
				score = extGameParam->GetParam(GameParam_AnGangScore);
				break;
			}
		//case Interception::YangMa:
		//	{
		//		score = 0;
		//		break;
		//	}

		}
	}

// 	if(extGameParam->GetParam(GameParam_DiFen) > 0)
// 	{
// 		score *= extGameParam->GetParam(GameParam_DiFen);
// 	}

	return score;
}


ScoreItemZhuanYiImpl::ScoreItemZhuanYiImpl(PlayerPos::Type oriOwner, int score):m_OriOwner(oriOwner), m_Score(score){}

int ScoreItemZhuanYiImpl::Type() const
{
	return ScoreType_ZhuanYi;
}

int ScoreItemZhuanYiImpl::Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const
{
	realSrc = src;
	int score = 0;
	if(m_OriOwner == src)
		score = m_Score;

	return score;
}