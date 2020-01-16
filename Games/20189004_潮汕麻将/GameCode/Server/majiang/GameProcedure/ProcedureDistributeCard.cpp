
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureDistributeCard.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionPlayer.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionProcedureController.h"
#include "ExtensionTimer.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGameDirection.h"
#include "ExtensionGameParam.h"

#include "NotificationDistributeCard.h"
#include "NotificationFirstRun.h"

#include "Util.h"
#include "GameDef.h"

void SendDistributedCards(void **data, int dataCnt)
{
	GameContext *context = (GameContext *)data[0];
	OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
	// 发牌数据发到客户端
	extGamePlayerConnector->SendNotification(NotificationDistributeCard());
}


void StartProcedureFirstRun(void **data, int dataCnt)
{
	GameContext *context = (GameContext *)data[0];

	OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);

	extGamePlayerConnector->SendNotification(NotificationFirstRun());

	OBJ_GET_EXT(context, ExtensionProcedureController, extProcedureController);
	extProcedureController->StartProcedure(ProcedureType::AfterDrawCard);
}

void ProcedureDistributeJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	{
		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		void *pointers[] = {context};
		int pointserCnt = ARRAY_LEN(pointers);
		TimerData timerData(pointers, pointserCnt, StartProcedureFirstRun);
		extTimer->AddToQueue(0, timerData);
	}
}

void ProcedureDistributeCardImpl::OnStart()
{
    //发牌
    DistributeCards();

	{
		OBJ_GET_EXT(m_Context, ExtensionTimer, extTimer);
		void *pointers[] = {m_Context, this};
		int pointserCnt = sizeof(pointers) / sizeof(pointers[0]);
		TimerData timerData(pointers, pointserCnt, SendDistributedCards);
		extTimer->AddToQueue(0.5f, timerData);
	}

	Jump(NULL, 0);
}

void ProcedureDistributeCardImpl::DistributeCards()
{
    OBJ_GET_EXT(m_Context, ExtensionCardBlocks, extCardBlocks);
    CardBlocks *cardBlocks = extCardBlocks->GetCardBlocks();

	OBJ_GET_EXT(m_Context, ExtensionBankerSelector, extBankerSelector);
	PlayerPos::Type bankerPos = extBankerSelector->GetBankerPos();
	OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(m_Context, ExtensionGameDirection, extGameDirection);
	PlayerPos::Type cardBlocksStartPos = extGameDirection->GetStartCardBlockPos1();


	cardBlocks->SetStartPos(cardBlocksStartPos);
	cardBlocks->SetDrawPos(extGameDirection->GetStartDrawPos());
	cardBlocks->Shuffle();
	
	OBJ_GET_EXT(m_Context, ExtensionGameParam, extGameParam);


    PlayerList playerList = extPlayer->GetPlayerList();

    for(PlayerList::iterator itr = playerList.begin();
        itr != playerList.end();
        ++itr)
    {
        for(int i = 0; i < 12; ++i)
        {
            Card::Type card = extCardBlocks->Draw();
			card = extCardBlocks->OnDrawCard(card, itr->second->GetPos());
            itr->second->AddCard(card);
        }
    }

	// 庄家再抓2张
	CardList cards = extCardBlocks->DrawTwoWhenStart();
	Card::Type lastDrawnCard = Card::Invalid;
	for(int i = 0; i < cards.Count(); ++i)
	{
		Card::Type card = extCardBlocks->OnDrawCard(cards[i], extBankerSelector->GetBankerPos());
		extPlayer->GetPlayer(extBankerSelector->GetBankerPos())->AddCard(card);
		lastDrawnCard = card;
	}

	// 其他人再抓一张
	for(PlayerList::iterator itr = playerList.begin();
		itr != playerList.end();
		++itr)
	{
		if(itr->second->GetPos() != extBankerSelector->GetBankerPos())
		{
			Card::Type card = extCardBlocks->Draw();
			card = extCardBlocks->OnDrawCard(card, itr->second->GetPos());
			itr->second->AddCard(card);
		}

		extPlayer->SortHand(itr->second->GetPos());
	}

 	OBJ_GET_EXT(m_Context, ExtensionInterceptableCards, extIntcCards);
	extIntcCards->SetLatestDrawnCard(lastDrawnCard);
}

void ProcedureDistributeCardImpl::SelectJoker()
{
	/*OBJ_GET_EXT(m_Context, ExtensionHuPattern, extHuPattern);
	OBJ_GET_EXT(m_Context, ExtensionCardBlocks, extCardBlocks);
	OBJ_GET_EXT(m_Context, ExtensionGameDirection, extGameDirection);
	OBJ_GET_EXT(m_Context, ExtensionGameParam, extGameParam);


	int inverseDrawIdx = extGameParam->GetParam(GameParam_JokerIdx);
	if(inverseDrawIdx < 0 || inverseDrawIdx > 6)
	inverseDrawIdx = 6;

	Card::Type inverseDrawCard = extCardBlocks->SpecInverseGetCard(inverseDrawIdx);

	Card::Type jokerCard = Card::GetFollowing(inverseDrawCard);

	jokerCard = extCardBlocks->OnDrawCard(jokerCard, INVALID_PLAYER_POS);

	extHuPattern->AddJoker(jokerCard);
	extHuPattern->SetJokersLimitCount(4);*/
}
