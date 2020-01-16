#include "BankerSelector.h"

BankerSelector::BankerSelector():m_Banker(PlayerPos::Max){}

void BankerSelector::SetGameOwner(PlayerPos::Type owner)
{
    m_Banker = owner;
}

PlayerPos::Type BankerSelector::GetBanker() const
{
    return m_Banker;
}

void BankerSelector::SetBanker(PlayerPos::Type playerPos)
{
	m_Banker = playerPos;
}
