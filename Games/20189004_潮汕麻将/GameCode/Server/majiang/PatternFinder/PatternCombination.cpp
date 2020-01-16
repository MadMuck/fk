#include "PatternCombination.h"
#include "Logger.h"


PatternResult PatternCombinationImpl::Find(const CardStatistic &statistic, size_t &idx) const
{
    size_t highIdx = ((idx & 0xff00) >> 8);
    size_t lowIdx = (idx & 0x00ff);

    Pattern *pattern = NULL;

    while(highIdx < m_PatternList.size())
    {
        const Pattern &pattern = *(m_PatternList.begin() + highIdx);

        PatternResult ret = pattern.Find(statistic, lowIdx);
        idx = (highIdx << 8) + lowIdx;

        if(!ret.IsValid())
        {
            ++highIdx;
            lowIdx = 0;
            idx = (highIdx << 8) + lowIdx;
        }
        else
        {
            return ret;
        }
    }

    return PatternResult();
}

PatternCombinationImpl *PatternCombinationImpl::Add(const Pattern &pattern)
{
    m_PatternList.push_back(pattern);
    return this;
}


PatternCombination::PatternCombination():Pattern(new PatternCombinationImpl())
{}

PatternCombination::PatternCombination(const PatternCombination &other):Pattern(other)
{

}

PatternCombination &PatternCombination::Add(const Pattern &pattern)
{
    ((PatternCombinationImpl *)m_Impl.GetObj())->Add(pattern);
    return *this;
}
