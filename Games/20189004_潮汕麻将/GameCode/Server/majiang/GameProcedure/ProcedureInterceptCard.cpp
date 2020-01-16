#include "GameContext.h"
#include "ProcedureController.h"
#include "ProcedureType.h"
#include "ProcedureInterceptCard.h"

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
#include "ExtensionAbandonedAction.h"

#include "QuestionInterception.h"
#include "NotificationInterceptionResult.h"

#include "Util.h"

#include "SpecProcedureTypeDef.h"

#include <map>
#include <list>
#include <set>

void ProcedureInterceptCardJumpDelegateImpl::Jump(GameContext *context, void **data, int dataCnt) const
{
	bool result = *(bool *)data[0];
	if(result)
	{
		const PlayerInterception *actions = (PlayerInterception *)data[1];
		JumpAfterAction(context, *actions);
	}
	else
	{
		JumpWhenNoAction(context);
	}
}

void ProcedureInterceptCardJumpDelegateImpl::JumpWhenNoAction(GameContext *ctx) const
{
	OBJ_GET_EXT(ctx, ExtensionGameDirection, extGameDir);
	OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
	// 设置下一个行动的玩家, 下个玩家抽牌
	PlayerPos::Type nextPos = extGameDir->GetNextPos(extPlayer->GetActivePlayerPos());
	LOGGER_FILE(ctx, " active player changed " <<(int)extPlayer->GetActivePlayerPos() <<" -> "<<(int)nextPos);
	extPlayer->SetActivePlayerPos(nextPos);

	OBJ_GET_EXT(ctx, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::NormalDrawCard);
}

void ProcedureInterceptCardJumpDelegateImpl::JumpAfterAction(GameContext *context, const PlayerInterception &playerActions) const
{
	OBJ_GET_EXT(context, ExtensionInterceptableCards, extIntcCards);
	extIntcCards->SetPlayerInterception(playerActions);

	OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
	extProcCtrl->StartProcedure(ProcedureType::DetermineIntercept);
}




class DefaultInterceptCardQuesitonDelegate : public InterceptCardQuestionDelegate
{
	int m_PlayerCanActionCnt; // 可拦牌的玩家数量

	int m_ResponsedCnt;
public:

	DefaultInterceptCardQuesitonDelegate():m_PlayerCanActionCnt(0), m_ResponsedCnt(0){}

	void Init()
	{
		m_ResponsedCnt = 0;
		m_PlayerCanActionCnt = 0;
	}

	bool OnQuestionResponsed(GameContext *context, PlayerPos::Type playerPos, const Interception &action, const InterceptionList &interceptions,
		const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		++m_ResponsedCnt;
		return true;
	}

	bool OnQuestion(GameContext *context, PlayerPos::Type playerPos, const InterceptionList &interceptions, 
		const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		++m_PlayerCanActionCnt;
		// 发出拦牌选项
		sendDelegate->SendQuestion(context, playerPos, interceptions);

		return true;
	}

	bool OnQuestionFinish(GameContext *context, const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		return m_PlayerCanActionCnt > 0;
	}

	bool IsResponseFinished() const
	{
		return m_ResponsedCnt >= m_PlayerCanActionCnt;
	}

	bool PriorityGreater(GameContext *context, PlayerPos::Type srcPlayerPos, PlayerPos::Type playerPos1, 
		int interceptionType1, PlayerPos::Type playerPos2,  int interceptionType2) const
	{
		return interceptionType1 > interceptionType2;
	}

	bool PriorityEqual(GameContext *context, PlayerPos::Type srcPlayerPos, PlayerPos::Type playerPos1, int interceptionType1,
		PlayerPos::Type playerPos2, int interceptionType2) const
	{
		return interceptionType1 == interceptionType2;
	}

};




InterceptCardSendQuestionDelegate::InterceptCardSendQuestionDelegate(const RefCntObj<InterceptionResponseDelegate> &responseDelegate)
	:m_ResponseDelegate(responseDelegate){}


void InterceptCardSendQuestionDelegate::SendQuestion(GameContext *context, PlayerPos::Type playerPos, const InterceptionList &interceptions) const
{
	QuestionInterception question(playerPos, interceptions, m_ResponseDelegate);
	OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extConnector);
	extConnector->SendQuestion(question);
}









class InterceptionDelegate : public InterceptionResponseDelegate
{
public:

	PlayerInterception ActionsToCompare; // 所有玩家选择的动作，所有人都选择过后裁决出优先级最高的
	GameContext *m_Context;
	ProcedureImpl *m_ProcedureImpl;

	RefCntObj<InterceptCardQuestionDelegate> m_QuestionDelegate;
	RefCntObj<InterceptCardSendQuestionDelegate> m_SendDelegate;

