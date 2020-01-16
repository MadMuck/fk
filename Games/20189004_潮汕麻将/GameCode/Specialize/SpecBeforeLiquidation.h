#ifndef BEFORE_LIQUIDATION_H
#define BEFORE_LIQUIDATION_H

// ����֮ǰҪִ�е�

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



// ���
void ChaJiao(GameContext *context)
{
	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(context, ExtensionBankerSelector, extBankerSelector);
	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
	OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
	OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
	const WinnerList &winners = extLiquidation->GetWinners();

	

	std::set<PlayerPos::Type> playerReadyHand; // �����Ƶ����
	std::set<PlayerPos::Type> playerNotReadyHand; // δ�����Ƶ����
	std::set<PlayerPos::Type> playerNotQueMen; // ��ȱ�淨��δȱһ�����
	for(int i = 0; i < context->GetPlayerCount(); ++i)
	{
		if(!winners.Contains(i))
		{
			// δ���Ƶ����

			if(DefaultTingHandlerImpl().IsReadyHand(context, i, CardStatistic(extPlayer->GetPlayer(i)->GetCards(), extPlayer->GetPlayer(i)->GetInterceptions())))
			{
				// ��������״̬,������������״̬
				playerReadyHand.insert(i);
				extPlayer->GetPlayerStatus(i).SetBool(PlayerStatus_ReadyHand,true);
			}
			else
			{
				playerNotReadyHand.insert(i);
			}

			//δȱһ��
			if( extGameParam->IsTrue(GameParam_DingQue)
				&& extQueMen->FilterCards(i, extPlayer->GetPlayer(i)->GetCards()).Count() > 0 )
			{
				playerNotQueMen.insert(i);
			}
		}
	}

	// �����Ƶ����Ҳ����Ӯ��,�����������֮���໥����
	WinnerList fakeWinners = winners;

	for(std::set<PlayerPos::Type>::iterator itrWinner = playerReadyHand.begin();
		itrWinner != playerReadyHand.end();
		++itrWinner)
	{
		fakeWinners.Add(*itrWinner);
	}
	

	// δ���Ƶ������δ���������Ƶ�������(������Ӯ�ķ�)
	for(std::set<PlayerPos::Type>::iterator itrWinner = playerReadyHand.begin();
		itrWinner != playerReadyHand.end();
		++itrWinner)
	{
		Player *playerWinner = extPlayer->GetPlayer(*itrWinner);
		SpecScoreInfo scoreInfo = SpecCardScoreDelegate::GetMaxScore(context, playerWinner->GetCards(), playerWinner->GetInterceptions(), 
			*itrWinner, extBankerSelector->GetBankerPos());



		OBJ_GET_EXT(context, ExtensionScoreItemList, extScoreItemList);

		for(std::set<PlayerPos::Type>::iterator itrLoser = playerNotReadyHand.begin();	//δ�������
			itrLoser != playerNotReadyHand.end();
			++itrLoser)
		{
			// ��ӽ�����
			extScoreItemList->AddItem(*itrWinner, ScoreItem(
				new ScoreItemHuImpl(context, 
				*itrLoser,
				extLiquidation->GetWinners(),
				scoreInfo.score,
				ScoreType_ChaJiao)
				)
			);
		}

		for(std::set<PlayerPos::Type>::iterator itrNotQueMen= playerNotQueMen.begin();	//��ȱ�淨��δȱһ�����
			itrNotQueMen != playerNotQueMen.end();
			++itrNotQueMen)
		{
			if(*itrNotQueMen == *itrWinner)
				continue;
			int score = scoreInfo.score;
			if(playerNotReadyHand.find(*itrNotQueMen) == playerNotReadyHand.end())		//���������2�����㣬δ���������Ѿ�������һ��
			{
				score *= 2;
			}
			// ��ӽ�����
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

		// ��¼���Ʒ���
		extScoreItemList->SetWinningFan(*itrWinner, TotalFan);
	}

}

// �����˸ܷ�
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