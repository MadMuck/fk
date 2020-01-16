#ifndef MSG_DELEGATE_WAIT_DISCARD_H
#define MSG_DELEGATE_WAIT_DISCARD_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionQueMen.h"
#include "ExtensionPlayerReadyHand.h"
#include "QuestionWaitDiscard.h"
#include "PlayerActionDelegate.h"
#include "PlayerActionDelegateListener.h"
#include "UpgradeMessageEx.h"
#include "SpecCardScoreDelegate.h"
#include "MsgDelegateInterception.h"

static void AutoPassOutCard(void **data, int dataCnt)
{
	GameContext *context = (GameContext *)data[0];
	tagOutPaiEx* msg = (tagOutPaiEx *)data[1];

	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
	bool res = extMsgDelegate->GetDelegate(MSG_DELEGATE_WAIT_DISCARD_CARD).OnResponse(context, msg->byUser, msg, sizeof(tagOutPaiEx), THING_OUT_PAI);
	if(!res) res = extMsgDelegate->GetDelegate(MSG_DELEGATE_READY_HAND_DISCARD).OnResponse(context, msg->byUser, msg, sizeof(tagOutPaiEx), THING_OUT_PAI);

	if(res) extPlayerActDelegate->OnPlayerResponse(msg->byUser, THING_OUT_PAI);

	delete msg;
}

class MsgDelegateWaitDiscard : public MessageDelegateImpl
{
	static bool OnDiscardCard(MsgDelegateWaitDiscard *waitDiscard, GameContext *context, PlayerPos::Type playerPos, int selectIdx)
	{
		if(!waitDiscard->m_Valid || waitDiscard->m_Data.playerPos != playerPos) return false;

		if(selectIdx < 0 || waitDiscard->m_RealCandidates.Find(waitDiscard->m_Data.cards[selectIdx]) == 0)
		{
			return false;
		}

		LOGGER_FILE(context, "player "<<(int)playerPos<<" discard card "<<(int)waitDiscard->m_Data.cards[selectIdx]<<"(idx "<<selectIdx<<")");

		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->OnResponse(playerPos, 0, selectIdx);

		waitDiscard->m_Valid = false;

		// 出牌后("等待出牌"结束)删除托管监听器
		OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
		extPlayerActDelegate->RemoveListener(playerPos);

		// 更新玩家听牌状态
		OBJ_GET_EXT(context, ExtensionPlayerReadyHand, extPlayerReadyHand);
		extPlayerReadyHand->OnDiscard(playerPos);

		return true;
	}

	static void AutoDiscardCallback(GameContext *context, int actionType, PlayerPos::Type playerPos, void *data)
	{
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);

		MsgDelegateWaitDiscard *waitDiscard = (MsgDelegateWaitDiscard *)data;

		// 候选列表中的最后一张
		Card::Type card = waitDiscard->m_RealCandidates[waitDiscard->m_RealCandidates.Count() - 1];

		// 从原列表中计算索引
		const CardList &cards = waitDiscard->m_Data.cards;
		int selectIdx = 0;
		for(int i = 0; i < cards.Count(); ++i)
		{
			if(card == cards[i])
			{
				selectIdx = i;
				break;
			}
		}

		if(!OnDiscardCard(waitDiscard, context, playerPos, selectIdx))
		{
			LOGGER_FILE(context, "auto discard error, oriList "<<cards<<"; realList "<<waitDiscard->m_RealCandidates<<";");
		}

		//if(!extPlayer->GetPlayerStatus(playerPos).GetFlag(PlayerStatus_ActionDelegate))
		//{
		//	int timeOutCount = extPlayer->GetPlayerStatus(playerPos).GetFlag(PlayerStatus_TimeOutCount);
		//	extPlayer->GetPlayerStatus(playerPos).SetFlag(PlayerStatus_TimeOutCount,++timeOutCount);

