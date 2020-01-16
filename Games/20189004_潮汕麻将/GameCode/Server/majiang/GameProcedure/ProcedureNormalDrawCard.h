#ifndef PROCEDURE_NORMAL_DRAW_CARD_H
#define PROCEDURE_NORMAL_DRAW_CARD_H

#include "Procedure.h"
#include "Interception.h"

class ProcedureNormalDrawJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;

	virtual void JumpWhenCanNotDraw(GameContext *context) const;

	virtual void JumpWhenDraw(GameContext *context) const;
};

class ProcedureNormalDrawJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureNormalDrawJumpDelegate() : ProcedureJumpDelegate(new ProcedureNormalDrawJumpDelegateImpl()){}
};

////////////////////////////////////////////////////////////////////////////


class DrawCardDelegate
{
public:
	virtual ~DrawCardDelegate(){}
	virtual Card::Type OnDraw(GameContext *context, PlayerPos::Type playerPos) = 0;
};

////////////////////////////////////////////////////////////////////////////

class ProcedureNormalDrawCardImpl : public ProcedureImpl
{
protected:
	RefCntObj<DrawCardDelegate> m_DrawDelegate;
public:
    ProcedureNormalDrawCardImpl(GameContext *ctx, const RefCntObj<DrawCardDelegate> &drawDelegate, 
		const ProcedureJumpDelegate &delegate):ProcedureImpl(ctx, delegate), m_DrawDelegate(drawDelegate){}
	void OnPrepare();
    void OnStart();
};

class ProcedureNormalDrawCard : public Procedure
{
public:
    ProcedureNormalDrawCard(GameContext *ctx, const RefCntObj<DrawCardDelegate> &drawDelegate,
		const ProcedureJumpDelegate &delegate):
	Procedure(new ProcedureNormalDrawCardImpl(ctx, drawDelegate, delegate)){}
};

#endif
