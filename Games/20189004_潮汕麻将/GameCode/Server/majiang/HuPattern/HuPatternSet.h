#ifndef HU_PATTERN_SET_H
#define HU_PATTERN_SET_H

#include <vector>
#include "RefCntObj.h"
#include "HuType.h"
#include "Pattern.h"
#include "PatternJokerList.h"
#include "PatternListGetter.h"

struct HuTypePatternPair
{
	HuTypePatternPair(PatternListGetter *getter);
    int dependency;
    int huType;
    RefCntObj<PatternListGetter> patternGetter;
};

class HuPatternSetImpl
{
    std::vector<HuTypePatternPair> m_List;
public:
    void Load(const PatternJokerList &jokerList);

    int Count() const;

    const HuTypePatternPair &Get(int idx) const;

	const HuTypePatternPair *GetPatternList(int huType) const;

	std::vector<HuTypePatternPair> GetAllPatternList() const;
};

class HuPatternSet : public RefCntObj<HuPatternSetImpl>
{
public:
    HuPatternSet();
};

#endif
