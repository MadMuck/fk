#ifndef PATTERN_JOKER_CONST_H
#define PATTERN_JOKER_CONST_H

#include "Pattern.h"
#include "PatternJokerList.h"
#include "Card.h"

class PatternJokerConstImpl : public PatternImpl
{
    Statistic<Card::Type> m_Stat;
    PatternJokerList m_Jokers;
public:
    PatternJokerConstImpl(const PatternJokerList &jokers, const CardList &cards);
    PatternJokerConstImpl(const PatternJokerList &jokers);
    void Add(Card::Type card, int cnt);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternJokerConst : public Pattern
{
public:
    PatternJokerConst(const PatternJokerList &jokers);
    PatternJokerConst(const PatternJokerList &jokers, Card::Type card, int cnt);
    PatternJokerConst &Add(Card::Type card, int cnt);
};

#endif
