#include "CardStatistic.h"

CardStatistic::CardStatistic()
{}

CardStatistic::CardStatistic(const CardStatistic &other)
{
    m_Stat = other.m_Stat;
    m_Interceptions = other.m_Interceptions;
}

CardStatistic::CardStatistic(const CardList &cardList):m_Stat(cardList)
{}

CardStatistic::CardStatistic(const CardList &cardList, const InterceptionList &interceptions):
    m_Stat(cardList), m_Interceptions(interceptions)
{}

const CardStatistic::ValuesType &CardStatistic::Values() const
{
    return m_Stat.Values();
}

int CardStatistic::Num(Card::Type val) const
{
    return m_Stat.Num(val);
}

bool CardStatistic::Sub(Card::Type val, int count)
{
    return m_Stat.Sub(val, count);
}

void CardStatistic::Sub(const CardList &valList)
{
    m_Stat.Sub(valList);
}

void CardStatistic::Add(Card::Type val, int count)
{
    m_Stat.Add(val, count);
}

void CardStatistic::Add(const CardList &valList)
{
    m_Stat.Add(valList);
}

const InterceptionList &CardStatistic::GetInterceptions() const
{
    return m_Interceptions;
}

void CardStatistic::Add(const Interception &intc)
{
    m_Interceptions.Add(intc);
}

void CardStatistic::Sub(const Interception &intc)
{
    for(InterceptionList::iterator itr = m_Interceptions.begin();
        itr != m_Interceptions.end();
        ++itr)
    {
        if((*itr).type == intc.type &&
            (*itr).cards == intc.cards)
        {
            m_Interceptions.erase(itr);
            break;
        }
    }
}

const CardStat &CardStatistic::GetStat() const
{
	return m_Stat;
}
