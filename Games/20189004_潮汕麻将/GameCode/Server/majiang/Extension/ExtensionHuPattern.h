#ifndef EXTENSION_HU_PATTERN_H
#define EXTENSION_HU_PATTERN_H

#include "GameContext.h"
#include "HuPatternMatch.h"

class JokerListImpl : public PatternJokerListImpl
{
	int m_JokersLimit;
	CardList m_CardList;
public:
	JokerListImpl():m_JokersLimit(0){}

	const CardList &GetJokers() const
	{
		return m_CardList;
	}

	int JokersLimitCount() const
	{
		return m_JokersLimit;
	}

	void SetJokersLimitCount(int limit)
	{
		m_JokersLimit = limit;
	}

	void AddJoker(Card::Type card)
	{
		if(m_CardList.Find(card) == 0)	m_CardList.Add(card);
	}

	void Clear()
	{
		m_CardList.clear();
	}
};

class JokerList : public PatternJokerList
{
public:
	JokerList():PatternJokerList(new JokerListImpl()){}
};

class ExtensionHuPattern : public GameContextExtension
{

	PatternJokerList m_JokerList;

    HuPatternMatch m_HuPatternMatch;
public:
    ExtensionHuPattern();

    HuPatternMatch &GetHuPattern();

	HuPatternMatchResult Match(const CardStatistic &stat, bool all = false) const;

	const PatternJokerList &GetPatternJokerList() const;

	// 加入一种万能牌
	void AddJoker(Card::Type card);

	const CardList &GetJokers() const;

	int GetJokersLimitCount() const;

	void SetJokersLimitCount(int limit);

	void ClearJoker();

	void OnGameStart();
};

#endif
