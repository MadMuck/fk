#ifndef PATTERN_SET_H
#define PATTERN_SET_H

#include "Pattern.h"

class PatternSetImpl : public PatternImpl
{
    int m_Size;
public:
    PatternSetImpl(int size);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternSet : public Pattern
{
public:
    PatternSet(int size);
};

#endif