		//	if(timeOutCount >= extGameParam->GetParam(GameParam_TimeOutAuto))
		//	{
		//		Usertouguan data;
		//		data.enable = true;
		//		data.desk = playerPos;
		//		for(int i = 0; i < context->GetPlayerCount(); ++i)
		//		{
		//			data.bTuoGuan[i] = extPlayer->GetPlayerStatus(playerPos).GetBool(PlayerStatus_ActionDelegate);
		//		}
		//		MsgDelegateEnableActionDelegate().OnResponse(context, playerPos, &data, sizeof(Usertouguan), ASS_AUTOCARD);
		//	}	
		//}
	}

protected:
	QuestionWaitDiscardData m_Data;
	bool m_Valid;
	CardList m_RealCandidates;

	static CardList StaticGetCandidateCards(GameContext *context, PlayerPos::Type playerPos, const CardList &hand)
	{
		CardList ret = hand;
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

		// 打缺门
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
		ret = extQueMen->FilterCards(playerPos, hand);
		if(ret.Count() == 0)
			ret = hand;

		return ret;
	}

	virtual CardList GetCandidateCards(GameContext *context, PlayerPos::Type playerPos, const CardList &oriCards)
	{
		return StaticGetCandidateCards(context, playerPos, oriCards);
	}

	virtual bool EnableAutoDiscard(GameContext *context) const
	{
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
		return extProcCtrl->RunningProcedureType() != ProcedureType::ReadyHandDiscard;
	}

