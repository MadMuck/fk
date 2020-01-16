#ifndef CARD_STATISTIC_H
#define CARD_STATISTIC_H

#include "Statistic.h"
#include "Interception.h"
#include "Card.h"

class CardStatistic
{
    CardStat m_Stat;
    InterceptionList m_Interceptions;
public:
    typedef CardStat::ValuesType ValuesType;

    CardStatistic();

    CardStatistic(const CardStatistic &other);

    CardStatistic(const CardList &cardList);

    CardStatistic(const CardList &cardList, const InterceptionList &interceptions);

    const ValuesType &Values() const;

    int Num(Card::Type val) const;

    bool Sub(Card::Type val, int count);

    void Sub(const CardList &valList);

    void Add(Card::Type val, int count);

    void Add(const CardList &valList);

    const InterceptionList &GetInterceptions() const;

    void Add(const Interception &intc);

    void Sub(const Interception &intc);

	const CardStat &GetStat() const;
};


#endif
