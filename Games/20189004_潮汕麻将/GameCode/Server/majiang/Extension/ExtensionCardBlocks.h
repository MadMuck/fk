#ifndef EXTENSION_CARD_BLOCKS_H
#define EXTENSION_CARD_BLOCKS_H

#include "GameContext.h"
#include "CardBlocks.h"
#include "RefCntObj.h"
#include <map>

class DrawCardValidatorImpl
{
public:
	virtual ~DrawCardValidatorImpl(){}

	virtual bool CanDraw(GameContext *context) const;

	virtual Card::Type OnDraw(GameContext *context, Card::Type oriCard, PlayerPos::Type playerPos)
	{
		return oriCard;
	}
};

typedef RefCntObj<DrawCardValidatorImpl> DrawCardValidator;

class ExtensionCardBlocks : public GameContextExtension
{
protected:
    RefCntObj<CardBlocks> m_CardBlocks;

	CardList m_DiscardedCards; // 被打出的牌(除去被拦的牌)

	std::map<PlayerPos::Type, CardList> m_PlayerDiscarededCard; // 每个玩家打出的牌(除去被拦的牌)

	std::map<PlayerPos::Type, CardList> m_RealPlayerDiscardedCard; // 每个玩家打出的牌

	std::map<PlayerPos::Type, int> m_DrawCardCount; // 每个玩家抓牌数量

	DrawCardValidator m_DrawCardValidator;

	int m_DiscardedCardCnt; // 打过的牌数量

public:
	ExtensionCardBlocks();

    CardBlocks *GetCardBlocks();

	const CardList &GetDiscardedCards() const;

	CardList GetDiscardedCards(PlayerPos::Type playerPos) const;

	CardList GetRealDiscardedCards(PlayerPos::Type playerPos) const;

	void DiscardCard(PlayerPos::Type playerPos, Card::Type card);

	void RemoveLatestDiscardedCard(PlayerPos::Type playerPos);

	void RemoveDiscardedCard(PlayerPos::Type playerPos, Card::Type card);

	void SetDrawValidator(const DrawCardValidator &validator);

	bool CanDraw() const;

	CardList DrawTwoWhenStart();

	Card::Type Draw();

	Card::Type InverseDraw();

	Card::Type SpecInverseGetCard(int idx);

    void Init();

	CardStat GetRevealedCards(const CardList &hand) const;

	int GetTotalDiscardedCardCnt() const;

	// 出牌最多的玩家的出牌数量
	int MaxDiscardedCount() const;

	int GetDrawnCardCount(PlayerPos::Type playerPos) const;

	Card::Type OnDrawCard(Card::Type oriCard, PlayerPos::Type playerPos);

    void OnGameReset();

	void OnGameStart();

	void OnGameFinish();

    void OnCreate();
};

#endif
