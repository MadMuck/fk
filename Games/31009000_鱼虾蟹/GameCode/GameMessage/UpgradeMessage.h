#pragma once
//支持类型
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//版本定义
#define GAME_MAX_VER					1					//现在最高版本
#define GAME_LESS_VER					1					//现在最低版本
#define GAME_CHANGE_VER					0					//修改版本
//文件名字定义
#define GAMENAME						TEXT("鱼虾蟹")
//名字 ID
#define NAME_ID							31009000								
//游戏信息
#define PLAY_COUNT						11					//游戏人数
#define	BET_ARES						47					//下注区域数
#define BETSTYLE						5					//筹码类型
#define RECORD_COUNT					20					//最大开奖记录
#define BET_INFO_MAX					10					//单次发送下注信息最大长度
#define NUO_COUNT						3					//单个玩家单局最大挪次数
//下注区域赔率
static	const	int		G_iArePeiLv[BET_ARES] = {5,5,5,5,5,5,6,6,6,6,6,6,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

//static  const   int     G_iNoToPoint[8]={1,2,3,6,6,5,4,1};

//1虎 2葫芦 3鱼 4蟹 5虾 6鸡
static  const   int     G_iNoToPoint[6]={1,2,3,4,5,6};			//单压,下注区域0~5

static  const   int     G_iBaoZiPoint[6]={1,2,3,4,5,6};			//单压豹子,下注区域6~11

//任选2, C 6 2 = 15, 下注区域12~26
static  const   int     G_iChooseTwo[15][2]={{1,2},{1,3},{1,4},{1,5},{1,6},{2,3},{2,4},{2,5},{2,6},{3,4},{3,5},{3,6},{4,5},{4,6},{5,6}};		

//任选3, C 6 3 = 20，下注区域27~46
static  const   int     G_iChooseThree[20][3]={{1,2,3},{1,2,4},{1,2,5},{1,2,6},{1,3,4},{1,3,5},{1,3,6},{1,4,5},{1,4,6},{1,5,6},{2,3,4},{2,3,5},{2,3,6},{2,4,5},{2,4,6},{2,5,6},{3,4,5},{3,4,6},{3,5,6},{4,5,6}};

enum AresValue
{
	DanYa_Start = 0,
	DanYa_End = DanYa_Start + sizeof(G_iNoToPoint) / sizeof(G_iNoToPoint[0]) - 1,
	BaoZi_Start,
	BaoZi_End = BaoZi_Start + sizeof(G_iBaoZiPoint) / sizeof(G_iBaoZiPoint[0]) - 1,
	RenEr_Start,
	RenEr_End = RenEr_Start + sizeof(G_iChooseTwo) / sizeof(G_iChooseTwo[0]) - 1,
	RenSan_Start,
	RenSan_End = RenSan_Start + sizeof(G_iChooseThree) / sizeof(G_iChooseThree[0]) - 1,
};

//游戏状态
#define	GS_WAIT_SETGAME					0				//等待东家设置状态
#define	GS_WAIT_ARGEE					1				//等待同意设置
#define	GS_ROB_NT						19				//抢庄阶段
#define	GS_FREE_STATUS					20				//游戏空闲阶段
#define	GS_BET_STATUS					21				//游戏下注阶段
#define GS_ROLL_STATUS                  22              //游戏摇骰阶段
#define	GS_SEND_STATUS					23				//发牌阶段
#define	GS_WAIT_NEXT					24				//等待下一盘开始阶段

//消息ID
#define ASS_GAME_FREE					53				//空闲消息					BeginFreeMsg
#define ASS_BEGIN_BET					54				//开始下注					BeginBetMsg
#define ASS_ROLL_DICE                   55              //开始摇骰					RollDiceMsg
#define ASS_SHOW_DICE					56				//开始开骰					BeginSendCardMsg
#define ASS_UPDATE_BET					57				//更新下注					TUpdateHistoryBet(作为gamestation的补充消息)
#define ASS_APPLY_NT					58				//申请上庄消息				ApplyNtMsg
#define ASS_APPLY_RESULT				59				//申请上庄结果消息			ApplyNtResultMsg
#define ASS_APPLY_LEAVE					60				//申请下庄消息				ApplyLiveNtMsg
#define ASS_LEAVE_RESULT				61				//申请下庄结果消息			ApplyLiveNtResultMsg
#define ASS_USER_BET					62				//玩家下注消息				UserBetMsg
#define ASS_BET_RESULT					63				//玩家下注结果消息			UserBetResultMsg
#define ASS_GAME_OPEN                   64              //开奖结算					BeginAward
#define ASS_SUPER_RESULT				65				//超端用户控制结果消息包	SuperUserControlReslut,文档没有说明,暂不使用
#define ASS_SUPER_SEND					66				//超端用户向客服端发送结果	SuperUserControlStart,文档没有说明,暂不使用
#define ASS_SUPER_USER					67				//超端用户消息				SuperUserState,文档没有说明,暂不使用

#define ASS_WAITE_NT		            69              //无庄等待消息				WaiteNtMsg
#define ASS_CONTINUE_BET                70              //续压消息					CS_ContinueBetMsg,文档没有说明,暂不使用
#define ASS_CONTINUE_BET_RESULT         71              //续压结果					CS_ContinueBetResultMsg,文档没有说明,暂不使用
#define ASS_CLEAR_BET                   72              //清空消息					不使用
#define ASS_CLEAR_BET_RESULT            73              //清空结果					不使用

#define S_C_AWARD_RECORD_NTF            74              //历史开奖记录				S_C_AwardRecord
#define S_C_ROB_NT_NTF					75				//抢庄通知					NULL	
#define C_S_BET_INFO_REQ				76				//玩家下注信息请求			TMSG_BET_INFO_REQ
#define S_C_BET_INFO_RSP				77				//玩家下注信息回复			TMSG_BET_INFO_RSP	
#define C_S_CHANGE_BET_REQ				78				//挪注请求					TMSG_CHANGE_BET_REQ
#define S_C_CHANGE_BET_RSP				79				//挪注回复					TMSG_CHANGE_BET_RSP
#define S_C_UPDATE_BET_NTF				80				//更新下注信息				TMSG_UPDATE_BET_NTF
#define S_C_GAME_COUNT_NTF				81				//剩余局数通知				TMSG_GAME_COUNT_NTF
#define S_C_CALCULATE_BOARD				82				//大结算					TMSG_CALCULATE_BOARD	

#define S_C_NUO_INFO_RSP				83				//玩家挪信息回复			TMSG_NUO_INFO_RSP	

#pragma pack(1)

//开奖记录结构体
struct STruOpenAward
{
	BYTE byWinArea[3];			//单局开奖区域

	STruOpenAward()
	{
		memset(byWinArea, 255, sizeof(byWinArea));
	}
};

//开奖记录消息包
struct S_C_AwardRecord
{
	STruOpenAward stAward[RECORD_COUNT];          //历史开奖记录

	S_C_AwardRecord()
	{
		memset(stAward, 255, sizeof(stAward));
	}
};

//申请上庄消息
struct	ApplyNtMsg
{
	BYTE	byDeskStation;		//申请座位号
	ApplyNtMsg()
	{
		byDeskStation = 255;
	}
};

//超端用户控制的结果
struct	SuperUserControlReslut
{
	BYTE	bSuperDeskStation;	//超端用户座位号
	BYTE    bySuperReslut[3];	//超端用户控制的结果
	BYTE    bShowRobotBet;		//超端用户控制机器人下注显示
	SuperUserControlReslut()
	{
		bSuperDeskStation = 255;
		memset(bySuperReslut, 255,sizeof(bySuperReslut));
		bShowRobotBet = 0;
	}
};

//超端用户向客服端发送结果
struct	SuperUserControlStart
{
	BYTE	byDeskStation;		//超端用户座位号
	BYTE    bySuperStart[3];	//超端用户控制的结果
	SuperUserControlStart()
	{
		byDeskStation = 255;
		memset(bySuperStart, 255,sizeof(bySuperStart));
	}
};

// 通知客户端是否为超级客户端状态消息结构
struct SuperUserState
{
	BYTE byDeskStation;      /**< 玩家位置 */
	bool bEnable;           /**< 是否开通 */
	SuperUserState()
	{
		memset(this,0,sizeof(SuperUserState));
	}
};

//-------------------------------------------------------------------------------
//无庄等待
struct WaiteNtMsg
{
	bool	bWait;
	WaiteNtMsg()
	{
		memset(this,0,sizeof(WaiteNtMsg));
	}
};
//通知下注消息包
struct BeginBetMsg
{
	BYTE	byNtStation;	//庄家位置
	BYTE    byNtCount;     //庄家坐庄次数
	BYTE    byGameStation;//游戏状态
	BYTE    byNtWaitList[PLAY_COUNT];//庄家列表
	long long i64NtScore;//庄家成绩
	BeginBetMsg()
	{
		memset(this,0,sizeof(BeginBetMsg));
		memset(byNtWaitList,255,sizeof(byNtWaitList));
		byNtStation=255;
	}
};

//续压消息包
struct CS_ContinueBetMsg
{
	BYTE byDeskStation;
	CS_ContinueBetMsg()
	{
		byDeskStation=255;
	}
};

//续压结果(总下注)
struct	CS_ContinueBetResultMsg
{
    bool          bFlag;               //是否能续压标识
	BYTE	      byDeskStation;		//下注座位号
	CS_ContinueBetResultMsg()
	{
		byDeskStation = 255;
		memset(this,0,sizeof(CS_ContinueBetResultMsg));
	}
};

//清空消息包
struct CS_CleartBetMsg
{
	BYTE byDeskStation;
	CS_CleartBetMsg()
	{
		byDeskStation = 255;
	}
};
//清空押注结果
struct SC_ClearBetResultMsg
{
	BYTE	      byDeskStation;		//下注座位号
	long long     i64SumAllBet;       //玩家总下注
	long long     i64UserAllBet;      //当前玩家总下注
	long long     i64BeforeClearBet;  //清空之前的总下注
	long long     i64UserBetCount[BET_ARES]; //闲家在每个区域的下注情况
	long long	  i64AreaBetAllSum[BET_ARES];		//各区域的总下注
	SC_ClearBetResultMsg()
	{
		memset(this,0,sizeof(i64AreaBetAllSum));
	}
};
//开奖消息包
struct BeginAward
{
	long long     i64PlayerGrade[PLAY_COUNT];//玩家成绩
	long long     i64PlayCoin[PLAY_COUNT];//玩家输赢金币
    long long     i64AreaGain[BET_ARES];//玩家在每个区域的输赢情况(新添加flog)
	long long     i64ReturnMoney[PLAY_COUNT];//返还的金币数 
	BeginAward()
	{
		memset(this,0,sizeof(BeginAward));
	}
};

//发牌消息包
struct BeginSendCardMsg
{
	BYTE byGameStation;				//游戏状态
	BYTE byWinPrize[3];				//开奖位置flog

	BeginSendCardMsg()
	{
		byGameStation = 0;
		memset(byWinPrize, 255, sizeof(byWinPrize));//flog
	}
};

//摇骰消息包
struct RollDiceMsg
{
	BYTE  byGameStation;
	RollDiceMsg()
	{
		byGameStation = 255;
	}
};

//更新历史下注
struct TUpdateHistoryBet
{
	long long	i64AreaBetAllSum[BET_ARES];	//所有玩家各区域的总下注
	long long	i64UserBetSum[BET_ARES];	//当前玩家各区域的总下注
	long long	i64SumAllBet;				//所有玩家总下注数
	long long   i64UserAllBet;			    //当前玩家总下注
	int			iChouMaCount[6][BETSTYLE];	//每个区域下注筹码数，只显示6个单压区域

	TUpdateHistoryBet()
	{
		memset(this,0,sizeof(i64AreaBetAllSum));
	}
};

//空闲消息包
struct BeginFreeMsg
{
	bool	bFlag;			//空闲标识
	BYTE	byGameStation ;         ///当前的游戏状态
	BYTE	byFreeTime;		//空闲时间
	BYTE    byNtStation;//庄家位置
	BYTE    byNtcount;//庄家坐庄次数
	BYTE    byWaitNtList[PLAY_COUNT];	//上庄列表
	long long i64NtScore;//庄家成绩
	BeginFreeMsg()
	{
		memset(this,0,sizeof(BeginFreeMsg));
		byNtStation = 255;
		memset(byWaitNtList, 255, sizeof(byWaitNtList));
	}
};

//申请上庄结果消息
struct	ApplyNtResultMsg
{
	bool    bNtFlag;			//在没有庄家的情况下上庄的标志
	bool	bApplay;			//是否因为金币没有达到上庄金币而上庄不成功
	BYTE	byDeskStation;		//申请座位号
	BYTE	byWaitNtList[PLAY_COUNT];	//申请上庄列表
	BYTE	byNtStation;		//庄家的位置
	BYTE    byNtcount;//庄家坐庄次数
	BYTE    bGameStation;
	long long i64NtScore;//庄家成绩
	ApplyNtResultMsg()
	{
		memset(this,0,sizeof(ApplyNtResultMsg));
		memset(byWaitNtList,255,sizeof(byWaitNtList));
		byNtStation=255;
		bApplay=false;
		bNtFlag=false;
	}
};

//申请下庄消息包
struct	ApplyLiveNtMsg
{
	BYTE	byDeskStation;		//申请座位号
	ApplyLiveNtMsg()
	{
		byDeskStation = 255;
	}
};

//申请下庄结果消息包
struct	ApplyLiveNtResultMsg
{
	BYTE    byNtStation;//庄家位置
	BYTE	byDeskStation;		//申请座位号
	BYTE    byWaitNtList[PLAY_COUNT];	//上庄列表
	BYTE    byNtcount;//庄家坐庄次数
	long long i64NtScore;//庄家成绩
	ApplyLiveNtResultMsg()
	{
		memset(this, 0, sizeof(ApplyLiveNtResultMsg));
		memset(byWaitNtList,255,sizeof(byWaitNtList));
		byNtStation=255;
		byDeskStation=255;
	}
};

//玩家下注
struct	UserBetMsg
{
	BYTE	byDeskStation;		//下注座位号
	BYTE	byBetArea;			//下注区域
	BYTE	byChouMaType;		//下注筹码类型
	UserBetMsg()
	{
		memset(this,0,sizeof(UserBetMsg));
	}
};

//可选玩法标志位
enum PM_FLAGS
{
	None = 0,
	PM_MEM_CHUAN = 1,			//门串
	PM_BAO_ZI = 1 << 1,		//豹子
	PM_NUO = 1 << 2,		//挪
	PM_NUO_NEEDBET = 1 << 3,		//先下注再挪
	PM_2DICE_CHUAN = 1 << 4,		//双骰子连串1:8(无此位为1:10)
	PM_2DICE_BAOZI = 1 << 5,		//双骰子豹子1:15(无此位为1:10)
	PM_3DICE_GETTWO = 1 << 6,		//三骰子三中二1:2 1:5(无此位为1:2.5 1:5)
	PM_3DICE_BAOZI = 1 << 7,		//三骰子豹子1:25(无此位为1:30)
	PM_FREE_ROBNT = 1 << 8,		//自由抢庄(无此位为固定庄家)
};


//创建房间玩法
struct tagPM
{
	int  iPlayingMethod;	//玩法
	int  iBetLimit;			//下注限制
	BYTE byDiceNum;			//骰子数
	BYTE byReadyTime;		//准备时间
	BYTE byRobNtTime;		//抢庄时间
	BYTE byBetTime;			//下注时间
	BYTE byNuoLimit;		//挪限制(百分比)

	tagPM()
	{
		memset(this, 0, sizeof(tagPM));
	}
};

//游戏状态基础数据包
struct	GameStation_Base
{
    bool        bShowChouMa;                //是否显示筹码
	BYTE		bGameStation;		        //游戏状态
	BYTE		byRobNTtime;				//抢庄时间
	BYTE		byFreeTime;					//空闲时间
	BYTE		byBetTime;			        //下注时间
	BYTE        byRolldiceTime;             //摇骰时间
	BYTE		bySendTime;	                //开牌时间
	BYTE		byRemaindTime;				//剩余时间
	BYTE		byNtStation;				//庄家位置
	BYTE		byMaxNtPeople;				//最大上庄人数
	BYTE		byBetWaitTime;				//到下注阶段发牌等待时间
	BYTE		byOpenAwardWaitTime;		//到开奖阶段发牌等待时间
	BYTE		byWaitNtList[PLAY_COUNT];	//上庄列表
	int			iNtPlayCount;		        //庄家坐庄次数
	int			iApplyNtLimite;		        //上庄条件
	long long	i64NtGrade;					//庄家成绩
	long long   i64PlayerGrade;				//闲家成绩
	long long   lSingleNoteLimit;           //剩余下注数
	int			iChouMaValue[BETSTYLE];		//筹码值
	tagPM	    playMode;					//玩法

	GameStation_Base()
	{
		memset(this,0, sizeof(GameStation_Base));
	}
};

//下注结果(总下注)
struct	UserBetResultMsg
{
	bool		bDynamicBet;		//是否动态下注
	BYTE	    byDeskStation;		//下注座位号
	BYTE		byBetArea;			//下注区域
	BYTE		byChouMaType;		//下注筹码类型
	long long   i64SumAllBet;       //总下注
	long long   i64UserAllBet;      //当前玩家总下注
	long long   i64UserBetCount[BET_ARES]; //闲家在每个区域的下注情况
	long long	i64AreaBetAllSum[BET_ARES];		//各区域的总下注
	long long   i64RemainedNotes;                 //剩余下注数(记得告诉ls)
	BYTE		bErrorCode;			//0成功 1:超过区域下注限制 2:钱不够 3:庄家不能下注 4:超过个人限制 5:参数错误 6:庄家不存在

	UserBetResultMsg()
	{
		memset(this,0,sizeof(UserBetResultMsg));
	}
};

//通知局数
struct TMSG_GAME_COUNT_NTF
{
	int iRemainCount;			// 剩余局数
	int iTotalCount;		// 总局数

	TMSG_GAME_COUNT_NTF()
	{
		iRemainCount = 0;
		iTotalCount = 0;
	}
};
			
//玩家下注信息请求		
struct TMSG_BET_INFO_REQ
{
	BYTE byBetStation;						//请求哪个玩家的位置

	TMSG_BET_INFO_REQ()
	{
		byBetStation = 255;
	}
};	



struct UserBetInfo
{
	BYTE byBetStation;
	int  i64UserBetCount[BET_ARES]; //所有玩家在每个区域的下注情况

	UserBetInfo()
	{
		byBetStation = 255;
		memset(i64UserBetCount, 0, sizeof(i64UserBetCount));
	}
};

//玩家下注信息回复	
//如果定义i64UserBetInfos长度为最大游戏人数11，则此消息包大小超过单次发送限制(MAX_SEND_SIZE 2044)
//所以这里为了方便操作，定义i64UserBetInfos长度为10（BET_INFO_MAX，最大游戏人数11 - 1，因为庄家不能下注没有下注信息）
//因此UserBetInfo的i64UserBetCount类型也改为了int，若后续出现该值大于int的取值范围，则只能改回longlong类型，按拆包发送处理
struct TMSG_BET_INFO_RSP
{
	UserBetInfo   i64UserBetInfos[BET_INFO_MAX]; //所有玩家在每个区域的下注情况(除庄家)

	TMSG_BET_INFO_RSP()
	{

	}
};

struct UserNuoData
{
	BYTE byBefore;			//从哪个区域挪
	BYTE byAfter;			//挪到哪个区域
	int  iNuoMoney;			//挪多少

	void Clear()
	{
		byBefore = 255;
		byAfter = 255;
		iNuoMoney = 0;
	}

	UserNuoData()
	{
		Clear();
	}
};

struct UserNuoInfo
{
	BYTE byBetStation;
	UserNuoData sUserNuoData[NUO_COUNT];

	void Clear()
	{
		byBetStation = 255;
		for (int i = 0; i < NUO_COUNT; ++i)
		{
			sUserNuoData[i].Clear();
		}
	}

	UserNuoInfo()
	{
		Clear();
	}
};

//作为所有玩家下注信息的补充消息
struct TMSG_NUO_INFO_RSP
{
	UserNuoInfo   sUserNuoInfos[BET_INFO_MAX];			//所有玩家在挪情况(除庄家)

	void Clear()
	{
		for (int i = 0; i < BET_INFO_MAX; ++i)
		{
			sUserNuoInfos[i].Clear();
		}
	}

	TMSG_NUO_INFO_RSP()
	{
		Clear();
	}
};


//挪请求
struct TMSG_CHANGE_BET_REQ
{
	BYTE byOriBetArea;				//原下注区域
	BYTE byNewBetArea;				//要挪到的下注区域
	int  byChangeCount[BETSTYLE];	//挪注筹码数(每种挪几个)

	TMSG_CHANGE_BET_REQ()
	{
		byOriBetArea = 255;
		byNewBetArea = 255;
		memset(byChangeCount, 0, sizeof(byChangeCount));
	}
};

//挪回复
struct TMSG_CHANGE_BET_RSP
{
	BYTE byErrorCode;			//0:成功 1:超过原下注区域下注值 2:超过区域限制 3:超过挪限制次数 4:选择区域错误 5:筹码类型错误 6:被挪玩家错误 

	TMSG_CHANGE_BET_RSP()
	{
		byErrorCode = 0;
	}
};

//挪了之后筹码数据更新通知
struct TMSG_UPDATE_BET_NTF
{
	long long   i64UserBetCount[BET_ARES];		//自己在每个区域的下注情况
	long long	i64AreaBetAllSum[BET_ARES];		//各区域的总下注

	TMSG_UPDATE_BET_NTF()
	{
		memset(this, 0, sizeof(TMSG_UPDATE_BET_NTF));
	}
};

//大结算
struct TMSG_CALCULATE_BOARD
{
	long long i64WinMoney[PLAY_COUNT];

	TMSG_CALCULATE_BOARD()
	{
		memset(i64WinMoney, 0, sizeof(i64WinMoney));
	}
};

#pragma pack()