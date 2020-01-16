
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureNormalDrawCard.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionProcedureController.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionPlayer.h"

#include "NotificationNormalDrawCard.h"

#include "Logger.h"
#include "Util.h"


void ProcedureNormalDrawJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	Card::Type card = *(Card::Type *)data[0];
	if(card != Card::Invalid)
		JumpWhenDraw(context);
	else
		JumpWhenCanNotDraw(context);
}

void ProcedureNormalDrawJumpDelegateImpl::JumpWhenCanNotDraw(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::Liquidation);
}

void ProcedureNormalDrawJumpDelegateImpl::JumpWhenDraw(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::AfterDrawCard);
}

void ProcedureNormalDrawCardImpl::OnPrepare()
{
	OBJ_GET_EXT(m_Context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->ClearHistory();
}

void ProcedureNormalDrawCardImpl::OnStart()
{
	GameContext *ctx = m_Context;

	OBJ_GET_EXT(ctx, ExtensionProcedureController, extProcCtrl);
	OBJ_GET_EXT(ctx, ExtensionCardBlocks, extCardBlocks);
	OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(ctx, ExtensionInterceptableCards, extIntcCards);

	Card::Type drawnCard = Card::Invalid;
	if(extCardBlocks->CanDraw())
	{
		Player *activePlayer = extPlayer->GetActivePlayer();

		drawnCard = m_DrawDelegate->OnDraw(m_Context, activePlayer->GetPos());

		// 加入新抽的卡
		activePlayer->AddCard(drawnCard);

		extIntcCards->SetLatestDrawnCard(drawnCard); // 记录最近抓的牌

		OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extConnector);
		// 通知客户端，抽卡了
		extConnector->SendNotification(NotificationNormalDrawCard(activePlayer->GetPos(), drawnCard, true));
	}

	void *pointers[] = {&drawnCard};
	Jump(pointers, ARRAY_LEN(pointers));
}
