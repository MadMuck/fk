#include "PatternIntcSet.h"
#include "PatternResultInterception.h"
#include "PatternType.h"

PatternIntcSetImpl::PatternIntcSetImpl(int size):m_Size(size){}

PatternResult PatternIntcSetImpl::Find(const CardStatistic &stat, size_t &idx) const
{
    const InterceptionList &interceptions = stat.GetInterceptions();

    size_t interceptionsCnt = (size_t)interceptions.Count();
    if(idx >= interceptionsCnt)
    {
        idx = interceptionsCnt;
        return PatternResult();
    }

    while(idx < interceptionsCnt)
    {
        InterceptionList::const_iterator itr = interceptions.begin() + idx;
        ++idx;

        CardStat cardStat(itr->cards);
        const CardStat::ValuesType &values = cardStat.Values();

        if(values.size() == 1 && cardStat.Num(*values.begin()) == m_Size)
            return PatternResult(new PatternResultInterceptionImpl(PatternType_IntcSet, *itr));
    }

    return PatternResult();
}

PatternIntcSet::PatternIntcSet(int size):Pattern(new PatternIntcSetImpl(size)){}
