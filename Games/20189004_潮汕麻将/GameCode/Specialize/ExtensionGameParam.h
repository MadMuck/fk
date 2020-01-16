#ifndef RQMJ_EXTENSION_GAME_PARAM_H
#define RQMJ_EXTENSION_GAME_PARAM_H

#include "GameContext.h"
#include <string>

// 配置文件中的值
#define GameParam_PlayerCount "PlayerCount"
#define GameParam_DecisionTimeLimit "DecisionTimeLimit"
#define GameParam_UseDelegateWhenLeft "UseDelegateWhenLeft"
#define GameParam_DelegateActionDelay "DelegateActionDelay"
#define GameParam_UseDelegateWhenTimeOut "UseDelegateWhenTimeOut"
#define GameParam_AutoBegin "AutoBegin"
#define GameParam_BeginWait "BeginWait"
#define GameParam_DingQueTime "DingQueTime"
#define GameParam_HuanZhangTime "HuanZhangTime"
#define GameParam_MultipleLight "MultipleLight" // 开关：一炮多响
#define GameParam_ReadyHandAutoDiscard "ReadyHandAutoDiscard" // 开关：听牌状态中自动打牌
#define GameParam_AbandonWin "AbandonWin" // 开关：弃胡功能
#define GameParam_ActionDelegateEnabled "ActionDelegateEnabled"
#define GameParam_LiquidationDelay "LiquidationDelay"
#define GameParam_ReservedCardCount "ReservedCardCount"
#define GameParam_PlayMode "PlayMode"	//游戏玩法（经典血战玩法，广安玩法）

#define GameParam_ShowTax "ShowTax"
#define GameParam_ShowUserID "ShowUserID"

// 分数
#define GameParam_GameScorePoint "BasePoint"//底分
#define GameParam_MingGangScore "MingGangScore"
#define GameParam_AnGangScore "AnGangScore"
#define GameParam_BuGangScore "BuGangScore"
#define GameParam_FanLimit "FanLimit" //总番数限制？？？

// 内部参数
#define GameParam_HuanZhang "HuanZhang"
#define GameParam_DingQue "DingQue"
#define GameParam_DiFen	  "DiFen"

//潮汕麻将玩法选项
#define GameParam_PlayMethod "PlayMethod"
#define GameParam_bFanMa "bFanMa"//是否翻马
#define GameParam_FanMaNum "FanMaNum"//翻马个数
//#define GameParam_FanMaMulti	"FanMaMulti"//翻马倍数
#define GameParam_FanMaTime "PaiTouTime" 
#define	 GameParam_bFengDing "bFengDing" //是否封顶
#define	 GameParam_FengDingMulti "FengDingMulti" //封顶倍数

//可胡牌型
#define GameParam_bMaGenGang				"bMaGenGang"///马跟杠
#define GameParam_bMaGenDiFen				"bMaGenDiFen"///马跟底分
#define GameParam_PM_KeJiePaoHu			"PM_KeJiePaoHu"//可点炮胡
#define GameParam_PM_GangBao				"PM_GangBao" //杠包自摸（抢杠胡包三家）
#define GameParam_PM_BiHu						"PM_BiHu" //逼胡
#define GameParam_PM_QiangGangHu		"PM_QiangGangHu"
#define GameParam_PM_QiXiaoDui				"PM_QiXiaoDui" //七小对
#define GameParam_PM_HunYiSe				"PM_HunYiSe"
#define GameParam_PM_QingYiSe				"PM_QingYiSe"
#define GameParam_PM_GangKai				"PM_GangKai"
#define GameParam_PM_PengPengHU		"PM_PengPengHU" //对对胡（碰碰胡）
#define GameParam_PM_ShiSanYao				"PM_ShiSanYao" //十三幺
#define GameParam_PM_TianDiHu				"PM_TianDiHu"  //天地胡
#define GameParam_PM_18LuoHan				"PM_18LuoHan" 
#define GameParam_PM_SuperQiDui			"PM_SuperQiDui" //豪华七对
#define GameParam_PM_DoubleSpQiDui		"PM_DoubleSpQiDui"//双豪华七对
#define GameParam_PM_TripleSpQiDui		"PM_TripleSpQiDui"	//三豪华七对
#define GameParam_PM_YaoJiu					"PM_YaoJiu" //幺九
#define GameParam_PM_SmalSiXi				"PM_SmalSiXi"//小四喜
#define GameParam_PM_BigSiXi					"PM_BigSiXi" //大四喜
#define GameParam_PM_SmallSanYuan		"PM_SmallSanYuan"//小三元
#define GameParam_PM_BigSanYuan			"PM_BigSanYuan" //大三元

class ExtensionGameParam : public GameContextExtension
{
	std::map<std::string, int> m_Params;

public:
	int GetParam(const std::string &paramName, int defaultVal = 0) const
	{
		int ret = defaultVal;
		std::map<std::string, int>::const_iterator itr = m_Params.find(paramName);
		if(itr != m_Params.end())
		{
			ret = itr->second;
		}
		return ret;
	}

