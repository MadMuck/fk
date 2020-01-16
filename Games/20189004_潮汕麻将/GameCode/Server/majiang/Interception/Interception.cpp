#include "Interception.h"

Interception::Interception():type(Interception::Invalid)
{}

Interception::Interception(const Interception &other)
{
    type = other.type;
    src = other.src;
    cards = other.cards;
}


void InterceptionChi::Init(Card::Type targetCard, int cardPos)
{
    type = Interception::Chi;
    src.card = targetCard;
    for(int i = 0; i < 3; ++i)
    {
        cards.Add(i - cardPos + targetCard);
    }
}

InterceptionChi::InterceptionChi(Card::Type targetCard, int cardPos):Interception()
{
    Init(targetCard, cardPos);
}

InterceptionChi::InterceptionChi(PlayerPos::Type srcPlayer, Card::Type targetCard, int cardPos):Interception()
{
    Init(targetCard, cardPos);
    src.playerPos = srcPlayer;
}



void InterceptionPeng::Init(Card::Type targetCard)
{
    type = Interception::Peng;
    src.card = targetCard;
    for(int i = 0; i < 3; ++i)
    {
        cards.Add(targetCard);
    }
}

InterceptionPeng::InterceptionPeng(Card::Type targetCard):Interception()
{
    Init(targetCard);
}

InterceptionPeng::InterceptionPeng(PlayerPos::Type srcPlayer, Card::Type targetCard):Interception()
{
    Init(targetCard);
    src.playerPos = srcPlayer;
}



void InterceptionGang::Init(Interception::Type gangType, Card::Type targetCard)
{
    type = gangType;
    src.card = targetCard;
    for(int i = 0; i < 4; ++i)
    {
        cards.Add(targetCard);
    }
}

InterceptionGang::InterceptionGang(Interception::Type gangType, Card::Type targetCard):Interception()
{
    Init(gangType, targetCard);
}

InterceptionGang::InterceptionGang(PlayerPos::Type srcPlayer,
    Interception::Type gangType,
    Card::Type targetCard):Interception()
{
    Init(gangType, targetCard);
    src.playerPos = srcPlayer;
}


void InterceptionCollection::Init(Interception::Type t, Card::Type targetCard, const CardList &cardList)
{
	type = t;
	cards = cardList;
	src.card = targetCard;
}

InterceptionCollection::InterceptionCollection(Interception::Type t, Card::Type targetCard, const CardList &cardList)
{
	Init(t, targetCard, cardList);
}

InterceptionCollection::InterceptionCollection(PlayerPos::Type srcPlayer, Interception::Type t, Card::Type targetCard, const CardList &cardList)
{
	Init(t, targetCard, cardList);
	src.playerPos = srcPlayer;
}


InterceptionHu::InterceptionHu(Card::Type targetCard):Interception()
{
    type = Interception::Hu;
    src.card = targetCard;
}

InterceptionHu::InterceptionHu(PlayerPos::Type srcPlayer, Card::Type targetCard):Interception()
{
    type = Interception::Hu;
    src.playerPos = srcPlayer;
    src.card = targetCard;
}



int InterceptionList::Count() const
{
    return (int)size();
}

InterceptionList InterceptionList::operator + (const InterceptionList &other) const
{
    InterceptionList res = *this;
    return res += other;
}

InterceptionList &InterceptionList::operator += (const InterceptionList &other)
{
    for(InterceptionList::const_iterator itr = other.begin();
        itr != other.end();
        ++itr)
    {
		Add(*itr);
    }
    return *this;
}

InterceptionList &InterceptionList::Add(const Interception &interception)
{
    push_back(interception);
    return *this;
}

void InterceptionList::Clear()
{
    clear();
}

void InterceptionList::SetPlayerPos(PlayerPos::Type player)
{
    for(iterator itr = begin();
        itr != end();
        ++itr)
    {
        itr->src.playerPos = player;
    }
}

InterceptionList::const_iterator InterceptionList::Find(Interception::Type type, Card::Type card) const
{
	return const_cast<InterceptionList *>(this)->Find(type, card);
}

InterceptionList::iterator InterceptionList::Find(Interception::Type type, Card::Type card)
{
	iterator ret = end();
	for(iterator itr = begin(); itr != end(); ++itr)
	{
		if(itr->type == type && (card == Card::Invalid || itr->src.card == card))
		{
			ret = itr;
			break;
		}
	}
	return ret;
}

InterceptionList::const_reverse_iterator InterceptionList::FindLatest(const std::set<Interception::Type> &type, Card::Type card) const
{
	return const_cast<InterceptionList *>(this)->FindLatest(type, card);
}

InterceptionList::reverse_iterator InterceptionList::FindLatest(const std::set<Interception::Type> &type, Card::Type card)
{
	reverse_iterator ret = rend();
	for(reverse_iterator itr = rbegin(); itr != rend(); ++itr)
	{
		if(type.find(itr->type) != type.end() && (card == Card::Invalid || itr->src.card == card))
		{
			ret = itr;
			break;
		}
	}
	return ret;
}

std::ostream &operator << (std::ostream &ostrm, const Interception &interception)
{
    return ostrm<<"type:"<<(int)interception.type<<" from:"<<(int)interception.src.playerPos<<
        " card:"<<(int)interception.src.card<<" cards:"<<interception.cards;
}

std::ostream &operator << (std::ostream &ostrm, const InterceptionList &interceptionList)
{
    for(InterceptionList::const_iterator itr = interceptionList.begin();
        itr != interceptionList.end();
        ++itr)
    {
        ostrm<<(*itr)<<std::endl;
    }
    return ostrm;
}



ReadyHandSelection::ReadyHandSelection(){}

ReadyHandSelection::ReadyHandSelection(const InterceptionList &interceptions)
{
	for(InterceptionList::const_iterator itr = interceptions.begin();
		itr != interceptions.end();
		++itr)
	{
		if(itr->type == Interception::Ting)
		{
			iterator find_itr = find(itr->src.card);
			if(find_itr == end())
			{
				find_itr = insert(std::make_pair(itr->src.card, CardStat())).first;
			}
			find_itr->second.Add(itr->cards);
		}
	}
}

