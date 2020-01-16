#include "ActionValidator.h"
#include "PatternFinder.h"
#include "HuPatternMatch.h"
#include "PatternConst.h"
#include "ExtensionHuPattern.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionPlayer.h"
#include "ExtensionGameDirection.h"
#include "ExtensionLogger.h"
#include "ExtensionGameParam.h"
#include "PlayerStatusDef.h"
#include "SpecCardScoreDelegate.h"
bool InterceptionValidatorImpl::IsValid(GameContext *context, PlayerPos::Type player, Interception::Type type, const CardInfo &card) const 
{
	bool ret = true;
	if(type == Interception::Ting)
	{
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		ret = !extPlayer->GetPlayerStatus(player).GetBool(PlayerStatus_ReadyHand);
	}
	return ret;
}

InterceptionList DefaultChiHandlerImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cardList,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card) const
{
	InterceptionList res;

	Card::Type start = Card::Invalid;
	Card::Type end = Card::Invalid;
	if(card >= Card::WanStart && card < Card::WanEnd)
	{
		start = Card::WanStart;
		end = Card::WanEnd;
	}
	else if(card >= Card::TiaoStart && card < Card::TiaoEnd)
	{
		start = Card::TiaoStart;
		end = Card::TiaoEnd;
	}
	else if(card >= Card::BingStart && card < Card::BingEnd)
	{
		start = Card::BingStart;
		end = Card::BingEnd;
	}

	if(start != Card::Invalid)
	{
		// 查找 [card - 2, card - 1] [card - 1, card + 1] [card + 1, card + 2]
		// 以上三种情况分别对应card在吃牌中的位置 0, 1, 2
		for(int pos = 0; pos < 3; ++pos)
		{
			// 确保card在吃牌中的位置有效
			if(card >= start + pos && card <= end - (3 - pos))
			{
				CardList pattern;
				// 将吃牌中card以外的两张牌加入pattern
				if(pos != 0) pattern.Add(card - pos);
				if(pos != 1) pattern.Add(card - pos + 1);
				if(pos != 2) pattern.Add(card - pos + 2);
				pattern.Add(card);
				if(cardList.Find(pattern))
				{
					res.Add(InterceptionChi(srcPlayer, card, pos));
				}
			}
		}
	}

	return res;
}

InterceptionList DefaultPengHandlerImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cardList,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card) const
{
	InterceptionList res;
	if(cardList.Find(card) >= 3)
	{
		res.Add(InterceptionPeng(srcPlayer, card));
	}
	return res;
}

InterceptionList DefaultGangHandlerImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cardList,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card) const
{
	InterceptionList res;
	CardStat stat(cardList);

	switch(type)
	{
	case Interception::MingGang:
		if(stat.Num(card) >= 4)
		{
			res.Add(InterceptionGang(srcPlayer, type, card));
		}
		break;
	case Interception::AnGang:
		{
			OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
		//	CardList jokerList = extHuPattern->GetJokers();
			CardStat::ValuesType values = stat.Values();
			for(CardStat::ValuesType::iterator itr = values.begin(); itr != values.end(); ++itr)
			{
				if(stat.Num(*itr) >= 4 /*&& 0 == jokerList.Find(*itr)*/)	//万能牌不能暗杠
				{
					res.Add(InterceptionGang(srcPlayer, type, *itr));
				}
			}

		}
		break;
	case Interception::BuGang:
		for(InterceptionList::const_iterator itr = interceptions.begin(); itr != interceptions.end(); ++itr)
		{
			if(itr->type == Interception::Peng && itr->src.card == card)
			{
				res.Add(InterceptionGang(srcPlayer, type, card));
			}
			//else if(itr->type == Interception::Peng && cardList.Find(itr->src.card) != 0)	//养马
			//{
			//	res.Add(InterceptionGang(srcPlayer, Interception::YangMa, itr->src.card));
			//}
		}

		break;
	}

	return res;
}

DefaultCollectionHandlerImpl::DefaultCollectionHandlerImpl(const CardList &collection):m_Collection(collection){}

InterceptionList DefaultCollectionHandlerImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cardList,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card) const
{
	InterceptionList res;
	PatternConst ptnConst(m_Collection);
	CardStatistic stat(cardList);
	if(Card::Invalid != card) stat.Add(card, 1);
	size_t tryIdx = 0;
	if(ptnConst.Find(stat, tryIdx).IsValid())
	{
		res.Add(InterceptionCollection(srcPlayer, type, card, m_Collection) );
	}
	return res;
}

