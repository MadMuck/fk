#ifndef PATTERN_NORMAL_WIN_H
#define PATTERN_NORMAL_WIN_H

#include "Pattern.h"
#include "PatternJokerList.h"

class PatternNormalWinImpl : public PatternImpl
{
	PatternJokerList m_Jokers;

public:

	PatternNormalWinImpl(const PatternJokerList &jokerList);

	PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternNormalWin : public Pattern
{
public:
	PatternNormalWin(const PatternJokerList &jokerList):Pattern(new PatternNormalWinImpl(jokerList)){}
};

#endif