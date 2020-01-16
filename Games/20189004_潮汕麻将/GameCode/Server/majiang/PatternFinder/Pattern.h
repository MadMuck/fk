#ifndef PATTERN_H
#define PATTERN_H

#include "Card.h"
#include "CardStatistic.h"
#include "RefCntObj.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////
class PatternResultImpl
{
public:
    virtual ~PatternResultImpl(){}

	virtual int Type() const = 0;

    virtual void Consume(CardStatistic &stat) const = 0;

    virtual void Recover(CardStatistic &stat) const = 0;

    virtual const CardList &GetCards() const = 0;
};

//////////////////////////////////////////////////////////////////////////////
class PatternResult
{
    RefCntObj<PatternResultImpl> m_Impl;

public:
    PatternResult();

    PatternResult(PatternResultImpl *impl);

	int Type() const;

    bool IsValid() const;

    void Consume(CardStatistic &stat) const;

    void Recover(CardStatistic &stat) const;

    const CardList &GetCards() const;
};

//////////////////////////////////////////////////////////////////////////////
class PatternResultList : public std::vector<PatternResult>
{
public:

    void Add(const PatternResult &result);

    PatternResultList operator + (const PatternResultList &other) const;

    PatternResultList &operator += (const PatternResultList &other);

};


//////////////////////////////////////////////////////////////////////////////
class PatternImpl
{
public:

    virtual ~PatternImpl(){}

    virtual PatternResult Find(const CardStatistic &statistic, size_t &idx) const = 0;
};


//////////////////////////////////////////////////////////////////////////////
class PatternImplRecycler : public RefCntObjRecycler<PatternImpl>
{
};

class Pattern
{
protected:
    RefCntObj<PatternImpl, PatternImplRecycler> m_Impl;
public:

    Pattern(PatternImpl *imp);

    Pattern(const Pattern &other);

    PatternResult Find(const CardStatistic &statistic, size_t &idx) const;

};


//////////////////////////////////////////////////////////////////////////////
class PatternList : public std::vector<Pattern>
{
public:

    void Release();
};

std::ostream &operator << (std::ostream &ostrm, const PatternResult &result);

#endif
