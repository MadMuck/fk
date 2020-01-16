#include "Card.h"
#include <algorithm>
#include <map>
#include "Statistic.h"

namespace Card
{
    int GetNumber(Type card)
    {
        Type start = Invalid;
        Type end = Invalid;
        GetValueInterval(card, start, end);
        return card - start + 1;
    }
	//xiugai
	bool IsZi(Type card)
	{
		return card >= FengStart && card < ZiEnd; //东南西北中发白。
	}
    bool GetValueInterval(Type card, Type &start, Type &end)
    {
        start = end = Invalid;
        if(card >= WanStart && card < WanEnd) {start = WanStart; end = WanEnd;}
        if(card >= TiaoStart && card < TiaoEnd) {start = TiaoStart; end = TiaoEnd;}
        if(card >= BingStart && card < BingEnd) {start = BingStart; end = BingEnd;}
		if(card >= FengStart && card < FengEnd) {start = FengStart; end = FengEnd;}
		if(card >= ZiStart && card < ZiEnd) {start = ZiStart; end = ZiEnd;}
        return start != Invalid;
    }

	bool SpecGetValueInterval(Type card, Type &start, Type &end)
	{
		start = end = Invalid;
		if(card >= WanStart && card < WanEnd) {start = WanStart; end = WanEnd;}
		if(card >= TiaoStart && card < TiaoEnd) {start = TiaoStart; end = TiaoEnd;}
		if(card >= BingStart && card < BingEnd) {start = BingStart; end = BingEnd;}
		if(card >= FengStart && card < ZiEnd) {start = FengStart; end = ZiEnd;}
		return start != Invalid;
	}

	Type GetFollowing(Type card)
	{
		Type ret = Card::Invalid;
		Type start, end;
		if(SpecGetValueInterval(card, start, end))
		{
			ret = card + 1;
			if(ret >= end)
			{
				ret = start;
			}
		}
		return ret;
	}

	Type GetLast(Type card)
	{
		Type ret = Card::Invalid;
		Type start, end;
		if(SpecGetValueInterval(card, start, end))
		{
			ret = card - 1;
			if(ret < start)
			{
				ret = end - 1;
			}
		}
		return ret;
	}
}

CardList::CardList(){}

CardList::CardList(const std::vector<Card::Type> &cards)
{
	insert(end(), cards.begin(), cards.end());
}

int CardList::Count() const
{
    return (int)size();
}

void CardList::Add(Card::Type card, int count)
{
    for(int i = 0; i < count; ++i)
    {
        push_back(card);
    }
}

void CardList::AddContinuous(Card::Type card, int count)
{
    Card::Type start, end;
    if(Card::GetValueInterval(card, start, end))
    {
        int len = count < end - card ? count : end - card;
        for(int i = 0; i < len; ++i)
        {
            push_back(card + i);
        }
    }
}

void CardList::Remove(Card::Type card, int count)
{
    int removed = 0;
    int totalCnt = (int)size();
    for(int i = totalCnt - 1; i >= 0; --i)
    {
        if(at(i) == card)
        {
            ++removed;
            erase(begin() + i);

            if(removed == count)
            {
                break;
            }
        }
    }
}

int CardList::Find(Card::Type card) const
{
    int res = 0;
    int totalCnt = (int)size();
    for(int i = 0; i < totalCnt; ++i)
    {
        if(at(i) == card)
        {
            ++res;
        }
    }
    return res;
}

bool CardList::Find(const CardList &cardList) const
{
    std::map<Card::Type, int> stat1, stat2;
    for(CardList::const_iterator itr = cardList.begin();
        itr != cardList.end();
        ++itr)
    {
        ++stat1[*itr];
    }

    for(const_iterator itr = begin();
        itr != end();
        ++itr)
    {
        ++stat2[*itr];
    }

    bool res = true;
    for(std::map<Card::Type, int>::iterator itr = stat1.begin();
        itr != stat1.end();
        ++itr)
    {
        if(itr->second > stat2[itr->first])
        {
            res = false;
            break;
        }
    }
    return res;
}

void CardList::Clear()
{
    clear();
}

void CardList::Sort()
{
    std::sort(begin(), end());
}

CardList CardList::operator-(const CardList &other) const
{
    CardList res = *this;
    return res -= other;
}

CardList &CardList::operator-=(const CardList &other)
{
    for(CardList::const_iterator itr = other.begin();
        itr != other.end();
        ++itr)
    {
        CardList::iterator find_itr = std::find(begin(), end(), *itr);
        if(find_itr != end())
        {
            erase(find_itr);
        }
    }
    return *this;
}

CardList CardList::operator+(const CardList &other) const
{
    CardList res = *this;
    return res += other;
}

CardList &CardList::operator+=(const CardList &other)
{
    insert(end(), other.begin(), other.end());
    return *this;
}

bool CardList::operator == (const CardList &other) const
{
    Statistic<Card::Type> stat1(*this);
    Statistic<Card::Type> stat2(other);
    return stat1 == stat2;
}

bool CardList::operator != (const CardList &other) const
{
    return !(*this == other);
}

std::ostream &operator << (std::ostream &ostrm, const CardList &cardList)
{
    for(CardList::const_iterator itr = cardList.begin();
        itr != cardList.end();
        ++itr)
    {
        ostrm<<(int)(*itr);
        if(itr + 1 != cardList.end())
        {
            ostrm<<",";
        }
    }
    return ostrm;
}
