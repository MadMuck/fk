#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureGameStart.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGameDirection.h"
#include "ExtensionPlayer.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionTimer.h"
#include "ExtensionLiquidation.h"

#include "NotificationGameBegin.h"
#include "Util.h"

void StartProcedureDistributeCard(void **data, int dataCnt)
{
	GameContext *context = (GameContext *)data[0];
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcedureController);
	extProcedureController->StartProcedure(ProcedureType::DistributeCard);
}

void ProcedureGameStartJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	OBJ_GET_EXT(context, ExtensionTimer, extTimer);
	void *pointers[] = {context};
	int pointerCnt = ARRAY_LEN(pointers);
	TimerData timerData = TimerData(pointers, pointerCnt, StartProcedureDistributeCard);
	extTimer->Add(2.0f, timerData);
}

void ProcedureGameStartImpl::OnStart()
{
    GameContext *ctx = m_Context;

	OBJ_GET_EXT(m_Context, ExtensionLiquidation, extLiquidation);
	OBJ_GET_EXT(ctx, ExtensionBankerSelector, extBankerSelector);
	OBJ_GET_EXT(ctx, ExtensionGameDirection, extGameDirection);

    // 读取庄家位置

	PlayerPos::Type bankerPos = extBankerSelector->GetBankerSelector()->GetBanker();
	if(bankerPos == PlayerPos::Max)
	{
		bankerPos = Random() % (m_Context->GetPlayerCount());
		extBankerSelector->GetBankerSelector()->SetBanker(bankerPos);
	}

	extGameDirection->GenerateStartCardBlockPos1(bankerPos, 4);

	OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
	extPlayer->SetActivePlayerPos(bankerPos);

	extPlayer->SetDes(ctx->GetPlayerCount(), bankerPos);

	// 通知游戏开始
	OBJ_GET_EXT(ctx, ExtensionGamePlayerConnector, extGamePlayerConnector);
	extGamePlayerConnector->SendNotification(NotificationGameBegin());

	// 切换到发牌阶段
	Jump(NULL, 0);
}
