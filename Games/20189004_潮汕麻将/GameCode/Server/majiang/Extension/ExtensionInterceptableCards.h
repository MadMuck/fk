#ifndef EXTENSION_INTERCEPTABLE_CARDS_H
#define EXTENSION_INTERCEPTABLE_CARDS_H

#include "GameContext.h"
#include "Interception.h"
#include "RefCntObj.h"
#include "ExtensionHuPattern.h"
#include <list>
#include <map>

typedef std::list<CardInfo> InterceptableCards;

struct InterceptionSelection
{
	InterceptionSelection(){}
	InterceptionSelection(const Interception &action, const InterceptionList &actions):selected(action), selections(actions){}

	Interception selected;
	InterceptionList selections;
};

typedef std::map<PlayerPos::Type, InterceptionSelection> PlayerInterception;

class ExtensionInterceptableCards : public GameContextExtension
{
protected:
    InterceptableCards m_InterceptableCards;

	Card::Type m_LatestDrawnCard; // 最近抓的牌

	std::map<PlayerPos::Type, ReadyHandSelection> m_ReadyHandSelection;

	PlayerInterception m_PlayerInterception;

public:

	ExtensionInterceptableCards();

    const InterceptableCards &GetInterceptableCards();

    void AddInterceptableCard(PlayerPos::Type playerPos, Card::Type card);

    void ClearInterceptableCards();

	void SetLatestDrawnCard(Card::Type card);

	Card::Type GetLatestDrawnCard() const;

	void ClearLatestDrawnCard();

	void SetReadyHandSelection(PlayerPos::Type playerPos, const ReadyHandSelection &selection);

	ReadyHandSelection GetReadyHandSelection(PlayerPos::Type playerPos) const;

	void ClearReadyHandSelection();

	// 仲裁后的玩家拦牌选择结果
	void SetPlayerInterception(const PlayerInterception &interceptions);

	const PlayerInterception &GetPlayerInterception() const;

	void ClearPlayerInterception();

    void OnGameStart();
};

#endif
