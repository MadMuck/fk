#ifndef PATTERN_RESULT_INTERCEPTION_H
#define PATTERN_RESULT_INTERCEPTION_H

#include "Pattern.h"
#include "Interception.h"

class PatternResultInterceptionImpl : public PatternResultImpl
{
	int m_Type;
    Interception m_Interception;
public:
    PatternResultInterceptionImpl(int type, const Interception &interception);

	int Type() const;

    void Consume(CardStatistic &stat) const;

    void Recover(CardStatistic &stat) const;

    const CardList &GetCards() const;
};

#endif
