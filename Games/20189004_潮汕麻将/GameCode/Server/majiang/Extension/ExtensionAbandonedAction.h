

#ifndef EXTENSION_ABANDONDED_ACTION_H
#define EXTENSION_ABANDONDED_ACTION_H

#include "GameContext.h"
#include "Card.h"
#include "Interception.h"
#include "PlayerPos.h"
#include <map>

class ExtensionAbandonedAction : public GameContextExtension
{
	// ÆúºúµÄÅÆ
	//	std::map<PlayerPos::Type, std::set<Card::Type> > m_AbandonedWin;
	std::map<PlayerPos::Type, Card::Type> m_AbandonedWin;
	std::map<PlayerPos::Type, Card::Type> m_AbandonePeng;
	std::map<PlayerPos::Type, Card::Type> m_AbandoneGang;
public:
	void OnGameStart()
	{
		m_AbandonedWin.clear();
		m_AbandonePeng.clear();
		m_AbandoneGang.clear();
	}

	void Abandoned(PlayerPos::Type playerPos, Interception::Type type, Card::Type card)
	{
		if(Interception::Hu == type)
		{
			m_AbandonedWin[playerPos]= card;
		}
		else if(Interception::Peng == type)
		{
			m_AbandonePeng[playerPos] = card;
		}
		else if(Interception::BuGang == type || Interception::MingGang == type  )
		{
			m_AbandoneGang[playerPos] = card;
		}
	}

	bool HasAbandoned(PlayerPos::Type playerPos, Interception::Type type, Card::Type card = Card::Invalid)
	{
		bool ret = false;
		if(Interception::Hu == type)
		{
			ret = m_AbandonedWin.find(playerPos) != m_AbandonedWin.end() && card ==  m_AbandonedWin.find(playerPos)->second;
		}
		else if (Interception::Peng == type)
		{
			ret = (m_AbandonePeng.find(playerPos) != m_AbandonePeng.end() && card == m_AbandonePeng.find(playerPos)->second);
		}
		else if(Interception::BuGang == type || Interception::MingGang == type)
		{
			ret = (m_AbandoneGang.find(playerPos) != m_AbandoneGang.end() && card == m_AbandoneGang.find(playerPos)->second);
		}
		return ret;
	}

	void Clear(PlayerPos::Type playerPos)
	{
		m_AbandonedWin.erase(playerPos);
		m_AbandonePeng.erase(playerPos);
		m_AbandonedWin.erase(playerPos);
	}
};

#endif