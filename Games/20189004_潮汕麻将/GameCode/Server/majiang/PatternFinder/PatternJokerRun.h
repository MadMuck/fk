#ifndef PATTERN_JOKER_RUN_H
#define PATTERN_JOKER_RUN_H

#include "Pattern.h"
#include "PatternJokerList.h"

class PatternJokerRunImpl : public PatternImpl
{
    int m_Size;
    PatternJokerList m_Jokers;
public:
    PatternJokerRunImpl(int size, const PatternJokerList &jokers);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternJokerRun : public Pattern
{
public:
    PatternJokerRun(int size, const PatternJokerList &jokers);
};

#endif