	InterceptionDelegate(GameContext *context, ProcedureImpl *impl, 
		const RefCntObj<InterceptCardQuestionDelegate> &questionDelegate):
	m_Context(context), m_ProcedureImpl(impl), m_QuestionDelegate(questionDelegate),
		m_SendDelegate(NULL){}

	void SetSendDelegate(const RefCntObj<InterceptCardSendQuestionDelegate> &delegate)
	{
		m_SendDelegate = delegate;
	}

	void OnAction(PlayerPos::Type playerPos, const Interception &action, const InterceptionList &actions)
	{	
		if(m_QuestionDelegate->OnQuestionResponsed(m_Context, playerPos, action, actions, m_SendDelegate))
		{
			ActionsToCompare[playerPos] = InterceptionSelection(action, actions);
		}
		CheckIsFinished();
	}

	void OnIgnore(PlayerPos::Type playerPos, const InterceptionList &interceptions)
	{
		OBJ_GET_EXT(m_Context, ExtensionAbandonedAction, extAbandonedAction);
		// 放弃
		for(InterceptionList::const_iterator itr = interceptions.begin();
			itr != interceptions.end();
			++itr)
		{
			// 记录所有被放弃的行为
			extAbandonedAction->Abandoned(playerPos, itr->type, itr->src.card);
		}

		InterceptionSelection selection = InterceptionSelection();
		selection.selections = interceptions;

		// 操作结果通知
		OBJ_GET_EXT(m_Context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->SendNotification(NotificationInterceptionResult(playerPos, selection.selected));

		m_QuestionDelegate->OnQuestionResponsed(m_Context, playerPos, selection.selected, selection.selections, m_SendDelegate);

		CheckIsFinished();
	}

	void CheckIsFinished()
	{
		if(m_QuestionDelegate->IsResponseFinished())
		{
			// 已经收到所有玩家的反馈
			OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
			Interception::Type mostPrior = Interception::Invalid;
			PlayerPos::Type playerPos = extPlayer->GetActivePlayerPos();

			// 结果
			PlayerInterception interceptionResult;

			for(PlayerInterception::iterator itr = ActionsToCompare.begin();
				itr != ActionsToCompare.end();
				++itr)
			{
				// 比较优先级, 找出最高的优先级
				if(m_QuestionDelegate->PriorityGreater(m_Context, 
					extPlayer->GetActivePlayerPos(), 
					itr->first, itr->second.selected.type, 
					playerPos, mostPrior)) 
				{
					mostPrior = itr->second.selected.type;
					playerPos = itr->first;
				}
			}

			OBJ_GET_EXT(m_Context, ExtensionInterceptableCards, extIntcCards);
			//清空可拦牌
			extIntcCards->ClearInterceptableCards();

			if(mostPrior != Interception::Invalid)
			{
				std::set<Card::Type> cardsToRemove;

				for(PlayerInterception::iterator itr = ActionsToCompare.begin();
					itr != ActionsToCompare.end();
					++itr)
				{
					// 找出与最高优先级同等级的
					if(m_QuestionDelegate->PriorityEqual(m_Context, 
						extPlayer->GetActivePlayerPos(), 
						itr->first, itr->second.selected.type, 
						playerPos, mostPrior)) 
					{
						interceptionResult[itr->first] = itr->second;

						// 记录被拦截玩家要删的"已出"牌
						cardsToRemove.insert(itr->second.selected.src.card);
					}
				}

				// 删除"已出"牌
				for(std::set<Card::Type>::iterator itr = cardsToRemove.begin();
					itr != cardsToRemove.end();
					++itr)
				{
					Player *interceptedPlayer = extPlayer->GetActivePlayer();
					OBJ_GET_EXT(m_Context, ExtensionCardBlocks, extCardBlocks);
					extCardBlocks->RemoveDiscardedCard(interceptedPlayer->GetPos(), *itr);
				}

				bool result = true;
				void *data[] = {&result, &interceptionResult};
				m_ProcedureImpl->Jump(data, ARRAY_LEN(data));

			}// valid action end
			else
			{
				bool result = false;
				void *data[] = {&result};
				m_ProcedureImpl->Jump(data, ARRAY_LEN(data));
			} 
		}
	}
};

ProcedureInterceptCardImpl::ProcedureInterceptCardImpl(GameContext *ctx, const ProcedureJumpDelegate &delegate):
ProcedureImpl(ctx, delegate),m_QuestionDelegate(new DefaultInterceptCardQuesitonDelegate())
{
}

void ProcedureInterceptCardImpl::OnStart()
{
	GameContext *ctx = m_Context;

	m_QuestionDelegate->Init();

	InterceptionDelegate *responseDelegate = new InterceptionDelegate(m_Context, this, m_QuestionDelegate);
	RefCntObj<InterceptionResponseDelegate> response(responseDelegate);

	RefCntObj<InterceptCardSendQuestionDelegate> sendDelegate(new InterceptCardSendQuestionDelegate(response));

	responseDelegate->SetSendDelegate(sendDelegate);


	OBJ_GET_EXT(ctx, ExtensionGameDirection, extGameDir);
	OBJ_GET_EXT(ctx, ExtensionInterceptableCards, extIntcCards);
	OBJ_GET_EXT(ctx, ExtensionActionValidator, extActValidator);
	OBJ_GET_EXT(ctx, ExtensionPlayer, extPlayer);
	OBJ_GET_EXT(ctx, ExtensionHuPattern, extHuPattern);
	OBJ_GET_EXT(ctx, ExtensionProcedureController, extProcCtrl);

	const InterceptableCards &interceptableCards =
		extIntcCards->GetInterceptableCards();

	for(InterceptableCards::const_iterator itr = interceptableCards.begin();
		itr != interceptableCards.end();
		++itr)
	{
		CardInfo cardInfo = *itr;
//		CardList jokerList = extHuPattern->GetJokers();
		if(cardInfo.card == Card::Invalid /*|| jokerList.Find(cardInfo.card)*/)	//万能牌打出时无效
		{
			continue;
		}

		PlayerPos::Type interceptedPlayer = cardInfo.playerPos;
		Card::Type card = cardInfo.card;

		LOGGER_FILE(ctx, "check interceptable card "<<(int)card<<" from Player "<<(int)interceptedPlayer);

		InterceptionCheckType interceptionCheckList = extActValidator->GetCheckType(Type());


		OBJ_GET_EXT(m_Context, ExtensionGameDirection, extGameDir);
		PlayerPos::Type playerPosToCheck = extGameDir->GetNextPos(interceptedPlayer);

		while(playerPosToCheck != interceptedPlayer)
		{
			Player *player = extPlayer->GetPlayer(playerPosToCheck);
			PlayerPos::Type actionPlayerPos = player->GetPos();
			if(player->GetPos() != interceptedPlayer)
			{
				InterceptionList interceptionList;

				CardList cardsToCheck = player->GetCards();
				cardsToCheck.Add(card);

				for(InterceptionCheckType::const_iterator itr = interceptionCheckList.begin();
					itr != interceptionCheckList.end();
					++itr)
				{
					//if(extProcCtrl->RunningProcedureType() == ProcedureType_QiangGang)	//有杠爆关联者的话只能抢杠杠爆关联者
					//{
					//	PlayerPos::Type gangBaoPos = extPlayer->GetGangBaoPos(player->GetPos());
					//	if(gangBaoPos != PlayerPos::Max && gangBaoPos != interceptedPlayer)
					//		continue;
					//}

					if(extActValidator->IsInterceptionValid(player->GetPos(), *itr, CardInfo(interceptedPlayer, card)))
					{
						interceptionList += extActValidator->HandlerInterception(player->GetPos(), interceptedPlayer, 
							cardsToCheck, player->GetInterceptions(), *itr, card);
					}
				}

				if(interceptionList.Count() > 0)
				{
					LOGGER_FILE(ctx, "Player "<<(int)player->GetPos()<<" have actions "<<interceptionList<<" with hand "<<
						player->GetCards()<<" interceptions "<<player->GetInterceptions()<<" card "<<(int)card);

					interceptionList.SetPlayerPos(interceptedPlayer);

					if(m_QuestionDelegate->OnQuestion(ctx, player->GetPos(), interceptionList, sendDelegate))
					{
						LOGGER_FILE(ctx, "question sent");
					}
					else
					{
						LOGGER_FILE(ctx, "question was blocked by question delegate");
					}
				}
			}

			playerPosToCheck = extGameDir->GetNextPos(playerPosToCheck);
		}
	}

	OnCheckFinish(m_QuestionDelegate->OnQuestionFinish(ctx, sendDelegate));
}

void ProcedureInterceptCardImpl::SetQuestionDelegate(const RefCntObj<InterceptCardQuestionDelegate> &delegate)
{
	m_QuestionDelegate = delegate;
}

void ProcedureInterceptCardImpl::OnCheckFinish(bool haveAction)
{
	if(!haveAction)
	{
		OBJ_GET_EXT(m_Context, ExtensionInterceptableCards, extIntcCards);
		// 清除可拦截的牌
		extIntcCards->ClearInterceptableCards();

		bool result = false;
		void *data[] = {&result};
		Jump(data, ARRAY_LEN(data));
	}
}


void ProcedureInterceptCard::SetQuestionDelegate(const RefCntObj<InterceptCardQuestionDelegate> &delegate)
{
	((ProcedureInterceptCardImpl*)m_Impl.GetObj())->SetQuestionDelegate(delegate);
}
