#ifndef PATTERN_JOKER_LIST_H
#define PATTERN_JOKER_LIST_H

#include "Card.h"
#include "RefCntObj.h"

class PatternJokerListImpl
{
public:
    virtual ~PatternJokerListImpl(){}
    virtual const CardList &GetJokers() const = 0;
	virtual void AddJoker(Card::Type card){}
	virtual void Clear(){}
    virtual int JokersLimitCount() const = 0;
	virtual void SetJokersLimitCount(int limit) = 0;
};

class PatternJokerList
{
protected:
    RefCntObj<PatternJokerListImpl> m_Impl;
public:
    PatternJokerList(PatternJokerListImpl *impl);
    const CardList &GetJokers() const;
	void AddJoker(Card::Type card);
	void Clear();
    int JokersLimitCount() const;
	void SetJokersLimitCount(int limit);
};

#endif
