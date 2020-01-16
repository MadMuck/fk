#include "AdapterUtil.h"

#include "UpgradeMessageEx.h"
#include "GameContext.h"
#include "ExtensionLiquidation.h"
#include "ExtensionScoreItemList.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGameParam.h"
#include "ScoreTypeDef.h"

unsigned char *CopyCards(unsigned char buf[], int bufSize, const CardList &cards, bool invisible)
{
	memset(buf, 255, bufSize);
	int len = bufSize / sizeof(buf[0]);
	for(CardList::const_iterator itr = cards.begin(); itr != cards.end(); ++itr)
	{
		if(itr - cards.begin() < len)
			buf[itr - cards.begin()] = (invisible ? 0 : *itr);
		else
			break;
	}
	return buf;
}

unsigned char *CopyCards(unsigned char buf[], int bufSize, const std::vector<TwoCards> &cards, bool invisible)
{
	int idx = 0;
	memset(buf, 255, bufSize);
	int len = bufSize / sizeof(buf[0]);
	for(std::vector<TwoCards>::const_iterator itr = cards.begin();
		itr != cards.end();
		++itr)
	{
		if(idx < len)
		{
			buf[idx] = (itr->Count() >= 2 ? (invisible ? 0 : itr->GetCard(0)) : 255);
		}

		if(idx + 1 < len)
		{
			buf[idx+1] = (itr->Count() >= 1 ? (invisible ? 0 : itr->GetCard(1)) : 255);
		}

		idx += 2;
	}
	return buf;
}

