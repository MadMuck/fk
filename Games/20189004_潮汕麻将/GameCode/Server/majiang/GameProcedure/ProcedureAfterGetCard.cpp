
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureAfterGetCard.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionProcedureController.h"
#include "ExtensionActionValidator.h"
#include "ExtensionPlayer.h"
#include "ExtensionHuPattern.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionLiquidation.h"
#include "ExtensionAbandonedAction.h"

#include "QuestionInterception.h"
#include "NotificationInterceptionResult.h"
#include "QuestionReadyHandDiscard.h"

#include "ExtensionLogger.h"

#include "Util.h"

void ProcedureAfterGetCardJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	bool result = *(bool *)data[0];
	if(result)
	{
		const PlayerInterception *action = (PlayerInterception *)data[1];
		JumpWithAction(context, *action);
	}
	else
	{
		JumpWhenNoAction(context);
	}

	//OBJ_GET_EXT(context, ExtensionInterceptableCards, extIntcCards);
	//extIntcCards->ClearLatestDrawnCard(); // 清除最近抓的牌
}

void ProcedureAfterGetCardJumpDelegateImpl::JumpWhenNoAction(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::WaitSendCard);
}

void ProcedureAfterGetCardJumpDelegateImpl::JumpWithAction(GameContext *context, const PlayerInterception &action) const
{
	OBJ_GET_EXT(context, ExtensionInterceptableCards, extIntcCards);
	extIntcCards->SetPlayerInterception(action);

	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::DetermineIntercept);
}

//////////////////////////////////////////////////////////////////////////////////////

class AfterGetCardInterceptResponse : public InterceptionResponseDelegate
{
public:
	GameContext *ctx;
	PlayerInterception m_Action;
	ProcedureImpl *m_ProcedureImpl;

	AfterGetCardInterceptResponse(GameContext *context, ProcedureImpl *impl):ctx(context), m_ProcedureImpl(impl){}

	void OnIgnore(PlayerPos::Type playerPos, const InterceptionList &actions)
	{
		// 记录放弃的操作
		OBJ_GET_EXT(ctx, ExtensionAbandonedAction, extAbandonedAction);
		const InterceptionList &abandonedActions = actions;
		for(InterceptionList::const_iterator itr = abandonedActions.begin();
			itr != abandonedActions.end();
			++itr)
		{
			extAbandonedAction->Abandoned(playerPos, itr->type, itr->src.card);
		}

		// 操作结果通知
		OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->SendNotification(NotificationInterceptionResult(playerPos, Interception()));

		bool result = false;
		void *data[] = {&result};
		m_ProcedureImpl->Jump(data, ARRAY_LEN(data));
	}

	void OnAction(PlayerPos::Type pos, const Interception &action, const InterceptionList &actions)
	{
		m_Action[pos] = InterceptionSelection(action, actions);

		bool result = true;
		void *data[] = {&result, &m_Action};
		m_ProcedureImpl->Jump(data, ARRAY_LEN(data));
	}
};

////////////////////////////////////////////////////////////////////////////////////

void ProcedureAfterGetCardImpl::OnStart()
{
	GameContext *ctx = m_Context;
	OBJ_GET_EXT(ctx, ExtensionActionValidator, extActValidator);
	OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(ctx, ExtensionCardBlocks, extCardBlocks);
	OBJ_GET_EXT(ctx, ExtensionHuPattern, extHuPattern);
	OBJ_GET_EXT(ctx, ExtensionInterceptableCards, extIntcCards);

	// 检查抽牌玩家有哪些可执行的动作
	Player *activePlayer = extPlayer->GetActivePlayer();
	if(!activePlayer)
	{
		LOGGER_FILE(ctx, "It seems that players are not initialized");
		return;
	}

	PlayerPos::Type playerPos = activePlayer->GetPos();
	const InterceptionList &intercepted = activePlayer->GetInterceptions();
	const CardList &cardList = activePlayer->GetCards();

	InterceptionList interceptionList;

	CardList cardsToCheck = activePlayer->GetCards();
	if(cardsToCheck.Count() == 0)
	{
		LOGGER_FILE(ctx, "Player does not have any cards in hand");
		return;
	}

	Card::Type latestCard = extIntcCards->GetLatestDrawnCard();

	LOGGER_FILE(ctx, "Player "<<(int)activePlayer->GetPos()<<" find action with hand "<<cardsToCheck<<" and interceptionList "<<
		activePlayer->GetInterceptions()<<" latest draw card "<<(int)latestCard );

	const InterceptionCheckType &interceptionCheckList = extActValidator->GetCheckType(Type());
	for(InterceptionCheckType::const_iterator itr = interceptionCheckList.begin();
		itr != interceptionCheckList.end();
		++itr)
	{
		if(extActValidator->IsInterceptionValid(playerPos, *itr, CardInfo(playerPos, latestCard)))
		{
			interceptionList += extActValidator->HandlerInterception(playerPos, playerPos, 
				cardsToCheck, activePlayer->GetInterceptions(), *itr, latestCard);
		}
	}

	LOGGER_FILE(ctx, "done");

	if(interceptionList.Count() > 0)
	{
		interceptionList.SetPlayerPos(activePlayer->GetPos());
		// 询问玩家执行何种操作，等待反馈
		RefCntObj<InterceptionResponseDelegate> response(new AfterGetCardInterceptResponse(ctx, this));
		QuestionInterception question(activePlayer->GetPos(), interceptionList, response);
		OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extConnector);

		LOGGER_FILE(ctx, "Player "<<(int)activePlayer->GetPos()<<" have actions "<<interceptionList);

		extConnector->SendQuestion(question);
	}
	else
	{
		bool result = false;
		void *data[] = {&result};
		Jump(data, ARRAY_LEN(data));
	}
}