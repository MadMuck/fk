#ifndef PROCEDURE_AFTER_GET_CARD_H
#define PROCEDURE_AFTER_GET_CARD_H

#include "Procedure.h"
#include "Interception.h"
#include "ExtensionInterceptableCards.h"

class ProcedureAfterGetCardJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;

	virtual void JumpWhenNoAction(GameContext *context) const;

	virtual void JumpWithAction(GameContext *context, const PlayerInterception &actions) const;
};

class ProcedureAfterGetCardJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureAfterGetCardJumpDelegate():ProcedureJumpDelegate(new ProcedureAfterGetCardJumpDelegateImpl()){}
};

class ProcedureAfterGetCardImpl : public ProcedureImpl
{
public:
	ProcedureAfterGetCardImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate){}
	void OnStart();
};

class ProcedureAfterGetCard : public Procedure
{
public:
	ProcedureAfterGetCard(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureAfterGetCardImpl(ctx, delegate)){}
};

#endif