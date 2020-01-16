/*
游戏过程
*/

#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "GameContext.h"
#include "RefCntObj.h"

class ProcedureImpl;

class ProcedureJumpDelegateImpl
{
public:
	virtual ~ProcedureJumpDelegateImpl(){}

	virtual void Jump(GameContext *context, void **data, int dataCnt) const = 0;
};

class ProcedureJumpDelegate
{
protected:
	RefCntObj<ProcedureJumpDelegateImpl> m_Impl;
public:
	ProcedureJumpDelegate(ProcedureJumpDelegateImpl *impl):m_Impl(impl){}

	void Jump(GameContext *context, void **data, int dataCnt) const
	{
		m_Impl->Jump(context, data, dataCnt);
	}
};

class ProcedureImpl
{
protected:
	int m_Type;
    GameContext *m_Context;
	ProcedureJumpDelegate m_JumpDelegate;
public:
    ProcedureImpl(GameContext *context, const ProcedureJumpDelegate &jumpDelegate):m_Type(0), m_Context(context), m_JumpDelegate(jumpDelegate){}
    virtual ~ProcedureImpl(){}
	void SetType(int type) {m_Type = type;}
	int Type()const{ return m_Type;}
	virtual void OnPrepare(){}
    virtual void OnStart(){}
    virtual void OnEnd(){}

	void Jump(void **data, int dataCnt) const
	{
		m_JumpDelegate.Jump(m_Context, data, dataCnt);
	}
};

class Procedure
{
protected:
    RefCntObj<ProcedureImpl> m_Impl;
public:
    Procedure(ProcedureImpl *impl);
	void SetType(int type) { m_Impl->SetType(type); }
	int Type()const{ return m_Impl->Type();}
	void OnPrepare();
    void OnStart();
    void OnEnd();
	void Jump(void **data, int dataCnt) const
	{
		m_Impl->Jump(data, dataCnt);
	}
};

#endif
