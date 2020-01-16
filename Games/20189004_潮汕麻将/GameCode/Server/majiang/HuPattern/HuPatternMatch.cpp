#include "HuPatternMatch.h"
#include "PatternFinder.h"
#include "Logger.h"

bool HuPatternMatchResult::Contains(int val) const
{
    return find(val) != end();
}


void HuPatternMatch::SetPatterns(const HuPatternSet &patterns)
{
    m_PatternSet = patterns;
	EnableAll();
}

HuPatternMatchResult HuPatternMatch::Match(const CardStatistic &stat, bool all) const
{
    HuPatternMatchResult result;

    int cnt = m_PatternSet->Count();

    for(int i = 0; i < cnt; ++i)
    {
        const HuTypePatternPair &data = m_PatternSet->Get(i);
		if(m_EnabledTypes.find(data.huType) == m_EnabledTypes.end())
		{
			continue;
		}

        if(data.dependency < 0 || result.Contains(data.dependency))
        {
            // 无依赖 或 依赖项已经满足
			
            MeldList cardsMatched = PatternFinder::Match(stat, data.patternGetter->Get(stat));
            if(cardsMatched.size() > 0)
            {
                // 匹配成功
                result.insert(std::make_pair(data.huType, cardsMatched));
				if(!all)
				{
					break;
				}
            }
        }
    }

    return result;
}

const HuTypePatternPair *HuPatternMatch::GetPatternList(int huType) const
{
	return m_PatternSet->GetPatternList(huType);
}

std::vector<HuTypePatternPair> HuPatternMatch::GetAllPatternList() const
{
	return m_PatternSet->GetAllPatternList();
}

void HuPatternMatch::EnableWinType(int type)
{
	m_EnabledTypes.insert(type);
}

void HuPatternMatch::DisableWinType(int type)
{
	m_EnabledTypes.erase(type);
}

void HuPatternMatch::EnableAll()
{
	std::vector<HuTypePatternPair> allType = GetAllPatternList();
	for(std::vector<HuTypePatternPair>::iterator itr = allType.begin();
		itr != allType.end();
		++itr)
	{
		EnableWinType(itr->huType);
	}
}

void HuPatternMatch::DisableAll()
{
	m_EnabledTypes.clear();
}

std::set<int> HuPatternMatch::GetEnabledWinType() const
{
	return m_EnabledTypes;
}

void HuPatternMatch::SetEnabledWinType(const std::set<int> &enabledList)
{
	m_EnabledTypes = enabledList;
}
