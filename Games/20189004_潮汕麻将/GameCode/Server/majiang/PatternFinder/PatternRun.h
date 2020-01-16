#ifndef PATTERN_RUN_H
#define PATTERN_RUN_H

#include "Pattern.h"

class PatternRunImpl : public PatternImpl
{
    int m_Size;
public:
    PatternRunImpl(int size);
    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternRun : public Pattern
{
public:
    PatternRun(int size);
};

#endif
