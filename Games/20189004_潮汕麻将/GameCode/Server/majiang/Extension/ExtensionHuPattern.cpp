#include "ExtensionHuPattern.h"
#include "PatternJokerList.h"
#include "ExtensionCardBlocks.h"

ExtensionHuPattern::ExtensionHuPattern():m_JokerList(new JokerListImpl())
{
    HuPatternSet huPatternSet;
    huPatternSet->Load(m_JokerList);
    m_HuPatternMatch.SetPatterns(huPatternSet);
}

HuPatternMatch &ExtensionHuPattern::GetHuPattern()
{
    return m_HuPatternMatch;
}

HuPatternMatchResult ExtensionHuPattern::Match(const CardStatistic &stat, bool all) const
{
	return m_HuPatternMatch.Match(stat, all);
}

const PatternJokerList &ExtensionHuPattern::GetPatternJokerList() const
{
	return m_JokerList;
}

void ExtensionHuPattern::AddJoker(Card::Type card)
{
	m_JokerList.AddJoker(card);
}

const CardList &ExtensionHuPattern::GetJokers() const
{
	return m_JokerList.GetJokers();
}

int ExtensionHuPattern::GetJokersLimitCount() const
{
	return m_JokerList.JokersLimitCount();
}

void ExtensionHuPattern::SetJokersLimitCount(int limit)
{
	m_JokerList.SetJokersLimitCount(limit);
}

void ExtensionHuPattern::ClearJoker()
{
	m_JokerList.Clear();
}

void ExtensionHuPattern::OnGameStart()
{
	ClearJoker();
}