InterceptionList DefaultHuHandlerImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cardList,
	const InterceptionList &interceptionList,
	Interception::Type type, Card::Type card) const
{
	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
	const HuPatternMatch &huPatternMatch = extHuPattern->GetHuPattern();

	InterceptionList res;

	CardList tmp = cardList;

	CardStatistic stat(tmp, interceptionList);

	CardList jokerList = extHuPattern->GetJokers();
	int jokernum = 0;

	//for(CardList::iterator itr = jokerList.begin(); itr != jokerList.end(); ++itr)
	//{
	//	jokernum += stat.Num(*itr);
	//}
	bool QiDui =SpecCardScoreDelegate::IsQiDui(stat, context,card);
	if(huPatternMatch.Match(stat).size() > 0 || QiDui)
	{
		InterceptionHu interception(card);
		interception.cards = tmp;
		interception.src.playerPos = srcPlayer;
		interception.src.card = card;
		res.Add(interception);
	}

	return res;
}

InterceptionList DefaultTingHandlerImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cards,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card) const
{
	CardStatistic stat(cards, interceptions);

	InterceptionList res;

	CardStatistic::ValuesType cardValues = stat.Values(); // copy card values

	CardStat revealedCards = GetRevealedCards(context, player);

	for(CardStatistic::ValuesType::const_iterator itr = cardValues.begin();
		itr != cardValues.end();
		++itr)
	{
		Card::Type cardToRemove = *itr;
		// 删掉一张牌, 再加入一张其他的牌，看能不能和牌
		Interception ting;
		ting.type = Interception::Ting;
		ting.src.card = cardToRemove;

		stat.Sub(cardToRemove, 1);

		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
		CardList jokerlist = extHuPattern->GetJokers();

		bool bDanDiaoJoker12 = false;

		//if(!extGameParam->IsTrue(GameParam_DanDiaoJoker12))		//单吊鬼牌
		//{
		//	if(1 == stat.Values().size() && 1 == stat.Num(stat.Values()[0]) && 1 == jokerlist.Find(stat.Values()[0]))
		//	{
		//		ting.cards.Add(stat.Values()[0], 1);
		//		bDanDiaoJoker12 = true;
		//	}
		//}

		if(!bDanDiaoJoker12)
		{
			size_t tryIdx = 0;
			while(true)
			{
				Card::Type cardToWin = Card::Invalid;
				int remainCnt = 0;
				if(!CardToWin(context, stat, revealedCards, tryIdx, cardToWin, remainCnt))
				{
					break;
				}

				ting.cards.Add(cardToWin, 1);
			}
		}
		

		stat.Add(cardToRemove , 1);

		if(ting.cards.Count() > 0)
		{
			res.Add(ting);
		}
	}

	return res;
}

bool DefaultTingHandlerImpl::CardToWin(GameContext *context, 
	CardStatistic &stat,
	const CardStat &revealedCards,
	size_t &tryIdx,
	Card::Type &outputCard,
	int &remaining) const
{
	OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
	OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
	const CardStat &cardBlockStat = extCardBlocks->GetCardBlocks()->GetCardStat();
	const CardStat::ValuesType &uniqCards = cardBlockStat.Values();

	bool ret = false;
	size_t maxIdx = uniqCards.size();
	while(tryIdx < maxIdx)
	{
		Card::Type tryCard = *(uniqCards.begin() + tryIdx);
		++tryIdx;

		int remainCnt = revealedCards.Num(tryCard); // 剩余张数
		if(stat.Num(tryCard) < 4)
		{
			stat.Add(tryCard, 1);//加入一张牌，看是否能和

			if(extHuPattern->Match(stat).size() > 0 || SpecCardScoreDelegate::IsQiDui(stat,context,tryCard))
			{
				// 这张能听，记录下来
				outputCard = tryCard;
				remaining = remainCnt;
				ret = true;
			}

			stat.Sub(tryCard, 1);

			if(ret) break;
		}
	}
	return ret;
}

