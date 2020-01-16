#ifndef PATTERN_PAIRS_H
#define PATTERN_PAIRS_H

#include "Pattern.h"
#include "PatternJokerList.h"

class PatternPairsImpl : public PatternImpl
{
	int m_Size;
	PatternJokerList m_Jokers;
public:
	PatternPairsImpl(int size, const PatternJokerList &jokers);
	PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternPairs : public Pattern
{
public:
	PatternPairs(int size, const PatternJokerList &jokers);
};

#endif
