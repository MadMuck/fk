#ifndef TEST_SCORE_H
#define TEST_SCORE_H

#include <cassert>
#include "SpecCardScoreDelegate.h"
#include "GameContextCreator.h"
#include "ExtensionGameParam.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHuPattern.h"
#include "Util.h"

class TestScore
{
public:

	void Run()
	{
		GameContext *context = GameContextCreator::Create();
		
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		

		char cards[] = {29};
		CardList cardList;
		cardList.insert(cardList.begin(), cards, cards + ARRAY_LEN(cards));
		InterceptionList interceptions;
		interceptions.Add(InterceptionPeng(21));
		interceptions.Add(InterceptionPeng(28));
		interceptions.Add(InterceptionGang(Interception::MingGang, 24));
		interceptions.Add(InterceptionGang(Interception::MingGang, 8));

		PlayerPos::Type winner = PlayerPos::P0;
		PlayerPos::Type bankerPos = PlayerPos::P3;

		extCardBlocks->DiscardCard(winner, 1);

		SpecScoreInfo scoreInfo = SpecCardScoreDelegate::GetMaxScore(context, cardList, interceptions, winner, bankerPos);
		assert(scoreInfo.fan == 4);
	}
};

#endif