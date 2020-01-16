#include "ScoreItemPaiTou.h"

#include "ExtensionGameParam.h"
#include "ExtensionFanMa.h"
#include "ExtensionLiquidation.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionScoreItemList.h"
#include "ExtensionPlayer.h"
#include "ScoreTypeDef.h"
#include "ExtensionLogger.h"
ScoreItemPaiTouImpl::ScoreItemPaiTouImpl(GameContext *context, PlayerPos::Type trigger, const WinnerList &winners, int score, int type, bool baoPei):
m_GameContext(context), m_Trigger(trigger), m_Winners(winners), m_Score(score), m_Type(type), m_BaoPei(baoPei)
{}


int ScoreItemPaiTouImpl::Type() const
{
	return m_Type;
}

//param1��destӮ�ң�param2:srcΪ���ܵ�������
int ScoreItemPaiTouImpl::Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const
{
	realSrc = src;
	if(m_Trigger != dest && m_BaoPei)
	{
		realSrc = m_Trigger;
	}

	int score = 0;

	// ���Ե�ʱδ������ҽ��㣨TriggerΪ�����˽��������ң���������ʱtriger��Ϊdest��������ʱtrigerΪ������ң���
	OBJ_GET_EXT(m_GameContext, ExtensionFanMa, extFanMa);
	if(m_Trigger == dest ||
		m_Trigger == src ||
		m_BaoPei)
	{	
		int ZhongMaCnt= extFanMa->GetZhongMaCount(dest);
		score = m_Score * ZhongMaCnt;
	}

	OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extGameParam);
	int baseMaMulti = 1;
	if(extGameParam->IsTrue(GameParam_bMaGenDiFen) && extGameParam->GetParam(GameParam_DiFen) > 0)
	{	//����׷�
		baseMaMulti = 3 * extGameParam->GetParam(GameParam_DiFen);
	}
	else if(extGameParam->IsTrue(GameParam_bMaGenGang))
	{	//�����
		OBJ_GET_EXT(m_GameContext, ExtensionScoreItemList, extScoreItemList);
		OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
		ScoreItemList ScoreItem = extScoreItemList->GetItemList(dest);
		for (auto it = ScoreItem.begin(); it != ScoreItem.end(); ++it)
		{
			if(it->Type() != ScoreType_MingGang || it->Type() != ScoreType_AnGang || it->Type() != ScoreType_BuGang) continue;
			PlayerPos::Type BankerPos = extBankerSelector->GetBankerPos();
			for (int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
			{
				PlayerPos::Type src = PlayerPos::P0 + i;
				if (dest != src)
				{
					PlayerPos::Type realSrc = src;
					baseMaMulti += it->Score(dest, src, BankerPos, realSrc);
				}
			}
		}
		LOGGER_FILE(m_GameContext,"����ܻ�����Ϊ��"<<baseMaMulti);
	}
	score *= baseMaMulti;
	//����ܣ�Ҫ����������ҵĸܷ�
	//if(extGameParam->IsTrue(GameParam_bMaGenGang))
	//{
	//	//��ȡscoreItemList,��ȡӮ�Ҹܷ�
	//	OBJ_GET_EXT(m_GameContext,ExtensionLiquidation,extLiquidation);
	//	OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
	//	OBJ_GET_EXT(m_GameContext, ExtensionScoreItemList, extScoreItemlist);
	//	OBJ_GET_EXT(m_GameContext, ExtensionPlayer,extPlayer);
	//	Liquidation &liquidation = extLiquidation->GetLiquidation();
	//	liquidation.Clear();
	//	liquidation.Prepare(extBankerSelector->GetBankerPos(), m_GameContext->GetPlayerCount());

	//	const PlayerList &players = extPlayer->GetPlayerList();
	//	for(PlayerList::const_iterator itr = players.begin();
	//		itr != players.end();
	//		++itr)
	//	{
	//		PlayerPos::Type playerPos = itr->second->GetPos();

	//		ScoreItemList scoreItemList = extScoreItemlist->GetItemList(playerPos);
	//	
	//		for(ScoreItemList::iterator itr2 = scoreItemList.begin(); itr2 != scoreItemList.end(); ++itr2)
	//		{
	//			liquidation.Add(playerPos, *itr2); //
	//		}
	//	}
	//	baseMaScore += liquidation.GetScoreOfType(dest).GetScore(ScoreType_Gang);
	//	score  = baseMaScore * ZhongMaCnt;
	//	//������׷ֵ�����£�
	//	//�������
	//	liquidation.Clear();
	//}
	return score;
}