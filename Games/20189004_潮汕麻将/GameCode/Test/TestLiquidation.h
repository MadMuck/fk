#ifndef TEST_Liquidation_H
#define TEST_Liquidation_H

#include <cassert>
#include "SpecCardScoreDelegate.h"
#include "GameContextCreator.h"
#include "ExtensionGameParam.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHuPattern.h"
#include "Util.h"

class TestLiquidation
{
public:

	void Run()
	{
		GameContext *context = GameContextCreator::Create();



		PlayerPos::Type winner = PlayerPos::P0;
		PlayerPos::Type bankerPos = PlayerPos::P3;

		Liquidation liquidation;
		liquidation.Prepare(bankerPos,4);

		Interception interception;
		interception.type = Interception::Peng;
		interception.src.card = 35;
		CardList cardlist;
		cardlist.push_back(35);
		cardlist.push_back(35);
		cardlist.push_back(35);
		interception.cards = cardlist;

		WinnerList winners;
		winners.push_back(winner);

		ScoreItemGangImpl *impl = new ScoreItemGangImpl(context, interception, winners, 1);

		liquidation.Add(winner,impl);

		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);


		char cards[] = {29};
		CardList cardList;
		cardList.insert(cardList.begin(), cards, cards + ARRAY_LEN(cards));
		InterceptionList interceptions;
		interceptions.Add(InterceptionPeng(21));
		interceptions.Add(InterceptionPeng(28));
		interceptions.Add(InterceptionGang(Interception::MingGang, 24));
		interceptions.Add(InterceptionGang(Interception::MingGang, 8));

		

		extCardBlocks->DiscardCard(winner, 1);

		SpecScoreInfo scoreInfo = SpecCardScoreDelegate::GetMaxScore(context, cardList, interceptions, winner, bankerPos);
		int i = 0;
	}
};

#endif