public:

	MsgDelegateWaitDiscard():m_Valid(false){}

	typedef PlayerActionDelegateListener<MsgDelegateWaitDiscard> DiscardDelegateListener;

	///////////////////////////////////////////////////////////

	void SendTingCardInfo(GameContext *context)
	{
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);
		OBJ_GET_EXT(context, ExtensionBankerSelector,extBankerSelector);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		INIT_MSG(WaitOutCards, msg, 0, dataToSend, dataSendSize);

		DefaultTingHandlerImpl TingHandler;
		CardStat revealedCards = TingHandler.GetRevealedCards(context, m_Data.playerPos);

		InterceptionList interceptionList = extPlayer->GetPlayer(m_Data.playerPos)->GetInterceptions();

		InterceptionList tinglist = TingHandler.ToDo(context,m_Data.playerPos,m_Data.playerPos,m_Data.cards,interceptionList,Interception::Ting,0);

		if(tinglist.size() == 0)
			return;

		for(InterceptionList::iterator itr = tinglist.begin(); itr != tinglist.end(); ++itr)
		{
			CardList temp_cards = m_Data.cards;
			temp_cards.Remove(itr->src.card,1);
			if(itr->type == Interception::Ting && extQueMen->FilterCards(m_Data.playerPos,temp_cards).size() == 0)
			{
				for(int i = 0; i < sizeof(msg) / sizeof(WaitOutCard); ++i)
				{
					if(msg.OutCards[i].OutCard != 255)
						continue;
					msg.OutCards[i].OutCard = itr->src.card;

					CardList::iterator itr2 = itr->cards.begin();
					for(int j = 0; j < sizeof(msg.OutCards[i].TingCards) / sizeof(TingInfo); ++j)
					{
						if(msg.OutCards[i].TingCards[j].Value != 255)
							continue;
						if(itr2 != itr->cards.end())
						{
							msg.OutCards[i].TingCards[j].Value = *itr2;
							msg.OutCards[i].TingCards[j].Count = revealedCards.Num(*itr2);


							CardList cardList = m_Data.cards;
							cardList.Remove(itr->src.card,1);
							cardList.Add(*itr2);
							CardInfo src;
							src.card = *itr2;
							src.playerPos = PlayerPos::Max;

							WinType wintype;

							SpecScoreInfo scoreInfo = SpecCardScoreDelegate::GetScore(context, cardList, interceptionList, src, m_Data.playerPos, extBankerSelector->GetBankerPos(), &wintype);

							msg.OutCards[i].TingCards[j].Fan = scoreInfo.fan;
							++itr2;
						}
						else
						{
							break;
						}
					}
					break;
				}
			}
		}
		extGameRunningData->SendDataToPlayer(m_Data.playerPos,dataToSend, dataSendSize, THING_WAIT_OUT);	
	}

	void OnSend(GameContext *context, void *data)
	{
		m_Data = *(QuestionWaitDiscardData *)data;

		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		PlayerPos::Type playerPos = m_Data.playerPos;

		// 读取一下可出牌列表
		m_RealCandidates = GetCandidateCards(context, playerPos, m_Data.cards);
		LOGGER_FILE(context, "waiting on player "<<(int)m_Data.playerPos<<" to discard, with candidates :"<<m_RealCandidates<<" !");

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		float delegateOriDelay = extGameParam->GetParam(GameParam_DecisionTimeLimit) * 1.0f;
		float delegateActionDelay = delegateOriDelay;

		SendTingCardInfo(context);

		m_Valid = true;
		bool autoDiscard = false;

		//if(EnableAutoDiscard(context) &&
		//	extPlayer->GetPlayerStatus(m_Data.playerPos).GetBool(PlayerStatus_ReadyHand))
		//{
		//	// 已经听牌了, 自动出牌
		//	autoDiscard = extGameParam->IsTrue(GameParam_ReadyHandAutoDiscard);
		//}

		//if(!autoDiscard && extPlayer->GetPlayerStatus(m_Data.playerPos).GetBool(PlayerStatus_ActionDelegate))
		//{
		//	// 处于托管状态
		//	autoDiscard = true;
		//}

		if(autoDiscard)
		{
			delegateActionDelay = 1;
		}

		if(extGameParam->GetParam(GameParam_ActionDelegateEnabled) > 0 || autoDiscard)
		{
			// 设置超时自动出牌
			DiscardDelegateListener::AddActionDelegate(context, playerPos, AutoDiscardCallback, delegateActionDelay, THING_OUT_PAI, this);

			if(!autoDiscard && 
				extGameParam->GetParam(GameParam_ActionDelegateEnabled) > 0)
			{
				// 玩家没有托管， 加个监听器
				OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
				float oriDelay = extGameParam->GetParam(GameParam_DecisionTimeLimit) * 1.0f;
				extPlayerActDelegate->AddListener(playerPos, 
					DiscardDelegateListener::Create(this, AutoDiscardCallback, delegateOriDelay, THING_OUT_PAI));
			}
		}

		tagOutPai_NTF msg;
		msg.bDeskStation = playerPos;
		extGameRunningData->SendDataToAll(&msg, sizeof(msg), THING_BEGIN_OUT);
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayConnector);
		if(extPlayer->GetActivePlayerPos() == playerPos && extGamePlayConnector->IsPlayerBlocking(playerPos,0))
		{
			OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
			OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
			OBJ_GET_EXT(context, ExtensionTimer, extTimer);
			InterceptionResponseData intcData(InterceptionResponseData::Ignore, NULL, 0);
			extMsgDelegate->GetDelegate(MSG_DELEGATE_INTERCEPTION).OnResponse(context, playerPos, &intcData, sizeof(intcData), ASS_PASS_CHECK);
			extPlayerActDelegate->OnPlayerResponse(playerPos, ASS_PASS_CHECK);

			LOGGER_FILE(context, "player "<<(int)playerPos << " out card while having interception, auto pass");

			tagOutPaiEx *msg = new tagOutPaiEx();
			memcpy(msg,data,sizeof(tagOutPaiEx));

			void *pointers[] = {context,msg};
			int pointerCnt = ARRAY_LEN(pointers);
			TimerData timerData = TimerData(pointers, pointerCnt, AutoPassOutCard);
			extTimer->Add(0.05f, timerData);
			return true;
		}

		LOGGER_FILE(context, "player "<<(int)playerPos<<" tried to discard(valid:"<<(int)m_Valid<<", expected player:"<<(int)m_Data.playerPos<<")");
		if(!m_Valid || playerPos != m_Data.playerPos)
		{
			return false;
		}

		tagOutPaiEx msg = *(tagOutPaiEx *)data;

		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);

		Card::Type discardedCard = msg.byPs;

		LOGGER_FILE(context, "the card is "<<(int)discardedCard);

		int selectIdx = -1;
		for(int i = 0; i < m_Data.cards.Count(); ++i)
		{
			if(discardedCard == m_Data.cards[i])
			{
				selectIdx = i;
				break;
			}
		}

		return OnDiscardCard(this, context, playerPos, selectIdx);
	}
};

#endif