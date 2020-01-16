#include "ExtensionInterceptableCards.h"

ExtensionInterceptableCards::ExtensionInterceptableCards(){}

const InterceptableCards &ExtensionInterceptableCards::GetInterceptableCards()
{
    return m_InterceptableCards;
}

void ExtensionInterceptableCards::AddInterceptableCard(PlayerPos::Type playerPos, Card::Type card)
{
    m_InterceptableCards.push_back(CardInfo(playerPos, card));
}

void ExtensionInterceptableCards::ClearInterceptableCards()
{
    m_InterceptableCards.clear();
}

void ExtensionInterceptableCards::SetLatestDrawnCard(Card::Type card)
{
	m_LatestDrawnCard = card;
}

Card::Type ExtensionInterceptableCards::GetLatestDrawnCard() const
{
	return m_LatestDrawnCard;
}

void ExtensionInterceptableCards::ClearLatestDrawnCard()
{
	m_LatestDrawnCard = Card::Invalid;
}

void ExtensionInterceptableCards::SetReadyHandSelection(PlayerPos::Type playerPos, const ReadyHandSelection &selection)
{
	m_ReadyHandSelection[playerPos] = selection;
}

ReadyHandSelection ExtensionInterceptableCards::GetReadyHandSelection(PlayerPos::Type playerPos) const
{
	std::map<PlayerPos::Type, ReadyHandSelection>::const_iterator itr = m_ReadyHandSelection.find(playerPos);
	if(itr != m_ReadyHandSelection.end()) return itr->second;
	return ReadyHandSelection();
}

void ExtensionInterceptableCards::ClearReadyHandSelection()
{
	m_ReadyHandSelection.clear();
}

// 玩家拦牌中间结果
void ExtensionInterceptableCards::SetPlayerInterception(const PlayerInterception &interceptions)
{
	m_PlayerInterception = interceptions;
}

const PlayerInterception &ExtensionInterceptableCards::GetPlayerInterception() const
{
	return m_PlayerInterception;
}

void ExtensionInterceptableCards::ClearPlayerInterception()
{
	m_PlayerInterception.clear();
}

void ExtensionInterceptableCards::OnGameStart()
{
	ClearReadyHandSelection();
    ClearInterceptableCards();
	ClearPlayerInterception();
	m_LatestDrawnCard = Card::Invalid;
}
