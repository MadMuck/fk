#include "GameContext.h"
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureWaitSendCard.h"
#include "ExtensionPlayer.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionCardBlocks.h"

#include "QuestionWaitDiscard.h"
#include "NotificationDiscardCard.h"

void ProcedureSendCardJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::InterceptCard);
}

class DiscardResponse : public DiscardResponseDelegate
{
public:
    GameContext *ctx;
	ProcedureImpl *m_ProcedureImpl;

	DiscardResponse(GameContext *context, ProcedureImpl *impl) : ctx(context), m_ProcedureImpl(impl)
	{
	
	}

    void OnSendCard(PlayerPos::Type playerPos, Card::Type card) const
    {
        OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
        OBJ_GET_EXT(ctx, ExtensionProcedureController, extProcCtrl);
        OBJ_GET_EXT(ctx, ExtensionInterceptableCards, extIntcCard);

        Player *activePlayer = extPlayer->GetActivePlayer();
        activePlayer->DiscardCard(card);
		extPlayer->SortHand(activePlayer->GetPos());

		OBJ_GET_EXT(ctx, ExtensionCardBlocks, extCardBlocks);
		extCardBlocks->DiscardCard(activePlayer->GetPos(), card);

        extIntcCard->AddInterceptableCard(activePlayer->GetPos(), card);

		m_ProcedureImpl->Jump(NULL, 0);

		OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->SendNotification(NotificationDiscardCard(playerPos, card));
    }
};

void ProcedureWaitSendCardImpl::OnStart()
{
    GameContext *ctx = m_Context;

	RefCntObj<DiscardResponseDelegate> response = RefCntObj<DiscardResponseDelegate>(new DiscardResponse(ctx, this));

    OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);

    Player *activePlayer = extPlayer->GetActivePlayer();
    QuestionWaitDiscard question(activePlayer->GetPos(),
        activePlayer->GetCards(),
        response);

    OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extConnector);
    extConnector->SendQuestion(question);
}
