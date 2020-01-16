#ifndef SPEC_CARD_SCORE_DELEGATE_H
#define SPEC_CARD_SCORE_DELEGATE_H

#include "ExtensionLiquidation.h"
#include "ExtensionScoreItemList.h"
#include "ExtensionProcedureController.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHuPattern.h"
#include "ExtensionPlayer.h"
#include "ExtensionGameParam.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionAbandonedAction.h"
#include "ExtensionInvalidInterception.h"
#include "ExtensionBankerSelector.h"
#include "ExtensionCardBlocks.h"
#include "ExtensionHaiDi.h"
#include "ExtensionActionValidator.h"
#include "NormalWinCheckUtil.h"
#include "ExtensionLogger.h"
#include "ExtensionGameParam.h"

#include "HuType.h"

#include "PatternFinder.h"
#include "PatternType.h"
#include "WinType.h"
#include "PlayerStatusDef.h"

#include "ScoreItemHu.h"
#include "ScoreItemGang.h"
#include "ScoreItemPaiTou.h"

#include "SpecProcedureTypeDef.h"
#include "ScoreTypeDef.h"

class SpecScoreInfo
{
public:
	SpecScoreInfo():fan(0), score(0){}
	int fan;
	int score;
};

class SpecCardScoreDelegate : public CardScoreDelegate
{
	GameContext *m_GameContext;

public:

	SpecCardScoreDelegate(GameContext *context):m_GameContext(context)
	{
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
	}

	static int GetFanWithWinTypes(GameContext *context, const WinTypeList &winTypeList)
	{
		OBJ_GET_EXT(context,ExtensionGameParam,extGameParam);
		int multi = 0;
		if(winTypeList.HasType(WIN_TYPE_JI_HU))//鸡胡
			multi = 1;

		if(winTypeList.HasType(WIN_TYPE_TIAN_HU) || winTypeList.HasType(WIN_TYPE_DI_HU)) //天地胡
			multi  += extGameParam->IsTrue(GameParam_PM_TianDiHu) ? 10 : 2 ;
		if(winTypeList.HasType(WIN_TYPE_SHI_SAN_YAO)) //十三幺
			multi += extGameParam->IsTrue(GameParam_PM_ShiSanYao) ? 10 : 2;
		if(winTypeList.HasType(WIN_TYPE_18LUOHAN))//18罗汉
			multi +=  extGameParam->IsTrue(GameParam_PM_18LuoHan) ? 10 : 2;

		if(winTypeList.HasType(WIN_TYPE_TRIPLE_S_QIDUI))//三豪华七对
			multi +=  extGameParam->IsTrue(GameParam_PM_TripleSpQiDui) ? 8 : 2;
		else if(winTypeList.HasType(WIN_TYPE_DOUBLE_S_QIDUI))//双豪华七对
			multi +=  extGameParam->IsTrue(GameParam_PM_DoubleSpQiDui) ? 6 : 2;
		else if(winTypeList.HasType(WIN_TYPE_SUPER_QIDUI))//豪华七对
			multi +=  extGameParam->IsTrue(GameParam_PM_SuperQiDui) ? 4 : 2;
		else if(winTypeList.HasType(WIN_TYPE_QIXIAODUI))//七对
			multi +=  extGameParam->IsTrue(GameParam_PM_QiXiaoDui) ? 3 : 2;

		if(winTypeList.HasType(WIN_TYPE_YAO_JIU))//幺九
			multi +=  extGameParam->IsTrue(GameParam_PM_YaoJiu) ? 2 : 2;

		if(winTypeList.HasType(WIN_TYPE_QING_YI_SE))//清一色
			multi += extGameParam->IsTrue(GameParam_PM_QingYiSe) ? 3 : 2;
		else if(winTypeList.HasType(WIN_TYPE_HUN_YI_SE))//混一色
			multi += extGameParam->IsTrue(GameParam_PM_HunYiSe) ? 2 : 2;

		if(winTypeList.HasType(WIN_TYPE_DA_SIXI))//大四喜
			multi += extGameParam->IsTrue(GameParam_PM_BigSiXi) ? 6 : 2;
		else if(winTypeList.HasType(WIN_TYPE_XIAO_SIXI))//小四喜
			multi += extGameParam->IsTrue(GameParam_PM_SmalSiXi) ? 4 : 2;

		if(winTypeList.HasType(WIN_TYPE_DA_SANYUAN))//大三元
			multi += extGameParam->IsTrue(GameParam_PM_BigSanYuan) ? 6 : 2;
		else if(winTypeList.HasType(WIN_TYPE_XIAO_SANYUAN))//小三元
			multi += extGameParam->IsTrue(GameParam_PM_SmallSanYuan) ? 4 : 2;

		if(winTypeList.HasType(WIN_TYPE_DUI_DUI_HU))//对对胡
			multi +=extGameParam->IsTrue(GameParam_PM_PengPengHU) ? 2 : 2;

		//以下三项为附加倍数
		if(winTypeList.HasType(WIN_TYPE_HAIDILAO)) //海底捞两倍
			multi += 2;//固定两倍
		if(winTypeList.HasType(WIN_TYPE_QIANG_GANG)) //抢杠胡
			multi += extGameParam->IsTrue(GameParam_PM_QiangGangHu) ? 2 : 1 ;
		if(winTypeList.HasType(WIN_TYPE_GANG_BAO)) //杠爆
			multi += extGameParam->IsTrue(GameParam_PM_GangKai) ? 2 : 1 ;

		return multi;
	}

