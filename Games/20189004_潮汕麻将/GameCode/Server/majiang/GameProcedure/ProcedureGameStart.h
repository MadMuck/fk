#ifndef PROCEDURE_GAME_START_H
#define PROCEDURE_GAME_START_H

#include "Procedure.h"

class ProcedureGameStartJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;
};

class ProcedureGameStartJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureGameStartJumpDelegate():ProcedureJumpDelegate(new ProcedureGameStartJumpDelegateImpl()){}
};

class ProcedureGameStartImpl : public ProcedureImpl
{
public:
	ProcedureGameStartImpl(GameContext *context, const ProcedureJumpDelegate &delegate):ProcedureImpl(context, delegate){}
    void OnStart();
};

class ProcedureGameStart : public Procedure
{
public:
    ProcedureGameStart(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureGameStartImpl(ctx, delegate)){}
};

#endif