	bool IsTrue(const std::string &paramName, bool defaultVal = false) const
	{       
		return GetParam(paramName, defaultVal ? 1 : 0) > 0;
	}

	void SetParam(const std::string &paramName, int value)
	{
		m_Params[paramName] = value;
	}

	void OnGameReset()
	{
		m_Params.clear();
	}
};


template<typename T>
class GameParamLoader
{

	static void LoadIntValue(T &f, const char *seg, ExtensionGameParam *extGameParam, const std::string &name, int defaultValue, bool useExistingAsDefault)
	{
		if(useExistingAsDefault)
		{
			defaultValue = extGameParam->GetParam(name);
		}
		extGameParam->SetParam(name, f.GetKeyVal(seg, name.c_str(), defaultValue));
	}

	static void Load(T &f, const char *segName, ExtensionGameParam *extGameParam, bool useExistingAsDefault)
	{
		LoadIntValue(f, segName, extGameParam, GameParam_PlayerCount, 4, useExistingAsDefault); //四人

		LoadIntValue(f, segName, extGameParam, GameParam_GameScorePoint, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_MingGangScore, 3, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_BuGangScore, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_AnGangScore, 2, useExistingAsDefault);
		

		LoadIntValue(f, segName, extGameParam, GameParam_DecisionTimeLimit, 20, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_DelegateActionDelay, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_BeginWait, 20, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_UseDelegateWhenTimeOut, 0, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_UseDelegateWhenLeft, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_AutoBegin, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_DingQueTime, 10, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_HuanZhangTime, 30, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_MultipleLight, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_ReadyHandAutoDiscard, 0, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_AbandonWin, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_ActionDelegateEnabled, 0, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_LiquidationDelay, 500, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_ReservedCardCount, 0, useExistingAsDefault);

		LoadIntValue(f, segName, extGameParam, GameParam_ShowTax, 1, useExistingAsDefault);
		LoadIntValue(f, segName, extGameParam, GameParam_ShowUserID, 1, useExistingAsDefault);

		//潮汕麻将玩法
		LoadIntValue(f, segName, extGameParam, GameParam_PlayMethod, 0, useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_bFanMa,1,useExistingAsDefault); //翻马
		LoadIntValue(f,segName, extGameParam, GameParam_FanMaNum,2,useExistingAsDefault); //翻二马
		LoadIntValue(f, segName, extGameParam, GameParam_FanLimit, 99, useExistingAsDefault);//？？

//		LoadIntValue(f,segName, extGameParam, GameParam_FanMaMulti,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_bFengDing,0,useExistingAsDefault); //0不封顶


		LoadIntValue(f,segName, extGameParam, GameParam_bMaGenGang,0,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_bMaGenDiFen,1,useExistingAsDefault); //马跟底分

		LoadIntValue(f,segName, extGameParam, GameParam_PM_KeJiePaoHu,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_GangBao,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_BiHu,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_QiangGangHu,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_QiXiaoDui,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_HunYiSe,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_QingYiSe,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_GangKai,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_PengPengHU,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_ShiSanYao,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_TianDiHu,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_18LuoHan,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_SuperQiDui,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_DoubleSpQiDui,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_TripleSpQiDui,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_YaoJiu,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_SmalSiXi,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_BigSiXi,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_BigSanYuan,1,useExistingAsDefault);
		LoadIntValue(f,segName, extGameParam, GameParam_PM_SmallSanYuan,1,useExistingAsDefault);
		//默认玩法
//		LoadIntValue(f, segName, extGameParam, GameParam_YaoOrJiu, 1, useExistingAsDefault);
//		LoadIntValue(f, segName, extGameParam, GameParam_HaveJoker, 1, useExistingAsDefault);
//	LoadIntValue(f, segName, extGameParam, GameParam_DanDiaoJoker12, 1, useExistingAsDefault);
	//	LoadIntValue(f, segName, extGameParam, GameParam_PaiTouCount, 2, useExistingAsDefault);
	
//		LoadIntValue(f, segName, extGameParam, GameParam_TianDiHuDouble, 1, useExistingAsDefault);
//		LoadIntValue(f, segName, extGameParam, GameParam_FourJoker, 1, useExistingAsDefault);
//		LoadIntValue(f, segName, extGameParam, GameParam_QiangGangDouble, 1, useExistingAsDefault);
//		LoadIntValue(f, segName, extGameParam, GameParam_TimeOutAuto, 1, useExistingAsDefault);
//		LoadIntValue(f, segName, extGameParam, GameParam_TimeOutAutoCount, 1, useExistingAsDefault);

//		LoadIntValue(f, segName, extGameParam, GameParam_JokerIdx, 3, useExistingAsDefault);

			


		if(extGameParam->GetParam(GameParam_DiFen) < 1)
			extGameParam->SetParam(GameParam_DiFen, 1);



	}


public:

	static void Load(T &f, ExtensionGameParam *extGameParam, int gameID, int roomID)
	{
		const char *gameSeg = "game";
		Load(f, gameSeg, extGameParam, false);

		char roomSeg[128];
		sprintf(roomSeg, "%d_%d", gameID, roomID);
		Load(f, roomSeg, extGameParam, true);
	}
};

#endif