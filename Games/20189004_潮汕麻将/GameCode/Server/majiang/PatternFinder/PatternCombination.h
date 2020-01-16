#ifndef PATTERN_COMBINATION_H
#define PATTERN_COMBINATION_H

#include "Pattern.h"

class PatternCombinationImpl : public PatternImpl
{
    PatternList m_PatternList;
public:

    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;

    PatternCombinationImpl *Add(const Pattern &pattern);
};

class PatternCombination : public Pattern
{
public:
    PatternCombination();

    PatternCombination(const PatternCombination &other);

    PatternCombination &Add(const Pattern &pattern);
};

#endif
