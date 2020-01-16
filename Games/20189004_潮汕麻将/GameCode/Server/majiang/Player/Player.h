#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerPos.h"
#include "Card.h"
#include "Interception.h"

class Player
{
    PlayerPos::Type m_Pos; // 玩家位置
    CardList m_Cards; // 玩家手牌
    InterceptionList m_Interceptions; // 玩家已经执行的拦牌操作（包括胡牌操作）
//	int m_InterceptFan;//拦牌分（用于结算前获取拦牌的分数）
public:
    Player(PlayerPos::Type pos);

    PlayerPos::Type GetPos() const;

	// 加一张牌
    void AddCard(Card::Type card);

	// 加一堆牌
	void AddCards(const CardList &cards);

	// 出牌（实际上也是删除一张牌）
    bool DiscardCard(Card::Type card);

	// 删除一堆牌
    void RemoveCards(const CardList &cards);

	// 删除一个拦牌操作
	void RemoveInterception(const Interception &interception);

    const CardList &GetCards() const;

	CardList &GetMutableCards();

	// 增加一个拦牌操作
    void AddInterception(const Interception &interception);

    const InterceptionList &GetInterceptions() const;

    void Clear();
};

#endif
