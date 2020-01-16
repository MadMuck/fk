#ifndef MSG_DELEGATE_HUAN_ZHANG_H
#define MSG_DELEGATE_HUAN_ZHANG_H

#include "UpgradeMessageEx.h"
#include "MessageDelegate.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionQueMen.h"
#include "ExtensionLogger.h"

#include "QuestionHuanZhang.h"

#include <map>
#include <algorithm>

#include "UtilTemplate.h"

class MsgDelegateHuanZhang : public MessageDelegateImpl
{
	std::map<PlayerPos::Type, CardList> m_Cards;

	static void AutoSelect(GameContext *context, int type, PlayerPos::Type playerPos, void *data)
	{
		MsgDelegateHuanZhang *msgDelegate = (MsgDelegateHuanZhang *)data;

		std::map<int, int> cardTypeNum; // 各种花色的牌的数量

		const CardList &cards = msgDelegate->m_Cards[playerPos];
		CardStat stat(cards);
		const CardStat::ValuesType &values = stat.Values();
		for(CardStat::ValuesType::const_iterator itr = values.begin();
			itr != values.end();
			++itr)
		{
			cardTypeNum[*itr / 10] += stat.Num(*itr);
		}

		// 找数量最多的花色
		std::vector<Util::SortData<int, int> > dataToSort;
		for(std::map<int,int>::iterator itr = cardTypeNum.begin();
			itr != cardTypeNum.end();
			++itr)
		{
			dataToSort.push_back(Util::SortData<int, int>(itr->second, itr->first));
		}

		std::sort(dataToSort.begin(), dataToSort.end(), std::greater<Util::SortData<int, int> >());

		int selectedCardType;

		if(dataToSort.empty())
		{
			LOGGER_FILE(context, "auto huan zhang error, cards size = " << cards.size() << ", cards = " << cards);


			//做临时处理
			std::map<int, int> cardTypeNum;
			OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
			CardList card = extPlayer->GetPlayer(playerPos)->GetCards();

			if(card.empty())
			{
				LOGGER_FILE(context, "auto huan zhang error, return");
			}

			for(auto itr = card.begin(); itr != card.end(); ++itr)
			{
				cardTypeNum[*itr / 10]++;
			}

			int maxnum = 0;
			int maxtype = 0;
			for(std::map<int, int>::iterator itr = cardTypeNum.begin(); itr != cardTypeNum.end(); ++itr)
			{
				if(itr->second > maxnum)
				{
					maxtype = itr->first;
					maxnum = itr->second;
				}
			}

			selectedCardType = maxtype;

		}else
		{
			selectedCardType = dataToSort.front().value;
		}

		std::vector<int> selectedIdx;
		for(int i = 0; i < cards.Count(); ++i)
		{
			if(cards[i] / 10 == selectedCardType)
			{
				selectedIdx.push_back(i);
			}
			if(selectedIdx.size() == 3)
				break;
		}

		OnSelect(msgDelegate, context, playerPos, selectedIdx, THING_CHANGE_NOTIFY);
	}

	static void OnSelect(MsgDelegateHuanZhang *msgDelegate, GameContext *context, PlayerPos::Type playerPos, const std::vector<int> &selectedIdx, int msgID)
	{
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);

		for(std::vector<int>::const_iterator itr = selectedIdx.begin();
			itr != selectedIdx.end();
			++itr)
		{
			extGamePlayerConnector->OnResponse(playerPos, msgID, *itr);
		}

		msgDelegate->m_Cards.erase(playerPos);
	}

	void SendQuestionToPlayer(GameContext *context, PlayerPos::Type playerPos)
	{

		INIT_MSG(tagChangePsEx, msg, 0, dataToSend, dataSendSize);
		msg.bNotify = true;
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);
		extGameRunningData->SendDataToPlayer(playerPos, dataToSend, dataSendSize, THING_CHANGE_NOTIFY);

		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		if(extGameParam->GetParam(GameParam_ActionDelegateEnabled) > 0)
		{
			float oriDelay = extGameParam->GetParam(GameParam_HuanZhangTime) * 1.0f;
			float delay = oriDelay;

			if(extPlayer->GetPlayerStatus(playerPos).GetBool(PlayerStatus_ActionDelegate))
			{
				delay = 1;
			}

			// 设置超时自动换张
			PlayerActionDelegateListener<MsgDelegateHuanZhang>::AddActionDelegate(context, playerPos, AutoSelect, delay, THING_CHANGE_NOTIFY, this);

			if(!extPlayer->GetPlayerStatus(playerPos).GetBool(PlayerStatus_ActionDelegate))
			{
				// 玩家没有托管， 加个监听器
				OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
				extPlayerActDelegate->AddListener(playerPos, 
					PlayerActionDelegateListener<MsgDelegateHuanZhang>::Create(this, AutoSelect, oriDelay, THING_CHANGE_NOTIFY));
			}
		}

		LOGGER_FILE(context, "question huan zhang");
	}

public:

	void OnSend(GameContext *context, void *data)
	{
		HuanZhangQuestionData *questionData = (HuanZhangQuestionData*)data;
		m_Cards[questionData->playerPos] = questionData->cards;

		SendQuestionToPlayer(context, questionData->playerPos);
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		if(m_Cards.find(playerPos) == m_Cards.end()) 
			return false;

		EXTRACT_REQUEST(tagChangePsEx, data, dataSize, req);
		if(!req)
		{
			LOGGER_FILE(context, "data size error");
			return false;
		}

		std::set<int> cardType;
		for(int i = 0; i < ARRAY_LEN(req->byChangePs); ++i)
		{
			cardType.insert(req->byChangePs[i] / 10);
		}

		if(cardType.size() > 1)
		{
			// 不只一种花色
			LOGGER_FILE(context, "invalid selection "<<(int)req->byChangePs[0]<<" "<<(int)(int)req->byChangePs[1]<<" "<<(int)req->byChangePs[2]);
			LOGGER_FILE(context, "question again");
			SendQuestionToPlayer(context, playerPos);
			return false;
		}

		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		bool ret = false;
		std::vector<int> selectIdx;
		for(int i = 0; i < ARRAY_LEN(req->byChangePs); ++i)
		{
			CardList cards = m_Cards[playerPos];
			for(int j = 0; j < cards.Count(); ++j)
			{
				if(cards[j] == req->byChangePs[i])
				{
					cards[j] = 0; // 标记为已选
					selectIdx.push_back(j);
					break;
				}
			}
		}

		ret = (int)selectIdx.size() == ARRAY_LEN(req->byChangePs);

		if(ret)
		{
			OnSelect(this, context, playerPos, selectIdx, msgID);
		}

		return ret;
	}
};

#endif