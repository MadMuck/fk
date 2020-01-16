#ifndef PROCEDURE_WAIT_SEND_CARD_H
#define PROCEDURE_WAIT_SEND_CARD_H

#include "Procedure.h"

class ProcedureSendCardJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;
};

class ProcedureSendCardJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureSendCardJumpDelegate():ProcedureJumpDelegate(new ProcedureSendCardJumpDelegateImpl()){}
};

class ProcedureWaitSendCardImpl : public ProcedureImpl
{
public:
    ProcedureWaitSendCardImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate){}
    void OnStart();
};

class ProcedureWaitSendCard : public Procedure
{
public:
    ProcedureWaitSendCard(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureWaitSendCardImpl(ctx, delegate)){}
};

#endif
