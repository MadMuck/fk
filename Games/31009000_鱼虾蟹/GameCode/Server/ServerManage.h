#pragma once
#include "GameDesk.h"
#include <afxmt.h>
#include <deque>
#include<list>

//定时器 ID
#define IDT_USER_CUT				1L				//断线定时器 ID
//游戏结束标志定义
#define GF_NORMAL					10				//游戏正常结束
#define GF_SAFE						11				//游戏安全结束
#define GF_NO_CALL_SCORE			13				//无人叫分
#define GF_TERMINATE				14				//意外停止游戏
#define GF_AHEAD_END				15				//提前结束

//碰碰车  
//定时器类型
#define		TIME_ONE_SECOND			31				//一秒计时器
#define		TIME_FREE_TIME			32				//空闲定时器
#define		TIME_XIAZHU_TIME		33				//下注定时器
#define     TIME_ROLL_TIME          34              //摇骰定时器
#define		TIME_SEND_TIME			35				//发牌定时器
#define		TIME_GAME_FINISH		36				//游戏结束定时器
#define		TIME_ROB_NT				37				//抢庄定时器


#define MAX_NAME_INFO                   256
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // 游戏ID

//*****************************************************************************************
//游戏桌类
class CServerGameDesk : public CGameDesk
{
private:
	bool				m_bIsSuperControl;								//是否是超端用户控制结果
	bool                m_bShowChouMa;									//是否显示筹码

	BYTE				m_byRobNTtime;									//抢庄阶段时间
	BYTE				m_byFreeTime;									//空闲阶段时间
	BYTE				m_byBetTime;									//下注阶段时间
	BYTE                m_byRolldiceTime;								//摇骰阶段时间
	BYTE				m_byOpenCardTime;								//开奖阶段时间
	BYTE				m_byWinPrizeStart;								//开奖起始位置
	BYTE        		m_byWinPrize[3];								//本局开奖位置
	BYTE				m_byBetWaitTime;								//到下注阶段发牌等待时间
	BYTE				m_byOpenAwardWaitTime;							//到下注阶段发牌等待时间
	BYTE        		m_byMaxNtPeople;								//最大上庄人数
	BYTE				m_byNtStation;									//庄家位置
	BYTE				m_byNuoLimit;									//挪限制
	BYTE				m_byDiceNum;									//骰子数
	int					m_iPlayingMethod;								//玩法
	
	bool				m_bIsRobotWinAndLostAutoCtrl;					//是否开启机器人输赢自动控制
	//机器人在各个奖池赢钱概率控制
	int					m_iRobotWinLuckyAtA1;							/*<机器人在区域1赢钱的概率*/
	int					m_iRobotWinLuckyAtA2;							/*<机器人输赢控制：机器人在区域2赢钱的概率*/
	int					m_iRobotWinLuckyAtA3;							/*<机器人输赢控制：机器人在区域3赢钱的概率*/
	int					m_iRobotWinLuckyAtA4;							/*<机器人输赢控制：机器人在区域4赢钱的概率*/

	long long			m_iRobotWinMoneyA1;								//<机器人赢钱区域1
	long long			m_iRobotWinMoneyA2;								//<机器人赢钱区域2
	long long			m_iRobotWinMoneyA3;								//<机器人赢钱区域3
	long long			m_iResetRobotHaveWinMoney;						//<机器人输赢控制：直接从配置文件中读取机器人已经赢钱的数目

	int					m_iNtPlayCount;									//庄家坐庄次数
	int					m_iNtPlayLimite;								//庄家坐庄次数限制
	long long           m_lSingleNoteLimit;                             //单个人下注上限
	long long           m_lRemainedNotes;                               //剩余下注数
	long long			m_iApplyNtLimite;								//上庄条件
	long long			m_iUserBetSumLimite;							//个人总下注限制	
	long long			m_iBetAreaLimite[BET_ARES];						//下注各区域限制

	int					m_iRemaindTime;			//阶段剩余时间

