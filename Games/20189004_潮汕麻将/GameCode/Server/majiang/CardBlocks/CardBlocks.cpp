#include "CardBlocks.h"
#include <cstdlib>
#include <algorithm>
#include "Util.h"
#include "Statistic.h"


CardBlocks::CardBlocks(int playerCount):m_RowCount(playerCount), m_StartPos(PlayerPos::P0),m_StartDrawPos(0),
m_DrawPos(0), m_DrawPosTail(0), m_Head(0), m_Tail(0), m_Count(0)
{
	for(int i = 0; i < m_RowCount; ++i)
	{
		m_Blocks.push_back(std::vector<TwoCards>());
	}
}

int CardBlocks::GetRowCount() const
{
	return m_RowCount;
}

PlayerPos::Type CardBlocks::GetStartPos() const
{
    return m_StartPos;
}

int CardBlocks::GetStartDrawPos() const
{
	return m_StartDrawPos;
}

void CardBlocks::SetStartPos(PlayerPos::Type startPos)
{
    m_StartPos = startPos;
}

void CardBlocks::SetDrawPos(int drawPos)
{
	m_StartDrawPos = drawPos;
}

void CardBlocks::AddCards(Card::Type start, Card::Type end, int count)
{
    for(int i = start; i < end; ++i)
    {
		m_Cards.Add(i, count);//为什么这两个add（）不是一个？多态
		m_CardStat.Add(i, count);
    }
}

int CardBlocks::MaxCount() const
{
    return (int)m_Cards.size();
}

int CardBlocks::Count() const
{
    return m_Count;
}

void CardBlocks::Reset()
{
    for(int i = 0; i < m_RowCount; ++i)
    {
        m_Blocks[i].clear();
    }
    m_Cards.clear();
	m_CardStat.Clear();
    m_Count = 0;
    m_Head = 0;
    m_Tail = 0;
}

void CardBlocks::Shuffle()
{
    for(int i = 0; i < m_RowCount; ++i)
    {
        m_Blocks[i].clear();
    }

    m_Count = m_Cards.size();
    for(int i = 0; i < m_Count; ++i)
    {
		std::random_shuffle(m_Cards.begin(), m_Cards.end());
    }

	int columnCnt = m_Count / 2 / m_RowCount;
	int rowSize = columnCnt * 2 + ((m_Count / 2) % columnCnt > 0 ? 1 : 0) * 2;

    for(int i = 0; i < m_Count; ++i)
    {
        int row = i / rowSize;
        int idx = (i - row * rowSize) / 2;
        if((int)m_Blocks[row].size() <= idx)
        {
            m_Blocks[row].push_back(TwoCards());
        }
        m_Blocks[row][idx].AddCard(m_Cards[i]);
    }

    m_Head = m_StartPos;
    m_Tail = m_StartPos;
	m_DrawPos = m_StartDrawPos;
	m_DrawPosTail = m_Blocks[m_Tail].size() - m_StartDrawPos;
}

CardList CardBlocks::DrawTwoWhenStart()
{
	if(m_DrawPos >= (int)m_Blocks[m_Head].size())
	{
		m_DrawPos = 0;
		m_Head += 1;
		m_Head %= m_RowCount;
	}

	CardList ret;

	int drawPos = m_DrawPos;
	int head = m_Head;
	if(m_Blocks[head][drawPos].Count() < 2)
	{
		++drawPos;
		if(drawPos >= (int)m_Blocks[head].size())
		{
			drawPos = 0;
			head += 1;
			head %= m_RowCount;
		}
	}

	ret.Add(m_Blocks[head][drawPos].GetCard());

	drawPos += 2;
	if(drawPos >= (int)m_Blocks[head].size())
	{
		drawPos = 0;
		head += 1;
		head %= m_RowCount;
	}

	ret.Add(m_Blocks[head][drawPos].GetCard());
	m_Count -= 2;
	return ret;
}

Card::Type CardBlocks::Draw()
{
    Card::Type card = Card::Invalid;
    if(m_Count > 0)
    {
		while(true)
		{
			if(m_DrawPos >= (int)m_Blocks[m_Head].size())
			{
				m_DrawPos = 0;
				m_Head += 1;
				m_Head %= m_RowCount;
			}

			std::vector<TwoCards>::iterator itr = m_Blocks[m_Head].begin() + m_DrawPos;

			if(itr->Count() > 0)
			{
				card = itr->GetCard();
				break;
			}
			else
			{
				++ m_DrawPos;
			}
		}
        --m_Count;
    }
    return card;
}

Card::Type CardBlocks::InverseDraw()
{
    Card::Type card = Card::Invalid;
    if(m_Count > 0)
    {
		while(true)
		{
			if(m_DrawPosTail >= (int)m_Blocks[m_Tail].size())
			{
				m_DrawPosTail = 0;
				m_Tail += (m_RowCount - 1);
				m_Tail %= m_RowCount;
			}

			std::vector<TwoCards>::iterator itr = m_Blocks[m_Tail].end() - 1 - m_DrawPosTail;
			if(itr->Count() > 0)
			{
				card = itr->GetCard();
				break;
			}
			else
			{
				++m_DrawPosTail;
			}
		}  
        --m_Count;
    }
    return card;
}

Card::Type CardBlocks::SpecInverseGetCard(int idx)
{
	int temp_DrawPosTail = m_DrawPosTail;
	int temp_m_Tail = m_Tail;
	Card::Type card = Card::Invalid;
	if(m_Count > 0)
	{
		while(idx > 1)
		{
			++temp_DrawPosTail;
			--idx;
			if(temp_DrawPosTail >= (int)m_Blocks[temp_m_Tail].size())
			{
				temp_DrawPosTail = 0;
				temp_m_Tail += (m_RowCount - 1);
				temp_m_Tail %= m_RowCount;
			}
		}

		while(true)
		{
			if(temp_DrawPosTail >= (int)m_Blocks[temp_m_Tail].size())
			{
				temp_DrawPosTail = 0;
				temp_m_Tail += (m_RowCount - 1);
				temp_m_Tail %= m_RowCount;
			}

			std::vector<TwoCards>::iterator itr = m_Blocks[temp_m_Tail].end() - 1 - temp_DrawPosTail;
			if(itr->Count() > 0)
			{
				card = itr->GetCard();
				break;
			}
			else
			{
				++temp_DrawPosTail;
			}
		}  
		--m_Count;
	}
	return card;
}

const std::vector<std::vector<TwoCards> > &CardBlocks::GetAllCards() const
{
	return m_Blocks;
}

const CardStat &CardBlocks::GetCardStat() const
{
	return m_CardStat;
}

/////////////////////////////////////////////////////////////////////

TwoCards::TwoCards():m_Idx(2)
{
    cards[0] = Card::Invalid; cards[1] = Card::Invalid;
}

void TwoCards::AddCard(Card::Type card)
{
    if(m_Idx > 0)
    {
        --m_Idx;
        cards[m_Idx] = card;
    }
}

Card::Type TwoCards::GetCard()
{
	Card::Type card = Card::Invalid;
    if(m_Idx < 2) card = GetCard(m_Idx++);
	return card;
}

Card::Type TwoCards::GetCard(int idx) const
{
	Card::Type card = Card::Invalid;
	if(idx >= 0 && idx < 2)
	{
		card = cards[idx];
	}
	return card;
}

int TwoCards::Count() const
{
    return std::max(0, 2 - m_Idx);
}
