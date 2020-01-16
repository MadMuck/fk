#ifndef PATTERN_CONST_H
#define PATTERN_CONST_H

#include "Pattern.h"
#include "Card.h"

class PatternConstImpl : public PatternImpl
{
    Statistic<Card::Type> m_Stat;
public:
    PatternConstImpl(const CardList &cards);
    PatternConstImpl();
    void Add(Card::Type card, int cnt);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternConst : public Pattern
{
public:
    PatternConst();
	PatternConst(const CardList &cards);
    PatternConst(Card::Type card, int cnt);
    PatternConst &Add(Card::Type card, int cnt);
};

#endif
