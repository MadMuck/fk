
#include "ProcedureType.h"
#include "ProcedureLiquidation.h"
#include "ExtensionProcedureController.h"
#include "ExtensionLiquidation.h"
#include "ExtensionPlayer.h"
#include "ExtensionHuPattern.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionBankerSelector.h"
#include "Liquidation.h"

#include "NotificationLiquidation.h"

void ProcedureLiquidationJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::Idle);
}


void ProcedureLiquidationImpl::OnStart()
{
    GameContext *ctx = m_Context;
    OBJ_GET_EXT(m_Context, ExtensionLiquidation, extLiquidation);
    OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
    OBJ_GET_EXT(m_Context, ExtensionHuPattern, extHuPattern);
	OBJ_GET_EXT(m_Context, ExtensionBankerSelector, extBankerSelector);

//	extLiquidation->ScoreItemListStatistic();//结算项分数统计
    Liquidation &liquidation = extLiquidation->GetLiquidation();
    liquidation.Clear();

	liquidation.Prepare(extBankerSelector->GetBankerPos(), m_Context->GetPlayerCount());

	const PlayerList &players = extPlayer->GetPlayerList();
	for(PlayerList::const_iterator itr = players.begin();
		itr != players.end();
		++itr)
	{
		PlayerPos::Type playerPos = itr->second->GetPos();

		ScoreItemList scoreItemList = m_ItemListDelegate->GetScoreItemList(playerPos);

		for(ScoreItemList::iterator itr2 = scoreItemList.begin(); itr2 != scoreItemList.end(); ++itr2)
		{
			liquidation.Add(playerPos, *itr2);
		}
	}

	if(extLiquidation->GetWinners().size() == 0)
	{
		extLiquidation->AddDrawCount();
	}

	extLiquidation->AddGameCount();

 // 发出结算消息,在调用平台ChangeUserPoint接口后调用
//     NotificationLiquidation notification(liquidation);
//     OBJ_GET_EXT(m_Context, ExtensionGamePlayerConnector, extGamePlayerConnector);
//     extGamePlayerConnector->SendNotification(notification);

	Jump(NULL, 0);
}