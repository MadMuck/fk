#ifndef PATTERN_JOKER_SET_H
#define PATTERN_JOKER_SET_H

#include "Pattern.h"
#include "PatternJokerList.h"

class PatternJokerSetImpl : public PatternImpl
{
    int m_Size;
    PatternJokerList m_Jokers;
public:
    PatternJokerSetImpl(int size, const PatternJokerList &jokers);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternJokerSet : public Pattern
{
public:
    PatternJokerSet(int size, const PatternJokerList &jokers);
};

#endif