	static SpecScoreInfo GetMaxScore(GameContext *context, const CardList &cardList, const InterceptionList &interceptions, 
		PlayerPos::Type playerPos, PlayerPos::Type bankerPos)
	{
		SpecScoreInfo maxScoreInfo;

		CardList cardsToCheck = cardList;
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
		const CardStat &cardStat = extCardBlocks->GetCardBlocks()->GetCardStat();

		DefaultTingHandlerImpl TingHandler;
		CardStatistic stat(cardList, interceptions);
		CardList cards = TingHandler.AllCardsToWin(context,playerPos,stat);
		std::set<Card::Type> TingCards;
		for(CardList::iterator itr = cards.begin(); itr != cards.end(); ++itr)
		{
			TingCards.insert(*itr);
		}

		WinType &winType = extLiquidation->GetWinType();
		WinType temp_winType;

		for(std::set<Card::Type>::const_iterator itr = TingCards.begin();
			itr != TingCards.end();
			++itr)
		{
			cardsToCheck.Add(*itr);
			sort(cardsToCheck.begin(),cardsToCheck.end());
			CardInfo cardInfo(PlayerPos::Max, *itr); // 不算自摸
			SpecScoreInfo scoreInfo = GetScore(context, cardsToCheck, interceptions, cardInfo, playerPos, bankerPos, &temp_winType);
			if(scoreInfo.score > maxScoreInfo.score)
			{
				maxScoreInfo = scoreInfo;
				winType.Clear(playerPos);
				WinTypeList temp_winTypeList = temp_winType.GetWinType(playerPos);
				for(WinTypeList::iterator itr = temp_winTypeList.begin();
					itr != temp_winTypeList.end();
					++itr)
				{
					winType.SetWinType(playerPos, *itr);
				}
			}
			cardsToCheck.Remove(*itr, 1);
		}

		return maxScoreInfo;
	}

