#ifndef EXTENSION_FAN_MA_H
#define EXTENSION_FAN_MA_H

#include "GameContext.h"
#include "Card.h"
#include "PlayerPos.h"
#include <map>

#define  PLAY_COUNT 4

class ExtensionFanMa : public GameContextExtension
{
	CardList m_MaCards;														// ������
	CardList m_TotalZhongMaCard;										// �������������ƣ����������Ҫ������
	std::map<PlayerPos::Type, CardList> m_ZhongMaData;	// �������
	std::map<int, CardList> m_MaCardToDistance;					// ���ƶ�Ӧλ�ã�ׯ�Ҿ���ׯ�ҵľ��룩

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
				m_MaCardToDistance[0].push_back(card);		//ׯ��
			else if(2 == tempcard % 4)
				m_MaCardToDistance[1].push_back(card);		//ׯ���¼�
			else if(3 == tempcard % 4)
				m_MaCardToDistance[2].push_back(card);		//ׯ�ҶԼ�
			else if(0 == tempcard % 4)
				m_MaCardToDistance[3].push_back(card);		//ׯ���ϼ�
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

	CardList GetMaCardToDistance(int idistance) //idistance Ϊ��Ҷ�Ӧ�ķ�λ
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

	int GetDistance(PlayerPos::Type DeakPos, PlayerPos::Type origin) const//Ϊ�˽��2,3�˳�������
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
				idx = (idx + playerCount - 1) % playerCount;		//��ʱ��
				++length;
				if(length > playerCount)
					break;
			}

// 			if(playerCount == 3 && length == 2)	//����2����Ϊׯ���ϼ�
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