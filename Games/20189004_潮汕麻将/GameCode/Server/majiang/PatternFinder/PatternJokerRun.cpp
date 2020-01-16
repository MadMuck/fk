#include "PatternJokerRun.h"
#include "PatternResultCards.h"
#include "Logger.h"
#include "PatternType.h"
#include <list>

PatternJokerRunImpl::PatternJokerRunImpl(int size, const PatternJokerList &jokers):m_Size(size),  m_Jokers(jokers){}

PatternResult PatternJokerRunImpl::Find(const CardStatistic &stat, size_t &idx) const
{
    const CardStatistic::ValuesType &values = stat.Values();
    size_t totalProgress = values.size() * m_Size;
    if(idx >= totalProgress)
    {
        idx = totalProgress;
        return PatternResult();
    }

    int jokerCnt = m_Jokers.JokersLimitCount();

	size_t mainIdx = idx / m_Size;
	size_t subIdx = idx % m_Size;

    while(mainIdx < values.size())
    {
        CardStatistic::ValuesType::const_iterator itr = values.begin() + mainIdx;

		Card::Type valueStart, valueEnd;
		Card::GetValueInterval(*itr, valueStart, valueEnd);

		int maxPos = *itr - valueStart;
		if(maxPos > m_Size - 1) maxPos = m_Size - 1;

		int minPos = *itr + m_Size - valueEnd;
		if(minPos < 0) minPos = 0;

		if(subIdx < (size_t)minPos)
		{
			subIdx = (size_t)minPos;
		}

        if(*itr >= Card::FengStart && *itr < Card::FengEnd ||
			subIdx > (size_t)maxPos)
		{
			++mainIdx;
			subIdx = 0;
			idx = mainIdx * m_Size + subIdx;
			continue;
		}

		Card::Type prevCheckCard = Card::Invalid;
		if(mainIdx > 0)
		{
			prevCheckCard = *(values.begin() + (mainIdx - 1));
		}

		for(int pos = subIdx; pos <= maxPos; ++pos)
		{
			subIdx = pos + 1;

			if(Card::Invalid != prevCheckCard && *itr - pos <= prevCheckCard)
			{
				continue;
			}

			CardList cards;

			std::list<int> absencePos;
			for(int i = 0; i < m_Size; ++i)
			{
				Card::Type card = *itr + i - pos;
				if(stat.Num(card) <= 0)
				{
					absencePos.push_back(i);
				}
				else
				{
					cards.Add(card);
				}
			}

			if((int)absencePos.size() > jokerCnt)
			{
				continue;
			}

			int absenceCnt = (int)absencePos.size();
			bool found = (absenceCnt == 0);

			if(!found)
			{

				CardStatistic tmpStat = stat;
				tmpStat.Sub(cards);
				const CardList &jokers = m_Jokers.GetJokers();
				for(CardList::const_iterator itr2 = jokers.begin();
					itr2 != jokers.end();
					++itr2)
				{
					int cnt = tmpStat.Num(*itr2);
					if(cnt == 0) continue;

					int insertCnt = cnt;
					if(insertCnt > (int)absencePos.size())
						insertCnt = absencePos.size();

					for(int i = 0; i < insertCnt; ++i)
					{
						cards.insert(cards.begin() + *absencePos.begin(), *itr2);
						absencePos.pop_front();
					}

					tmpStat.Sub(*itr2, insertCnt);

					if(absencePos.size() == 0)
					{
						found = true;
						break;
					}
				}
			}

			if(found)
			{
				idx = mainIdx * m_Size + subIdx;
				int patternType = absenceCnt > 0 ? PatternType_JokerRun : PatternType_Run;
				//LOGGER_CONSOLE("joker run "<< cards);
				return PatternResult(new PatternResultCardsImpl(patternType, cards));
			}
		}
	}

    return PatternResult();
}

PatternJokerRun::PatternJokerRun(int size,  const PatternJokerList &jokers):Pattern(new PatternJokerRunImpl(size, jokers)){}
