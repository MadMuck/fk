#include "PatternNormalWin.h"
#include "PatternType.h"
#include "PatternResultCards.h"
#include "NormalWinCheckUtil.h"


class NormalWinOnFound
{
public:
	void operator() (const std::vector<PatternResult> &result){}
};

PatternNormalWinImpl::PatternNormalWinImpl(const PatternJokerList &jokerList):
	m_Jokers(jokerList)
{

}

PatternResult PatternNormalWinImpl::Find(const CardStatistic &statistic, size_t &idx) const
{
	if(idx == 0)
	{
		idx = 1;

		NormalWinCheckUtil<NormalWinOnFound> checkUtil;
		checkUtil.ChangeHandPaiData(statistic, m_Jokers.GetJokers());

		if(checkUtil.CheckPingHu(NormalWinOnFound()))
		{
			CardList cards;
			const CardStat::ValuesType &values = statistic.GetStat().Values();
			for(CardStat::ValuesType::const_iterator itr = values.begin();
				itr != values.end();
				++itr)
			{
				cards.Add(*itr, statistic.Num(*itr));
			}

			return PatternResult(new PatternResultCardsImpl(PatternType_NormalWin, cards));
		}
	}
	return PatternResult();
}
