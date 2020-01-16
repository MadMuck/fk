#ifndef MSG_DELEGATE_INTERCEPTION_H
#define MSG_DELEGATE_INTERCEPTION_H

#include "GameContext.h"
#include "MessageDelegate.h"
#include <map>
#include "QuestionInterception.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionProcedureController.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionLogger.h"
#include "ExtensionGameParam.h"
#include "ExtensionPlayer.h"
#include "PlayerActionDelegate.h"
#include "PlayerActionDelegateListener.h"
#include "ExtensionLogger.h"

#include "UpgradeMessageEx.h"
#include "AdapterUtil.h"
#include "Util.h"


struct InterceptionResponseData
{
	enum ActionType
	{
		Chi,
		Peng,
		Gang,
		Win,
		Ting,
		Ignore
	};

	InterceptionResponseData(ActionType t, void *d, int s):actionType(t), data(d), dataSize(s){}

	int actionType;
	void *data;
	int dataSize;
};

///////////////////////////////////////////////////////////////////////////
// 等待拦牌
class MsgDelegateInterception : public MessageDelegateImpl
{
protected:
	std::map<PlayerPos::Type, QuestionInterceptionData> m_InterceptionData;

	static void AutoInterception(GameContext *context, int actionType, PlayerPos::Type playerPos, void *data)
	{
		MsgDelegateInterception *msgDelegate = (MsgDelegateInterception *)data;
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);

		//自动操作时可胡牌自动胡牌
		InterceptionList interceptionList = msgDelegate->m_InterceptionData[playerPos].interceptions;
		for(InterceptionList::iterator itr = interceptionList.begin(); itr != interceptionList.end(); ++itr)
		{
			if(itr->type == Interception::Hu)
			{
				tagHuPai msg;
				msg.byDianPao = itr->src.playerPos;
				msg.byPs = itr->src.card;
				msg.byUser = playerPos;
				msg.bZimo = playerPos == itr->src.playerPos;
				InterceptionResponseData responseData(InterceptionResponseData::Win, &msg, sizeof(msg));
				OnSelectAction(msgDelegate, context, playerPos, responseData);
				return;
			}
		}
		// 过
		InterceptionResponseData responseData(InterceptionResponseData::Ignore, NULL, 0);
			OnSelectAction(msgDelegate, context, playerPos, responseData);

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

public:
	
	typedef PlayerActionDelegateListener<MsgDelegateInterception> InterceptionDelegateListener;

