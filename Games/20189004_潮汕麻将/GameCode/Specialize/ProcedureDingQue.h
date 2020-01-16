#ifndef PROCEDURE_DING_QUE_H
#define PROCEDURE_DING_QUE_H	

#include "Procedure.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionQueMen.h"
#include "QuestionDingQue.h"
#include "ExtensionTimer.h"

#include "Util.h"

#include "SpecProcedureTypeDef.h"
#include "ProcedureType.h"

void StartProcedureAfterDrawCard(void **data, int dataCnt)
{
	GameContext *context = (GameContext *)data[0];
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::AfterDrawCard);
}

class ProcedureDingQueJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
public:

	void Jump(GameContext *context, void **data, int dataCnt) const
	{
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->SendNotification(NotificationFirstRun());

		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		void *pointers[] = {context};
		int pointerCnt = ARRAY_LEN(pointers);
		TimerData timerData = TimerData(pointers, pointerCnt, StartProcedureAfterDrawCard);
		extTimer->AddToQueue(0, timerData);	
	}
};

class ProcedureDingQueJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureDingQueJumpDelegate():ProcedureJumpDelegate(new ProcedureDingQueJumpDelegateImpl()){}
};


class DingQueRspDelegate : public DingQueResponseDelegate
{
	GameContext *m_GameContext;
	ProcedureJumpDelegate m_JumpDelegate;
	
public:
	DingQueRspDelegate(GameContext *context, const ProcedureJumpDelegate &jumpDelegate):
	  m_GameContext(context),m_JumpDelegate(jumpDelegate){}

	void OnSelect(PlayerPos::Type playerPos, int type)
	{
		OBJ_GET_EXT(m_GameContext, ExtensionQueMen, extQueMen);
		extQueMen->Set(playerPos, type);
		int determinedCnt = 0;
		for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
		{
			if(extQueMen->HasDetermined(i))
				++determinedCnt;
		}

		if(determinedCnt == m_GameContext->GetPlayerCount())
		{
			// ¶¨È±½áÊø
			m_JumpDelegate.Jump(m_GameContext, NULL, 0);
		}
	}
};

class ProcdureDingQueImpl : public ProcedureImpl
{
public:
	ProcdureDingQueImpl(GameContext *context):ProcedureImpl(context, ProcedureDingQueJumpDelegate()){}

	void OnStart()
	{
		OBJ_GET_EXT(m_Context, ExtensionGameParam, extGameParam);
		if(extGameParam->IsTrue(GameParam_DingQue))
		{
			RefCntObj<DingQueResponseDelegate> rspDelegate(new DingQueRspDelegate(m_Context, m_JumpDelegate));

			for(int i = 0; i < m_Context->GetPlayerCount(); ++i)
			{
				OBJ_GET_EXT(m_Context, ExtensionGamePlayerConnector, extGamePlayerConnector);
				extGamePlayerConnector->SendQuestion(QuestionDingQue(i, rspDelegate));
			}
		}
		else
		{
			Jump(NULL, 0);
		}
	}
};

class ProcedureDingQue : public Procedure
{
public:
	ProcedureDingQue(GameContext *context):Procedure(new ProcdureDingQueImpl(context)){}
};

#endif