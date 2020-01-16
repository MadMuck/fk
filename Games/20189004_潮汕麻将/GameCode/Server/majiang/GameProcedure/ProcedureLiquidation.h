#ifndef PROCEDURE_LIQUIDATION_H
#define PROCEDURE_LIQUIDATION_H

#include "Procedure.h"
#include "Interception.h"
#include "Liquidation.h"
#include "HuPatternMatch.h"

class ProcedureLiquidationJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;
};

class ProcedureLiquidationJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureLiquidationJumpDelegate() : ProcedureJumpDelegate(new ProcedureLiquidationJumpDelegateImpl()){}
};


class ProcedureLiquidationImpl : public ProcedureImpl
{
	RefCntObj<ScoreItemListDelegate> m_ItemListDelegate;
public:
    ProcedureLiquidationImpl(GameContext *ctx, const RefCntObj<ScoreItemListDelegate> &scoreItemDelegate, 
		const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate),
		m_ItemListDelegate(scoreItemDelegate){}

    void OnStart();
};

class ProcedureLiquidation : public Procedure
{
public:
    ProcedureLiquidation(GameContext *ctx, const RefCntObj<ScoreItemListDelegate> &scoreItemDelegate,
		const ProcedureJumpDelegate &delegate):Procedure(new ProcedureLiquidationImpl(ctx, scoreItemDelegate, delegate)){}
};

#endif
