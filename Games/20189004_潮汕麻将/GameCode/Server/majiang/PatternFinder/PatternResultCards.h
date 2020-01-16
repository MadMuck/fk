#ifndef PATTERN_RESULT_CARDS_H
#define PATTERN_RESULT_CARDS_H

#include "Pattern.h"

class PatternResultCardsImpl : public PatternResultImpl
{
	int m_Type;
	CardList m_Cards;
	CardList m_IsJoker;
public:
	PatternResultCardsImpl(int type, const CardList &cards);

	PatternResultCardsImpl(int type, const CardList &cards, const CardList &isJoker);

	int Type() const;

	void Consume(CardStatistic &stat) const;

	void Recover(CardStatistic &stat) const;

	const CardList &GetCards() const;

	const CardList &GetIsJoker() const;
};

#endif
