#ifndef EXTENSION_LIQUIDATION_H
#define EXTENSION_LIQUIDATION_H

#include "GameContext.h"
#include "Liquidation.h"
#include "Card.h"
#include "Interception.h"

#include <set>

struct AppendInterceptionInfo
{
	int idx;
	Interception interception;
	
	AppendInterceptionInfo(int index, const Interception &intc):
		idx(index), interception(intc){}
};

class WinTypeList : public std::list<unsigned char>
{
public:
	bool HasType(unsigned char type) const;

	void Add(unsigned char type);

	void Remove(unsigned char type);
};

std::ostream &operator << (std::ostream &ostrm, const WinTypeList &winTypeList);

class WinType
{
	std::map<PlayerPos::Type, WinTypeList > m_WinTypes;

public:

	WinTypeList GetWinType(PlayerPos::Type playerPos) const;

	void SetWinType(PlayerPos::Type playerPos, unsigned char type);

	void RemoveType(PlayerPos::Type playerPos, unsigned char type);

	void Clear(PlayerPos::Type playerPos);

	void ClearAll();
};

class WinnerList : public std::vector<PlayerPos::Type>
{
public:
	bool Contains(PlayerPos::Type pos) const;

	void Add(PlayerPos::Type pos);

	PlayerPos::Type Get(int num) const;

	int Count() const;
};

/////////////////////////////////////////////////////////////////////////
enum WinMode
{
	WinMode_Invalid,
	WinMode_Self, // 自摸
	WinMode_Fire,  // 点炮
	WinMode_ChaJiao //查叫
};

class CardScoreDelegate
{
public:
	virtual ~CardScoreDelegate(){}

	// cardList 被检测的手牌(包含进牌)
	// interceptions 玩家以拦牌数据
	// src 进牌数据
	// playerPos 被检测的玩家位置
	// bankerPos 庄家位置
	// reveal 仅预先查看分数，非实际结算计分
	virtual int GetScore(const CardList &cardList, const InterceptionList &interceptions, const CardInfo &src, 
		PlayerPos::Type playerPos, PlayerPos::Type bankerPos, bool reveal) const = 0;

	virtual void OnAddInterception(PlayerPos::Type srcPlayerPos, const std::map<PlayerPos::Type, AppendInterceptionInfo> &actions) = 0;
};

class ExtensionLiquidation : public GameContextExtension
{
protected:
    Liquidation m_Liquidation;
	int m_DrawCount; // 平局(流局)次数
	int m_GameCount; // 总局数
	WinType m_WinType;
	WinnerList m_Winners;
	std::map<PlayerPos::Type, WinMode> m_WinMode;
	std::map<PlayerPos::Type, PlayerPos::Type> m_Loser;
	std::map<PlayerPos::Type, int> m_WinRound; // 胡牌的回合
	int m_MaxMulti;//每局最大倍数
	RefCntObj<CardScoreDelegate> m_CardScoreDelegate;
public:

	ExtensionLiquidation();

	void ScoreItemListStatistic();

    Liquidation &GetLiquidation();

	void AddDrawCount();

	int GetDrawCount() const;

	void AddGameCount();

	int GetGameCount() const;

	void AddWinner(PlayerPos::Type playerPos, WinMode winMode, int round);

	void AddWinnerAndLoser(PlayerPos::Type playerPos, PlayerPos::Type loser, int round);

	const WinnerList &GetWinners() const;

	const PlayerPos::Type GetLoser(PlayerPos::Type winner) const;

	bool IsLoser(PlayerPos::Type playerPos) const;

	WinMode GetWinMode(PlayerPos::Type playerPos) const;

	void SetCardScoreDelegate(const RefCntObj<CardScoreDelegate> &delegate);

	WinType &GetWinType();

	int GetWinRound(PlayerPos::Type playerPos) const;

	int GetScore(const CardList &cardList, const InterceptionList &interceptions, const CardInfo &src, PlayerPos::Type playerPos, PlayerPos::Type bankerPos, bool reveal) const;

	void OnAddInterception(PlayerPos::Type srcPlayer, const std::map<PlayerPos::Type, AppendInterceptionInfo> &appendAction);

	void OnGameStart();

	void OnGameReset();

	void SetMaxMulti(int multi);

	int GetMaxMulti();

	void Reset();
};

#endif
