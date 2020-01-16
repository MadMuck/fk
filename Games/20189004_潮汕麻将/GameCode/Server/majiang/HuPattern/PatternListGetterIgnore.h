#ifndef PATTERN_LIST_GETTER_IGNORE_H
#define PATTERN_LIST_GETTER_IGNORE_H

#include "PatternListGetter.h"

template<typename T>
class PatternListGetterIgnore : public PatternListGetter
{
	PatternList m_PatternList;
	T m_Ignore;
	int m_IgnoreLimit;
public:
	PatternListGetterIgnore(const PatternList &patternList, const T &predicate, int ignoreLimit):
	  m_PatternList(patternList), m_Ignore(predicate), m_IgnoreLimit(ignoreLimit)
	{

	}

	PatternList Get(const CardStatistic &stat) const
	{
		PatternList patternList = m_PatternList;
		int removeCnt = 0;
		const InterceptionList &intcs = stat.GetInterceptions();
		for(InterceptionList::const_iterator itr = intcs.begin();
			itr != intcs.end();
			++itr)
		{
			if(removeCnt < m_IgnoreLimit && m_Ignore(*itr))
			{
				++removeCnt;
			}
		}
		if(removeCnt > 0)
		{
			for(int i = 0; i < removeCnt; ++i)
			{
				patternList.pop_back();
			}
		}
		return patternList;
	}
};

#endif