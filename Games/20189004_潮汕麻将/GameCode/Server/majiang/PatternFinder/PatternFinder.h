#ifndef PATTERN_FINDER_H
#define PATTERN_FINDER_H

#include "Statistic.h"
#include "Pattern.h"
#include <map>
#include <iostream>

extern int PatternFinderMatchCnt;
extern int PatternResultCnt;

typedef std::vector<PatternResult> MeldList;

class PatternFinder
{
public:

    static MeldList Match(const CardStatistic &stat, const PatternList &patternList);

    static std::vector<MeldList> MatchAll(const CardStatistic &stat, const PatternList &patternList);
};

std::ostream &operator << (std::ostream &ostrm, const MeldList &melds);

#endif
