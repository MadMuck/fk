#ifndef EXTENSION_QUE_MEN_H
#define EXTENSION_QUE_MEN_H

#include "GameContext.h"
#include "PlayerPos.h"


// 缺门数据
class ExtensionQueMen : public GameContextExtension
{
	typedef std::map<PlayerPos::Type, int> QueMenType;

	QueMenType m_QueMenType;

public:
	void OnGameStart()
	{
		m_QueMenType.clear();
	}

	void Set(PlayerPos::Type playerPos, int type)
	{
		m_QueMenType[playerPos] = type;
	}

	int Get(PlayerPos::Type playerPos) const
	{
		int ret = 255;
		if(m_QueMenType.find(playerPos) != m_QueMenType.end())
		{
			ret = m_QueMenType.find(playerPos)->second;
		}
		return ret;
	}
	
	// 是否已经定缺
	bool HasDetermined(PlayerPos::Type playerPos) const
	{
		return m_QueMenType.find(playerPos) != m_QueMenType.end();
	}

	CardList FilterCards(PlayerPos::Type playerPos, const CardList &cards) const
	{
		int type = Get(playerPos);
		CardList ret;
		for(CardList::const_iterator itr = cards.begin();
			itr != cards.end();
			++itr)
		{
			if((*itr) / 10 == type)
			{
				ret.Add(*itr);
			}
		}
		return ret;
	}
};

#endif