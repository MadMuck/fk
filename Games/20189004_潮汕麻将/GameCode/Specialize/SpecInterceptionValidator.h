#ifndef SPEC_INTERCEPTION_VALIDATOR_H
#define SPEC_INTERCEPTION_VALIDATOR_H

#include "ActionValidator.h"
#include "ExtensionPlayer.h"
#include "PlayerStatusDef.h"
#include "ExtensionProcedureController.h"
#include "ExtensionActionValidator.h"
#include "ExtensionGameDirection.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionGameParam.h"
#include "ExtensionAbandonedAction.h"
#include "ExtensionQueMen.h"
#include "ExtensionHuPattern.h"
#include "SpecProcedureTypeDef.h"
class SpecInterceptionValidatorImpl : public InterceptionValidatorImpl
{
public:

	SpecInterceptionValidatorImpl(){}

	bool IsValid(GameContext *context, PlayerPos::Type playerPos, Interception::Type type, const CardInfo &cardInfo) const
	{
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		bool ret = true;

		switch(type)
		{
		case Interception::Hu:
			{
				OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
				Player *player = extPlayer->GetPlayer(playerPos);
				//OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
				//if(extQueMen->FilterCards(playerPos, player->GetCards()).Count() > 0)
				//{
				//	ret = false;
				//}
				OBJ_GET_EXT(context, ExtensionGameParam,extGameParam);
				OBJ_GET_EXT(context, ExtensionProcedureController,extProcCtrl);
				
				if(!extGameParam->GetParam(GameParam_PM_KeJiePaoHu) && playerPos != cardInfo.playerPos && extProcCtrl->RunningProcedureType() != ProcedureType_QiangGang)
				{
					ret  = false;
				}
 				OBJ_GET_EXT(context, ExtensionAbandonedAction, extAbandonedAction);
 				if(ret && extAbandonedAction->HasAbandoned(playerPos, Interception::Hu,cardInfo.card))
 				{
 					ret = false;
 				}		

			//	OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
				//OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
			//	CardList jokerlist = extHuPattern->GetJokers();

				//if(!extGameParam->IsTrue(GameParam_DanDiaoJoker12) && 0 == jokerlist.Find(cardInfo.card))		//若没有选择此玩法，最后一张牌为鬼牌，则该张鬼牌不能作为万能牌，只能作为本张牌含义
				//{
				//	CardList cards = extPlayer->GetPlayer(playerPos)->GetCards();

				//	if(playerPos == cardInfo.playerPos && cards.size() == 2 && cards[0] != cards[1])	//自摸
				//		ret = false;
				//	else if(playerPos != cardInfo.playerPos && cards.size() == 1 && cards[0] != cardInfo.card)
				//		ret = false;
				//}
			}
			break;
		case Interception::Peng:
			{
				//OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
				//if(extCardBlocks->GetCardBlocks()->Count() <= 3)	//最后三张牌不能碰
				//	ret = false;
				OBJ_GET_EXT(context, ExtensionAbandonedAction, extAbandonedAction);
				if(extAbandonedAction->HasAbandoned(playerPos, Interception::Peng,cardInfo.card))
				{
					ret = false;
				}
			}
			break;
		}

		if(Interception::MingGang == type ||
			Interception::AnGang == type ||
			Interception::BuGang == type)
		{
			OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
			if(!extCardBlocks->CanDraw())
			{
				// 不能抓牌的话就不能杠
				ret = false;
			}
			OBJ_GET_EXT(context, ExtensionAbandonedAction, extAbandonedAction);
			if(ret && extAbandonedAction->HasAbandoned(playerPos, Interception::MingGang,cardInfo.card) || extAbandonedAction->HasAbandoned(playerPos, Interception::BuGang,cardInfo.card))
			{
				ret = false;
			}
		}
		
		return ret;
	}

};

class SpecInterceptionValidator : public InterceptionValidator
{
public:
	SpecInterceptionValidator():InterceptionValidator(new SpecInterceptionValidatorImpl()){}
};


#endif