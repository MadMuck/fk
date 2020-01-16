#include "PatternIntcRun.h"
#include "PatternResultInterception.h"
#include "PatternType.h"

PatternIntcRunImpl::PatternIntcRunImpl(int size):m_Size(size){}

PatternResult PatternIntcRunImpl::Find(const CardStatistic &stat, size_t &idx) const
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

        bool found = (itr->cards.Count() == (size_t)m_Size) && 
			(itr->type == Interception::Chi || itr->type == Interception::Jian || itr->type == Interception::Feng);

        if(found)
            return PatternResult(new PatternResultInterceptionImpl(PatternType_IntcRun, *itr));
    }

    return PatternResult();
}

PatternIntcRun::PatternIntcRun(int size):Pattern(new PatternIntcRunImpl(size)){}
