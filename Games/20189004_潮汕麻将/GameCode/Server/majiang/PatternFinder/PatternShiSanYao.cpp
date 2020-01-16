#include "PatternShiSanYao.h"
#include "PatternType.h"
#include "PatternResultCards.h"
#include "NormalWinCheckUtil.h"
#include "Card.h"

PatternShiSanYaoImpl::PatternShiSanYaoImpl(const PatternJokerList &jokerList):
m_Jokers(jokerList)
{

}

PatternResult PatternShiSanYaoImpl::Find(const CardStatistic &statistic, size_t &idx) const
{
	if(statistic.GetStat().Values().size() != 14)

	if(idx == 0)
	{
		idx = 1;

		char cards_13[] = {1,9,11,19,21,29,31,32,33,34,35,36,37};
		CardList card13List;
		CardList jokerList = m_Jokers.GetJokers();
		int jokerCount = 0;
		card13List.insert(card13List.begin(), cards_13, cards_13 + sizeof(cards_13));

		CardList cardsToCheck;
		CardList cards;
		const CardStat::ValuesType &values = statistic.GetStat().Values();
		for(CardStat::ValuesType::const_iterator itr = values.begin();
			itr != values.end();
			++itr)
		{
			if(0 == card13List.Find(*itr) && 0 == jokerList.Find(*itr))		//存在非十三幺的牌并且不是鬼牌直接返回
				return PatternResult();

			if(jokerList.empty() || 0 == jokerList.Find(*itr))		//检测的牌除去万能牌
				cardsToCheck.Add(*itr,statistic.Num(*itr));
			else
				jokerCount += statistic.Num(*itr);

			cards.Add(*itr,statistic.Num(*itr));
		}

		for(auto itr = cardsToCheck.begin(); itr != cardsToCheck.end(); ++itr)
		{
			card13List.Remove(*itr,1);
		}

		if(card13List.size() <= jokerCount)
			return PatternResult(new PatternResultCardsImpl(PatternType_ShiSanYao, cards));
	}
	return PatternResult();
}