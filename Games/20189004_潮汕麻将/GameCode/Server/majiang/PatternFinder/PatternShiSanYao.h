#ifndef PATTERN_SHI_SAN_YAO_H
#define PATTERN_SHI_SAN_YAO_H

#include "Pattern.h"
#include "PatternJokerList.h"

class PatternShiSanYaoImpl : public PatternImpl
{
	PatternJokerList m_Jokers;

public:

	PatternShiSanYaoImpl(const PatternJokerList &jokerList);

	PatternResult Find(const CardStatistic &statistic, size_t &idx) const;
};

class PatternShiSanYao : public Pattern
{
public:
	PatternShiSanYao(const PatternJokerList &jokerList):Pattern(new PatternShiSanYaoImpl(jokerList)){}
};

#endif
