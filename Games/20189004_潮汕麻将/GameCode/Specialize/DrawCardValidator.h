#ifndef SPEC_DRAW_CARD_VALIDATOR_H
#define SPEC_DRAW_CARD_VALIDATOR_H

#include "ExtensionCardBlocks.h"
#include "ExtensionPlayer.h"
#include "ExtensionAbandonedAction.h"
#include "ProcedureNormalDrawCard.h"
#include "ExtensionProcedureController.h"
#include "CardCfg.h"
#include <map>

class SpecDrawCardDelegate : public DrawCardDelegate
{
public:

	Card::Type OnDraw(GameContext *context, PlayerPos::Type playerPos)
	{
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
		Card::Type ret = Card::Invalid;
		if(extProcCtrl->RunningProcedureType() != ProcedureType::DrawCardAfterKong)
		{
			ret = extCardBlocks->Draw();
		}
		else
		{
			ret = extCardBlocks->InverseDraw();
		}
		ret = extCardBlocks->OnDrawCard(ret, playerPos);
		return ret;
	}

	static RefCntObj<DrawCardDelegate> Create()
	{
		return RefCntObj<DrawCardDelegate>(new SpecDrawCardDelegate());
	}
};


/////////////////////////////////////////////////////////////////////
class SpecDrawCardValidator : public DrawCardValidatorImpl
{
	int m_ReservedCardCnt; // ±£¡Ù∂‡…Ÿ≈∆(√Œ)
	std::map<int, int> m_PlayerData;

	int m_SpecialCardDrawnCnt;

public:
	
	SpecDrawCardValidator(const std::map<int, int> &playerData, int reservedCardCnt):
	  m_PlayerData(playerData),
	  m_ReservedCardCnt(reservedCardCnt),
	  m_SpecialCardDrawnCnt(0)
	{}

	bool CanDraw(GameContext *context) const
	{
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		return extCardBlocks->GetCardBlocks()->Count() > m_ReservedCardCnt;
	}

	Card::Type OnDraw(GameContext *context, Card::Type oriCard, PlayerPos::Type playerPos)
	{
		if(playerPos >= PlayerPos::P0 && playerPos < PlayerPos::Max)
		{
			// ∂Ø≈∆Ω‚≥˝∆˙∫˙◊¥Ã¨
			OBJ_GET_EXT(context, ExtensionAbandonedAction, extAbandonedAction);
			extAbandonedAction->Clear(playerPos);
		}

		Card::Type card = oriCard;
		std::map<int, int>::const_iterator itr = m_PlayerData.find(playerPos);
		if(itr != m_PlayerData.end())
		{
			int playerData = itr->second;
			OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
			Player *player = extPlayer->GetPlayer(playerPos);
			bool reset = true;
			if(!player)
			{
				reset = m_SpecialCardDrawnCnt++ == 0;
			}
			else
			{
				reset = player->GetCards().Count() == 0;
			}
			card = GetSpecifiedCard((long int) playerData, card, reset, true);
		}
		return card;
	}
};

#endif