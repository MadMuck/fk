#ifndef EXTENSION_HUAN_ZHANG_H
#define EXTENSION_HUAN_ZHANG_H

#include "GameContext.h"
#include "Card.h"
#include "PlayerPos.h"
#include <map>

class ExtensionHuanZhang : public GameContextExtension
{
	std::map<PlayerPos::Type, CardList> m_Selection;			//玩家换三张选择的牌
	std::map<PlayerPos::Type, CardList> m_SwapCard;				//玩家换三张得到的牌
public:
	void OnGameStart()
	{
		Clear();
	}

	void Clear()
	{
		m_Selection.clear();
		m_SwapCard.clear();
	}

	void Clear(PlayerPos::Type playerPos)
	{
		m_Selection.erase(playerPos);
		m_SwapCard.erase(playerPos);
	}

	CardList Get(PlayerPos::Type playerPos) const
	{
		if(m_Selection.find(playerPos) != m_Selection.end())
			return m_Selection.at(playerPos);
		return CardList();
	}

	void Add(PlayerPos::Type playerPos, Card::Type card)
	{
		m_Selection[playerPos].Add(card);
	}

	void SetSwapCard(PlayerPos::Type playerPos, CardList cards)
	{
		m_SwapCard[playerPos] = cards;
	}

	CardList GetswapCard(PlayerPos::Type playerpos)
	{
		if(m_SwapCard.find(playerpos) != m_SwapCard.end())
		{
			return m_SwapCard[playerpos];
		}
		return CardList();
	}
};

#endif
