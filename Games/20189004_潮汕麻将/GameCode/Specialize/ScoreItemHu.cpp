#include "ScoreItemHu.h"

#include "SpecCardScoreDelegate.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGameParam.h"
#include "ExtensionPlayer.h"
#include "PlayerStatusDef.h"
#include "ScoreTypeDef.h"
#include "ExtensionFanMa.h"


ScoreItemHuImpl::ScoreItemHuImpl(GameContext *context, PlayerPos::Type trigger, const WinnerList &winners, int score, int type, bool baoPei):
	m_GameContext(context), m_Trigger(trigger), m_Winners(winners), m_Score(score), m_Type(type), m_BaoPei(baoPei)
{}


int ScoreItemHuImpl::Type() const
{
	return m_Type;
}

int ScoreItemHuImpl::Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const
{
	realSrc = src;
	if(m_Trigger != dest && m_BaoPei)
	{
		realSrc = m_Trigger;
	}

	int score = 0;

	if(m_Trigger == dest ||
		m_Trigger == src ||
		m_BaoPei)
	{
		// 自摸 或者 src是点炮玩家			
		score = m_Score;
	}

	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);

	if(extGameParam->GetParam(GameParam_DiFen) > 0)
	{
		score *= extGameParam->GetParam(GameParam_DiFen);
	}

	return score;
}
