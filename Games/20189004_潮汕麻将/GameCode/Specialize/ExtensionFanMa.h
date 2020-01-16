#ifndef EXTENSION_FAN_MA_H
#define EXTENSION_FAN_MA_H

#include "GameContext.h"
#include "Card.h"
#include "PlayerPos.h"
#include <map>

#define  PLAY_COUNT 4

class ExtensionFanMa : public GameContextExtension
{
	CardList m_MaCards;														// 翻马牌
	CardList m_TotalZhongMaCard;										// 所有玩家中马的牌（结算界面需要高亮）
	std::map<PlayerPos::Type, CardList> m_ZhongMaData;	// 中马情况
	std::map<int, CardList> m_MaCardToDistance;					// 马牌对应位置（庄家距离庄家的距离）

public:
	ExtensionFanMa()
	{
		m_MaCardToDistance.clear();
		for(Card::Type card = Card::Wan1; card != Card::ZiEnd; ++card)
		{
			int tempcard = card % 10;
			if(0 == tempcard)
				continue;
			else if(1 == tempcard % 4)					
				m_MaCardToDistance[0].push_back(card);		//庄家
			else if(2 == tempcard % 4)
				m_MaCardToDistance[1].push_back(card);		//庄家下家
			else if(3 == tempcard % 4)
				m_MaCardToDistance[2].push_back(card);		//庄家对家
			else if(0 == tempcard % 4)
				m_MaCardToDistance[3].push_back(card);		//庄家上家
		}
	}

	void OnGameStart()
	{
		m_MaCards.clear();
		m_TotalZhongMaCard.Clear();
		m_ZhongMaData.clear();
	}

	void AddMaCard(Card::Type maCard)//
	{
		m_MaCards.push_back(maCard);
	}

	void AddZhongMaData(PlayerPos::Type playerPos, Card::Type card)//
	{
		m_ZhongMaData[playerPos].push_back(card);
		m_TotalZhongMaCard.push_back(card);
	}

	CardList GetMaCardToDistance(int idistance) //idistance 为玩家对应的方位
	{
		auto itr = m_MaCardToDistance.find(idistance);
		if(itr != m_MaCardToDistance.end())
			return m_MaCardToDistance[idistance];
		else
			return CardList();
	}

	CardList GetMaCards()
	{
		return m_MaCards;
	}

	int GetZhongMaCount(PlayerPos::Type playerPos)
	{
		auto itr = m_ZhongMaData.find(playerPos);
		if(itr != m_ZhongMaData.end())
			return m_ZhongMaData[playerPos].size();
		else
			return 0;
	}

	CardList GetZhongMaData(PlayerPos::Type playerPos)
	{
		auto itr = m_ZhongMaData.find(playerPos);
		if(itr != m_ZhongMaData.end())
			return m_ZhongMaData[playerPos];
		else
			return CardList();
	}

	CardList GetTotalZhongMaCard()
	{
		return m_TotalZhongMaCard;
	}

	int GetDistance(PlayerPos::Type DeakPos, PlayerPos::Type origin) const//为了解决2,3人场的问题
	{
		int length = 0;

		if(DeakPos >= PlayerPos::Max)
		{
			length = PlayerPos::Max;
		}
		else if(2 == m_Context->GetPlayerCount())
		{
			if(DeakPos == origin)
				length = 0;
			else
				length = 2;
		}
		else
		{
			PlayerPos::Type idx = origin;
			int playerCount = m_Context->GetPlayerCount();
			while(idx != DeakPos)
			{
				idx = (idx + playerCount - 1) % playerCount;		//逆时针
				++length;
				if(length > playerCount)
					break;
			}

// 			if(playerCount == 3 && length == 2)	//三人2距离为庄家上家
// 				length = 3;
		}

		return length;
	}

	void OnGameFinish()
	{
		m_MaCards.clear();
		m_TotalZhongMaCard.Clear();
		m_ZhongMaData.clear();
	}
};

#endif