#ifndef PATTERN_INTC_SET_H
#define PATTERN_INTC_SET_H

#include "Pattern.h"

class PatternIntcSetImpl : public PatternImpl
{
    int m_Size;
public:
    PatternIntcSetImpl(int size);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternIntcSet : public Pattern
{
public:
    PatternIntcSet(int size);
};

#endif
