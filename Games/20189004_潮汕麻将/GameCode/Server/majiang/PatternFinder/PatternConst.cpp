#include "PatternConst.h"
#include "PatternResultCards.h"
#include "PatternType.h"

PatternConstImpl::PatternConstImpl(const CardList &cards):m_Stat(cards)
{}

PatternConstImpl::PatternConstImpl(){}

void PatternConstImpl::Add(Card::Type card, int cnt)
{
    m_Stat.Add(card, cnt);
}

PatternResult PatternConstImpl::Find(const CardStatistic &statistic, size_t &idx) const
{
    if(idx > 0)
    {
        idx = 1;
        return PatternResult();
    }

    const CardStatistic::ValuesType values1 = statistic.Values();
    const CardStatistic::ValuesType values2 = m_Stat.Values();

    bool match = true;
    for(CardStatistic::ValuesType::const_iterator itr = values2.begin();
        itr != values2.end();
        ++itr)
    {
        if(m_Stat.Num(*itr) > statistic.Num(*itr))
        {
            match = false;
            break;
        }
    }

    if(match)
    {
        CardList matchResult;
        for(CardStatistic::ValuesType::const_iterator itr = values2.begin();
            itr != values2.end();
            ++itr)
        {
            matchResult.Add(*itr, m_Stat.Num(*itr));
        }
        idx = 1;
        return PatternResult(new PatternResultCardsImpl(PatternType_Const, matchResult));
    }

    return PatternResult();
}

PatternConst::PatternConst():Pattern(new PatternConstImpl()){}

PatternConst::PatternConst(const CardList &cards):Pattern(new PatternConstImpl(cards))
{
}

PatternConst::PatternConst(Card::Type card, int cnt):Pattern(new PatternConstImpl())
{
    Add(card, cnt);
}

PatternConst &PatternConst::Add(Card::Type card, int cnt)
{
    ((PatternConstImpl*)(m_Impl.GetObj()))->Add(card, cnt);
    return *this;
}
