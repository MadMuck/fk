#ifndef PATTERN_LIST_GETTER_COMMON_H
#define PATTERN_LIST_GETTER_COMMON_H

#include "PatternListGetter.h"

class PatternListGetterCommon : public PatternListGetter
{
	PatternList m_PatternList;
public:
	PatternListGetterCommon(const PatternList &patternList);
	PatternList Get(const CardStatistic &stat) const;
};

#endif