#ifndef BEFORE_LIQUIDATION_H
#define BEFORE_LIQUIDATION_H

// 结算之前要执行的

#include "GameContext.h"
#include "ExtensionPlayer.h"
#include "ExtensionLiquidation.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionScoreItemList.h"
#include "SpecCardScoreDelegate.h"
#include "ScoreItemHu.h"
#include "ActionValidator.h"
#include "PlayerStatusDef.h"
#include <set>
#include "ScoreTypeDef.h"



// 查叫
void ChaJiao(GameContext *context)
{
	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
	OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
	const WinnerList &winners = extLiquidation->GetWinners();

	

	std::set<PlayerPos::Type> playerReadyHand; // 能听牌的玩家
	std::set<PlayerPos::Type> playerNotReadyHand; // 未能听牌的玩家
	std::set<PlayerPos::Type> playerNotQueMen; // 定缺玩法下未缺一门玩家
	for(int i = 0; i < context->GetPlayerCount(); ++i)
	{
		if(!winners.Contains(i))
		{
			// 未胡牌的玩家

			if(DefaultTingHandlerImpl().IsReadyHand(context, i, CardStatistic(extPlayer->GetPlayer(i)->GetCards(), extPlayer->GetPlayer(i)->GetInterceptions())))
			{
				// 处于听牌状态,更新最终听牌状态
				playerReadyHand.insert(i);
				extPlayer->GetPlayerStatus(i).SetBool(PlayerStatus_ReadyHand,true);
			}
			else
			{
				playerNotReadyHand.insert(i);
			}

			//未缺一门
			if( extGameParam->IsTrue(GameParam_DingQue)
				&& extQueMen->FilterCards(i, extPlayer->GetPlayer(i)->GetCards()).Count() > 0 )
			{
				playerNotQueMen.insert(i);
			}
		}
	}

	// 把听牌的玩家也算作赢家,避免听牌玩家之间相互结算
	WinnerList fakeWinners = winners;

	for(std::set<PlayerPos::Type>::iterator itrWinner = playerReadyHand.begin();
		itrWinner != playerReadyHand.end();
		++itrWinner)
	{
		fakeWinners.Add(*itrWinner);
	}
	

	// 未听牌的玩家向未胡、能听牌的玩家赔分(最大可能赢的分)
	for(std::set<PlayerPos::Type>::iterator itrWinner = playerReadyHand.begin();
		itrWinner != playerReadyHand.end();
		++itrWinner)
	{
		Player *playerWinner = extPlayer->GetPlayer(*itrWinner);
		SpecScoreInfo scoreInfo = SpecCardScoreDelegate::GetMaxScore(context, playerWinner->GetCards(), playerWinner->GetInterceptions(), 
			*itrWinner, extBankerSelector->GetBankerPos());



		OBJ_GET_EXT(context, ExtensionScoreItemList, extScoreItemList);

		for(std::set<PlayerPos::Type>::iterator itrLoser = playerNotReadyHand.begin();	//未听牌玩家
			itrLoser != playerNotReadyHand.end();
			++itrLoser)
		{
			// 添加结算项
			extScoreItemList->AddItem(*itrWinner, ScoreItem(
				new ScoreItemHuImpl(context, 
				*itrLoser,
				extLiquidation->GetWinners(),
				scoreInfo.score,
				ScoreType_ChaJiao)
				)
			);
		}

		for(std::set<PlayerPos::Type>::iterator itrNotQueMen= playerNotQueMen.begin();	//定缺玩法下未缺一门玩家
			itrNotQueMen != playerNotQueMen.end();
			++itrNotQueMen)
		{
			if(*itrNotQueMen == *itrWinner)
				continue;
			int score = scoreInfo.score;
			if(playerNotReadyHand.find(*itrNotQueMen) == playerNotReadyHand.end())		//已听牌添加2倍结算，未听牌上面已经结算了一次
			{
				score *= 2;
			}
			// 添加结算项
			extScoreItemList->AddItem(*itrWinner, ScoreItem(
				new ScoreItemHuImpl(context, 
				*itrNotQueMen,
				extLiquidation->GetWinners(),
				score,
				ScoreType_ChaJiao)
				)
				);
		}
		
		WinType &winType = extLiquidation->GetWinType();

		int TotalFan = scoreInfo.fan;

		// 记录胡牌番数
		extScoreItemList->SetWinningFan(*itrWinner, TotalFan);
	}

}

// 流局退杠分
void RefoundGangScore(GameContext *context)
{
	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
	const WinnerList &winners = extLiquidation->GetWinners();

	if(!winners.empty())	
		return;

	for(int i = 0; i < context->GetPlayerCount(); ++i)
	{
		PlayerPos::Type playerPos = i;
		if(!winners.Contains(playerPos))
		{
			OBJ_GET_EXT(context, ExtensionScoreItemList, extScoreItemList);
			extScoreItemList->RemoveItem(playerPos, ScoreType_Gang);
			extScoreItemList->RemoveItem(playerPos, ScoreType_MingGang);
			extScoreItemList->RemoveItem(playerPos, ScoreType_AnGang);
			extScoreItemList->RemoveItem(playerPos, ScoreType_BuGang);
		}
	}
}

void DoBeforeLiquidation(GameContext *context)
{
	//ChaJiao(context);
	RefoundGangScore(context);
}

#endif