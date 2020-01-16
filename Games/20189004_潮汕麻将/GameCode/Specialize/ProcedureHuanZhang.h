#ifndef PROCEDURE_HUAN_ZHANG_H
#define PROCEDURE_HUAN_ZHANG_H	

#include "Procedure.h"
#include "ExtensionProcedureController.h"
#include "ExtensionQueMen.h"
#include "ExtensionPlayer.h"
#include "ExtensionGameParam.h"
#include "QuestionHuanZhang.h"
#include "NotificationHuanZhangFinish.h"
#include "ExtensionHuanZhang.h"
#include "ExtensionTimer.h"
#include "ExtensionLogger.h"

#include "Util.h"

#include "SpecProcedureTypeDef.h"
#include "ProcedureType.h"

#include <map>

class ProcedureHuanZhangJumpDelegateImpl : public ProcedureJumpDelegateImpl
{
	static void JumpToDingQue(void **data, int dataCnt)
	{
		GameContext *context = (GameContext *)data[0];
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcedureController);
		extProcedureController->StartProcedure(ProcedureType_DingQue);
	}
public:

	void Jump(GameContext *context, void **data, int dataCnt) const
	{
		OBJ_GET_EXT(context, ExtensionTimer, extTimer);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		void *pointers[] = {context};
		float time = 0.0f;
		if(extGameParam->IsTrue(GameParam_HuanZhang))		//开启了换张才留时间给客户端播放动作，否则直接转到定缺
		{
			time = 5.0f;
		}
		extTimer->Add(time, TimerData(pointers, ARRAY_LEN(pointers), JumpToDingQue));
	}
};

class ProcedureHuanZhangJumpDelegate : public ProcedureJumpDelegate
{
public:
	ProcedureHuanZhangJumpDelegate():ProcedureJumpDelegate(new ProcedureHuanZhangJumpDelegateImpl()){}
};


class HuanZhangRspDelegate : public HuanZhangResponseDelegate
{
	GameContext *m_GameContext;
	ProcedureJumpDelegate m_JumpDelegate;

	std::map<PlayerPos::Type, CardList> m_PlayerSelectedCards;
	
public:
	HuanZhangRspDelegate(GameContext *context, const ProcedureJumpDelegate &jumpDelegate):
	  m_GameContext(context),m_JumpDelegate(jumpDelegate){}

	bool OnSelect(PlayerPos::Type playerPos, Card::Type card)
	{
		OBJ_GET_EXT(m_GameContext, ExtensionHuanZhang, extHuanZhang);

		m_PlayerSelectedCards[playerPos].Add(card);
		// 玩家选择总计3张牌，结束对其询问
		bool finished =  m_PlayerSelectedCards[playerPos].Count() == 3;

		if(finished)
		{
			
			for(CardList::iterator itr = m_PlayerSelectedCards[playerPos].begin();
				itr != m_PlayerSelectedCards[playerPos].end();
				++itr)
			{
				extHuanZhang->Add(playerPos, *itr);
			}

			LOGGER_FILE(m_GameContext, "player "<< (int)playerPos <<" change cards : "<< m_PlayerSelectedCards[playerPos]);
		}

		int finishedCnt = 0;
		for(std::map<PlayerPos::Type, CardList>::iterator itr = m_PlayerSelectedCards.begin();
			itr != m_PlayerSelectedCards.end();
			++itr)
		{
			if(itr->second.Count() >= 3)
			{
				++finishedCnt;
			}
		}

		if(m_GameContext->GetPlayerCount() == finishedCnt)
		{
			// 随机产生交换位置
			std::vector<PlayerPos::Type> swapPosition;
			for(int i = 0; i < m_GameContext->GetPlayerCount(); ++i)
			{
				swapPosition.push_back(i);
			}
			std::random_shuffle(swapPosition.begin(), swapPosition.end());
			for(int i = 0; i < (int)swapPosition.size(); ++i)
			{
				if(swapPosition[i] == i)
				{
					int swapIdx = (i + 1) % swapPosition.size();
					std::swap(swapPosition[i], swapPosition[swapIdx]);
				}
			}

			LOGGER_FILE(m_GameContext, "swapPosition 0 -> "<< (int)swapPosition[0] << ", swapPosition 1 -> "<< (int)swapPosition[1] << 
										", swapPosition 2 -> "<< (int)swapPosition[2] << ", swapPosition 3  -> "<< (int)swapPosition[3]);

			// 交换牌
			OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
			OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
			OBJ_GET_EXT(m_GameContext, ExtensionInterceptableCards, extIntcCards);
			Card::Type lastcard = extIntcCards->GetLatestDrawnCard();
			Player *banker = extPlayer->GetPlayer(extBankerSelector->GetBankerPos());
			CardList banker_getcards;

			for(int i = 0; i < (int)swapPosition.size(); ++i)
			{
				const CardList &cards = m_PlayerSelectedCards[(PlayerPos::Type)i];

				Player *playerA = extPlayer->GetPlayer((PlayerPos::Type)i);
				Player *playerB = extPlayer->GetPlayer(swapPosition[i]);

				playerA->RemoveCards(cards);
				playerB->AddCards(cards);
				extHuanZhang->SetSwapCard(playerB->GetPos(), cards);		//记录换三张得到的牌
				extPlayer->SortHand(playerB->GetPos());

				if(playerB->GetPos() == banker->GetPos())
				{
					banker_getcards = cards;
				}
			}

			OBJ_GET_EXT(m_GameContext, ExtensionGamePlayerConnector, extGamePlayerConnector);
			extGamePlayerConnector->SendNotification(NotificationHuanZhangFinish());

			//原最后一张抓牌被换走了
			if(0 == banker->GetCards().Find(lastcard) && !banker_getcards.empty())
			{
				lastcard = banker_getcards.back();
			}

			extIntcCards->SetLatestDrawnCard(lastcard);

			m_JumpDelegate.Jump(m_GameContext, NULL, 0);
		}

		return finished;
	}
};

class ProcdureHuanZhangImpl : public ProcedureImpl
{
public:
	ProcdureHuanZhangImpl(GameContext *context):ProcedureImpl(context, ProcedureHuanZhangJumpDelegate()){}

	void OnStart()
	{
		RefCntObj<HuanZhangResponseDelegate> rspDelegate(new HuanZhangRspDelegate(m_Context, m_JumpDelegate));

		OBJ_GET_EXT(m_Context, ExtensionGameParam, extGameParam);
		if(extGameParam->IsTrue(GameParam_HuanZhang))
		{
			for(int i = 0; i < m_Context->GetPlayerCount(); ++i)
			{
				OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
				QuestionHuanZhang question(i, extPlayer->GetPlayer(i)->GetCards(), rspDelegate);
				OBJ_GET_EXT(m_Context, ExtensionGamePlayerConnector, extGamePlayerConnector);
				extGamePlayerConnector->SendQuestion(question);
			}
		}
		else
		{
			// 不换张， 直接到下个流程
			Jump(NULL, 0);
		}
	}
};

class ProcedureHuanZhang : public Procedure
{
public:
	ProcedureHuanZhang(GameContext *context):Procedure(new ProcdureHuanZhangImpl(context)){}
};

#endif