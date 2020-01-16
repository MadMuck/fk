#include "HuPatternSet.h"
#include "PatternSet.h"
#include "PatternIntcSet.h"
#include "PatternRun.h"
#include "PatternIntcRun.h"
#include "PatternConst.h"
#include "PatternCombination.h"
#include "PatternJokerSet.h"
#include "PatternJokerRun.h"
#include "PatternJokerConst.h"
#include "PatternPairs.h"
#include "PatternNormalWin.h"
#include "PatternShiSanYao.h"
#include "Card.h"

#include "PatternListGetterCommon.h"
#include "PatternListGetterIgnore.h"


class IgnoreInterception
{
public:
	bool operator() (const Interception &intc) const
	{
		return intc.type != Interception::Hu && intc.cards.Count() >= 3;
	}
};

class IgnorePeng
{
public:
	bool operator() (const Interception &intc) const
	{
		return intc.type == Interception::Peng ||
			intc.type == Interception::MingGang || 
			intc.type == Interception::AnGang ||
			intc.type == Interception::BuGang ;
//			intc.type == Interception::YangMa;
	}
};

class IgnoreZhan
{
public:
	bool operator() (const Interception &intc) const
	{
		return intc.type == Interception::Zhan || intc.type == Interception::ZhanTing;
	}
};

HuTypePatternPair::HuTypePatternPair(PatternListGetter *getter):huType(HuType_Invalid), dependency(-1), patternGetter(getter)
{}

void HuPatternSetImpl::Load(const PatternJokerList &jokerList)
{
	{
		// 普通牌型
		PatternList patternList;
		patternList.push_back(PatternNormalWin(jokerList));
		m_List.push_back(HuTypePatternPair(new PatternListGetterCommon(patternList)));
		HuTypePatternPair &data = *m_List.rbegin();
		data.dependency = -1;
		data.huType = HuType_PingHu;
	}

// 	{
// 		// 七对
// 		PatternList patternList;
// 		patternList.push_back(PatternPairs(7, jokerList));
// 		m_List.push_back(HuTypePatternPair(new PatternListGetterCommon(patternList)));
// 		HuTypePatternPair &data = *m_List.rbegin();
// 		data.dependency = -1;
// 		data.huType = HuType_QiDui;
// 	}

	{
		// 十三幺
		PatternList patternList;
		patternList.push_back(PatternShiSanYao(jokerList));
		m_List.push_back(HuTypePatternPair(new PatternListGetterCommon(patternList)));
		HuTypePatternPair &data = *m_List.rbegin();
		data.dependency = -1;
		data.huType = HuType_ShiSanYao;
	}
}

int HuPatternSetImpl::Count() const
{
    return (int)m_List.size();
}

const HuTypePatternPair &HuPatternSetImpl::Get(int idx) const
{
    return m_List.at(idx);
}

 const HuTypePatternPair *HuPatternSetImpl::GetPatternList(int huType) const
{
	const HuTypePatternPair *ret = NULL;
	for(std::vector<HuTypePatternPair>::const_iterator itr = m_List.begin(); itr != m_List.end(); ++itr)
	{
		if(huType == itr->huType)
		{
			ret = &(*itr);
			break;
		}
	}
	return ret;
}

std::vector<HuTypePatternPair> HuPatternSetImpl::GetAllPatternList() const
{
	std::vector<HuTypePatternPair> ret;
	for(std::vector<HuTypePatternPair>::const_iterator itr = m_List.begin(); itr != m_List.end(); ++itr)
	{
		ret.push_back(*itr);
	}
	return ret;
}

HuPatternSet::HuPatternSet():RefCntObj<HuPatternSetImpl>(new HuPatternSetImpl())
{}
