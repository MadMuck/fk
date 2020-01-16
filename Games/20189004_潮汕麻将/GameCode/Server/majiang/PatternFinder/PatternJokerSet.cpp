#include "PatternJokerSet.h"
#include "PatternResultCards.h"
#include "PatternType.h"
#include "Logger.h"

PatternJokerSetImpl::PatternJokerSetImpl(int size, const PatternJokerList &jokers):m_Size(size),  m_Jokers(jokers){}

PatternResult PatternJokerSetImpl::Find(const CardStatistic &stat, size_t &idx) const
{
    const CardStatistic::ValuesType &values = stat.Values();
	int jokerCnt = m_Jokers.JokersLimitCount();
    size_t valuesCnt = values.size() * (jokerCnt + 1);
    if(idx >= valuesCnt)
    {
        idx = valuesCnt;
        return PatternResult();
    }

    

    while(idx < valuesCnt)
    {
		size_t subIdx = idx % values.size();
		int targetAbsenceCnt = idx / values.size();

        CardStatistic::ValuesType::const_iterator itr = values.begin() + subIdx;
        ++subIdx;

		if(subIdx >= values.size())
		{
			subIdx = 0;
			idx = (targetAbsenceCnt + 1) * values.size();
		}
		else
		{
			idx = subIdx + targetAbsenceCnt * values.size();
		}

        int absenceCnt = m_Size - stat.Num(*itr);
        if(absenceCnt < 0) absenceCnt = 0;

        if(absenceCnt > targetAbsenceCnt)
        {
            continue;
        }

		absenceCnt = targetAbsenceCnt;

        CardList cards;
        cards.Add(*itr, m_Size - absenceCnt);

		bool found = absenceCnt == 0;

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
				if(insertCnt > absenceCnt)
					insertCnt = absenceCnt;

				cards.Add(*itr2, insertCnt);
				tmpStat.Sub(*itr2, insertCnt);
				absenceCnt -= insertCnt;

				if(absenceCnt == 0)
				{
					found = true;
					break;
				}
			}
		}

        if(found)
        {
		    int patternType = targetAbsenceCnt > 0 ? PatternType_JokerSet : PatternType_Set;
			//LOGGER_CONSOLE("joker set "<<cards);
            return PatternResult(new PatternResultCardsImpl(patternType, cards));
        }
    }

    return PatternResult();
}

PatternJokerSet::PatternJokerSet(int size,  const PatternJokerList &jokers):Pattern(new PatternJokerSetImpl(size, jokers)){}