	void OnSend(GameContext *context, void *data)
	{
		QuestionInterceptionData &intcData = *((QuestionInterceptionData *)data);
		m_InterceptionData[intcData.playerPos] = intcData;

		OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
		OBJ_GET_EXT(context, ExtensionGameRunningData, extGameRunningData);

		{
			INIT_MSG(SC_NotifyHaveAction, msg, 0, dataToSend, dataSendSize);
			msg.byDeskStation = intcData.playerPos;
			extGameRunningData->SendDataToAll(dataToSend, dataSendSize, THING_HAVE_ACTION);
		}


		{
			INIT_MSG(tagCPGNotifyEx, msg, 0, dataToSend, dataSendSize);

			FillInterceptionData(intcData.playerPos, intcData.interceptions, msg);
			msg.bZhuaPai = extProcCtrl->GetProcedureController()->RunningProcedureType() == ProcedureType::AfterDrawCard;

			OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);

			extGameRunningData->SendDataToPlayer(intcData.playerPos, dataToSend, dataSendSize, THING_CPG_NOTIFY);

			bool autoHu = false;
			for(auto itr = intcData.interceptions.begin(); itr != intcData.interceptions.end();++itr)
			{
				if(itr->type == Interception::Hu)
				{
					autoHu = true;
					break;
				}
			}
			if(autoHu && extGameParam->IsTrue(GameParam_PM_BiHu) /*|| extGameParam->GetParam(GameParam_ActionDelegateEnabled) > 0*/)
			{
				float oriDelay = /*extGameParam->GetParam(GameParam_DecisionTimeLimit) **/ 1.0f;
				float delay = oriDelay;
				OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

				if(extPlayer->GetPlayerStatus(intcData.playerPos).GetBool(PlayerStatus_ActionDelegate))
				{
					delay = 1;
				}
				else
				{
					// 没有开托管时，加一个托管行为监听
					OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
					extPlayerActDelegate->AddListener(intcData.playerPos, 
						InterceptionDelegateListener::Create(this, AutoInterception, oriDelay, THING_CPG_NOTIFY));
				}

				// 设置超时自动决定拦牌选项
				InterceptionDelegateListener::AddActionDelegate(context, intcData.playerPos, AutoInterception, delay, THING_CPG_NOTIFY, this);
			}

			LOGGER_FILE(context, "intercepted card : "<<(int)msg.byPs);
			LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" have actions "<<intcData.interceptions);
		}

		
	}

	bool OnResponse(GameContext *context, PlayerPos::Type playerPos, void *data, int dataSize, int msgID)
	{
		InterceptionResponseData &intcResponseData = *((InterceptionResponseData *)data);

		OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
		extPlayerActDelegate->OnPlayerResponse(playerPos, THING_CPG_NOTIFY);

		return OnSelectAction(this, context, playerPos, intcResponseData);
	}

	static bool OnSelectAction(MsgDelegateInterception *msgDelegate, GameContext *context, PlayerPos::Type playerPos, 
		const InterceptionResponseData &intcResponseData)
	{
		std::map<PlayerPos::Type, QuestionInterceptionData>::iterator dataItr = msgDelegate->m_InterceptionData.find(playerPos);
		if(dataItr == msgDelegate->m_InterceptionData.end())
		{
			LOGGER_FILE(context, "user have not any actions");
			return false;
		}

		QuestionInterceptionData &intcData = dataItr->second;

		switch(intcResponseData.actionType)
		{
		case InterceptionResponseData::Chi:
			msgDelegate->Chi(context, intcData, *(tagChiPai *)intcResponseData.data, intcResponseData.dataSize);
			break;
		case InterceptionResponseData::Peng:
			msgDelegate->Peng(context, intcData, *(tagPengPai *)intcResponseData.data, intcResponseData.dataSize);
			break;
		case InterceptionResponseData::Gang:
			msgDelegate->Gang(context, intcData, *(tagGangPai *)intcResponseData.data, intcResponseData.dataSize);
			break;
		case InterceptionResponseData::Win:
			msgDelegate->Win(context, intcData, *(tagHuPai *)intcResponseData.data, intcResponseData.dataSize);
			break;
		case InterceptionResponseData::Ting:
			msgDelegate->Ting(context, intcData);
			break;
		case InterceptionResponseData::Ignore:
			msgDelegate->Ignore(context, intcData);
			break;
		}

		msgDelegate->m_InterceptionData.erase(dataItr);

		// "等待拦牌" 结束， 删除托管监听器
		OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
		extPlayerActDelegate->RemoveListener(playerPos);

		return true;
	}

	void Chi(GameContext *context, const QuestionInterceptionData &intcData, const tagChiPai &msg, int dataSize)
	{
		LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" select chi "
			<<(int)msg.byChiPs[0]<<" "<<(int)msg.byChiPs[1]<<" "<<(int)msg.byChiPs[2]);

		int selectIdx = -1;
		for(int i = 0; i < intcData.interceptions.Count(); ++i)
		{
			CardList cards;
			cards.insert(cards.begin(), msg.byChiPs, msg.byChiPs + 3);
			if(intcData.interceptions[i].cards == cards)
			{
				selectIdx = i;
				break;
			}
		}
		if(selectIdx < 0)
		{
			LOGGER_ERROR_FILE(context, "no such action");
			return;
		}
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->OnResponse(intcData.playerPos, THING_CHI_PAI, selectIdx);
	}

	void Peng(GameContext *context, const QuestionInterceptionData &intcData, const tagPengPai &msg, int dataSize)
	{
		LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" select "<<(int)msg.byPs);

		InterceptionList::const_iterator itr = intcData.interceptions.Find(Interception::Peng, msg.byPs);
		if(itr == intcData.interceptions.end())
		{
			LOGGER_ERROR_FILE(context, "no action peng with card "<<(int)msg.byPs);
			return;
		}
		int selectIdx = itr - intcData.interceptions.begin();
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->OnResponse(intcData.playerPos, THING_PENG_PAI, selectIdx);
	}

	void Gang(GameContext *context, const QuestionInterceptionData &intcData, const tagGangPai &msg, int dataSize)
	{
		LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" select "<<(int)msg.byPs);

		InterceptionList::const_iterator itr = intcData.interceptions.end();

		Interception::Type findType[] = 
		{
			Interception::AnGang,
			Interception::BuGang,
			Interception::MingGang,
		//	Interception::YangMa,
			Interception::AnGangTing,
			Interception::BuGangTing,
			Interception::MingGangTing,
		};

		int findTypeLen = sizeof(findType) / sizeof(findType[0]);
		for(int i = 0; i < findTypeLen; ++i)
		{
			itr = intcData.interceptions.Find(findType[i], msg.byPs);
			if(itr != intcData.interceptions.end())
			{
				break;
			}
		}

		if(itr == intcData.interceptions.end())
		{
			LOGGER_ERROR_FILE(context, "no action gang with card "<<(int)msg.byPs);
			return;
		}

		int selectIdx = itr - intcData.interceptions.begin();
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->OnResponse(intcData.playerPos, THING_GANG_PAI, selectIdx);
	}

	void Win(GameContext *context, const QuestionInterceptionData &intcData, const tagHuPai &msg, int dataSize)
	{
		LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" select "<<(int)msg.byPs);

		InterceptionList::const_iterator itr = intcData.interceptions.Find(Interception::Hu, msg.byPs);
		if(itr == intcData.interceptions.end())
		{
			LOGGER_ERROR_FILE(context, "no action win with card "<<(int)msg.byPs);
			return;
		}
		int selectIdx = itr - intcData.interceptions.begin();
		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->OnResponse(intcData.playerPos, THING_HU_PAI, selectIdx);
	}

	void Ting(GameContext *context, const QuestionInterceptionData &intcData)
	{
		LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" ting ");

		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		int selectIdx = 0;
		for(InterceptionList::const_iterator itr = intcData.interceptions.begin();
			itr != intcData.interceptions.end();
			++itr)
		{
			if(Interception::Ting == itr->type)
			{
				selectIdx = itr - intcData.interceptions.begin();
				break;
			}
		}
		extGamePlayerConnector->OnResponse(intcData.playerPos, THING_TING_PAI, selectIdx);
	}

	void Ignore(GameContext *context, const QuestionInterceptionData &intcData)
	{
		LOGGER_FILE(context, "player "<<(int)intcData.playerPos<<" ignore ");

		OBJ_GET_EXT(context, ExtensionGamePlayerConnector, extGamePlayerConnector);
		extGamePlayerConnector->OnResponse(intcData.playerPos, -1, -1);
	}
};

#endif