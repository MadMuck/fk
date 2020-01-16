#ifndef PROCEDURE_DISTRIBUTE_CARD_H
#define PROCEDURE_DISTRIBUTE_CARD_H

#include "Procedure.h"

class ProcedureDistributeJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;
};

class ProcedureDistributeCardJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureDistributeCardJumpDelegate():ProcedureJumpDelegate(new ProcedureDistributeJumpDelegateImpl()){}
};

class ProcedureDistributeCardImpl : public ProcedureImpl
{
public:
    ProcedureDistributeCardImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate){}
    void OnStart();

    void DistributeCards();

	void SelectJoker();
};

class ProcedureDistributeCard : public Procedure
{
public:
    ProcedureDistributeCard(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureDistributeCardImpl(ctx, delegate)){}
};

#endif
