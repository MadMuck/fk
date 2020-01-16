#ifndef SPEC_PROCEDURE_JUMP_DELEGATE_H
#define SPEC_PROCEDURE_JUMP_DELEGATE_H

#include "SpecInterceptionValidator.h"

#include "ProcedureDistributeCard.h"
#include "ProcedureAfterGetCard.h"
#include "ProcedureNormalDrawCard.h"
#include "ProcedureDetermineIntercept.h"
#include "ProcedureLiquidation.h"
#include "ProcedureReadyHandDiscard.h"
#include "ExtensionPlayer.h"
#include "ExtensionGameDirection.h"
#include "ExtensionProcedureController.h"
#include "ExtensionTimer.h"

#include "ExtensionTimer.h"


#include "SpecProcedureTypeDef.h"

//修改发牌之后的跳转， 跳到换张流程
class SpecDistributeCardJumpDelegateImpl : public ProcedureDistributeJumpDelegateImpl
{
	static void JumpToHuanZhang(void **data, int dataCnt)
	{
		GameContext *context = (GameContext *)data[0];
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcedureController);
		extProcedureController->StartProcedure(ProcedureType_HuanZhang);
	}
public:
	void Jump(GameContext *context, void **data, int dataCnt) const
	{
		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		void *pointers[] = {context};
		extTimer->Add(0.0f, TimerData(pointers, ARRAY_LEN(pointers), JumpToHuanZhang));
	}
};

class SpecDistributeCardJumpDelegate : public ProcedureJumpDelegate
{
public:
	SpecDistributeCardJumpDelegate():ProcedureJumpDelegate(new SpecDistributeCardJumpDelegateImpl()){}
};


// 修改结算过程后的跳转， 增加延迟
class SpecLiquidationJumpDelegateImpl : public ProcedureLiquidationJumpDelegateImpl
{
public:
	static void DelayJump(void **data, int dataCnt)
	{
		GameContext *context = (GameContext *)data[0];
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
		extProcCtrl->StartProcedure(ProcedureType::Idle);
	}

	void Jump(GameContext *context, void **data, int dataCnt) const
	{
		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		void *pointers[] = {context};
		extTimer->AddToQueue(0, TimerData(pointers, 1, DelayJump));
	}
};

class SpecLiquidationJumpDelegate : public ProcedureJumpDelegate
{
public:
	SpecLiquidationJumpDelegate():ProcedureJumpDelegate(new SpecLiquidationJumpDelegateImpl()){}
};





// 修改确定拦牌结果时的跳转
class SpecProcedureDetermineInterceptJumpDelegateImpl : public ProcedureDetermineInterceptJumpDelegateImpl
{
public:

	PlayerPos::Type GetNextActivePlayer(GameContext *context, const PlayerInterception &actions, int procedureType) const
	{
		PlayerPos::Type ret = ProcedureDetermineInterceptJumpDelegateImpl::GetNextActivePlayer(context, actions, procedureType);
		if(actions.begin()->second.selected.type == Interception::Hu &&
			procedureType != ProcedureType::Liquidation)
		{
			OBJ_GET_EXT(context, ExtensionGameDirection, extGameDir);
			OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
			ret = extGameDir->GetNextPos(actions.begin()->first);
		}
		return ret;
	}

	int GetNextProcedureType(GameContext *context, const PlayerInterception &actions) const
	{
		int procedureType = ProcedureDetermineInterceptJumpDelegateImpl::GetNextProcedureType(context, actions);
		Interception::Type actionType = actions.begin()->second.selected.type;
		if(actionType == Interception::BuGang /*|| actionType == Interception::YangMa*/)
		{
			procedureType = ProcedureType_QiangGang;
		}
		if(actionType == Interception::Hu)
		{
			procedureType = ProcedureType_FanMa;
		}
		return procedureType;
	}
};

class SpecProcedureDetermineInterceptJumpDelegate : public ProcedureJumpDelegate
{
public:
	SpecProcedureDetermineInterceptJumpDelegate():ProcedureJumpDelegate(new SpecProcedureDetermineInterceptJumpDelegateImpl()){}
};

#endif