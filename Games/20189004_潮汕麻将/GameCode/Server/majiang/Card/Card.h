#ifndef CARD_H
#define CARD_H

#include <vector>
#include <iostream>
#include "Statistic.h"

namespace Card
{
    enum CardValue
    {
        Invalid = 255,
        WanStart = 1,
        Wan1 = 1,
        Wan2,
        Wan3,
        Wan4,
        Wan5,
        Wan6,
        Wan7,
        Wan8,
        Wan9,
        WanEnd,
        TiaoStart = 11,
        Tiao1 = 11,
        Tiao2,
        Tiao3,
        Tiao4,
        Tiao5,
        Tiao6,
        Tiao7,
        Tiao8,
        Tiao9,
        TiaoEnd,
        BingStart = 21,
        Bing1 = 21,
        Bing2,
        Bing3,
        Bing4,
        Bing5,
        Bing6,
        Bing7,
        Bing8,
        Bing9,
        BingEnd,
		FengStart = 31,
		FengDong = 31,
		FengNan,
		FengXi,
		FengBei,
		FengEnd,
		ZiStart = 35,
		ZiZhong = 35,
		ZiFa,
		ZiBai,
		ZiEnd
    };

    typedef unsigned char Type;
	bool IsZi(Type card);
    int GetNumber(Type card);
    bool GetValueInterval(Type card, Type &start, Type &end);
	bool SpecGetValueInterval(Type card, Type &start, Type &end);
	Type GetFollowing(Type card);
	Type GetLast(Type card);
}

class CardList : public std::vector<Card::Type>
{
public:
	CardList();

	CardList(const std::vector<Card::Type> &cards);

    int Count() const;

    void Add(Card::Type card, int count = 1);

    void AddContinuous(Card::Type card, int count);

    void Remove(Card::Type card, int count);

    int Find(Card::Type card) const;

    bool Find(const CardList &cardList) const;

    void Clear();

    void Sort();

    CardList operator-(const CardList &other) const;

    CardList &operator-=(const CardList &other);

    CardList operator+(const CardList &other) const;

    CardList &operator+=(const CardList &other);

    bool operator == (const CardList &other) const;

    bool operator != (const CardList &other) const;
};

typedef Statistic<Card::Type> CardStat;

std::ostream &operator << (std::ostream &ostrm, const CardList &cardList);

#endif