	//游戏总的下注(包括真实玩家和机器人)
	long long			m_i64UserBetCount[PLAY_COUNT][BET_ARES];		//各玩家在每个区域的下注情况
	long long			m_i64LastBet[PLAY_COUNT][BET_ARES];				//各玩家在上把游戏中的下注情况
	long long           m_i64AreaGain[PLAY_COUNT][BET_ARES];			//各玩家在每个区域的输赢情况
	long long			m_i64AreaBetAllSum[BET_ARES];					//各区域的总下注
	long long			m_i64UserBetSum[PLAY_COUNT];					//各玩家的总下注	
	long long			m_i64SumAllBet;									//总下注数
	long long			m_iAreaBetLimite;								//区域下注限制

	//真实玩家的下注（不包括机器人）
	long long			m_i64PeopleBetCount[PLAY_COUNT][BET_ARES];		//各真实玩家在每个区域的下注情况
	long long			m_i64PeopleBetSum[PLAY_COUNT];					//各真实玩家的总下注	
	long long			m_i64PeopleAreaBetAllSum[BET_ARES];				//各区域的真实玩家的总下注
	long long			m_i64PeopleSumAllBet;							//真实玩家总下注数

	//結算信息
	long long     		m_i64PlayerGrade[PLAY_COUNT];					//玩家成绩
	long long     		m_i64PlayCoin[PLAY_COUNT];						//玩家输赢金币
	long long     		m_i64ReturnMoney[PLAY_COUNT];					//返还的金币数
	long long			m_i64ChangePlayCoin[PLAY_COUNT];				//扣税后玩家的得分

	deque<STruOpenAward> m_dstAward;									//开奖记录
	vector<long>		 m_vlSuperID;									//保存超端玩家的容器
	vector<BYTE>      	 m_blNtWaitList;								//上庄列表

	//超端控制显示
	BYTE                m_bSuperDeskStation;							//超端用户座位号
	bool				m_bShowRobotBet;								//超端用户控制机器人下注显示

	int					m_iRunTime[100];								//跑灯时间数据

	bool				m_bXiaZhuang;
	CString				dwjlog;

	//筹码类型值
	int					G_iChouMaValue[BETSTYLE];

	UserNuoData			m_sUserNuoData[PLAY_COUNT][NUO_COUNT];			//玩家挪情况

	long long			m_i64UserBetCountEx[PLAY_COUNT][BET_ARES];		//各玩家在每个区域的下注情况ex(挪注之后的，这里挪注相当于下注，只是不增加到个人下注值里，挪之后中了庄家赔钱，不中赔钱给庄家，跟其他人没关系)

	BYTE				m_byNuoCount[PLAY_COUNT];						//玩家挪次数

	int					m_iChouMaCount[BET_ARES][BETSTYLE];				//筹码记录，每个区域每个筹码下了几个

	long long			m_i64WinMoney[PLAY_COUNT];						//玩家输赢记录

	//函数定义
public:
	//构造函数
	CServerGameDesk(); //服务器启动时第一次调用
	//析构函数
	virtual ~CServerGameDesk();

	//重载函数
public:

	//游戏开始
	virtual bool GameBegin(BYTE bBeginFlag);
	//游戏结束(客服端退出时调用)
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//判断是否正在游戏（客服端退出时第二次调用）
	virtual bool IsPlayGame(BYTE bDeskStation);
	//游戏数据包处理函数
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	///拦截窗口消息（客服端启动时第二次调用）
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);

	//用户离开游戏桌(客服端退出时第一次调用)
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	/// 玩家坐下
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	//用户断线离开
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	//定时器消息
	virtual bool OnTimer(UINT uTimerID);

	//获取游戏状态信息（客服端启动时第三次调用）
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//重置游戏状态（客服端退出时调用）
	virtual bool ReSetGameState(BYTE bLastStation);

	//配置INI文件（服务器启动时第二次调用）
	virtual bool InitDeskGameStation();

	//处理函数
public:
	//服务端自动开始不由服务端控制
	BOOL StartGame();
	//读取静态变量
	BOOL LoadIni();
	//加载限注
	BOOL LoadExtIni(int iRoomID);
	//重置所有数据
	void ResetGameData();
	//删除所有定时器
	void KillAllTimer();

	//启动游戏计时器
	void	SetGameTimer(UINT uTimerID, BYTE byTime);
