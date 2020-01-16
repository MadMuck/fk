#ifndef HU_PATTERN_MATCH_H
#define HU_PATTERN_MATCH_H

#include <vector>
#include <map>
#include <set>

#include "PatternFinder.h"
#include "HuPatternSet.h"
#include "CardStatistic.h"

class HuPatternMatchResult : public std::map<int, MeldList>
{
public:
    bool Contains(int val) const;
};

class HuPatternMatch
{

    HuPatternSet m_PatternSet;

	std::set<int> m_EnabledTypes;

public:

    void SetPatterns(const HuPatternSet &patterns);

    HuPatternMatchResult Match(const CardStatistic &stat, bool all = false) const;

	const HuTypePatternPair *GetPatternList(int huType) const;

	std::vector<HuTypePatternPair> GetAllPatternList() const;

	void EnableWinType(int type);

	void DisableWinType(int type);

	void EnableAll();

	void DisableAll();

	std::set<int> GetEnabledWinType() const;

	void SetEnabledWinType(const std::set<int> &enabledList);
};

#endif
