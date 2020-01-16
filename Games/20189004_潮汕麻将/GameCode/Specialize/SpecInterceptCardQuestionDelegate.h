#ifndef SPEC_INTERCEPT_CARD_QUESTION_DELEGATE_H
#define SPEC_INTERCEPT_CARD_QUESTION_DELEGATE_H

#include "PlayerPos.h"
#include "ProcedureInterceptCard.h"
#include "ExtensionGamePlayerConnector.h"
#include "ExtensionPlayer.h"
#include "ExtensionGameDirection.h"
#include "ExtensionGameParam.h"
#include <map>
#include <set>

class SpecInterceptCardQuestionDelegate : public InterceptCardQuestionDelegate
{
	std::map<int, int> m_PriorityMap;
	int m_MostPrior;
	PlayerPos::Type m_PlayerPos;
	std::map<PlayerPos::Type, InterceptionList> m_PlayerActions;
	std::map<PlayerPos::Type, Interception> m_PlayerSelectedAction;
	std::set<PlayerPos::Type> m_QuestionedPlayer;
	bool m_ResponseFinished;
public:
	void Init()
	{
		// 拦牌类型优先级
		m_PriorityMap[Interception::Invalid] = 0;
		m_PriorityMap[Interception::Chi] = 1;
		m_PriorityMap[Interception::Peng] = 1;
		m_PriorityMap[Interception::MingGang] = 1;
		m_PriorityMap[Interception::AnGang] = 1;
		m_PriorityMap[Interception::BuGang] = 1;
	//	m_PriorityMap[Interception::YangMa] = 1;
		m_PriorityMap[Interception::Ting] = 2;
		m_PriorityMap[Interception::ChiTing] = 2;
		m_PriorityMap[Interception::PengTing] = 2;
		m_PriorityMap[Interception::ZhanTing] = 2;
		m_PriorityMap[Interception::Hu] = 3;


		m_MostPrior = Interception::Invalid;
		m_PlayerPos = PlayerPos::Max;
		m_PlayerActions.clear();
		m_PlayerSelectedAction.clear();
		m_QuestionedPlayer.clear();
		m_ResponseFinished = false;
	}

	int GetPriority(int interceptionType) const
	{
		int ret = 0;
		if(m_PriorityMap.find(interceptionType) != m_PriorityMap.end())
		{
			ret = m_PriorityMap.at(interceptionType);
		}
		return ret;
	}

	bool OnQuestionResponsed(GameContext *context, PlayerPos::Type playerPos, const Interception &action, const InterceptionList &interceptions,
		const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		m_PlayerSelectedAction[playerPos] = action;
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);

		// 玩家选择的行动优先级大于等于当前最大优先级，行动有效
		bool ret = PriorityGreater(context, extPlayer->GetActivePlayerPos(), playerPos, action.type, m_PlayerPos, m_MostPrior) ||
			PriorityEqual(context, extPlayer->GetActivePlayerPos(), playerPos, action.type, m_PlayerPos, m_MostPrior);

		if(ret)
		{
			m_MostPrior = action.type;
			m_PlayerPos = playerPos;
		}

		m_ResponseFinished = false;
		if(m_PlayerSelectedAction.size() == m_QuestionedPlayer.size())
		{
			m_ResponseFinished = !TryToSendQuestion(context, sendDelegate);
		}

		return ret;
	}

	bool OnQuestion(GameContext *context, PlayerPos::Type playerPos, const InterceptionList &interceptions, 
		const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		m_PlayerActions[playerPos] = interceptions;
		return true;
	}

	bool OnQuestionFinish(GameContext *context, const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		return TryToSendQuestion(context, sendDelegate);
	}

	bool IsResponseFinished() const
	{
		return m_ResponseFinished;
	}

	bool TryToSendQuestion(GameContext *context, const RefCntObj<InterceptCardSendQuestionDelegate> &sendDelegate)
	{
		int mostPrior = m_MostPrior;
		PlayerPos::Type playerPos = m_PlayerPos;

		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		PlayerPos::Type startPos = extPlayer->GetActivePlayerPos();
		OBJ_GET_EXT(context, ExtensionGameDirection, extGameDir);

		std::set<PlayerPos::Type> questionCandidates;

		// 按玩家行动顺序检查
		PlayerPos::Type playerToCheck = extGameDir->GetNextPos(startPos);
		while(playerToCheck != startPos)
		{
			if(m_QuestionedPlayer.find(playerToCheck) == m_QuestionedPlayer.end())
			{
				std::map<PlayerPos::Type, InterceptionList>::iterator itr = m_PlayerActions.find(playerToCheck);
				if(itr != m_PlayerActions.end())
				{
					InterceptionList &interceptions  = itr->second;
					for(InterceptionList::const_iterator itr2 = interceptions.begin();
						itr2 != interceptions.end();
						++itr2)
					{
						bool prioGreater = PriorityGreater(context, 
							extPlayer->GetActivePlayerPos(), 
							itr->first, itr2->type, 
							playerPos, mostPrior);

						bool prioEqual = PriorityEqual(context, 
							extPlayer->GetActivePlayerPos(), 
							itr->first, itr2->type, 
							playerPos, mostPrior);

						if(prioGreater || prioEqual)
						{
							if(prioGreater)
								questionCandidates.clear();

							mostPrior = itr2->type;
							playerPos = itr->first;
							questionCandidates.insert(playerPos);
						}
					}
				}

			}
			playerToCheck = extGameDir->GetNextPos(playerToCheck);
		}

		bool ret = questionCandidates.size() > 0;
		if(ret)
		{
			for(std::set<PlayerPos::Type>::iterator itr = questionCandidates.begin();
				itr != questionCandidates.end();
				++itr)
			{
				PlayerPos::Type playerToQuestion = *itr;
				m_QuestionedPlayer.insert(playerToQuestion);
				sendDelegate->SendQuestion(context, playerToQuestion, m_PlayerActions[playerToQuestion]);
			}
		}

		return ret;
	}

	bool PriorityGreater(GameContext *context, PlayerPos::Type srcPlayerPos, PlayerPos::Type playerPos1, 
		int interceptionType1, PlayerPos::Type playerPos2,  int interceptionType2) const
	{
		int p1 = GetPriority(interceptionType1);
		int p2 = GetPriority(interceptionType2);
		if (p1 > p2)
			return true;

		if(p1 == p2)
		{
			OBJ_GET_EXT(context, ExtensionGameDirection, extGameDir);
			return extGameDir->IsPreceding(playerPos1, playerPos2, srcPlayerPos);
		}

		return false;
	}

	bool PriorityEqual(GameContext *context, PlayerPos::Type srcPlayerPos, PlayerPos::Type playerPos1, int interceptionType1,
		PlayerPos::Type playerPos2, int interceptionType2) const
	{
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		// 一炮多响开关
		bool multipleLightEnabled = extGameParam->IsTrue(GameParam_MultipleLight);

		return GetPriority(interceptionType1) == GetPriority(interceptionType2) && 
			(playerPos1 == playerPos2 || 
			(multipleLightEnabled && GetPriority(interceptionType1) == GetPriority(Interception::Hu)));
	}

};

#endif