#include "PatternResultCards.h"

PatternResultCardsImpl::PatternResultCardsImpl(int type, const CardList &cards):m_Type(type), m_Cards(cards)
{
	for(int i = 0; i < m_Cards.size(); ++i)
	{
		m_IsJoker.push_back(0);
	}
}

PatternResultCardsImpl::PatternResultCardsImpl(int type, const CardList &cards, const CardList &isJoker):m_Type(type), m_Cards(cards), m_IsJoker(isJoker)
{

}

int PatternResultCardsImpl::Type() const
{
	return m_Type;
}

void PatternResultCardsImpl::Consume(CardStatistic &stat) const
{
	stat.Sub(m_Cards);
}

void PatternResultCardsImpl::Recover(CardStatistic &stat) const
{
	stat.Add(m_Cards);
}

const CardList &PatternResultCardsImpl::GetCards() const
{
	return m_Cards;
}

const CardList &PatternResultCardsImpl::GetIsJoker() const
{
	return m_IsJoker;
}

