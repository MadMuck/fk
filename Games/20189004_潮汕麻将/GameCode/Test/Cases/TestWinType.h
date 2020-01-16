#ifndef TEST_WIN_TYPE_H
#define TEST_WIN_TYPE_H

#include <cassert>
#include "SpecCardScoreDelegate.h"
#include "GameContextCreator.h"
#include "ExtensionGameParam.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHuPattern.h"
#include "Util.h"


class TestWinType
{
public:

	void Run()
	{
		GameContext *context = GameContextCreator::Create();

		OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);

		extHuPattern->AddJoker(25);
		extHuPattern->SetJokersLimitCount(4);

		/*char cards[] = {2,2,2,3,3,4,5,5,6,6,7,9,9,9};*/
		/*char cards[] = {1,1,4,4,5,5,5,5,18,18,19,19,35,35};*/
		char cards[] = {11,19,21,29,31,32,33,34,35,36,37,25,25,25};
		CardList cardList;
		cardList.insert(cardList.begin(), cards, cards + ARRAY_LEN(cards));

		InterceptionList interceptionList;
// 		interceptionList.Add(InterceptionPeng(29));
// 		interceptionList.Add(InterceptionGang(Interception::AnGang,36));
// 		interceptionList.Add(InterceptionGang(Interception::AnGang,3));
// 		interceptionList.Add(InterceptionGang(Interception::AnGang,4));


		PlayerPos::Type winner = PlayerPos::P0;
		PlayerPos::Type bankerPos = PlayerPos::P3;

		CardInfo src(winner,25);

		DefaultHuHandlerImpl HuHandler;

		auto result = HuHandler.ToDo(context,winner,winner,cardList,interceptionList,Interception::Hu,src.card);


		//WinTypeList lists =  SpecCardScoreDelegate::GetWinType(context,cardList,interceptionList,src,bankerPos);

		//int fan = SpecCardScoreDelegate::GetFanWithWinTypes(context,lists);

		//SpecScoreInfo scoreInfo = SpecCardScoreDelegate::GetMaxScore(context, cardList, interceptionList, winner, bankerPos);

		int i = 0;
	}
	
};

#endif