#include "GameContext.h"
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureReadyHandDiscard.h"
#include "ExtensionPlayer.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionLiquidation.h"
#include "ExtensionBankerSelector.h"

#include "QuestionReadyHandDiscard.h"
#include "NotificationDiscardCard.h"

void ProcedureReadyHandDiscardJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::InterceptCard);
}

class ReadyHandDiscardResponse : public ReadyHandDiscardResponseDelegate
{
public:
    GameContext *ctx;
	ProcedureImpl *m_ProcedureImpl;

	ReadyHandDiscardResponse(GameContext *context, ProcedureImpl *impl) : ctx(context), m_ProcedureImpl(impl)
	{
	
	}

    void OnSendCard(PlayerPos::Type playerPos, Card::Type card) const
    {
        OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
        OBJ_GET_EXT(ctx, ExtensionProcedureController, extProcCtrl);
        OBJ_GET_EXT(ctx, ExtensionInterceptableCards, extIntcCard);

        Player *player = extPlayer->GetPlayer(playerPos);
        player->DiscardCard(card);
		extPlayer->SortHand(player->GetPos());

		OBJ_GET_EXT(ctx, ExtensionCardBlocks, extCardBlocks);
		extCardBlocks->DiscardCard(player->GetPos(), card);

        extIntcCard->AddInterceptableCard(playerPos, card);

		OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->SendNotification(NotificationDiscardCard(playerPos, card));

		ReadyHandSelection readyHandSelection = extIntcCard->GetReadyHandSelection(playerPos);
		// 只保留打出的牌对应的听牌列表
		CardStat cardStat = readyHandSelection[card]; 
		readyHandSelection.clear();
		readyHandSelection[card] = cardStat;
		extIntcCard->SetReadyHandSelection(playerPos, readyHandSelection);

		m_ProcedureImpl->Jump(NULL, 0);
    }
};

void ProcedureReadyHandDiscardImpl::OnStart()
{
    GameContext *ctx = m_Context;

	RefCntObj<ReadyHandDiscardResponseDelegate> response(new ReadyHandDiscardResponse(ctx, this));

    OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(ctx, ExtensionInterceptableCards, extIntcCards);

	Player *activePlayer = extPlayer->GetActivePlayer();

	CardList tmpCards = activePlayer->GetCards();
	const InterceptionList &interceptions = activePlayer->GetInterceptions();

	std::map<Card::Type, CardStat> cardsToWin = extIntcCards->GetReadyHandSelection(activePlayer->GetPos());
	CardList cardCandidate;
	for(std::map<Card::Type, CardStat>::iterator itr = cardsToWin.begin();
		itr != cardsToWin.end();
		++itr)
	{
		cardCandidate.Add(itr->first);
	}

	QuestionReadyHandDiscardData quesitionData;
	quesitionData.m_PlayerPos = activePlayer->GetPos();
	quesitionData.m_Cards = cardCandidate;

	QuestionReadyHandDiscard question(quesitionData, response);

    OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extConnector);
    extConnector->SendQuestion(question);
}
