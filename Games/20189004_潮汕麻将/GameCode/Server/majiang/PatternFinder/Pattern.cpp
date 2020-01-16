#include "Pattern.h"
#include <cstdlib>

PatternResult::PatternResult(PatternResultImpl *impl):m_Impl(impl){}

PatternResult::PatternResult():m_Impl(NULL){}

bool PatternResult::IsValid() const
{
    return m_Impl.GetObj() != NULL;
}

int PatternResult::Type() const
{
	return m_Impl->Type();
}

void PatternResult::Consume(CardStatistic &stat) const
{
    m_Impl->Consume(stat);
}

void PatternResult::Recover(CardStatistic &stat) const
{
    m_Impl->Recover(stat);
}

const CardList &PatternResult::GetCards() const
{
    return m_Impl->GetCards();
}









void PatternResultList::Add(const PatternResult &result)
{
    push_back(result);
}

PatternResultList PatternResultList::operator + (const PatternResultList &other) const
{
    PatternResultList ret = *this;
    ret += other;
    return ret;
}

PatternResultList &PatternResultList::operator += (const PatternResultList &other)
{
    insert(end(), other.begin(), other.end());
    return *this;
}









Pattern::Pattern(PatternImpl *impl):m_Impl(impl){}

Pattern::Pattern(const Pattern &other):m_Impl(other.m_Impl)
{
}

PatternResult Pattern::Find(const CardStatistic &statistic, size_t &idx) const
{
    return m_Impl->Find(statistic, idx);
}








void PatternList::Release()
{
    clear();
}

std::ostream &operator << (std::ostream &ostrm, const PatternResult &result)
{
    return ostrm<<result.GetCards();
}
