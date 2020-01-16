
#include "ExtensionLiquidation.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionPlayer.h"
ExtensionLiquidation::ExtensionLiquidation():m_DrawCount(0), m_GameCount(0),
	m_CardScoreDelegate(NULL)
{

}

//void ExtensionLiquidation::ScoreItemListStatistic()
//{
//	OBJ_GET_EXT(m_Context,ExtensionBankerSelector, extBankerSelector):
//	OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
//	m_Liquidation.Clear();
//	m_Liquidation.Prepare(extBankerSelector->GetBankerPos(), m_Context->GetPlayerCount());
//	const PlayerList &players = extPlayer->GetPlayerList();
//	for(PlayerList::const_iterator itr = players.begin();
//		itr != players.end();
//		++itr)
//	{
//		PlayerPos::Type playerPos = itr->second->GetPos();
//
//		ScoreItemList scoreItemList = m_ItemListDelegate->GetScoreItemList(playerPos);
//
//		for(ScoreItemList::iterator itr2 = scoreItemList.begin(); itr2 != scoreItemList.end(); ++itr2)
//		{
//			m_Liquidation.Add(playerPos, *itr2);
//		}
//
//
//	}
//}

Liquidation &ExtensionLiquidation::GetLiquidation()
{
    return m_Liquidation;
}

void ExtensionLiquidation::AddDrawCount()
{
	++m_DrawCount;
}

int ExtensionLiquidation::GetGameCount() const
{
	return m_GameCount;
}

void ExtensionLiquidation::AddGameCount()
{
	++m_GameCount;
}

int ExtensionLiquidation::GetDrawCount() const
{
	return m_DrawCount;
}

void ExtensionLiquidation::AddWinner(PlayerPos::Type playerPos, WinMode winMode, int round)
{
	m_WinMode[playerPos] = winMode;
	m_Winners.Add(playerPos);
	m_WinRound[playerPos] = round;
}

void ExtensionLiquidation::AddWinnerAndLoser(PlayerPos::Type playerPos, PlayerPos::Type loser, int round)
{
	m_Winners.Add(playerPos);
	m_Loser[playerPos] = loser;
	m_WinMode[playerPos] = WinMode_Fire;
	m_WinRound[playerPos] = round;
}

const WinnerList &ExtensionLiquidation::GetWinners() const
{
	return m_Winners;
}

const PlayerPos::Type ExtensionLiquidation::GetLoser(PlayerPos::Type playerPos) const
{
	PlayerPos::Type loser = PlayerPos::Max;
	if(m_Loser.find(playerPos) != m_Loser.end())
	{
		loser = m_Loser.find(playerPos)->second;
	}
	return loser;
}

bool ExtensionLiquidation::IsLoser(PlayerPos::Type playerPos) const
{
	bool ret = false;
	for(std::map<PlayerPos::Type, PlayerPos::Type>::const_iterator itr = m_Loser.begin();
		itr != m_Loser.end();
		++itr)
	{
		if(itr->second == playerPos)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

WinMode ExtensionLiquidation::GetWinMode(PlayerPos::Type playerPos) const
{
	WinMode winMode = WinMode_Invalid;
	if(m_WinMode.find(playerPos) != m_WinMode.end())
	{
		winMode = m_WinMode.find(playerPos)->second;
	}
	return winMode;
}

void ExtensionLiquidation::SetCardScoreDelegate(const RefCntObj<CardScoreDelegate> &delegate)
{
	m_CardScoreDelegate = delegate;
}

WinType &ExtensionLiquidation::GetWinType()
{
	return m_WinType;
}

int ExtensionLiquidation::GetWinRound(PlayerPos::Type playerPos) const
{
	int ret = -1;
	if(m_WinRound.find(playerPos) != m_WinRound.end())
		ret = m_WinRound.at(playerPos);
	return ret;
}

void ExtensionLiquidation::SetMaxMulti(int multi)
{
	m_MaxMulti = multi;
}

int ExtensionLiquidation::GetMaxMulti()
{
	return m_MaxMulti;
}

int ExtensionLiquidation::GetScore(const CardList &cardList, const InterceptionList &interceptions, const CardInfo &src, 
	PlayerPos::Type playerPos, PlayerPos::Type bankerPos, bool reveal) const
{
	int ret = 0;
	if(m_CardScoreDelegate.GetObj())
	{
		ret = m_CardScoreDelegate->GetScore(cardList, interceptions, src, playerPos, bankerPos, reveal);
	}
	return ret;
}

void ExtensionLiquidation::OnAddInterception(PlayerPos::Type srcPlayer, const std::map<PlayerPos::Type, AppendInterceptionInfo> &appendAction)
{
	m_CardScoreDelegate->OnAddInterception(srcPlayer, appendAction);
}

void ExtensionLiquidation::OnGameStart()
{
	Reset();
}

void ExtensionLiquidation::OnGameReset()
{
	Reset();

	m_DrawCount = 0;
	m_GameCount = 0;
}

void ExtensionLiquidation::Reset()
{
	m_Liquidation.Clear();
	m_Winners.clear();
	m_WinRound.clear();
	m_WinMode.clear();
	m_Loser.clear();
	m_WinType.ClearAll();
	m_MaxMulti = 1;
}


/////////////////////////////////////////////////////////////////////////////////////////
bool WinTypeList::HasType(unsigned char type) const
{
	bool ret = false;
	for(const_iterator itr = begin(); itr != end(); ++itr)
	{
		if(*itr == type)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void WinTypeList::Add(unsigned char type)
{
	if(!HasType(type)) push_back(type);
}

void WinTypeList::Remove(unsigned char type)
{
	for(const_iterator itr = begin(); itr != end(); ++itr)
	{
		if(*itr == type)
		{
			erase(itr);
			break;
		}
	}
}

std::ostream &operator << (std::ostream &ostrm, const WinTypeList &winTypeList)
{
	for(WinTypeList::const_iterator itr = winTypeList.begin();
		itr != winTypeList.end();
		++itr)
	{
		ostrm << (int)*itr << " ";
	}
	return ostrm;
}

//////////////////////////////////////////////////////////


WinTypeList WinType::GetWinType(PlayerPos::Type playerPos) const
{
	std::map<PlayerPos::Type, WinTypeList>::const_iterator itr = m_WinTypes.find(playerPos);
	if(itr != m_WinTypes.end())
	{
		return itr->second;
	}
	return WinTypeList();
}

void WinType::SetWinType(PlayerPos::Type playerPos, unsigned char type)
{
	WinTypeList &types = m_WinTypes[playerPos];
	types.Add(type);
}

void WinType::RemoveType(PlayerPos::Type playerPos, unsigned char type)
{
	WinTypeList &types = m_WinTypes[playerPos];
	types.Remove(type);
}

void WinType::Clear(PlayerPos::Type playerPos)
{
	WinTypeList &types = m_WinTypes[playerPos];
	types.clear();
}

void WinType::ClearAll()
{
	m_WinTypes.clear();
}

///////////////////////////////////////////////////////
bool WinnerList::Contains(PlayerPos::Type pos) const
{
	bool ret = 0;
	for(const_iterator itr = begin(); itr != end(); ++itr)
	{
		if(*itr == pos)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void WinnerList::Add(PlayerPos::Type pos)
{
	push_back(pos);
}

PlayerPos::Type WinnerList::Get(int num) const
{
	return at(num);
}

int WinnerList::Count() const
{
	return (int)size();
}