unsigned char *CopyAllCardsInHand(unsigned char *buf, int rows, int bufSize, const PlayerList &players, const PlayerPosList &visiblePos)
{
	memset(buf, 255, bufSize);
	int rowSize = bufSize / rows;

	for(PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
	{
		unsigned char *p = buf + itr->second->GetPos() * rowSize;
		CopyCards(p, rowSize, itr->second->GetCards(), visiblePos.find(itr->second->GetPos()) == visiblePos.end());
	}
	return buf;
}

unsigned char *CopyCardBlocks(unsigned char *buf, int rows, int bufSize, const CardBlocks &cardBlocks, bool invisible)
{
	memset(buf, 255, bufSize);
	int rowSize = bufSize / rows;

	std::vector<std::vector<TwoCards> > cards = cardBlocks.GetAllCards();
	for(std::vector<std::vector<TwoCards> >::iterator itr = cards.begin(); itr != cards.end(); ++itr)
	{
		int pos = itr - cards.begin();
		CopyCards(buf + pos * rowSize, rowSize, *itr, invisible);
	}
	return buf;
}

int GetCpgType(Interception::Type type)
{
	int out = ACTION_NO;
	switch(type)
	{
	case Interception::Chi: out = ACTION_CHI; break;
	case Interception::Peng: out = ACTION_PENG; break;
	case Interception::MingGang: 
	case Interception::MingGangTing:
		out = ACTION_MING_GANG; break;
	case Interception::AnGang: 
	case Interception::AnGangTing:
		out = ACTION_AN_GANG; break;
	case Interception::BuGang: 
	case Interception::BuGangTing:
		out = ACTION_BU_GANG; break;
//	case Interception::YangMa:
//		out = ACTION_YANG_MA; break;
	case Interception::Hu: out = ACTION_HU; break;
	case Interception::Ting: out = ACTION_TING; break;
	default: break;
	}
	return out;
}

Interception::Type GetInterceptionType(unsigned char type)
{
	Interception::Type out = Interception::Invalid;
	switch(type)
	{
	case ACTION_CHI: out = Interception::Chi; break;
	case ACTION_PENG: out = Interception::Peng; break;
	case ACTION_MING_GANG: out = Interception::MingGang; break;
	case ACTION_AN_GANG: out = Interception::AnGang; break;
	case ACTION_BU_GANG: out = Interception::BuGang; break;
//	case ACTION_YANG_MA: out = Interception::YangMa; break;
	case ACTION_TING: out = Interception::Ting; break;
	default: break;
	}
	return out;
}

PlayerPosList ShownCardPlayerPosList(ExtensionPlayer *extPlayer)
{
	PlayerPosList ret;
	const PlayerList &players = extPlayer->GetPlayerList();
	for(PlayerList::const_iterator itr = players.begin();
		itr != players.end();
		++itr)
	{
		if(extPlayer->GetPlayerStatus(itr->second->GetPos()).GetBool(PlayerStatus_ShowCard))
		{
			ret.insert(itr->second->GetPos());
		}
	}
	return ret;
}

void CopyInterceptionData(GCPStructEx *out, int len, PlayerPos::Type playerPos,  const InterceptionList &interceptions)
{
	for(int i = 0; i < len; ++i)
	{
		out[i].Init();
	}

	for(InterceptionList::const_iterator itr = interceptions.begin(); itr != interceptions.end(); ++itr)
	{
		if(itr->type != Interception::Hu)
		{
			int idx = itr - interceptions.begin();
			out[idx].byType = GetCpgType(itr->type);
			out[idx].iStation = playerPos;
			out[idx].iBeStation = itr->src.playerPos;
			out[idx].iOutpai = itr->src.card;
			for(CardList::const_iterator itr2 = itr->cards.begin(); itr2 != itr->cards.end(); ++itr2)
			{
				out[idx].byData[itr2-itr->cards.begin()] = *itr2;
			}
		}
	}
}


void FillInterceptionData(PlayerPos::Type playerPos, const InterceptionList &interceptions, tagCPGNotify &out)
{
	out.Clear();

	int chiIdx = 0;
	int pengIdx = 0;
	int gangIdx = 0;

	for(InterceptionList::const_iterator itr = interceptions.begin(); itr != interceptions.end(); ++itr)
	{
		out.byUser = itr->src.playerPos;
		out.byPs = itr->src.card;
		out.bZhuaPai = itr->src.playerPos == playerPos;
		if(itr->type == Interception::Chi)
		{
			out.bChi = true;
			CopyCards(out.m_iChi[chiIdx], sizeof(out.m_iChi[chiIdx]), itr->cards, false);
			++chiIdx;
		}
		if(itr->type == Interception::Peng)
		{
			out.bPeng = true;
			++pengIdx;
		}
		if(itr->type == Interception::MingGang ||
			itr->type == Interception::AnGang ||
			itr->type == Interception::BuGang
		//	||itr->type == Interception::YangMa
			)
		{
			out.bGang = true;
			out.m_iGangData[gangIdx][0] = GetCpgType(itr->type);
			out.m_iGangData[gangIdx][1] = itr->src.card;
			++gangIdx;
		}
		if(itr->type == Interception::Hu)
		{
			out.bHu = true;
			out.m_iHuPs = itr->src.card;
		}
		if(itr->type == Interception::Ting)
		{
			out.bTing = true;
		}
		
		out.bCanAction = true;
	}
}

void FillWinningInfo(tagHuPaiEx &info, GameContext *context, PlayerPos::Type playerPos)
{
	info.Clear();

// 	OBJ_GET_EXT(context, ExtensionScoreItemList, extScoreItemList);
// 	Liquidation liquidation;
// 	OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
// 	liquidation.Prepare(extBankerSelector->GetBankerPos(), context->GetPlayerCount());
// 	for(int i = 0; i < context->GetPlayerCount(); ++i)
// 	{
// 		ScoreItemList scoreItemList = extScoreItemList->GetItemList(i);
// 		for(ScoreItemList::iterator itr = scoreItemList.begin();
// 			itr != scoreItemList.end();
// 			++itr)
// 		{
// 			liquidation.Add(i, *itr);
// 		}
// 	}
// 
// 	OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
// 	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
// 	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
// 	for(int i = 0; i < context->GetPlayerCount(); ++i)
// 	{
// 		Player *player = extPlayer->GetPlayer(i);
// 		CopyInterceptionData(info.m_UserGCPData[i], 
// 			sizeof(info.m_UserGCPData[i]) / sizeof(info.m_UserGCPData[i][0]),
// 			i, player->GetInterceptions());
// 
// 		info.iZongFen[i] = liquidation.GetScoreList().GetScore(i);
// 		info.iMoney[i] = info.iZongFen[i] * extGameParam->GetParam(GameParam_GameScorePoint);
// 		info.iHuFen[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_Hu);
// 		info.iGangFen[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_Gang);
// 		info.m_iChaJiao[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_ChaJiao);
// 		info.m_iZhuanYi[i] = liquidation.GetScoreOfType(i).GetScore(ScoreType_ZhuanYi);
// 		info.m_bIsHu[i] = extLiquidation->GetWinners().Contains(i);
// 		if(info.m_bIsHu[i])
// 		{
// 			if(extLiquidation->GetWinMode(i) == WinMode_Self)
// 				info.m_bIsZiMo[i] = true;
// 			else
// 				info.m_byDianPao[i] = extLiquidation->GetLoser(i);
// 
// 			info.m_iZongFan[i] = extScoreItemList->GetWinningFan(i);
// 			
// 			info.m_iGengCount[i] = CardStat(player->GetCards()).FindValuesWithNum(std::greater_equal<int>(), 4).size();
// 
// 			for(InterceptionList::const_iterator itr = player->GetInterceptions().begin();
// 				itr != player->GetInterceptions().end();
// 				++itr)
// 			{
// 				if(Interception::Hu == itr->type)
// 				{
// 					info.m_byPs[i] = itr->src.card;
// 					break;
// 				}
// 			}
// 
// 			
// 		}
// 	}
// 
// 	Player *curPlayer = extPlayer->GetPlayer(playerPos);
// 	info.byGengCount = CardStat(curPlayer->GetCards()).FindValuesWithNum(std::greater_equal<int>(), 4).size();
	
}

WinTypeList TranslateWinTypeList(const WinTypeList &innerWinTypes)
{
	WinTypeList WinTypes = innerWinTypes;

	if(WinTypes.HasType(WIN_TYPE_GANG_BAO))
		WinTypes.Remove(WIN_TYPE_GANG_BAO);

	if(WinTypes.HasType(WIN_TYPE_TIAN_HU))
		WinTypes.Remove(WIN_TYPE_TIAN_HU);

	if(WinTypes.HasType(WIN_TYPE_DI_HU))
		WinTypes.Remove(WIN_TYPE_DI_HU);
	
	return WinTypes;
}
