#include "PatternResultInterception.h"

PatternResultInterceptionImpl::PatternResultInterceptionImpl(int type, const Interception &interception):m_Type(type), m_Interception(interception)
{
}

int PatternResultInterceptionImpl::Type() const
{
	return m_Type;
}

void PatternResultInterceptionImpl::Consume(CardStatistic &stat) const
{
    stat.Sub(m_Interception);
}

void PatternResultInterceptionImpl::Recover(CardStatistic &stat) const
{
    stat.Add(m_Interception);
}

const CardList &PatternResultInterceptionImpl::GetCards() const
{
    return m_Interception.cards;
}
