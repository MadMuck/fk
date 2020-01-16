#include "Player.h"

Player::Player(PlayerPos::Type pos):m_Pos(pos)
{}

PlayerPos::Type Player::GetPos() const
{
    return m_Pos;
}

void Player::AddCard(Card::Type card)
{
    m_Cards.push_back(card);
}

void Player::AddCards(const CardList &cards)
{
	m_Cards += cards;
}

bool Player::DiscardCard(Card::Type card)
{
    bool res = false;
    for(CardList::iterator itr = m_Cards.begin();
        itr != m_Cards.end();
        ++itr)
    {
        if(*itr == card)
        {
            res = true;
            m_Cards.erase(itr);
            break;
        }
    }
    return res;
}

void Player::RemoveCards(const CardList &cards)
{
    m_Cards -= cards;
}

void Player::RemoveInterception(const Interception &interception)
{
	for(InterceptionList::iterator itr = m_Interceptions.begin();
		itr != m_Interceptions.end();
		++itr)
	{
		Interception &action = *itr;
		if(action.type == interception.type &&
			action.cards == interception.cards)
		{
			m_Interceptions.erase(itr);
			break;
		}
	}
}

const CardList &Player::GetCards() const
{
    return m_Cards;
}

CardList &Player::GetMutableCards()
{
	return m_Cards;
}

void Player::AddInterception(const Interception &interception)
{
	Interception addinterception = interception;

	if(interception.type == Interception::BuGang ||
		interception.type == Interception::BuGangTing
	//	||	interception.type == Interception::YangMa
		)
	{
		InterceptionPeng lastpeng(interception.src.card);
		PlayerPos::Type lastpeng_srcPos = addinterception.src.playerPos;

		for(InterceptionList::iterator itr = m_Interceptions.begin();
			itr != m_Interceptions.end();
			++itr)
		{
			Interception &action = *itr;
			if(action.type == lastpeng.type &&
				action.cards == lastpeng.cards)
			{
				lastpeng_srcPos = action.src.playerPos;
				break;
			}
		}

		addinterception.src.playerPos = lastpeng_srcPos;

		// 补杠要删掉之前的碰
		RemoveInterception(lastpeng);
	}
    m_Interceptions.push_back(addinterception);
}

const InterceptionList &Player::GetInterceptions() const
{
    return m_Interceptions;
}

void Player::Clear()
{
    m_Cards.clear();
    m_Interceptions.clear();
}
