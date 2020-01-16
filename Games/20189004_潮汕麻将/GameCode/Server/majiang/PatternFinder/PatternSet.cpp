#include "PatternSet.h"
#include "PatternResultCards.h"
#include "PatternType.h"

PatternSetImpl::PatternSetImpl(int size):m_Size(size){}

PatternResult PatternSetImpl::Find(const CardStatistic &stat, size_t &idx) const
{
    const CardStatistic::ValuesType &values = stat.Values();
    size_t valuesCnt = values.size();
    if(idx >= valuesCnt)
    {
        idx = valuesCnt;
        return PatternResult();
    }

    while(idx < valuesCnt)
    {
        CardStatistic::ValuesType::const_iterator itr = values.begin() + idx;
        ++idx;

        if(stat.Num(*itr) < m_Size) continue;

        CardList cards;
        cards.Add(*itr, m_Size);

        return PatternResult(new PatternResultCardsImpl(PatternType_Set, cards));
    }

    return PatternResult();
}

PatternSet::PatternSet(int size):Pattern(new PatternSetImpl(size)){}
