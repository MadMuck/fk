#include "PatternRun.h"
#include "PatternResultCards.h"
#include "PatternType.h"

PatternRunImpl::PatternRunImpl(int size):m_Size(size){}

PatternResult PatternRunImpl::Find(const CardStatistic &stat, size_t &idx) const
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

        Card::Type valueStart, valueEnd;
        Card::GetValueInterval(*itr, valueStart, valueEnd);

        if(*itr >= Card::ZiStart || *itr + m_Size > valueEnd) continue;

        CardList cards;
        for(int i = 0; i < m_Size; ++i)
        {
            Card::Type card = *itr + i;
            if(stat.Num(card) <= 0)
            {
                break;
            }
            else
            {
                cards.Add(card);
            }
        }

        if(cards.Count() == m_Size)
        {
            return PatternResult(new PatternResultCardsImpl(PatternType_Run, cards));
        }
    }

    return PatternResult();
}

PatternRun::PatternRun(int size):Pattern(new PatternRunImpl(size)){}
