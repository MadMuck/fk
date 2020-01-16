#ifndef INTERCEPTION_H
#define INTERCEPTION_H

#include "Card.h"
#include "CardInfo.h"
#include <vector>
#include <map>
#include <set>
#include <iostream>

struct Interception
{
    typedef char Type;
    enum InterceptionType
    {
        Invalid = 0,
        Chi = 1,
        Peng,
        MingGang,
        AnGang,
        BuGang,
//		YangMa,
		Zhan,
		Feng,
		Jian,
		Ting,
		ChiTing,
		PengTing,
		MingGangTing,
		AnGangTing,
		BuGangTing,
		ZhanTing,
		Hu,
        Max
    };

    Type type; // 拦牌类型
    CardInfo src; // 拦牌源：哪个玩家打的哪张牌
    CardList cards; // 拦牌类型相关的数据，如吃碰杠，cards存放拦牌结果; 对于听牌类的操作，存放可以听的牌

    Interception();

    Interception(const Interception &other);
};

struct InterceptionChi : public Interception
{
    void Init(Card::Type targetCard, int cardPos);

    InterceptionChi(Card::Type targetCard, int cardPos);

    InterceptionChi(PlayerPos::Type srcPlayer, Card::Type targetCard, int cardPos);
};

struct InterceptionPeng : public Interception
{
    void Init(Card::Type targetCard);

    InterceptionPeng(Card::Type targetCard);

    InterceptionPeng(PlayerPos::Type srcPlayer, Card::Type targetCard);
};

struct InterceptionGang : public Interception
{
    void Init(Interception::Type gangType, Card::Type targetCard);

    InterceptionGang(Interception::Type gangType, Card::Type targetCard);

    InterceptionGang(PlayerPos::Type srcPlayer, Interception::Type gangType, Card::Type targetCard);
};

struct InterceptionCollection : public Interception
{
	void Init(Interception::Type type, Card::Type targetCard, const CardList &cards);

	InterceptionCollection(Interception::Type type, Card::Type targetCard, const CardList &cards);

	InterceptionCollection(PlayerPos::Type srcPlayer, Interception::Type type, Card::Type targetCard, const CardList &cards);
};

struct InterceptionHu : public Interception
{
    InterceptionHu(Card::Type targetCard);

    InterceptionHu(PlayerPos::Type srcPlayer, Card::Type targetCard);
};

class InterceptionList : public std::vector<Interception>
{
public:
    int Count() const;

    InterceptionList operator + (const InterceptionList &other) const;

    InterceptionList &operator += (const InterceptionList &other);

    InterceptionList &Add(const Interception &interception);

    void Clear();

    void SetPlayerPos(PlayerPos::Type pos);

	const_iterator Find(Interception::Type type, Card::Type card = Card::Invalid) const;

	iterator Find(Interception::Type type, Card::Type card = Card::Invalid);

	const_reverse_iterator FindLatest(const std::set<Interception::Type> &type, Card::Type card = Card::Invalid) const;

	reverse_iterator FindLatest(const std::set<Interception::Type> &type, Card::Type card = Card::Invalid);
};

typedef std::map<PlayerPos::Type, InterceptionList> HuActions;

std::ostream &operator << (std::ostream &ostrm, const Interception &interception);

std::ostream &operator << (std::ostream &ostrm, const InterceptionList &interceptionList);

class ReadyHandSelection : public std::map<Card::Type, CardStat>
{
public:
	ReadyHandSelection();
	ReadyHandSelection(const InterceptionList &interceptions);
};

#endif
