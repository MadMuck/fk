#ifndef CARD_BLOCKS_H
#define CARD_BLOCKS_H

#include "PlayerPos.h"
#include "Card.h"
#include <vector>

class TwoCards
{
	int m_Idx;
	Card::Type cards[2];

public:
	TwoCards();

	void AddCard(Card::Type card);

	Card::Type GetCard();

	Card::Type GetCard(int idx) const;

	int Count() const;
};

class CardBlocks
{
    int m_RowCount;

    PlayerPos::Type m_StartPos;
	int m_StartDrawPos;

	int m_DrawPos;
	int m_DrawPosTail;
    int m_Head;
    int m_Tail;
    int m_Count;
    std::vector<std::vector<TwoCards> > m_Blocks;
    CardList m_Cards;
	CardStat m_CardStat;
public:
    CardBlocks(int playerCount);

	int GetRowCount() const;

    PlayerPos::Type GetStartPos() const;

	int GetStartDrawPos() const;

    void SetStartPos(PlayerPos::Type startPos);

	void SetDrawPos(int drawPos);

    void AddCards(Card::Type start, Card::Type end, int count);

    int MaxCount() const;

    int Count() const;

    void Reset();

    void Shuffle();

	CardList DrawTwoWhenStart();

    Card::Type Draw();

    Card::Type InverseDraw();

	Card::Type SpecInverseGetCard(int idx);

	const std::vector<std::vector<TwoCards> > &GetAllCards() const;

	const CardStat &GetCardStat() const;
};

#endif
