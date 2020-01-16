#include "ExtensionCardBlocks.h"
#include "ExtensionPlayer.h"
#include "ExtensionGameParam.h"
#include "ExtensionLogger.h"
#define CARDBLOCKS_COUNT 4	//牌墙数，本游戏牌墙数与游戏人数无关，固定值为4

bool DrawCardValidatorImpl::CanDraw(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
	//OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
	//int  FanCard = 0;
	//if(extGameParam->GetParam(GameParam_bFanMa)) FanCard = extGameParam->GetParam(GameParam_FanMaNum);
	//LOGGER_FILE(context,"应翻马"<< FanCard <<",牌墙剩余："<<extCardBlocks->GetCardBlocks()->Count());
	return extCardBlocks->GetCardBlocks()->Count() /*- FanCard */> 0 ;
}

ExtensionCardBlocks::ExtensionCardBlocks():m_CardBlocks(NULL), m_DrawCardValidator(new DrawCardValidatorImpl())
{

}

CardBlocks *ExtensionCardBlocks::GetCardBlocks()
{
    return (CardBlocks *)m_CardBlocks.GetObj();
}

const CardList &ExtensionCardBlocks::GetDiscardedCards() const
{
	return m_DiscardedCards;
}

CardList ExtensionCardBlocks::GetDiscardedCards(PlayerPos::Type playerPos) const
{
	CardList cardList;
	std::map<PlayerPos::Type, CardList>::const_iterator itr = m_PlayerDiscarededCard.find(playerPos);
	if(itr != m_PlayerDiscarededCard.end())
	{
		cardList = itr->second;
	}
	return cardList;
}

CardList ExtensionCardBlocks::GetRealDiscardedCards(PlayerPos::Type playerPos) const
{
	CardList cardList;
	std::map<PlayerPos::Type, CardList>::const_iterator itr = m_RealPlayerDiscardedCard.find(playerPos);
	if(itr != m_RealPlayerDiscardedCard.end())
	{
		cardList = itr->second;
	}
	return cardList;
}

void ExtensionCardBlocks::DiscardCard(PlayerPos::Type playerPos, Card::Type card)
{
	m_DiscardedCards.Add(card);
	m_RealPlayerDiscardedCard[playerPos].Add(card);
	m_PlayerDiscarededCard[playerPos].Add(card);

	++m_DiscardedCardCnt;
}

void ExtensionCardBlocks::RemoveLatestDiscardedCard(PlayerPos::Type playerPos)
{
	if(m_DiscardedCards.Count() > 0)
	{
		m_DiscardedCards.pop_back();
		m_PlayerDiscarededCard[playerPos].pop_back();
	}
}

void ExtensionCardBlocks::RemoveDiscardedCard(PlayerPos::Type playerPos, Card::Type card)
{
	m_DiscardedCards.Remove(card, 1);
	m_PlayerDiscarededCard[playerPos].Remove(card, 1);
}

void ExtensionCardBlocks::SetDrawValidator(const DrawCardValidator &validator)
{
	m_DrawCardValidator = validator;
}

bool ExtensionCardBlocks::CanDraw() const
{
	return m_DrawCardValidator->CanDraw(m_Context);
}

CardList ExtensionCardBlocks::DrawTwoWhenStart()
{
	return m_CardBlocks->DrawTwoWhenStart();
}

Card::Type ExtensionCardBlocks::Draw()
{
	return m_CardBlocks->Draw();
}

Card::Type ExtensionCardBlocks::InverseDraw()
{
	return m_CardBlocks->InverseDraw();
}

Card::Type ExtensionCardBlocks::SpecInverseGetCard(int idx)
{
	return m_CardBlocks->SpecInverseGetCard(idx);
}

void ExtensionCardBlocks::Init()
{
    // 根据需求设置有效的牌
	m_CardBlocks->Reset();
	 m_CardBlocks->AddCards(Card::WanStart, Card::WanEnd, 4);
    m_CardBlocks->AddCards(Card::TiaoStart, Card::TiaoEnd, 4);
    m_CardBlocks->AddCards(Card::BingStart, Card::BingEnd, 4);
	m_CardBlocks->AddCards(Card::FengStart, Card::FengEnd, 4);
	m_CardBlocks->AddCards(Card::ZiZhong, Card::ZiEnd, 4);

	m_DiscardedCards.clear();
	m_PlayerDiscarededCard.clear();
	m_RealPlayerDiscardedCard.clear();
	m_DrawCardCount.clear();

	m_DiscardedCardCnt = 0;
}

CardStat ExtensionCardBlocks::GetRevealedCards(const CardList &hand) const
{
	CardStat ret;

	OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
	const PlayerList &playerList = extPlayer->GetPlayerList();
	for(PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
	{
		const InterceptionList &interceptions = itr->second->GetInterceptions();
		for(InterceptionList::const_iterator itr2 = interceptions.begin();
			itr2 != interceptions.end();
			++itr2)
		{
			ret.Add(itr2->cards);
		}
	}

	ret.Add(m_DiscardedCards);

	ret.Add(hand);

	return ret;
}

int ExtensionCardBlocks::GetTotalDiscardedCardCnt() const
{
	return m_DiscardedCardCnt;
}

int ExtensionCardBlocks::MaxDiscardedCount() const
{
	int ret = 0;
	for(std::map<PlayerPos::Type, CardList>::const_iterator itr = m_RealPlayerDiscardedCard.begin();
		itr != m_RealPlayerDiscardedCard.end();
		++itr)
	{
		if(ret < itr->second.Count())
			ret = itr->second.Count();
	}
	return ret;
}

int ExtensionCardBlocks::GetDrawnCardCount(PlayerPos::Type playerPos) const
{
	int ret = 0;
	if(m_DrawCardCount.find(playerPos) != m_DrawCardCount.end())
		ret = m_DrawCardCount.at(playerPos);
	return ret;
}

Card::Type ExtensionCardBlocks::OnDrawCard(Card::Type oriCard, PlayerPos::Type playerPos)
{
	++m_DrawCardCount[playerPos];
	return m_DrawCardValidator->OnDraw(m_Context, oriCard, playerPos);
}

void ExtensionCardBlocks::OnGameReset()
{
    Init();
}

void ExtensionCardBlocks::OnGameStart()
{
	Init();
}

void ExtensionCardBlocks::OnGameFinish()
{
	Init();
}

void ExtensionCardBlocks::OnCreate()
{
	m_CardBlocks = RefCntObj<CardBlocks>(new CardBlocks(CARDBLOCKS_COUNT));
    Init();
}