	// 计算牌型(不包含天地胡、杠开、抢杠等与流程有关的牌型)
	static WinTypeList GetWinType(GameContext *context, const CardList &cardList, const InterceptionList &interceptions,const CardInfo &src, PlayerPos::Type playerPos)
	{
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
		CardStatistic stat(cardList, interceptions);
		WinTypeList winTypeList;

		int jokernum = 0;
		CardStatistic noJokerstat = stat;

		WinCheckUtilWithResult checkUtil;
		auto results = checkUtil.FindResult(stat);

		if(IsShiSanYao(noJokerstat, jokernum))
			winTypeList.Add(WIN_TYPE_SHI_SAN_YAO);			//十三幺

		if(!results.empty())
		{
			if(IsQingYiSe(noJokerstat))								
				winTypeList.Add(WIN_TYPE_QING_YI_SE);		//清一色
			if(IsHunYiSe(noJokerstat))								
				winTypeList.Add(WIN_TYPE_HUN_YI_SE);		//混一色

			if( IsDuiDuiHu(interceptions, results))
			{
				 if(IsYaoJiu(context, interceptions, results))
				 {
					 winTypeList.Add(WIN_TYPE_YAO_JIU);				//幺九(幺九的前提是对对胡)
				 }
				 else
				 {
					 winTypeList.Add(WIN_TYPE_DUI_DUI_HU);		//对对胡
				 }
			}
		}
		if(IsQiDui(stat, context, src.card))
		{	
			//三豪华七对
			if( TrippleHaoQi ==SpecQiDuiType(stat))
				winTypeList.Add(WIN_TYPE_TRIPLE_S_QIDUI);
			else if(DoubleHaoQi ==SpecQiDuiType(stat))
				winTypeList.Add(WIN_TYPE_DOUBLE_S_QIDUI);
			else if(HaoQi ==SpecQiDuiType(stat))
				winTypeList.Add(WIN_TYPE_SUPER_QIDUI);
			else 
				winTypeList.Add(WIN_TYPE_QIXIAODUI);
		}

		//大小三元
		if(IsDaSanYuan(stat))
			winTypeList.Add(WIN_TYPE_DA_SANYUAN);
		else if( IsXiaoSanYuan(stat))
			winTypeList.Add(WIN_TYPE_XIAO_SANYUAN);
		//大小四喜
		if( IsDaSiXi(stat))
		{
			winTypeList.Add(WIN_TYPE_DA_SIXI);
			winTypeList.Remove(WIN_TYPE_DUI_DUI_HU);
		}
		else if(IsXiaoSiXi(stat))
		{
			winTypeList.Add(WIN_TYPE_XIAO_SIXI);
			winTypeList.Remove(WIN_TYPE_DUI_DUI_HU);
		}


		//十八罗汉
		if(IsShiBaLuoHan(stat))
		{
			winTypeList.Add(WIN_TYPE_18LUOHAN);
			winTypeList.Remove(WIN_TYPE_DUI_DUI_HU);
		}

		if(winTypeList.empty())
			winTypeList.Add(WIN_TYPE_JI_HU);				//鸡胡
		//海底捞(附加到其余胡牌牌型后)
		if(IsHaiDIHu(context,playerPos))
		{
			winTypeList.Add(WIN_TYPE_HAIDILAO);
		}
		return winTypeList;
	}

	static int GetFan(GameContext *context, const CardList &cardList, const InterceptionList &interceptions, 
		const CardInfo &src, PlayerPos::Type playerPos, PlayerPos::Type bankerPos, WinType *winTypes)
		{
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);
		OBJ_GET_EXT(context, ExtensionLiquidation,extLiq);
		OBJ_GET_EXT(context, ExtensionPlayer, extPlayer);
		OBJ_GET_EXT(context, ExtensionCardBlocks, extCardBlocks);
		OBJ_GET_EXT(context, ExtensionProcedureController, extProcCtrl);
		//天地胡：只有庄家独有的权利，庄家起牌后即成胡牌牌型为天胡，起牌后，第1圈里闲家自摸的第1张牌胡牌为地胡
		bool tianHu = extCardBlocks->GetDrawnCardCount(playerPos) == cardList.Count() && playerPos == src.playerPos && playerPos == bankerPos;
		bool diHu = extCardBlocks->MaxDiscardedCount() <= 1 && 
			extCardBlocks->GetDrawnCardCount(playerPos) == cardList.Count()  && playerPos == src.playerPos && playerPos != bankerPos;

		WinTypeList winTypeList;
		//计算牌型
		winTypeList = GetWinType(context, cardList, interceptions,src, playerPos);

		int fan = 0;
		bool zimo = playerPos == src.playerPos;

