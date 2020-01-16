#include "Restatistic.h"
#include "Logger.h"

RestatisticResult::RestatisticResult():key(0), value(0)
{}



RestatisticMapper::RestatisticMapper(RestatisticMapperImpl *impl, RestatisticMapperRecycler *recycler):
    m_Impl(impl), m_Recycler(recycler)
{}

RestatisticResult RestatisticMapper::Map(int key, int value) const
{
    return m_Impl->Map(key, value);
}

void RestatisticMapper::Release()
{
    m_Recycler->Recycle(m_Impl);
}



void Restatistic::AddMapper(const RestatisticMapper &mapper)
{
    m_Mappers.push_back(mapper);
}

Statistic<int> Restatistic::Remap(const CardList &cards) const
{
    Statistic<int> result;
    for(CardList::const_iterator itr = cards.begin();
        itr != cards.end();
        ++itr)
    {
        result.Add(*itr, 1);
    }

    for(std::vector<RestatisticMapper>::const_iterator itr = m_Mappers.begin();
        itr != m_Mappers.end();
        ++itr)
    {
        Statistic<int> tmpResult;
        const Statistic<int>::ValuesType values = result.Values();
        for(Statistic<int>::ValuesType::const_iterator stat_itr = values.begin();
            stat_itr != values.end();
            ++stat_itr)
        {
            RestatisticResult reStatResult = (*itr).Map(*stat_itr, result.Num(*stat_itr));
            tmpResult.Add(reStatResult.key, reStatResult.value);
        }
        result = tmpResult;
    }
    return result;
}

void Restatistic::Release()
{
    for(std::vector<RestatisticMapper>::iterator itr = m_Mappers.begin();
        itr != m_Mappers.end();
        ++itr)
    {
        (*itr).Release();
    }
}