CardStat DefaultTingHandlerImpl::GetRevealedCards(GameContext *context, PlayerPos::Type playerPos) const
{
	OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	CardStat stat = extCardBlocks->GetCardBlocks()->GetCardStat();
	stat.Sub(extCardBlocks->GetDiscardedCards());
	stat.Sub(extPlayer->GetPlayer(playerPos)->GetCards());
	
	for(PlayerList::iterator itr = extPlayer->GetPlayerList().begin(); itr != extPlayer->GetPlayerList().end(); ++itr)
	{
		InterceptionList interceptionList = itr->second->GetInterceptions();
		for(InterceptionList::iterator itr2 = interceptionList.begin(); itr2 != interceptionList.end(); ++itr2)
		{
			if(itr2->type == Interception::BuGang/* || itr2->type == Interception::YangMa */|| itr2->type == Interception::MingGang || itr2->type == Interception::AnGang || itr2->type == Interception::Peng)
				stat.Sub(itr2->cards);

			if(itr2->type == Interception::Hu)	//已胡玩家胡的那张牌也要减掉
				stat.Sub(itr2->src.card,1);
		}
	}
	return stat;
}

bool DefaultTingHandlerImpl::IsReadyHand(GameContext *context, PlayerPos::Type playerPos, CardStatistic &stat) const
{
	CardStat revealedCards = GetRevealedCards(context, playerPos);
	size_t tryIdx = 0;
	Card::Type cardToWin = Card::Invalid;;
	int remainCnt = 0;
	// 存在能听的牌就成功了
	return CardToWin(context, stat, revealedCards, tryIdx, cardToWin, remainCnt);
}

CardList DefaultTingHandlerImpl::AllCardsToWin(GameContext *context, PlayerPos::Type playerPos, CardStatistic &stat) const
{
	CardStat revealedCards = GetRevealedCards(context, playerPos);
 	CardList ret;
	size_t tryIdx = 0;
	while(true)
	{
		Card::Type cardToWin = Card::Invalid;
		int remainCnt = 0;
		// 存在能听的牌就成功了
		if(CardToWin(context, stat, revealedCards, tryIdx, cardToWin, remainCnt))
		{
			ret.Add(cardToWin);
		}
		else
		{
			break;
		}
	}
	return ret;
}


InterceptionList InterceptionHandlerListImpl::ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cards,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card) const
{
	InterceptionList ret;
	std::map<Interception::Type, InterceptionHandler>::const_iterator itr = m_Handlers.find(type);
	if(itr != m_Handlers.end())
	{
		ret = itr->second->ToDo(context, player, srcPlayer, cards, interceptions, type, card);
	}
	return ret;
}

void InterceptionHandlerListImpl::Add(Interception::Type type, const InterceptionHandler &handler)
{
	Remove(type);
	m_Handlers.insert(std::make_pair(type, handler));
}

void InterceptionHandlerListImpl::Remove(Interception::Type type)
{
	m_Handlers.erase(type);
}

InterceptionHandler InterceptionHandlerListImpl::Get(Interception::Type type) const
{
	std::map<Interception::Type, InterceptionHandler>::const_iterator itr = m_Handlers.find(type);
	if(itr != m_Handlers.end())
	{
		return itr->second;
	}
	return InterceptionHandler(NULL);
}

InterceptionHandlerList GetDefaultInterceptionHandlers()
{
	InterceptionHandlerList handlers(new InterceptionHandlerListImpl());

	handlers->Add(Interception::Chi, InterceptionHandler(new DefaultChiHandlerImpl()));
	handlers->Add(Interception::Peng, InterceptionHandler(new DefaultPengHandlerImpl()));

	InterceptionHandler gangHandler = InterceptionHandler(new DefaultGangHandlerImpl());
	handlers->Add(Interception::MingGang, gangHandler);
	handlers->Add(Interception::AnGang, gangHandler);
	handlers->Add(Interception::BuGang, gangHandler);

	CardList fengCards; fengCards.AddContinuous(Card::FengDong, 4);
	handlers->Add(Interception::Feng, InterceptionHandler(new DefaultCollectionHandlerImpl(fengCards)));
	CardList jianCards; jianCards.AddContinuous(Card::ZiZhong, 3);
	handlers->Add(Interception::Jian, InterceptionHandler(new DefaultCollectionHandlerImpl(jianCards)));

	handlers->Add(Interception::Hu, InterceptionHandler(new DefaultHuHandlerImpl()));

	handlers->Add(Interception::Ting, InterceptionHandler(new DefaultTingHandlerImpl()));

	return handlers;
}