private:
	//获取玩家剩余多少钱
	long long GetUserRemaindMoney(BYTE byDeskStation);

	//获取玩家下注总额
	long long GetUserBetCount(BYTE byDeskStation);

	//处理玩家申请上庄消息
	void	DealUserApplyNt(void *pBuffer);

	//处理玩家申请下庄消息
	void	DealUserApplyLeaveNt(void *pBuffer);

	//处理玩家下注
	void	DealUserBet(void *pBuffer);

	//处理玩家续压
	void DealContinueBet(void *pBuffer);

	//玩家清空下注
	void DealClearBet(void *pBuffer);
	//是否在上庄列表中
	bool	IsInWaitNtList(BYTE byDeskStation);

	//发送摇骰消息
	void SendRolldice();
	//发送空闲消息
	void SendFree();

	//发送开奖消息
	void SendAllCard();

	//发送开奖记录
	void SendAwardRecord(BYTE );

	//判断任选二，返回值为倍数
	int shuangxx(BYTE winArea[],BYTE a, BYTE b);

	////判断任选三
	int sanxx(BYTE winArea[],BYTE a,BYTE b,BYTE c);

	//判断豹子
	int isBaozi(BYTE winArea[],BYTE a);
	//统计玩家金币
	void accountPlayCoin();

	//开奖
	void openAward();

	//记住路子（72）
	void rememberLuzi();

	//得到区域剩余下注数
	long long RemainingBettingNumber(BYTE);

	//超级用户验证
	void	SuperUserExamine(BYTE bDeskStation);

	//处理超端用户控制结果
	void	DealSuperUserMessage(void *pBuffer);

	//是否超端用户 超端用户要发送结果给他看到
	bool	IsSuperUser(BYTE byDeskStation);

	//机器人自动控制
	void RobotAutoControl();

	//向客服端发送下注数据(超端控制)
	void SendBetDataToClient(BYTE bDeskStation);
	//换庄家
	bool ChangeNt(BYTE byLastNTStation);
	////清理庄家
	//void ClearNt();
	//清理上庄列表
	void ClearNtList();
	//无庄等待
	void NoNtWait();
	//记录机器人所赢金币
	void RecordRobotWinMoney();

	//能否到负分（普通金币场，俱乐部金币场，俱乐部积分场，创建房间金币场等都不能到负分，只有创建房间积分场可以）
	bool CanMinusPoint();

	//获得骰子数
	int GetDiceNum();

	//从庄家列表中获取出庄家(抢庄)
	bool OnGetZhuang();

	//处理玩家下注信息请求
	void DealUserBetInfo(BYTE byDeskStation, void *pData);

	//处理玩家挪请求
	void DealUserNuo(BYTE byDeskStation, void *pData);

	//获得区域下注限制
	long long GetAreaBetLimite(int iArea);

	//下注区域是否有效
	bool GetAreaBetVaild(int iArea);

	//通知游戏局数
	void NotiGameCount(BYTE byDeskStation = 255);

	//随机生成骰子数据
	void RandDice();

	//获得某个区域的结果,中了返回倍数，没中返回-1
	//此处返回结果是正常的10倍，计算分数时需乘此结果后除10
	int GetAresResult(int iArea);

	//计算AI输赢
	__int64	CountAIWinMoney();

	//设置创建房间玩法
	void SetPlayMode();

	//是否有type玩法
	bool IsPlayModeVaild(int type);

	//发送大结算
	void SendScoreBoard();

private:
	//修改奖池数据1(平台->游戏)
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//修改奖池数据2(平台->游戏)
	virtual bool SetRoomPondEx(long long iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//修改奖池数据3(平台->游戏)
	virtual bool SetRoomPondEx(long long iReSetAIHaveWinMoney);
	//控制胜负
	/// 判断
	virtual BOOL Judge();
	/// 胜
	virtual BOOL JudgeWiner();
	/// 输
	virtual BOOL JudgeLoser();
	//GRM函数
	//更新奖池数据(游戏->平台)
	/// @return 机器人赢钱数
	virtual void UpDataRoomPond(long long iAIHaveWinMoney){};
};

/******************************************************************************************************/