		// 加番项
		bool gangKai = false; // 杠上开花
		bool gangPao = false; // 杠上炮
		bool qiangGang = false; // 抢杠

		
		if(zimo) // 自摸
		{
			fan = /*(context->GetPlayerCount()-1) **/ 3 ;
			gangKai = extProcCtrl->IsInLatestHistory(ProcedureType::DrawCardAfterKong);

			if(gangKai)		//直杠其他玩家后摸牌胡牌，称为杠爆
			{
				Interception lastGang = Interception();
				for(auto itr = interceptions.rbegin(); itr != interceptions.rend(); ++itr)
				{
					if(itr->type == Interception::MingGang || itr->type == Interception::AnGang || itr->type == Interception::BuGang /*|| itr->type == Interception::YangMa*/)
					{
						lastGang = *itr;
						break;
					}
				}

				if(lastGang.type == Interception::MingGang)		//这里添加的杠爆牌型只用做判断胡牌后牌头减半
				{
		//			extPlayer->SetGangBaoPos(playerPos, lastGang.src.playerPos);
					winTypeList.Add(WIN_TYPE_GANG_BAO);
				}
			}
			
		//	if(extPlayer->GetGangBaoPos(playerPos) != PlayerPos::Max)	//自摸且存在杠爆关联者，也为杠爆，此时关联者包赔
			//	winTypeList.Add(WIN_TYPE_GANG_BAO);
		}
		else if(src.playerPos != PlayerPos::Max)
		{//点炮
			fan = 3;
			qiangGang = extProcCtrl->IsInLatestHistory(ProcedureType_QiangGang);
			if(qiangGang)
				winTypeList.Add(WIN_TYPE_QIANG_GANG);
		}

		{
			if(tianHu)
				winTypeList.Add(WIN_TYPE_TIAN_HU);
			else if(diHu)
				winTypeList.Add(WIN_TYPE_DI_HU);
		}
		//所有的倍数
		int multi = GetFanWithWinTypes(context, winTypeList); 
		if(extGameParam->IsTrue(GameParam_bFengDing))
		{
			int FengDing = extGameParam->GetParam(GameParam_FengDingMulti);
			multi = multi > FengDing ? FengDing : multi;
		}
		//记录本局最大倍数
		extLiq->SetMaxMulti(multi);
		//最终胡分
		fan *=multi;

		if(winTypes)
		{
			winTypes->Clear(playerPos);
			for(WinTypeList::iterator itr = winTypeList.begin();
				itr != winTypeList.end();
				++itr)
			{
				winTypes->SetWinType(playerPos, *itr);
			}
		}

