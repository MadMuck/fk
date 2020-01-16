#ifndef PATTERN_INTC_RUN_H
#define PATTERN_INTC_RUN_H

#include "Pattern.h"

class PatternIntcRunImpl : public PatternImpl
{
    int m_Size;
public:
    PatternIntcRunImpl(int size);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternIntcRun : public Pattern
{
public:
    PatternIntcRun(int size);
};

#endif
