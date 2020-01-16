#ifndef PROCEDURE_QIANG_GANG_H
#define PROCEDURE_QIANG_GANG_H	

#include "ProcedureInterceptCard.h"
#include "Interception.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGameDirection.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionPlayer.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionLiquidation.h"
#include "ExtensionTimer.h"
#include "ExtensionInvalidInterception.h"
#include "NotificationInterceptionResult.h"

#include "Util.h"

class ProcedureQiangGangJumpDelegateImpl : public ProcedureInterceptCardJumpDelegateImpl
{
public:

	void JumpWhenNoAction(GameContext *context) const
	{
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcedureController);
		extProcedureController->StartProcedure(ProcedureType::DrawCardAfterKong);
	}
};

class ProcedureQiangGangJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureQiangGangJumpDelegate():ProcedureJumpDelegate(new ProcedureQiangGangJumpDelegateImpl()){}
};

class ProcdureQiangGangImpl : public ProcedureInterceptCardImpl
{
public:
	ProcdureQiangGangImpl(GameContext *context):ProcedureInterceptCardImpl(context, ProcedureQiangGangJumpDelegate()){}

	void OnStart()
	{
		OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
		const InterceptionList &interceptions = extPlayer->GetActivePlayer()->GetInterceptions();
		const Interception &gangAction = *interceptions.rbegin();

		// 把杠的牌加入到可拦截列表
		OBJ_GET_EXT(m_Context, ExtensionInterceptableCards, extIntcCards);
		extIntcCards->AddInterceptableCard(extPlayer->GetActivePlayerPos(), gangAction.src.card);

		OBJ_GET_EXT(m_Context, ExtensionActionValidator, extActValidator);
		InterceptionCheckType checkType;
		checkType.Add(Interception::Hu); // 仅检查胡牌
		extActValidator->SetCheckType(Type(), checkType);

		ProcedureInterceptCardImpl::OnStart();
	}
};

class ProcedureQiangGang : public Procedure
{
public:
	ProcedureQiangGang(GameContext *context):Procedure(new ProcdureQiangGangImpl(context)){}
};

#endif