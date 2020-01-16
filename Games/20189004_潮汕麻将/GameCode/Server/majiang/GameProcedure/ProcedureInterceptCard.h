#ifndef PROCEDURE_INTERCEPT_CARD_H
#define PROCEDURE_INTERCEPT_CARD_H

#include "Procedure.h"
#include "Interception.h"
#include "ExtensionInterceptableCards.h"
#include "QuestionInterception.h"

// 跳转代理
class ProcedureInterceptCardJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:
	void Jump(GameContext *context, void **data, int dataCnt) const;

	virtual void JumpWhenNoAction(GameContext *context) const;

	virtual void JumpAfterAction(GameContext *context, const PlayerInterception &playerActions) const;
};

class ProcedureInterceptCardJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureInterceptCardJumpDelegate():ProcedureJumpDelegate(new ProcedureInterceptCardJumpDelegateImpl()){}
};

// 询问发送代理（前置声明）
class InterceptCardSendQuestionDelegate;

// 询问处理代理
class InterceptCardQuestionDelegate
{
public:
	virtual ~InterceptCardQuestionDelegate(){}

	virtual void Init(){}

	virtual bool OnQuestionResponsed(GameContext *context, PlayerPos::Type playerPos, 
		const Interception &action, const InterceptionList &interceptions,
		const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate) = 0;

	virtual bool OnQuestion(GameContext *context, PlayerPos::Type playerPos, const InterceptionList &interceptions, 
		const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate) = 0;

	virtual bool OnQuestionFinish(GameContext *context, const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate) = 0;

	virtual bool IsResponseFinished() const = 0;

	virtual bool PriorityGreater(GameContext *context, PlayerPos::Type srcPlayerPos, PlayerPos::Type playerPos1, int interceptionType1, PlayerPos::Type playerPos2, int interceptionType2) const = 0;

	virtual bool PriorityEqual(GameContext *context, PlayerPos::Type srcPlayerPos, PlayerPos::Type playerPos1, int interceptionType1, PlayerPos::Type playerPos2, int interceptionType2) const = 0;
};

// 询问发送代理
class InterceptCardSendQuestionDelegate
{
	RefCntObj<InterceptionResponseDelegate> m_ResponseDelegate;
public:
	InterceptCardSendQuestionDelegate(const RefCntObj<InterceptionResponseDelegate> &responseDelegate);
	void SendQuestion(GameContext *context, PlayerPos::Type playerPos, const InterceptionList &interceptions) const;
};

// 拦牌过程
class ProcedureInterceptCardImpl : public ProcedureImpl
{
protected:
	RefCntObj<InterceptCardQuestionDelegate> m_QuestionDelegate;
public:
    ProcedureInterceptCardImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate);
    void OnStart();
	void SetQuestionDelegate(const RefCntObj<InterceptCardQuestionDelegate> &delegate);
	virtual void OnCheckFinish(bool haveAction);
};

class ProcedureInterceptCard : public Procedure
{
public:
    ProcedureInterceptCard(GameContext *ctx, const ProcedureJumpDelegate &delegate):Procedure(new ProcedureInterceptCardImpl(ctx, delegate)){}
	void SetQuestionDelegate(const RefCntObj<InterceptCardQuestionDelegate> &delegate);
};

#endif
