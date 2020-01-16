#include "PatternPairs.h"
#include "PatternResultCards.h"
#include "PatternType.h"
#include "Logger.h"
#include "UtilTemplate.h"

PatternPairsImpl::PatternPairsImpl(int size, const PatternJokerList &jokers):m_Size(size), m_Jokers(jokers){}

PatternResult PatternPairsImpl::Find(const CardStatistic &stat, size_t &idx) const
{
	if(idx > 0)
	{
		idx = 1;
		return PatternResult();
	}

	idx = 1;

	CardStat tmpStat = stat.GetStat();
	CardList cards;
	int absencePair = m_Size;

	const CardList &jokers = m_Jokers.GetJokers();
	CardList jokerCards;
	for(CardList::const_iterator itr = jokers.begin(); itr != jokers.end(); ++itr)
	{
		int num = tmpStat.Num(*itr);
		jokerCards.Add(*itr, num);
		tmpStat.Sub(*itr, num);
	}

	CardStat::ValuesType pairValue = tmpStat.FindValuesWithNum(std::greater_equal<int>(), 2);
	for(CardStat::ValuesType::iterator itr = pairValue.begin(); itr != pairValue.end(); ++itr)
	{
		int num = tmpStat.Num(*itr);
		int pairCnt = (num / 2);
		if(absencePair < pairCnt)
		{
			pairCnt = absencePair;
		}

		tmpStat.Sub(*itr, pairCnt * 2);
		absencePair -= pairCnt;

		cards.Add(*itr, 2 * pairCnt);

		if(absencePair <= 0) break;
	}

	int patternType = PatternType_Pairs;

	if(absencePair > 0)
	{
		// 对子数不够，用万能牌补
		
		CardStat::ValuesType normalCards = tmpStat.Values();

		int minCompensationPairsCnt = (int)jokerCards.size();
		if(jokerCards.size() >= normalCards.size())
		{
			minCompensationPairsCnt = (int)normalCards.size();
		}

		if(minCompensationPairsCnt > absencePair)
		{
			minCompensationPairsCnt = absencePair;
		}

		if(minCompensationPairsCnt > 0)
		{
			patternType = PatternType_JokerPairs;
		}

		CardStat jokerStat(jokerCards);
		std::vector<Util::SortData<int, Card::Type> > sortedJokersNum;
		const CardStat::ValuesType &jokersCardValue = jokerStat.Values();
		for(CardStat::ValuesType::const_iterator jokerValItr = jokersCardValue.begin();
			jokerValItr != jokersCardValue.end();
			++jokerValItr)
		{
			sortedJokersNum.push_back(Util::SortData<int, Card::Type>(jokerStat.Num(*jokerValItr), *jokerValItr));
		}
		std::sort(sortedJokersNum.begin(), sortedJokersNum.end(), std::greater<Util::SortData<int, Card::Type> >());

		// 由万能牌和非万能牌组成的对子
		for(int i = 0; i < minCompensationPairsCnt; ++i)
		{
			// 先加入非万能牌
			cards.Add(normalCards[i]);
			// 用数量最少的万能牌填充
			if(jokerStat.Num(sortedJokersNum.rbegin()->value) <= 0)
			{
				sortedJokersNum.pop_back();
			}
			Card::Type jokerCard = sortedJokersNum.rbegin()->value;
			jokerStat.Sub(jokerCard, 1);
			cards.Add(jokerCard);
		}
		absencePair -= minCompensationPairsCnt;

		// 仅由万能牌组成的对子
		if(absencePair > 0 && jokerStat.Total() > 1)
		{
			int pairsFullOfJoker = (int)jokerStat.Total() / 2;
			if(absencePair <= pairsFullOfJoker)
			{
				for(int i = 0; i < absencePair * 2; ++i)
				{
					if(jokerStat.Num(sortedJokersNum.rbegin()->value) <= 0)
					{
						sortedJokersNum.pop_back();
					}
					Card::Type jokerCard = sortedJokersNum.rbegin()->value;
					jokerStat.Sub(jokerCard, 1);
					cards.Add(jokerCard);

					if(i % 2 == 1 &&
						jokerCard != *cards.rbegin())
					{
						patternType = PatternType_JokerPairs;
					}
				}
				absencePair = 0;
			}
		}
	}

	if(absencePair <= 0)
	{
		return PatternResult(new PatternResultCardsImpl(patternType, cards));
	}
	

	return PatternResult();
}

PatternPairs::PatternPairs(int size,  const PatternJokerList &jokers):Pattern(new PatternPairsImpl(size, jokers)){}
