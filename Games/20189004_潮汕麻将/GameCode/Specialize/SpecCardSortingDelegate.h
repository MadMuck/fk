#ifndef SPEC_CARD_SORTING_DELEGATE_H
#define SPEC_CARD_SORTING_DELEGATE_H

#include "ExtensionPlayer.h"
#include "ExtensionQueMen.h"

class SpecCardSortingDelegate : public HandSortingDelegate
{
	GameContext *m_Context;
public:

	SpecCardSortingDelegate(GameContext *context):m_Context(context){}

	void Sort(PlayerPos::Type playerPos, CardList &hand)
	{
		OBJ_GET_EXT(m_Context, ExtensionHuPattern, extHuPattern);
		OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
		CardList jokerList = extHuPattern->GetJokers();
		if(!jokerList.empty())
		{
			CardList remaining = hand;
			CardList jokerCards;
			for(auto itr = jokerList.begin(); itr != jokerList.end(); ++itr)
			{
				int jokercount = hand.Find(*itr);

				jokerCards.Add(*itr,jokercount);
				remaining.Remove(*itr,jokercount);
			}

			remaining.Sort();
			jokerCards.Sort();
			hand.Clear();

			hand += jokerCards;
			hand += remaining;
		}
		else
		{
			hand.Sort();
		}
	}
};

#endif