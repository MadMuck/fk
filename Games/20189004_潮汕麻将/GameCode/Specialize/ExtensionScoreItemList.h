#ifndef EXTENSION_SCORE_ITEM_LIST_H
#define EXTENSION_SCORE_ITEM_LIST_H

#include "GameContext.h"
#include "Liquidation.h"
#include <map>

class ExtensionScoreItemList : public GameContextExtension
{
	std::map<PlayerPos::Type, ScoreItemList> m_ScoreItemLists;		// 结算用的分数项
	std::map<PlayerPos::Type, int> m_PlayerWinningFan; // 玩家胡牌番数
public:
	void OnGameStart()
	{
		m_ScoreItemLists.clear();
		m_PlayerWinningFan.clear();
	}

	ScoreItemList GetItemList(PlayerPos::Type playerPos) const
	{
		ScoreItemList ret;
		if(m_ScoreItemLists.find(playerPos) != m_ScoreItemLists.end())
		{
			ret = m_ScoreItemLists.find(playerPos)->second;
		}
		return ret;
	}

	void AddItem(PlayerPos::Type playerPos, const ScoreItem &scoreItem)
	{
		m_ScoreItemLists[playerPos].push_back(scoreItem);
	}

	void RemoveItem(PlayerPos::Type playerPos, int type)
	{
		std::map<PlayerPos::Type, ScoreItemList>::iterator itr = m_ScoreItemLists.find(playerPos);
		if(itr != m_ScoreItemLists.end())
		{
			ScoreItemList &scoreItemList = itr->second;
			for(int i = (int)scoreItemList.size() - 1; i >= 0; --i)
			{
				if(scoreItemList[i].Type() == type)
				{
					scoreItemList.erase(scoreItemList.begin() + i);
				}
			}
		}
	}

	bool RemoveLastItem(PlayerPos::Type playerPos, int type)
	{
		bool ret = false;
		std::map<PlayerPos::Type, ScoreItemList>::iterator itr = m_ScoreItemLists.find(playerPos);
		if(itr != m_ScoreItemLists.end())
		{
			ScoreItemList &scoreItemList = itr->second;
			for(int i = (int)scoreItemList.size() - 1; i >= 0; --i)
			{
				if(scoreItemList[i].Type() == type)
				{
					scoreItemList.erase(scoreItemList.begin() + i);
					ret = true;
					break;
				}
			}
		}
		return ret;
	}

	ScoreItemList GetItemOfType(PlayerPos::Type playerPos, int type) const
	{
		ScoreItemList ret;
		std::map<PlayerPos::Type, ScoreItemList>::const_iterator itr = m_ScoreItemLists.find(playerPos);
		if(itr != m_ScoreItemLists.end())
		{
			const ScoreItemList &scoreItemList = itr->second;
			for(int i = 0; i < (int)scoreItemList.size(); ++i)
			{
				if(scoreItemList[i].Type() == type)
				{
					ret.push_back(scoreItemList.at(i));
				}
			}
		}
		return ret;
	}

	void SetWinningFan(PlayerPos::Type playerPos, int fan)
	{
		m_PlayerWinningFan[playerPos] = fan;
	}

	int GetWinningFan(PlayerPos::Type playerPos) const
	{
		int ret = 0;
		if(m_PlayerWinningFan.find(playerPos) != m_PlayerWinningFan.end())
		{
			ret = m_PlayerWinningFan.at(playerPos);
		}
		return ret;
	}
};

#endif