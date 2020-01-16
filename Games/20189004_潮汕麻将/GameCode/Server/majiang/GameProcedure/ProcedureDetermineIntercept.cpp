#include "GameContext.h"
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureDetermineIntercept.h"

#include "ExtensionGameDirection.h"
#include "ExtensionInterceptableCards.h"
#include "ExtensionActionValidator.h"
#include "ExtensionPlayer.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionHuPattern.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionLiquidation.h"
#include "ExtensionLogger.h"
#include "ExtensionGameDirection.h"
#include "ExtensionCardBlocks.h"

#include "QuestionInterception.h"
#include "NotificationInterceptionResult.h"

#include "PlayerStatusDef.h"

#include "Util.h"

#include <map>
#include <list>
#include <set>

void ProcedureDetermineInterceptJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	const PlayerInterception *actions = (PlayerInterception *)data[0];
	JumpAfterAction(context, *actions);
}

int ProcedureDetermineInterceptJumpDelegateImpl::GetNextProcedureType(GameContext *context, const PlayerInterception &actions) const
{
	int procedureType = ProcedureType::WaitSendCard;

	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	const ProcedureController::JobList &history = extProcCtrl->GetProcedureHistory();
	if(history.size() > 1 && *(++history.begin()) == ProcedureType::InterceptCard)
	{
		procedureType = ProcedureType::AfterIntercept;
	}

	switch(actions.begin()->second.selected.type)
	{
	case Interception::MingGang:
	case Interception::AnGang:
	case Interception::BuGang:
//	case Interception::YangMa:
		procedureType = ProcedureType::DrawCardAfterKong;
		break;
	case Interception::Ting:
	case Interception::ChiTing:
	case Interception::PengTing:
	case Interception::ZhanTing:
		procedureType = ProcedureType::ReadyHandDiscard;
		break;
	case Interception::Feng:
		procedureType = ProcedureType::NormalDrawCard;
		break;
	case Interception::Hu:
		procedureType = ProcedureType::Liquidation;
		break;
	}

	return procedureType;
}

PlayerPos::Type ProcedureDetermineInterceptJumpDelegateImpl::GetNextActivePlayer(GameContext *context, const PlayerInterception &actions, int procedureType) const
{
	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	PlayerPos::Type nextPos = extPlayer->GetActivePlayerPos();
	if(procedureType != ProcedureType::Liquidation)
	{
		// 非胡牌情况下， 拦牌玩家接着行动
		nextPos = actions.begin()->first;
	}
	return nextPos;
}

void ProcedureDetermineInterceptJumpDelegateImpl::JumpAfterAction(GameContext *context, const PlayerInterception &actions) const
{
	int procedureType = GetNextProcedureType(context, actions);

	OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
	PlayerPos::Type nextPos = GetNextActivePlayer(context, actions, procedureType);
	if(extPlayer->GetActivePlayerPos() != nextPos)
	{
		LOGGER_FILE(context, " active player changed " <<(int)extPlayer->GetActivePlayerPos() <<" -> "<<(int)nextPos);
		extPlayer->SetActivePlayerPos(nextPos);
	}
	
	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure((int)procedureType);
}

void ProcedureDetermineInterceptImpl::OnStart()
{
    OBJ_GET_EXT(m_Context, ExtensionInterceptableCards, extIntcCards);
	OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(m_Context, ExtensionLiquidation, extLiquidation);

	PlayerInterception playerAction = extIntcCards->GetPlayerInterception();

	bool someOneWin = false;

	std::map<PlayerPos::Type, PlayerPos::Type> newWinnerAndLoser;

	std::map<PlayerPos::Type, AppendInterceptionInfo> appendingActions;

	PlayerPos::Type srcPlayer = PlayerPos::Max;

	for(PlayerInterception::const_iterator itr = playerAction.begin();
		itr != playerAction.end();
		++itr)
	{
		srcPlayer = itr->second.selected.src.playerPos;

		const Interception &action = itr->second.selected;
		Player *player = extPlayer->GetPlayer(itr->first);

		if(action.type != Interception::Invalid)
		{
			if(action.type != Interception::Hu && 
				action.type != Interception::Ting)
			{
				CardList cards = action.cards;
				if(action.src.playerPos != player->GetPos())
				{
					cards.Remove(action.src.card, 1);
				}
				player->RemoveCards(cards);
			}

			if(action.type != Interception::Ting)
			{
				Interception appendingAction = action;
				if(action.type > Interception::Ting && action.type < Interception::Hu)
				{
					appendingAction.type -= (Interception::ChiTing - Interception::Chi);
				}
				player->AddInterception(appendingAction);
				extPlayer->SortHand(player->GetPos());
				
				int idx = player->GetInterceptions().Count() - 1;
				appendingActions.insert(std::make_pair(player->GetPos(), AppendInterceptionInfo(idx, appendingAction)));
			}

			if(action.type == Interception::Hu)
			{
				someOneWin = true;

				if(action.src.playerPos != player->GetPos())
				{
					newWinnerAndLoser[player->GetPos()] = action.src.playerPos;
					player->AddCard(action.src.card);		//点炮胡把胡的牌加入手牌，方便客户端显示
				}
				else
				{
					newWinnerAndLoser[player->GetPos()] = PlayerPos::Max;
				}
			}
			else if(action.type == Interception::Ting ||
				action.type == Interception::ChiTing ||
				action.type == Interception::PengTing ||
				action.type == Interception::ZhanTing)
			{
				OBJ_GET_EXT(m_Context, ExtensionActionValidator, extActValidator);
				InterceptionList tingActions = extActValidator->HandlerInterception(player->GetPos(), player->GetPos(), 
					player->GetCards(), player->GetInterceptions(),
					Interception::Ting, Card::Invalid);

				// 记录听牌选项
				extIntcCards->SetReadyHandSelection(player->GetPos(), ReadyHandSelection(tingActions));

				extPlayer->GetPlayerStatus(player->GetPos()).SetBool(PlayerStatus_ReadyHand, true);
			}
		}
	}

	OBJ_GET_EXT(m_Context, ExtensionCardBlocks, extCardBlocks);
	int gameRound = extCardBlocks->GetCardBlocks()->MaxCount() - extCardBlocks->GetCardBlocks()->Count() - 13 * m_Context->GetPlayerCount();
	for(std::map<PlayerPos::Type, PlayerPos::Type>::iterator itr = newWinnerAndLoser.begin();
		itr != newWinnerAndLoser.end();
		++itr)
	{
		if(itr->second != PlayerPos::Max)
		{
			extLiquidation->AddWinnerAndLoser(itr->first, itr->second, gameRound);
		}
		else
		{
			extLiquidation->AddWinner(itr->first, WinMode_Self, gameRound);
		}
	}

	extLiquidation->OnAddInterception(srcPlayer, appendingActions);

	for(std::map<PlayerPos::Type, AppendInterceptionInfo>::iterator itr = appendingActions.begin();
		itr != appendingActions.end();
		++itr)
	{
		// 操作结果通知
		OBJ_GET_EXT(m_Context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->SendNotification(NotificationInterceptionResult(itr->first, itr->second.interception));
	}
	

	void *data[] = {&playerAction};
	Jump(data, ARRAY_LEN(data));
}
