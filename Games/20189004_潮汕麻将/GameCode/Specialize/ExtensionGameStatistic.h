#ifndef EXTENSION_GAME_STATISTIC_H
#define EXTENSION_GAME_STATISTIC_H

#include "PlayerPos.h"
#include "GameContext.h"
#include "UtilTemplate.h"
#include <map>
#include <vector>

enum GameStat_Type
{
	GameStat_BankerTimes,
	GameStat_WinTimes,
	GameStat_SelfFeedTimes,
	GameStat_CatchFireTimes,
	GameStat_LightTimes,
	GameStat_FindTreasureTimes,
	GameStat_WinOnStartTimes,
	GameStat_TreasureInTreasureTimes,
	GameStat_TotalScore,
	GameStat_MingGangTimes,
	GameStat_BuGangTimes,
	GameStat_AnGangTimes,
	GameStat_PaiTouTimes,
};


// 游戏统计数据
class ExtensionGameStatistic : public GameContextExtension
{
	typedef std::map<int, int> StatiticOfType;
	typedef std::map<PlayerPos::Type, StatiticOfType > PlayerStatisticData;

	PlayerStatisticData m_GameStatistic;
public:
	void OnGameReset()
	{
		m_GameStatistic.clear();
	}

	int Get(PlayerPos::Type playerPos, int type) const
	{
		int ret = 0;
		PlayerStatisticData::const_iterator findItr = m_GameStatistic.find(playerPos);
		if(findItr != m_GameStatistic.end())
		{
			StatiticOfType::const_iterator findItr2 = findItr->second.find(type);
			if(findItr2 != findItr->second.end())
			{
				ret = findItr2->second;
			}
		}
		return ret;
	}

	void Set(PlayerPos::Type playerPos, int type, int value)
	{
		m_GameStatistic[playerPos][type] = value;
	}

	void Add(PlayerPos::Type playerPos, int type, int value)
	{
		m_GameStatistic[playerPos][type] += value;
	}

	std::vector<PlayerPos::Type> FindPlayerWithMost(int type, const std::vector<PlayerPos::Type> pos) const
	{
		return FindTopPlayerAccordingToValue(type, pos, std::greater_equal<Util::SortData<int, PlayerPos::Type> >());
	}

	std::vector<PlayerPos::Type> FindPlayerWithLeast(int type, const std::vector<PlayerPos::Type> pos) const
	{
		return FindTopPlayerAccordingToValue(type, pos, std::less_equal<Util::SortData<int, PlayerPos::Type> >());
	}

	template<typename T>
	std::vector<PlayerPos::Type> FindTopPlayerAccordingToValue(int type, const std::vector<PlayerPos::Type> pos, const T &comparer) const
	{
		std::vector<Util::SortData<int, PlayerPos::Type> > dataToSort;

		for(std::vector<PlayerPos::Type>::const_iterator itr = pos.begin();
			itr != pos.end();
			++itr)
		{
			int value = Get(*itr, type);
			dataToSort.push_back(Util::SortData<int, PlayerPos::Type>(value, *itr));
		}

		std::sort(dataToSort.begin(), dataToSort.end(), comparer);

		std::vector<PlayerPos::Type> ret;

		ret.push_back(dataToSort.begin()->value);
		for(size_t i = 1; i < dataToSort.size(); ++i)
		{
			if(dataToSort[i].key == dataToSort[i-1].key)
			{
				ret.push_back(dataToSort[i].value);
			}
			else
			{
				break;
			}
		}

		return ret;
	}
};

#endif