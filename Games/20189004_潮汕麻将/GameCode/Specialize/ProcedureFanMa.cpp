#include "ProcedureType.h"
#include "ProcedureFanMa.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionGameDirection.h"
#include "ExtensionPlayer.h"
#include "ExtensionTimer.h"
#include "ExtensionGameParam.h"
#include "ExtensionFanMa.h"
#include "ExtensionProcedureController.h"
#include "ExtensionLiquidation.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionLogger.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionInterceptableCards.h"
#include "AdapterUtil.h"
#include "upgrademessage.h"
#include "CardStatistic.h"

#include "Util.h"
#include "GameDef.h"

void StartProcedureLiquidation(void **data, int dataCnt)
{
	GameContext *context = (GameContext *)data[0];
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcedureController);
	extProcedureController->StartProcedure(ProcedureType::Liquidation);
}

void ProcedureFanMaJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	OBJ_GET_EXT(context, ExtensionTimer, extTimer);
	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);

	bool bResult = *(bool *)data[0];

	void *pointers[] = {context};
	int pointerCnt = ARRAY_LEN(pointers);
	TimerData timerData = TimerData(pointers, pointerCnt, StartProcedureLiquidation);
	float time = 0;
	if(bResult)
		time = extGameParam->GetParam(GameParam_FanMaTime) * 1.0f;
	extTimer->Add(time, timerData);
}

void ProcedureFanMaImpl::OnStart()
{
	GameContext *context = m_Context;

	OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
	OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
	OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
	OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(context, ExtensionFanMa, extFanMa);
	OBJ_GET_EXT(context, ExtensionInterceptableCards, extIntcCards);

	WinnerList winnerList = extLiquidation->GetWinners();


//	todo::修改翻马逻辑--当前由开房玩法指定翻几马
	int FanMa = 0;
	if(extGameParam->GetParam(GameParam_bFanMa))
		FanMa = extGameParam->GetParam(GameParam_FanMaNum);

	bool bResult = false;

	if(FanMa > 0 && !winnerList.empty() /*&& extCardBlocks->CanDraw()*/)
	{
			//抢杠之后不翻马
		WinType wintype = extLiquidation->GetWinType();
		if(wintype.GetWinType(winnerList[0]).HasType(WIN_TYPE_QIANG_GANG))
		{
			FanMa = 0;
		}
		//bResult = true;
		//if(wintype.GetWinType(winnerList[0]).HasType(WIN_TYPE_GANG_BAO))
		//	PaiTouCount /= 2;

		//if(extGameParam->IsTrue(GameParam_QiangGangDouble) && wintype.GetWinType(winnerList[0]).HasType(WIN_TYPE_QIANG_GANG))
		//	PaiTouCount *= 2;

		//if(extGameParam->IsTrue(GameParam_TianDiHuDouble))
		//{
		//	if(wintype.GetWinType(winnerList[0]).HasType(WIN_TYPE_TIAN_HU) || wintype.GetWinType(winnerList[0]).HasType(WIN_TYPE_DI_HU))
		//		PaiTouCount *= 2;
		//}

	//	Card::Type lastPaiTou = extIntcCards->GetLatestDrawnCard();
		PaiTou msg;
		for(int i = 0; i < FanMa; ++i)
		{
			Card::Type card = extCardBlocks->Draw();
			//Card::Type maCard = extCardBlocks->OnDrawCard(card, INVALID_PLAYER_POS);
			extFanMa->AddMaCard(card);
			msg.m_byPaiTou[i] = card;
		}

		CardList MaCards = extFanMa->GetMaCards();

		for(auto itr_Winner = winnerList.begin(); itr_Winner != winnerList.end(); ++itr_Winner)
		{
			int idistance = extFanMa->GetDistance(*itr_Winner, extBankerSelector->GetBankerPos());
			 idistance = extPlayer->GetDes(*itr_Winner);
			 //当前赢家能中的马
			CardList MaCardToDistance = extFanMa->GetMaCardToDistance(idistance);
			for(auto itr_MaCard = MaCards.begin(); itr_MaCard != MaCards.end(); ++itr_MaCard)
			{
				if(MaCardToDistance.Find(*itr_MaCard) != 0)
					extFanMa->AddZhongMaData(*itr_Winner, *itr_MaCard);
			}

			LOGGER_FILE(context, "player " << (int)extPlayer->GetPlayer(*itr_Winner)->GetPos() << " distance = " << idistance << ", MaCardToDistance = " << MaCardToDistance
				<<" , zhong ma count = "<< extFanMa->GetZhongMaCount(*itr_Winner));
		}

		for(int i = 0; i < context->GetPlayerCount(); ++i)
		{
			CardStatistic stat(extFanMa->GetZhongMaData(i));

			CardStatistic::ValuesType values = stat.GetStat().Values();

			int idx = 0;

			for(auto itr = values.begin(); itr != values.end(); ++itr)
			{
				if(idx < MAX_PAI_TOU)
					msg.m_byZhongPaiTou[i][idx++] = *itr; 
			}
		}

		extGameRunningData->SendDataToAll(&msg, sizeof(msg), THING_PAI_TOU);


		LOGGER_FILE(context, "banker pos = " << (int)extBankerSelector->GetBankerPos() << ", macard = "<< MaCards);
	}
	
	// 翻马后跳转到结算流程
	void *pointers[] = {&bResult};
	Jump(pointers, ARRAY_LEN(pointers));
}
