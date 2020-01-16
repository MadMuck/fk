#ifndef PROCEDURE_READY_HAND_DISCARD_H
#define PROCEDURE_READY_HAND_DISCARD_H

#include "Procedure.h"

class ProcedureReadyHandDiscardJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;
};

class ProcedureReadyHandDiscardJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureReadyHandDiscardJumpDelegate():ProcedureJumpDelegate(new ProcedureReadyHandDiscardJumpDelegateImpl()){}
};

class ProcedureReadyHandDiscardImpl : public ProcedureImpl
{
public:
    ProcedureReadyHandDiscardImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate){}
    void OnStart();
};

class ProcedureReadyHandDiscard : public Procedure
{
public:
    ProcedureReadyHandDiscard(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureReadyHandDiscardImpl(ctx, delegate)){}
};

#endif
