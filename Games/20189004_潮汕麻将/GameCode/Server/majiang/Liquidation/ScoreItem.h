#ifndef SCORE_ITEM_H
#define SCORE_ITEM_H

#include "PlayerPos.h"
#include "RefCntObj.h"
#include <map>
#include <vector>


class ScoreItemImpl
{
public:
	virtual ~ScoreItemImpl(){}
	virtual int Type() const = 0;
	virtual int Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const = 0;
};

class ScoreItem
{
protected:
	RefCntObj<ScoreItemImpl> m_Impl;

public:
	ScoreItem(ScoreItemImpl *impl):m_Impl(impl){}
    virtual ~ScoreItem(){}
	int Type() const { return m_Impl->Type();}
    int Score(PlayerPos::Type dest, PlayerPos::Type src, PlayerPos::Type bankerPos, PlayerPos::Type &realSrc) const
	{
		return m_Impl->Score(dest, src, bankerPos, realSrc);
	}
};

typedef std::vector<ScoreItem> ScoreItemList;

#endif