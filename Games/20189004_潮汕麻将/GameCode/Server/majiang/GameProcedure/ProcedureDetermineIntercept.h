#ifndef PROCEDURE_DETERMINE_INTERCEPT_H
#define PROCEDURE_DETERMINE_INTERCEPT_H

#include "Procedure.h"
#include "Interception.h"
#include "ExtensionInterceptableCards.h"

class ProcedureDetermineInterceptJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;

	virtual int GetNextProcedureType(GameContext *context, const PlayerInterception &actions) const;

	virtual PlayerPos::Type GetNextActivePlayer(GameContext *context, const PlayerInterception &actions, int procedureType) const;

	virtual void JumpAfterAction(GameContext *context, const PlayerInterception &actions) const;
};

class ProcedureDetermineInterceptJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureDetermineInterceptJumpDelegate():ProcedureJumpDelegate(new ProcedureDetermineInterceptJumpDelegateImpl()){}
};

class ProcedureDetermineInterceptImpl : public ProcedureImpl
{
public:
    ProcedureDetermineInterceptImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate){}
    void OnStart();
};

class ProcedureDetermineIntercept : public Procedure
{
public:
    ProcedureDetermineIntercept(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureDetermineInterceptImpl(ctx, delegate)){}
};

#endif