		return fan;
	}

	static SpecScoreInfo GetScore(GameContext *context, const CardList &cardList, const InterceptionList &interceptions, 
		const CardInfo &src, PlayerPos::Type playerPos, PlayerPos::Type bankerPos, WinType *winTypes)
	{
		OBJ_GET_EXT(context, ExtensionGameParam, extGameParam);

		SpecScoreInfo scoreInfo;

		scoreInfo.fan = GetFan(context, cardList, interceptions, src, playerPos, bankerPos, winTypes);

		scoreInfo.score = scoreInfo.fan;

		return scoreInfo;
	}

	int GetScore(const CardList &cardList, const InterceptionList &interceptions, 
		const CardInfo &src, PlayerPos::Type playerPos, PlayerPos::Type bankerPos, bool reveal) const
	{
		return GetScore(m_GameContext, cardList, interceptions, src, playerPos, bankerPos, NULL).score;
	}

	// 拦牌、 胡牌回调
	void OnAddInterception(PlayerPos::Type srcPlayer, const std::map<PlayerPos::Type, AppendInterceptionInfo> &appendAction)
	{

		OBJ_GET_EXT(m_GameContext, ExtensionAbandonedAction, extAbandonedAction);
		OBJ_GET_EXT(m_GameContext, ExtensionCardBlocks, extCardBlocks);

		for(std::map<PlayerPos::Type, AppendInterceptionInfo>::const_iterator itr = appendAction.begin();
			itr != appendAction.end();
			++itr)
		{
			PlayerPos::Type playerPos = itr->first;
			const Interception &interception = itr->second.interception;
			int idx = itr->second.idx;

			// 动牌解除弃胡状态
			extAbandonedAction->Clear(playerPos);

			OBJ_GET_EXT(m_GameContext, ExtensionScoreItemList, extScoreItemList);
			OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);
			OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);

			// 产生分数结算项
			switch(interception.type)
			{
			case Interception::Peng:
				{
					Player *player = extPlayer->GetPlayer(playerPos);

					if(extPlayer->GetPlayer(playerPos)->GetCards().size() <= 2)		//碰后形成单吊最后一个放碰者为杠爆关联者
					{
						extPlayer->SetGangBaoPos(playerPos,interception.src.playerPos);
					}
				}
				break;
			case Interception::MingGang:
			case Interception::AnGang:
			case Interception::BuGang:
		//	case Interception::YangMa:
				{
					if(Interception::MingGang == interception.type)
					{
						Player *player = extPlayer->GetPlayer(playerPos);

						if(extPlayer->GetPlayer(playerPos)->GetCards().size() <= 2)		//明杠后形成单吊最后一个放杠者为杠爆关联者
						{
							extPlayer->SetGangBaoPos(playerPos,interception.src.playerPos);
						}
					}
					
					extScoreItemList->AddItem(playerPos, ScoreItem(
						new ScoreItemGangImpl(m_GameContext, 
						interception, 
						extLiquidation->GetWinners(), 
						idx)
						)
						);
					LOGGER_FILE(m_GameContext, "明杠"<<(int)(Interception::MingGang == interception.type)<<
																      "暗杠"<<(int)(Interception::AnGang == interception.type)<<
																   	 "补杠"<<(int)(Interception::BuGang == interception.type)
										 );
				}
				break;
			case Interception::Hu:
				{
					if(0 == extCardBlocks->CanDraw()) 
					{
						OBJ_GET_EXT(m_GameContext, ExtensionHaiDi, extHaiDi);
						extHaiDi->playerlist.insert(playerPos);
					}

					Player *player = extPlayer->GetPlayer(playerPos);
					CardList cards = player->GetCards();
// 					if(interception.src.playerPos != playerPos)
// 					{
// 						cards.Add(interception.src.card);
// 					}

					OBJ_GET_EXT(m_GameContext, ExtensionBankerSelector, extBankerSelector);
					PlayerPos::Type bankerPos = extBankerSelector->GetBankerPos();

					OBJ_GET_EXT(m_GameContext, ExtensionLiquidation, extLiquidation);

					WinType &winType = extLiquidation->GetWinType();

					const InterceptionList &interceptions = player->GetInterceptions();

					Interception winAction = interception;

					bool baoPei = false;

					PlayerPos::Type triggerPlayer = winAction.src.playerPos;

					// 计算分数，牌型
					SpecScoreInfo scoreInfo = GetScore(m_GameContext, cards, interceptions,
						winAction.src, playerPos, bankerPos, &winType);
					OBJ_GET_EXT(m_GameContext, ExtensionGameParam, extParam);
					if(extParam->IsTrue(GameParam_PM_GangBao) && winType.GetWinType(playerPos).HasType(WIN_TYPE_QIANG_GANG))		//抢杠胡由被抢的玩家全包
					{
						baoPei = true;
					}
					//else if(winType.GetWinType(playerPos).HasType(WIN_TYPE_GANG_BAO) && extPlayer->GetGangBaoPos(playerPos) != PlayerPos::Max) //杠爆全包？？貌似没有此需求
					//{
					//	baoPei = true;
					//	triggerPlayer = extPlayer->GetGangBaoPos(playerPos);
					//}

					// 添加胡结算项
					extScoreItemList->AddItem(playerPos, ScoreItem(
						new ScoreItemHuImpl(m_GameContext, 
						triggerPlayer,
						extLiquidation->GetWinners(),
						scoreInfo.score,
						ScoreType_Hu,
						baoPei)
						)
						);

					// 添加牌头算项（翻马解算）
					extScoreItemList->AddItem(playerPos, ScoreItem(
						new ScoreItemPaiTouImpl(m_GameContext, 
						triggerPlayer,
						extLiquidation->GetWinners(),
						1,//正常一个马为1分
						ScoreType_PaiTou,
						baoPei)
						)
						);

					int TotalFan = scoreInfo.fan;

					// 记录总番数
					extScoreItemList->SetWinningFan(playerPos, TotalFan);

					// 检查是否杠炮，杠分转移
					OBJ_GET_EXT(m_GameContext, ExtensionProcedureController, extProcCtrl);

					// 检查是否抢杠
					if(extProcCtrl->IsInLatestHistory(ProcedureType_QiangGang))
					{
						OBJ_GET_EXT(m_GameContext, ExtensionPlayer, extPlayer);
						PlayerPos::Type srcPlayer = interception.src.playerPos;

						// 回滚最后一个杠
						Interception gang = extPlayer->GetPlayer(srcPlayer)->GetInterceptions().back();
						if((gang.type == Interception::BuGang /*|| gang.type == Interception::YangMa*/) && gang.src.card == interception.src.card)
						{
							Interception peng = gang;
							peng.cards.pop_back();
							peng.type = Interception::Peng;

							extPlayer->GetPlayer(srcPlayer)->RemoveInterception(gang);
							extPlayer->GetPlayer(srcPlayer)->AddInterception(peng);

							OBJ_GET_EXT(m_GameContext, ExtensionScoreItemList, extScoreItemList);
							extScoreItemList->RemoveLastItem(srcPlayer, ScoreType_BuGang); // 删除最后一个杠分
						}
					}
				}
				break;
			}
		}
	}

	//对对胡,大对子
	static bool IsDuiDuiHu(const InterceptionList &interceptionList, const std::vector<std::vector<PatternResult> > &result)
	{
		//for(auto itr = interceptionList.begin(); itr != interceptionList.end(); ++itr)
		//{
		//	if(Interception::Chi == itr->type)
		//		return false;
		//}

		for(std::vector<std::vector<PatternResult> >::const_iterator itr = result.begin(); itr != result.end(); ++itr)
		{
			bool flag = true;
			for(std::vector<PatternResult>::const_iterator itr2 = itr->begin(); itr2 != itr->end(); ++itr2)
			{
				if(PatternType_JokerSet == itr2->Type() || PatternType_Set == itr2->Type() || PatternType_ThreeJoker == itr2->Type())
				{
					continue;
				}
				else
				{
					flag = false;
					break;
				}
			}

			if(flag)
			{
				return flag;
			}
		}

		return false;
	}

	// 判断花色数，return 0:字一色 1:清一色 2:混一色 3:缺一门 4:均不是
	static int IsYiSe(const CardStatistic &stat)
	{
		std::set<int> types;
		bool havezi = false;
		CardStatistic::ValuesType values = stat.GetStat().Values();
		for(CardStatistic::ValuesType::iterator itr = values.begin();
			itr != values.end();
			++itr)
		{
			if(*itr >= Card::FengStart)
				havezi = true;
			else
				types.insert((*itr) / 10);
		}

		const InterceptionList &interceptions = stat.GetInterceptions();
		for(InterceptionList::const_iterator itr = interceptions.begin();
			itr != interceptions.end();
			++itr)
		{
			if(itr->type == Interception::Hu)
				continue;
			const CardList &cards = itr->cards;
			for(CardList::const_iterator itr2 = cards.begin(); itr2 != cards.end(); ++itr2)
			{
				if(*itr2 >= Card::FengStart)
					havezi = true;
				else
					types.insert((*itr2) / 10);
			}
		}
		if(types.size() == 0 && havezi)
			return 0;
		if(types.size() == 1 && !havezi)
			return 1;
		else if(types.size() == 1 && havezi)
			return 2;
		else if(types.size() == 2)
			return 3;
		else
			return 4;
	}

	//清一色
	static bool IsQingYiSe(const CardStatistic &stat)
	{
		return 1 == IsYiSe(stat);
	}
	static bool IsHunYiSe(const CardStatistic &stat)
	{
		return 2 == IsYiSe(stat);
	}

	//字一色
	static bool IsZiYiSe(const CardStatistic &stat)
	{
		return 0 == IsYiSe(stat);
	}

	//海底捞月
	static bool IsHaiDIHu(GameContext *context, PlayerPos::Type playerPos)
	{
		OBJ_GET_EXT(context, ExtensionHaiDi, extHaiDi);
		OBJ_GET_EXT(context, ExtensionLiquidation, extLiquidation);
		if(1 == extHaiDi->playerlist.size() && extHaiDi->playerlist.find(playerPos) != extHaiDi->playerlist.end())
		{
			return extLiquidation->GetWinMode(playerPos) == WinMode_Self;
		}
		return false;
	}

	//四喜检测
	static int IsSiXi(const CardStatistic &stat)
	{
		CardList checkFeng;
		const InterceptionList &interceptions = stat.GetInterceptions();
		for(Card::Type i = Card::FengDong; i <= Card::FengBei; ++i)
		{
			checkFeng.Add(i);
			if(stat.Num(i) >=3)
			{
				checkFeng.Remove(i,1);
			}
		}
		for(InterceptionList::const_iterator itr = interceptions.begin(); itr != interceptions.end(); ++itr)
		{
			if(itr->type == Interception::MingGang || itr->type == Interception::AnGang || itr->type == Interception::BuGang || itr->type == Interception::Peng)
			{
				if(itr->src.card >= Card::FengStart && itr->src.card < Card::FengEnd)
				{
					checkFeng.Remove(itr->src.card,1);
				}
			}
		}
		if(1 == checkFeng.size())
		{
			if(stat.Num(checkFeng[0]) == 2)
			{
				return 1;				 //小四喜
			}
		}
		else if(0 == checkFeng.size())
		{
			return 2;					//大四喜
		}
		return 0;
	}

	//小三元
	static bool IsXiaoSiXi(const CardStatistic &stat)
	{
		return 1 == IsSiXi(stat);
	}

	//大三元
	static bool IsDaSiXi(const CardStatistic &stat)
	{
		return 2 == IsSiXi(stat);
	}

	//十八罗汉
	static bool IsShiBaLuoHan(const CardStatistic &stat)
	{
		const InterceptionList &interceptions = stat.GetInterceptions();
		int cnt = 0;
		for(InterceptionList::const_iterator itr = interceptions.begin(); itr != interceptions.end(); ++itr)
		{
			if(itr->type == Interception::MingGang || itr->type == Interception::AnGang || itr->type == Interception::BuGang)
			{
				++cnt;
			}
		}
		return 4 == cnt;
	}

	//三元检测
	static int IsSanYuan(const CardStatistic &stat)
	{
		CardList checkZFB;
		const InterceptionList &interceptions = stat.GetInterceptions();
		for(Card::Type i = Card::ZiZhong; i <= Card::ZiBai; ++i)
		{
			checkZFB.Add(i);
			if(stat.Num(i) >=3)
			{
				checkZFB.Remove(i,1);
			}
		}

		for(InterceptionList::const_iterator itr = interceptions.begin(); itr != interceptions.end(); ++itr)
		{
			if(itr->type == Interception::MingGang || itr->type == Interception::AnGang || itr->type == Interception::BuGang || itr->type == Interception::Peng)
			{
				if(itr->src.card == Card::ZiZhong || itr->src.card == Card::ZiFa || itr->src.card == Card::ZiBai)
				{
					checkZFB.Remove(itr->src.card,1);
				}
			}
		}

		if(1 == checkZFB.size())
		{
			if(stat.Num(checkZFB[0]) == 2)
			{
				return 1;					//小三元
			}
		}
		else if(0 == checkZFB.size())
		{
			return 2;						//大三元
		}

		return 0;
	}

	//小三元
	static bool IsXiaoSanYuan(const CardStatistic &stat)
	{
		return 1 == IsSanYuan(stat);
	}

	//大三元
	static bool IsDaSanYuan(const CardStatistic &stat)
	{
		return 2 == IsSanYuan(stat);
	}

	// 七对
	static bool IsQiDui(const CardStatistic &stat, GameContext * context, Card::Type card)
	{
		int pairsCnt = 0;
		CardStatistic::ValuesType values = stat.GetStat().FindValuesWithNum(std::greater_equal<int>(), 2);
		for (CardStatistic::ValuesType::iterator itr = values.begin();
			itr != values.end();
			++itr)
		{
			pairsCnt += stat.Num(*itr) / 2;
		}
		return pairsCnt >= 7;       
	}

	//七对前提下判断是否豪华七对
	enum QiDuiType{
		NoHaoQi,
		HaoQi =1 ,
		DoubleHaoQi ,
		TrippleHaoQi ,
	};
	static int SpecQiDuiType(const CardStatistic &stat)
	{
		int pair = stat.GetStat().FindValuesWithNum(std::greater_equal<int>(),4).size();
		if(pair == 1)
			return HaoQi;
		else if(pair == 2)
			return DoubleHaoQi;
		else if(pair == 3)
			return TrippleHaoQi;
		else 
			return NoHaoQi;
	}

	// 幺九 
	static bool IsYaoJiu(GameContext *context, const InterceptionList &interceptionList, const std::vector<std::vector<PatternResult> > &result)
	{
		char cards_13[] = {1,9,11,19,21,29};
		CardList card13List;
		int cardInterval;
		bool isFirst19 = true;
		card13List.insert(card13List.begin(), cards_13, cards_13 + sizeof(cards_13));
		for(InterceptionList::const_iterator itr = interceptionList.begin();
			itr != interceptionList.end();
			++itr)
		{
			if(itr->type == Interception::Hu)
				continue;
			const CardList &cards = itr->cards;
			for(CardList::const_iterator itr2 = cards.begin(); itr2 != cards.end(); ++itr2)
			{
				if(!card13List.Find(*itr2) && !Card::IsZi((int)*itr2))		//拦牌中存在非十三幺的牌直接返回
					return false;
				if(card13List.Find(*itr2))
				{
					if(isFirst19)
					{
						cardInterval = *itr2 / 10;
						isFirst19 = false;
					}
					else if(cardInterval != *itr2 /10) //同一种序数牌的1，9
					{
						return false;
					}	
				}
			}
		}
		OBJ_GET_EXT(context, ExtensionHuPattern, extHuPattern);
		for(std::vector<std::vector<PatternResult> >::const_iterator itr = result.begin(); itr != result.end(); ++itr)
		{
			for(std::vector<PatternResult>::const_iterator itr2 = itr->begin(); itr2 != itr->end(); ++itr2)
			{
				const CardList &cards = itr2->GetCards();
				for(CardList::const_iterator itr3 = cards.begin(); itr3 != cards.end(); ++itr3)
				{
					if(!card13List.Find(*itr3) && !Card::IsZi((int)*itr3))		//手牌中存在非十三幺且不是字牌的牌直接返回
						return false;
					
					if(card13List.Find(*itr3))
					{
						if(isFirst19)
						{
							cardInterval = *itr3 / 10;
							isFirst19 = false;
						}
						else if(cardInterval != *itr3 / 10) //同一种序数牌的1，9
						{
							return false;
						}	
					}
				}

			}
		}
		return true;
	}

	static bool IsShiSanYao(const CardStatistic &stat, int jokernum)
	{
		char cards_13[] = {1,9,11,19,21,29,31,32,33,34,35,36,37};
		CardList card13List;
		int jokerCount = jokernum;
		card13List.insert(card13List.begin(), cards_13, cards_13 + sizeof(cards_13));

		CardList cardsToCheck = stat.GetStat().Values();

		for(auto itr = cardsToCheck.begin(); itr != cardsToCheck.end(); ++itr)
		{
			if(!card13List.Find(*itr))		//存在非十三幺的牌直接返回
				return false;

			card13List.Remove(*itr,1);
		}

		if(card13List.size() <= jokerCount)
			return true;
		else
			return false;
	}
};
#endif