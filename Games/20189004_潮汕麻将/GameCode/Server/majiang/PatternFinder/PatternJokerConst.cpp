#include "PatternJokerConst.h"
#include "PatternResultCards.h"
#include "PatternType.h"

PatternJokerConstImpl::PatternJokerConstImpl(const PatternJokerList &jokers, const CardList &cards):m_Stat(cards),  m_Jokers(jokers)
{}

PatternJokerConstImpl::PatternJokerConstImpl(const PatternJokerList &jokers): m_Jokers(jokers){}

void PatternJokerConstImpl::Add(Card::Type card, int cnt)
{
    m_Stat.Add(card, cnt);
}

PatternResult PatternJokerConstImpl::Find(const CardStatistic &statistic, size_t &idx) const
{
    if(idx > 0)
    {
        idx = 1;
        return PatternResult();
    }

    idx = 1;

    const CardStatistic::ValuesType values1 = statistic.Values();
    const CardStatistic::ValuesType values2 = m_Stat.Values();

    int jokerCnt = m_Jokers.JokersLimitCount();

    CardList cards;
    int absenceCnt = 0;
    for(CardStatistic::ValuesType::const_iterator itr = values2.begin();
        itr != values2.end();
        ++itr)
    {
        int diff = m_Stat.Num(*itr) - statistic.Num(*itr);
        if(diff > 0)
        {
            absenceCnt += diff;
            cards.Add(*itr, m_Stat.Num(*itr) - diff);
        }
        else
        {
            cards.Add(*itr, m_Stat.Num(*itr));
        }
    }

    if(absenceCnt <= jokerCnt)
    {
        bool found = (absenceCnt == 0);
        if(!found)
        {
            const CardList &jokers = m_Jokers.GetJokers();
            CardStatistic tmpStat = statistic;
            tmpStat.Sub(cards);
            for(CardList::const_iterator itr = jokers.begin();
                itr != jokers.end();
                ++itr)
            {
                int cnt = tmpStat.Num(*itr);
                if(cnt >= absenceCnt)
                {
                    found = true;
                    cards.Add(*itr, absenceCnt);
                    break;
                }
                else
                {
                    cards.Add(*itr, cnt);
                    tmpStat.Sub(*itr, cnt);
                    absenceCnt -= cnt;
                }
            }
        }

        if(found)
        {
		    int patternType = absenceCnt > 0 ? PatternType_JokerConst : PatternType_Const;
            return PatternResult(new PatternResultCardsImpl(patternType, cards));
        }
    }

    return PatternResult();
}

PatternJokerConst::PatternJokerConst(const PatternJokerList &jokers):Pattern(new PatternJokerConstImpl(jokers)){}

PatternJokerConst::PatternJokerConst(const PatternJokerList &jokers, Card::Type card, int cnt):Pattern(new PatternJokerConstImpl(jokers))
{
    Add(card, cnt);
}

PatternJokerConst &PatternJokerConst::Add(Card::Type card, int cnt)
{
    ((PatternJokerConstImpl*)(m_Impl.GetObj()))->Add(card, cnt);
    return *this;
}
