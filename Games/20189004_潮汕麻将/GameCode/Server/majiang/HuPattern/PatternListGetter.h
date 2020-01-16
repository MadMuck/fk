#ifndef PATTERN_LIST_GETTER_H
#define PATTERN_LIST_GETTER_H

#include "CardStatistic.h"
#include "Pattern.h"

class PatternListGetter
{
public:
	virtual ~PatternListGetter(){}
	virtual PatternList Get(const CardStatistic &stat) const = 0;
};

#endif