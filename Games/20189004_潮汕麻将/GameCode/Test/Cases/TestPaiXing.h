#ifndef TEST_PAI_XING
#define TEST_PAI_XING

#include <cassert>
#include "SpecCardScoreDelegate.h"
#include "GameContextCreator.h"
#include "ExtensionGameParam.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHuPattern.h"
#include "Util.h"

class TestPaiXing
{
public:

	void Run()
	{
		GameContext *context = GameContextCreator::Create();

		/*char cards[] = {2,2,2,3,3,4,5,5,6,6,7,9,9,9};*/
		char cards[] = {1,1,1,1,2,2,2,2,3,3,4,4,5,5};
		CardList cardList;
		cardList.insert(cardList.begin(), cards, cards + ARRAY_LEN(cards));

		InterceptionList interceptionList;

// 		Interception interception3;
// 		interception3.type = Interception::Peng;
// 		interception3.src.card = 6;
// 
// 		interceptionList.push_back(interception3);

		PlayerPos::Type winner = PlayerPos::P0;
		PlayerPos::Type bankerPos = PlayerPos::P3;

		CardInfo src(winner,3);

		Interception interception1;
		interception1.type = Interception::MingGang;
		interception1.src.card = 3;

		Interception interception2;
		interception2.type = Interception::MingGang;
		interception2.src.card = 1;

		Interception interception3;
		interception3.type = Interception::Peng;
		interception3.src.card = 4;

		Interception interception4;
		interception4.type = Interception::Peng;
		interception4.src.card = 3;

// 		interceptionList.push_back(interception1);
// 		interceptionList.push_back(interception2);
// 		interceptionList.push_back(interception3);
// 		interceptionList.push_back(interception4);


		CardStatistic stat(cardList,interceptionList);

		OBJ_GET_EXT(context, ExtensionPlayer, extActionPlayer);
		extActionPlayer->GetPlayer(winner)->AddCards(cardList);

//		bool ret = SpecCardScoreDelegate::IsDanDiao(context,stat,src);
//		assert(ret);

	}
};

#endif