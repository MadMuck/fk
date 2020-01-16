#include "Liquidation.h"

Score::Score():m_PlayerPos(PlayerPos::P0), m_Score(0){}

Score::Score(PlayerPos::Type playerPos):m_PlayerPos(playerPos), m_Score(0){}

PlayerPos::Type Score::GetPlayerPos() const
{
    return m_PlayerPos;
}

int Score::GetScore() const
{
    return m_Score;
}

void Score::Add(int score)
{
    m_Score += score;
}

void Score::Clear()
{
    m_Score = 0;
}



int ScoreList::GetScore(PlayerPos::Type playerPos) const
{
    int ret = 0;
    const_iterator itr = find(playerPos);
    if(itr != end())
    {
        ret = itr->second.GetScore();
    }
    return ret;
}

int ScoreOfType::GetScore(int type) const
{
	int ret = 0;
	if(find(type) != end())
	{
		ret = find(type)->second.GetScore();
	}
	return ret;
}



Liquidation::Liquidation():m_PlayerCount(0)
{
    for(int i = 0; i < PlayerPos::Max; ++i)
    {
        PlayerPos::Type pos = PlayerPos::P0 + i;
        m_TotalScoreList[pos] = Score(pos);

		m_ScoreOfType[pos] = ScoreOfType();
    }
}

void Liquidation::Prepare(PlayerPos::Type bankerPos, int playerCount)
{
    m_BankerPos = bankerPos;
	m_PlayerCount = playerCount;
}

void Liquidation::Add(PlayerPos::Type playerPos, const ScoreItem &scoreItem)
{
    for(int i = 0; i < m_PlayerCount; ++i)
    {
        PlayerPos::Type src = PlayerPos::P0 + i; //src为输分玩家
        if(playerPos != src)
        {
			PlayerPos::Type realSrc = src; 
            int score = scoreItem.Score(playerPos, src, m_BankerPos, realSrc);
            m_TotalScoreList[playerPos].Add(score);
            m_TotalScoreList[realSrc].Add(-score);

			int scoreType = scoreItem.Type();
			m_ScoreOfType[playerPos][scoreType].Add(score);
			m_ScoreOfType[realSrc][scoreType].Add(-score);
        }
    }
}

const ScoreList &Liquidation::GetScoreList() const
{
    return m_TotalScoreList;
}

ScoreOfType Liquidation::GetScoreOfType(PlayerPos::Type playerPos) const
{
	std::map<PlayerPos::Type, ScoreOfType>::const_iterator itr = m_ScoreOfType.find(playerPos);
	if(itr != m_ScoreOfType.end())
	{
		return itr->second;
	}
	return ScoreOfType();
}

void Liquidation::Clear()
{
	m_BankerPos = PlayerPos::P0;
    for(ScoreList::iterator itr = m_TotalScoreList.begin();
        itr != m_TotalScoreList.end();
        ++itr)
    {
        itr->second.Clear();
    }

	for(std::map<PlayerPos::Type, ScoreOfType>::iterator itr = m_ScoreOfType.begin();
		itr != m_ScoreOfType.end();
		++itr)
	{
		itr->second.clear();
	}
}
