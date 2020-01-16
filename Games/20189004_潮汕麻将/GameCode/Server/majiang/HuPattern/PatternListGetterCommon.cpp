#include "PatternListGetterCommon.h"

PatternListGetterCommon::PatternListGetterCommon(const PatternList &patternList):m_PatternList(patternList){}

PatternList PatternListGetterCommon::Get(const CardStatistic &stat) const
{
	return m_PatternList;
}