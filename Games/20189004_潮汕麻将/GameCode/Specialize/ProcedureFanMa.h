#ifndef PROCEDURE_FAN_MA_H
#define PROCEDURE_FAN_MA_H

#include "Procedure.h"

class ProcedureFanMaJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;
};

class ProcedureFanMaJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureFanMaJumpDelegate():ProcedureJumpDelegate(new ProcedureFanMaJumpDelegateImpl()){}
};

class ProcedureFanMaImpl : public ProcedureImpl
{
public:
	ProcedureFanMaImpl(GameContext *context, const ProcedureJumpDelegate &delegate):ProcedureImpl(context, delegate){}
	void OnStart();
};

class ProcedureFanMa : public Procedure
{
public:
	ProcedureFanMa(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureFanMaImpl(ctx, delegate)){}
};

#endif
