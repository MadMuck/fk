#pragma once

#include <memory.h>
#include <stddef.h>

typedef unsigned char uchar;

///开发库版本
#define DEV_LIB_VER				1									///开发库版本
//支持类型
///支持类型定义
#define SUP_NORMAL_GAME			0x01								///普通游戏
#define SUP_MATCH_GAME			0x02								///比赛游戏
#define SUP_MONEY_GAME			0x04								///金币游戏
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

////////////////////////////////////////////////////////////////////////////////
#define GFF_FORCE_FINISH			0										///< 强行解除
#define GF_NORMAL               10                                  /// 游戏正常结束
#define GF_SAFE                 11                                  /// 游戏安全结束

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE		
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

#define AI_FAST							// 机器人快速出牌
#ifndef AI_MACHINE					// 取消快速机器人
#undef AI_FAST
#endif

// 支持类型
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					TEXT("MatchTable_BSMJ")

// 版本定义
#define GAME_MAX_VER					2									// 现在最高版本
#define GAME_LESS_VER					2									// 现在最低版本
#define GAME_CHANGE_VER					0									// 修改版本		
#define PLAY_COUNT						4									// 游戏人数	

// 游戏状态定义
#define GS_WAIT_SETGAME					0				// 等待东家设置状态
#define GS_WAIT_ARGEE					1				// 等待同意设置
#define GS_DICE							20				// 掷骰子状态
#define GS_SEND_CARD					21				// 发牌状态
#define GS_PLAY_GAME					22				// 游戏中状态
#define GS_WAIT_NEXT					23				// 等待下一盘开始 


//////组牌类型及动作类型////////////////////
/********************************************************************************/
#define ACTION_NO				0				//没有动作
#define ACTION_CHI				1				//吃牌动作
#define ACTION_PENG				2				//碰牌动作
#define ACTION_KAN				3				//坎牌牌动作
#define ACTION_AN_GANG			4				//暗杠动作
#define ACTION_BU_GANG			5				//补杠动作
#define ACTION_MING_GANG		6				//明杠动作
#define ACTION_CHI_TNG			7				//抢吃（吃后听牌）
#define ACTION_TING				8				//听动作
#define ACTION_HU				10				//糊动作
#define ACTION_YANG_MA			11				//养马

#define TYPE_JIANG_PAI			15				//将牌类型
#define TYPE_JINGDIAO_JIANG		16				//精钓将牌类型
#define TYPE_SHUN_ZI			17				//顺子类型
#define TYPE_AN_KE				18				//暗刻类型
#define TYPE_JING_KE			19				//财神刻子类型

///各种牌的数量
#define HAND_CARD_NUM			14				//最大手牌张数
#define OUT_CARD_NUM			60				//出牌张数
#define HUA_CARD_NUM			20				//花牌张数
#define MEN_CARD_NUM			40				//门牌张数
#define MAX_HUPAI_TYPE			10				//糊牌类型
#define MAX_HAND_PAI			14				//手牌数
#define MAX_PAI_TOU				16				//最大牌头数
#define MAX_TING_NUM			25				//最大听牌数
#pragma pack(push,1)

///吃碰杠牌数据结构
struct GCPStruct
{
	uchar  byType;    //吃碰杠类型
	uchar  iOutpai;   //别人出的牌
	uchar  byData[4]; //吃碰杠牌数据
	uchar  iStation;  //吃碰杠玩家位置
	uchar  iBeStation;//被吃碰杠玩家位置
	GCPStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byData,255,sizeof(byData));
		byType = 255;    //吃碰杠类型
		iOutpai = 255;   //别人出的牌
		iStation = 255;  //吃碰杠玩家位置
		iBeStation = 255;//被吃碰杠玩家位置
	};
};
//
///牌数据结构（保存牌值和张数）
struct PaiStruct
{
	uchar  pai;   //牌数据
	uchar  count; //牌张数
	PaiStruct()
	{
		Init();
	};
	void Init()
	{
		memset(this,255,sizeof(PaiStruct));
		count = 0;
	};
};

struct tagPM
{
	int iPlayingMethod;
	uchar nPlayerCount;

	bool bFengDing;			//是否封顶
	bool bFanMa;				//是否翻马
	uchar FengDingMulti;	//封顶倍数
	uchar FanMaNum;		//翻马个数
	tagPM()
	{
		iPlayingMethod = 0;
		nPlayerCount = 0;
		bFengDing = false;
		bFanMa = false;
	//	FanCardMulti = 1;
		FengDingMulti= 10;
		FanMaNum = 2;
//		bMaGenDiFen = false;
//		bMaGenGang = false;
	}
};

enum PM_FLAGS
{
	None = 0,
	PM_KeJiePaoHu		=1<<0,//可接炮胡
	PM_GangBao			=1<<1,//杠包自摸
	PM_BiHu					=1<<2,//逼胡

	PM_QiangGangHu	=1<<3,//抢杠胡 2
	PM_QiXiaoDui			=1<<4,//七小对 3
	PM_HunYiSe				=1<<5,//混一色 2
	PM_QingYiSe			=1<<6,//清一色 3
	PM_GangKai				=1<<7,//杠上开花 2
	PM_PengPengHU		=1<<8,//碰碰胡 2
	PM_ShiSanYao			=1<<9,//十三幺 10
	PM_TianDiHu			=1<<10,//天地胡 10
	PM_18LuoHan			=1<<11,//18罗汉 10
	PM_SuperQiDui		=1<<12,//豪华七对 4
	PM_DoubleSpQiDui	=1<<13,//双豪华七对 6
	PM_TripleSpQiDui		=1<<14,//三豪华七对 8
	PM_YaoJiu				=1<<15,//幺九 2
	PM_SmalSiXi				=1<<16,//小四喜4
	PM_BigSiXi				=1<<17,//大四喜 6
	PM_SmallSanYuan	=1<<18,//小三元 4
	PM_BigSanYuan		=1<<19,//大三元 6
	
	PM_MaGenGang		=1<<20,//马跟杠
	PM_MaGenDiFen		=1<<21,//马跟底分
	//////////////////////////////////////////////////
	//PM_HaveJoker = 1 << 1,				//有鬼
	//PM_NoJoker = 1 << 2,					//有鬼
	//PM_QiangGang_Double = 1 << 3,		//抢杠双倍牌头
	//PM_DanDiao_Joker = 1 << 4,				//12张单吊鬼
	//PM_YaoJiu_One_Or_Nine = 1 << 5,		//幺九胡1或9
	//PM_TianDiHu_Double = 1 << 6,			//天地胡双倍牌头
	//PM_ForeJoker_ZiMo = 1 << 7,				//四鬼自摸
	//PM_TimeOut_Auto = 1 << 8,				//超时托管（3分钟）
};


// 游戏数据包
/********************************************************************************/
//只用一个数据包，多余的数据可以不使用,断线重连使用
struct GameStationData
{
	//基本信息
	int					iVersion;						// 正副版本
	int					iVersion2;
	bool				bShowUserId;					// 是否显示所有玩家的id
	//游戏信息
	uchar				byNtStation;					// 庄家位置
	bool				m_bistuoguan[PLAY_COUNT];		// 是否托管
	uchar				m_byNowOutStation;				// 现在出牌用户
	bool				m_bTing[PLAY_COUNT];			// 是否听牌 

	int					Stationpara;
	uchar               ucAgree;                        //低四位表是否同意
	//牌数据
	uchar				m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//手牌不超过20张，0为结束标志
	uchar				m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//出牌不超过160张，0为结束标志
	uchar				m_iArMenPai[PLAY_COUNT][MEN_CARD_NUM];	//门牌不超过40张
	//游戏时间
	uchar				byAutoOutTime;		//自动出牌时间
	uchar				byOutTime;			//出牌时间
	uchar				byThinkTime;		//思考时间
	uchar				byBeginTime;		//开始等待时间
	bool				bAutoBegin;			//时间到了是否自动开始

	GameStationData()
	{
		Init();
	};
	void Init()
	{
		memset(this, 0, sizeof(GameStationData));	
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		memset(m_byArOutPai,255,sizeof(m_byArOutPai));
		memset(m_iArMenPai,255,sizeof(m_iArMenPai));

		byAutoOutTime = 1;		//自动出牌时间
		byOutTime = 20;			//出牌时间
		byThinkTime = 15;		//思考时间
		byBeginTime = 30;		//开始等待时间
		bAutoBegin = 0;			//时间到了是否自动开始
	}
};

struct GameStationData_Base
{
	//基本信息
	int					Stationpara;		// 游戏状态
	int					iVersion;			// 正副版本
	int					iVersion2;
	uchar               ucAgree;            //低四位表是否同意
	uchar				byAutoOutTime;		//自动出牌时间
	uchar				byOutTime;			//出牌时间
	uchar				byThinkTime;		//思考时间
	uchar				byBeginTime;		//开始等待时间
	tagPM				m_tagPM;			//玩法 
	uchar               m_byWindDes[PLAY_COUNT];
	uchar				m_BasePoint;	//底分
	GameStationData_Base()
	{
		memset(this, 0, sizeof(GameStationData_Base));	
		memset(m_byWindDes, 255, sizeof(m_byWindDes));
	};
};

//用户托管数据包
struct Usertouguan
{						
	uchar			desk;				// 谁托管
	bool			enable;				// 托管状态
	bool			bTuoGuan[4];		// 各个玩家托管状态
	void Clear()
	{}
};
/////用户同意结构
//struct MSG_GR_R_UserAgree
//{
//    uchar			bDeskNO;							///游戏桌号
//    uchar			bDeskStation;						///位置号码
//    uchar			bAgreeGame;							///同意标志
//};
//用户同意游戏
struct UserArgeeGame
{
	uchar			byPlayCount;				// 游戏盘数
	uchar			byCardCount;				// 牌数目
};

// 游戏结束统计数据包
struct GameCutStruct
{
	uchar				bDeskStation;					// 退出位置
	int					iTurePoint[4];					// 庄家得分
	int					iChangeMoney[8];				// 玩家银子
};

struct HaveThingStruct		// 有事离开
{
	uchar	pos;
	char	szMessage[61];
};

struct LeaveResultStruct	// 请求离开结果
{
	uchar	bLeaveStation; //想要离开玩家的位置
	uchar	bDeskStation;
	int		bArgeeLeave;
};

struct MessageStruct		// 客户端到服务器,消息
{
	char Message[200];
};

///////////////////////////////////////////////////////////////////////////

#define MAX_HEPAI_TYPES     256 ///最大糊牌类型

///麻将牌：各家门牌，翻精点，当前拿牌方向
struct MjMenPaiData 
{
	uchar    byMenPai[300];				//麻将的数量，玩家个数，门牌最大敦数，每敦上下2张牌 
	uchar    byNt;						//庄家位置
	uchar	byStation;					//当前抓牌玩家的位置
	uchar	byDir;						//当前抓牌方向(抓那个玩家门前的牌)
	uchar	byGetPaiDir;				//本局起牌方向
	uchar	byRemainDun;				//剩余敦数
	uchar	byGetPai;				    //起牌敦数
	uchar    byMenPaiDun[4];             //每个玩家门前的敦数
	uchar    byStartIndex;				//门牌起始索引		
	uchar    byEndIndex;				    //门牌结束索引		
	uchar    byCaiShenIndex;				//财神位置索引		
	uchar    byAllPaiNum;				//所有牌的数量		

	MjMenPaiData()
	{
		Init();
	};
	void Init()
	{
		memset(byMenPai,255,sizeof(byMenPai));
		memset(byMenPaiDun,255,sizeof(byMenPaiDun));//玩家门前的敦数
		byNt = 255;						//庄家位置
		byStation = 255;					//当前抓牌玩家的位置
		byDir = 255;						//当前抓牌方向(抓那个玩家门前的牌)
		byGetPaiDir = 255;				//本局起牌方向
		byRemainDun = 255;				//剩余敦数
		byGetPai = 255;				    //起牌敦数
		byStartIndex = 255;				//门牌起始索引
		byEndIndex = 255;				    //门牌结束索引
		byAllPaiNum = 255;				    //所有牌的数量
		byCaiShenIndex = 255;				//财神位置索引
	};
	uchar GetMenPaiNum()
	{
		uchar num = 0;
		for(int i=0;i<300;i++)
		{
			if(byMenPai[i] != 255)
			{
				num++;
			}
			if(num >= 254)
				break;
		}
		return num;
	};
	uchar GetOnePaiNum(uchar pai)
	{
		uchar num = 0;
		for(int i=0;i<300;i++)
		{
			if(byMenPai[i] == pai)
			{
				num++;
			}
		}
		return num;
	};
	bool DeleteAnMenPai(uchar pai)
	{
		if(pai == 255)
			return false;
		for(int i=0;i<300;++i)
		{
			if(byMenPai[i] != 255)
			{
				byMenPai[i] = 255;
				return true;
			}
		}
		return false;
	};
	bool ChangAnMenPai(uchar oldpai,uchar newpai)// 门牌中要被换的牌 oldpai,换来的新牌 newpai
	{
		if(oldpai == 255 || newpai == 255)
			return false;
		for(int i=0;i<300;++i)
		{
			if(byMenPai[i] == oldpai)
			{
				byMenPai[i] = newpai;//换牌成功
				return true;
			}
		}
		return false;
	};
};

/********************************************************************************/

// 数据包处理辅助标识

/********************************************************************************/
// #define MDM_GM_GAME_NOTIFY				180				///消息类型：游戏通知消息
// #define ASS_GM_GAME_STATION				2				///游戏状态
// #define ASS_GM_AGREE_GAME               1               ///用户同意

#define ASS_UG_USER_SET					70				//用户设置游戏
#define ASS_SEZIFINISH					72				//色子结束
#define ASS_AUTOCARD					74				//托管
#define ASS_PASS_CHECK					75				//"过"
#define ASS_HAVE_THING					76				//有事离开
#define ASS_LEFT_RESULT					77				//离开结果

#define ASS_SUPER						78				//超级客户端，发牌器
#define ASS_HUPAI_TEST					79				//超级客户端，糊牌测试

#define ASS_MJ_RULE						89				//麻将规则数据
#define ASS_EXIT						90				//退出
#define ASS_MESSAGE						91				//消息
#define	ASS_SET_STATE					92				//正在出牌人掉线重连后发给4个客户端的计时器信号
#define ASS_GM_SHORT_STOP               93              //暂停消息
#define ASS_NET_CAUSE_END               94              //网络原因导致游戏结束

//#define ASS_HUPAI_TEST					159			//超级客户端，糊牌测试
#define ASS_CHANGE_PAI					160				//用户换牌，测试用
#define ASS_SET_MOPAI					161				//用户设置准备要摸的牌
#define ASS_HUANGZHUANG					162				//强制荒庄

/********************************************************************************/

//////////////////////事件号////////////////////////////////////////////////////////////////////
//发牌前事件
enum MJTHINGS
{
	THING_BEGIN				=10,			//游戏开始事件					        
	THING_DONG_NT			,			//首局以东为庄事件					         
	THING_2SEZI_NT			,			//首局掷2颗色子的点数和为庄事件		   
	THING_2SEZI_NT_GP		,			//首局掷2颗色子定庄家和起牌点事件	
	THING_TURN_2SEZI_NT		,			//首局轮流掷2颗色子定庄家事件		
	THING_2SEZI_DIR			,			//掷2颗色子事件定起牌方向事件		
	THING_2SEZI_DIR_GP		,			//掷2颗色子事件定起牌方向和起牌点事件
	THING_2SEZI_GP			,			//掷2颗色子事件定起牌点事件		
	THING_2SEZI_NT_DIR_GP	,			//掷2颗色子事件定起牌点事件	
	//出牌前事件            
	THING_SEND_PAI			,			//发牌事件							
	THING_TIAO_PAI			,			//跳牌事件							
	THING_2SEZI_JING		,			//掷2颗色子定精牌事件			
	THING_1SEZI_JIN			,			//掷1颗色子定金牌事件
	THING_ALL_BUHUA		    ,			//为所有玩家补花事件				
	THING_ONE_BUHUA		    ,			//单个玩家补花事件					
	//正常动作事件          
	THING_BEGIN_OUT			,			//庄家开始发牌通知事件
	THING_DING_QUE          ,           //定缺事件
	THING_OUT_PAI			,			//出牌事件
	THING_ZHUA_PAI			,			//抓牌事件
	THING_CPG_NOTIFY		,			//杠吃碰通知事件事件
	THING_CHI_PAI			,			//吃牌事件
	THING_PENG_PAI			,			//碰牌事件
	THING_KAN_PAI			,			//坎牌事件
	THING_SAO_HU			,			//扫虎事件
	THING_GANG_PAI			,			//杠牌事件
	THING_TING_PAI			,			//听牌事件
	THING_WAIT_OUT			,			//等待出牌事件
	//其他动作事件          

	THING_CHANGE_NOTIFY		,			//通知玩家选牌
	THING_CHANGE_SEND_CARD	,			//重新发牌

	//糊牌及糊牌后事件
	THING_HU_PAI			,			//糊事件	
	THING_COUNT_FEN			,			//算分事件	
	THING_ENG_HANDLE		,			//结束处理事件
	THING_ENG_UN_NORMAL		,			//非正常结束处理事件
	THING_HAVE_ACTION		,			//有动作   

	S_C_UPDATE_CALCULATE_BOARD_SIG,		//更新结算榜
	S_C_UPDATE_REMAIN_JUSHU_SIG,		//更新剩余局数
	S_C_UPDATE_PLAY_METHOD,				//游戏玩法通知

	THING_PAI_TOU			,			//牌头事件

	THING_MAX				                //最大事件id
};                              
							
//////////////////////事件数据结构/////////////

///游戏开始事件
struct tagBegin
{
	uchar	byNt;		//庄家位置
	uchar	byRemainingCards;	// 剩余牌数
	uchar	m_byWindDes[PLAY_COUNT]; //玩家风位

	void Clear()
	{
		byNt	=255;		//庄家位置
		byRemainingCards = 0;
		memset(m_byWindDes, 255, sizeof(m_byWindDes));
	}
};

///以东为庄事件
struct tagDongNt
{
	uchar   byDo;		//0未发生，10已结束，中间状态为正在发生
	uchar   byNt;		//庄家位置
	uchar   byNo;		//事件的代号
	uchar   byNext;	//下一个事件的代号
	void Clear()
	{	
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byNt	=255;//庄家位置
		byNo = THING_BEGIN;//事件的代号
	}
};

///掷2颗色子的点数和为庄事件
struct tagTwoSeziNt
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//掷色子玩家
	uchar	byNt;//庄家位置
	uchar	bySezi0;//色子0数据
	uchar	bySezi1;//色子1数据
	bool	bSeziFinish[4];//记录已经播放完色子动画的玩家
	uchar	byNo;//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//掷色子玩家
		byNt	=255;//庄家位置
		bySezi0	=255;//色子0数据
		bySezi1	=255;//色子1数据	
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_NT;//事件的代号
	}
};

///掷2颗色子确定庄家和起牌点位置事件
struct tagTwoSeziNtAndGetPai
{
	uchar   byDo;	//0未发生，10已结束，中间状态为正在发生
	uchar   byUser;	//掷色子玩家
	uchar   byNt;	//庄家位置
	uchar   bySezi0;	//色子0数据
	uchar   bySezi1;	//色子1数据
	uchar   byGetPai;//起牌位置
	bool   bSeziFinish[4];//记录已经播放完色子动画的玩家
	uchar   byNo;//事件的代号
	uchar   byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//掷色子玩家
		byNt	=255;//庄家位置
		bySezi0	=255;//色子0数据
		bySezi1	=255;//色子1数据
		byGetPai=255;//起牌位置
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_NT_GP;//事件的代号
	}
};

///轮流掷2颗色子确定庄家
struct tagTurnSeziNt
{

	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byNt;//庄家位置
	uchar	byUser;//当前掷色子的玩家
	uchar	bySezi0;//色子0数据
	uchar	bySezi1;//色子1数据
	uchar	bySezi[4];//玩家0色子数据
	uchar	byMaxUser;//最大点数的玩家个数
	uchar	byMaxDian;//最大点数值
	bool	bSeziFinish[4];//记录已经播放完色子动画的玩家
	bool	bDoFinish[4];//记录已经掷完色子玩家
	bool	bDoSezi[4];//本轮要掷色子的玩家
	bool	bFirst;//是否第一轮掷色子
	uchar	byNo;//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byNt	=255;//庄家位置
		byUser	=255;//当前掷色子的玩家
		bySezi0	=255;//色子0数据
		bySezi1	=255;//色子1数据
		memset(bySezi,255,sizeof(bySezi));//玩家0色子数据
		byMaxUser	=255;//最大点数的玩家个数
		byMaxDian	=255;//最大点数值
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		memset(bDoFinish,0,sizeof(bDoFinish));
		memset(bDoSezi,0,sizeof(bDoSezi));
		bFirst = true;//是否第一轮掷色子
		byNo = THING_TURN_2SEZI_NT;//事件的代号
	}
};

///掷2颗色子确定起牌位置事件
struct tagTwoSeziDir
{
	uchar	byDo;			//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;			//掷色子玩家
	uchar	byNt;			//庄家
	uchar	bySezi0;		//色子0数据
	uchar	bySezi1;		//色子1数据
	uchar	byDirection;	//起牌方位
	bool	bSeziFinish[4];//记录已经播放完色子动画的玩家
	uchar	byNo;			//事件的代号
	uchar	byNext;		//下一个事件的代号
	void Clear()
	{
		byDo	=255;			//0未发生，10已结束，中间状态为正在发生
		byUser	=255;		//掷色子玩家
		bySezi0	=255;		//色子0数据
		bySezi1	=255;		//色子1数据
		byNt = 255;			//庄家
		byDirection	=255;	//起牌方位
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_DIR;//事件的代号
	}
};

///掷2颗色子确定起牌位置（点数和）和起牌点（最小点）事件
struct tagTwoSeziDirAndGetPai
{
	uchar	byUser;	//掷色子玩家
	uchar	bySezi0;	//色子0数据
	uchar	bySezi1;	//色子1数据
	uchar	byDirection;//起牌方位
	uchar	byGetPai;	//起牌点

	void Clear()
	{
		byUser	=255;	//掷色子玩家
		bySezi0	=255;	//色子0数据
		bySezi1	=255;	//色子1数据
		byDirection	=255;//起牌方位
		byGetPai	=255;	//起牌点
	}
};

///掷2颗色子确定起牌敦数事件
struct tagTwoSeziGetPai
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//掷色子玩家
	uchar	bySezi0;//色子0数据
	uchar	bySezi1;//色子1数据
	uchar	byGetPai;//起牌点
	bool	bSeziFinish[4];//记录已经播放完色子动画的玩家
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;		//0未发生，10已结束，中间状态为正在发生
		byUser	=255;	//掷色子玩家
		bySezi0	=255;	//色子0数据
		bySezi1	=255;	//色子1数据
		byGetPai	=255;	//起牌点
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_GP;//事件的代号
	}
};

///发牌事件
struct tagSendPai
{
	uchar	byRemainPai;//剩余牌数
	uchar	byAllPaiNum;//总牌数
	uchar	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//手牌数据
	uchar   m_byFanCard;			//翻出来的牌
	uchar   m_byJoker;				//万能牌

	void Clear()
	{
		byRemainPai	=255;//剩余牌数
		byAllPaiNum	=255;//总牌数
		m_byFanCard = 255;
		m_byJoker = 255;
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
	}
};
///跳牌事件
struct tagTiaoPai
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//跳牌玩家位置
	uchar	byNt;//庄家位置
	uchar	byIndex0;//跳牌位置0
	uchar	byIndex1;//跳牌位置1
	uchar	byRemainPai;//剩余牌数
	uchar	bySendPai;//发出牌数
	uchar	byGetPai;//起牌点
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	uchar	byMenPai[4][MEN_CARD_NUM];//门牌
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo		=255;//0未发生，10已结束，中间状态为正在发生
		byUser		=255;//跳牌玩家位置
		byNt		=255;//庄家位置
		byIndex0	=255;//跳牌位置0
		byIndex1	=255;//跳牌位置1
		byRemainPai	=255;//剩余牌数
		bySendPai	=255;//发出牌数
		byGetPai	=255;//起牌点
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(byMenPai, 255, sizeof(byMenPai));	//门牌
		byNo = THING_TIAO_PAI;//事件的代号
	}
};
///掷色子2颗色子定精牌事件
struct tagTwoSeziJing
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//掷色子玩家位置
	uchar	byNt;//庄家位置
	uchar	byFanPai;//翻牌点
	uchar	bySezi0;//色子0数据
	uchar	bySezi1;//色子1数据
	uchar	byMainJing;//正精牌值
	uchar	byLessJing;//副精的牌值
	uchar	byStartIndex;///开始索引
	uchar	byEndIndex;  ///结束索引
	uchar	byCaiShenIndex; ///财神索引
	uchar	byMenPai[4][MEN_CARD_NUM];//门牌
	bool	bSeziFinish[4];//记录已经播放完色子动画的玩家
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo		=255;//0未发生，10已结束，中间状态为正在发生
		byUser		=255;//掷色子玩家位置
		byNt		=255;//庄家位置
		byFanPai	=255;//翻牌点
		bySezi0		=255;//色子0数据
		bySezi1		=255;//色子1数据
		byMainJing	=255;//正精牌值
		byLessJing	=255;//副精的牌值
		byStartIndex	=255;///开始索引
		byEndIndex	=255;  ///结束索引
		byCaiShenIndex=255; ///财神索引
		memset(byMenPai, 255, sizeof(byMenPai));	//门牌
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_JING;//事件的代号
	}
};
///掷色子1颗色子定金牌事件
struct tagOneSeziJin
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//掷色子玩家位置
	uchar	byNt;//庄家位置
	uchar	byFanPai;//翻牌点
	uchar	bySezi0;//色子0数据
	uchar	byMainJing;//正精牌值
	uchar	byLessJing;//副精的牌值
	uchar	byStartIndex;///开始索引
	uchar	byEndIndex;  ///结束索引
	uchar	byCaiShenIndex; ///财神索引
	uchar	byMenPai[4][MEN_CARD_NUM];//门牌
	bool	bSeziFinish[4];//记录已经播放完色子动画的玩家
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo		=255;//0未发生，10已结束，中间状态为正在发生
		byUser		=255;//掷色子玩家位置
		byNt		=255;//庄家位置
		byFanPai	=255;//翻牌点
		bySezi0		=255;//色子0数据
		byMainJing	=255;//正精牌值
		byLessJing	=255;//副精的牌值
		byStartIndex	=255;///开始索引
		byEndIndex	=255;  ///结束索引
		byCaiShenIndex=255; ///财神索引
		memset(byMenPai, 255, sizeof(byMenPai));	//门牌
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_1SEZI_JIN;//事件的代号
	}
};

///所有玩家补花事件
struct tagAllBuHua
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser[4];//需要补花玩家位置
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	uchar	byMenPai[4][MEN_CARD_NUM];//门牌
	uchar	m_byArHuaPai[4][HUA_CARD_NUM];//花牌数据
	bool	bBuHuaFinish[4];//记录已经已经补花完成的玩家
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo = 255;//0未发生，10已结束，中间状态为正在发生
		memset(byUser, 255, sizeof(byUser));	//需要补花玩家位置
		memset(byMenPai, 255, sizeof(byMenPai));	//门牌
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//花牌数据	
		memset(bBuHuaFinish,0,sizeof(bBuHuaFinish));
		byNo = THING_ALL_BUHUA;//事件的代号
	}
};

///单个玩家补花事件
struct tagOneBuHua
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//补花玩家位置
	uchar	byHua;	  //抓到得花牌
	uchar	byPs;	  //补花的牌
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	uchar	byMenPai[4][MEN_CARD_NUM];//门牌
	uchar	m_byArHuaPai[4][HUA_CARD_NUM];//
	bool	bFinish;//是否补花结束
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//补花玩家位置
		byHua	=255;	  //抓到得花牌
		byPs	=255;	  //补花的牌
		memset(byMenPai, 255, sizeof(byMenPai));	//门牌
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//花牌数据
		bFinish=false;
		byNo = THING_ONE_BUHUA;//事件的代号
	}
};
///开始出牌通知事件
struct tagBeginOutPai
{
	uchar	byUser;	//首次出牌玩家位置
	uchar	byNt;	//庄家位置

	void Clear()
	{
		byUser	=255;//首次出牌玩家位置
		byNt	=255;//庄家位置
	}
};
///出牌事件
struct tagOutPai
{
	uchar	byUser;				//出牌玩家位置
	uchar	byPs;				//出牌的值
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	uchar	m_byArOutPai[4][OUT_CARD_NUM];//出牌不超过160张，0为结束标志

	void Clear()
	{
		byUser	=255;				//出牌玩家位置
		byPs		=255;				//出牌的值
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//出牌数据	
	}
};
///抓牌牌事件
struct tagZhuaPai
{
	uchar	byUser;		//抓牌玩家位置
	uchar	byPs;		//抓牌的值
	bool	bHead;		//抓牌的方向
	uchar	byRemainingCards; // 剩余牌数

	void Clear()
	{
		byUser	=255;		//抓牌玩家位置
		byPs		=255;		//抓牌的值
		bHead = true;
		byRemainingCards = 0;
	}
};
///吃碰杠糊通知事件事件
struct tagCPGNotify
{
	uchar	byUser;				//出牌玩家位置
	uchar	byPs;				//出牌的值
	bool	bZhuaPai;			//是否自己抓牌
	bool	bChi;				//是否能吃
	bool	bPeng;				//是否能碰
	bool	bGang;				//是否能杠
	bool	bTing;				//当前能否听牌
	bool	bHu;				//是否能糊
	bool    bCanAction;			//玩家是否有动作可做
	uchar	m_byTingCanOut[HAND_CARD_NUM];//听牌能打的牌
	uchar	m_iChi[3][3];		//能吃的牌
	uchar	m_iGangData[4][2];	//能杠的牌	
	uchar   m_iHuPs;			//胡的牌
	
	void Clear()
	{
		byUser	=255;			//出牌玩家位置
		byPs	=255;			//出牌的值
		bZhuaPai=false;			//是否自己抓牌
		bChi	=false;			//是否能吃
		bPeng	=false;			//是否能碰
		bGang	=false;			//是否能杠
		bTing	=false;			//当前能否听牌
		bHu		=false;			//是否能糊
		bCanAction = false;		//玩家是否有动作可做
		memset(m_byTingCanOut,255,sizeof(m_byTingCanOut));///听牌能打的牌
		memset(m_iChi,255,sizeof(m_iChi));//能吃的牌
		memset(m_iGangData,255,sizeof(m_iGangData));//能杠的牌	
		m_iHuPs = 255;
	}
};
///吃牌牌事件
struct tagChiPai
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//吃牌玩家位置
	uchar	byBeChi;//被吃牌玩家位置
	uchar	byType;//吃牌的类型（0吃大，1吃中间，2吃小，吃字牌）
	uchar	byPs;	//吃的牌值
	uchar	byChiPs[3];//吃牌的数组
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	uchar	m_byArOutPai[4][OUT_CARD_NUM];//出牌不超过160张，0为结束标志
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//吃牌玩家位置
		byBeChi	=255;//被吃牌玩家位置
		byType	=255;//吃牌的类型（0吃大，1吃中间，2吃小，吃字牌）
		byPs		=255;	//吃的牌值
		memset(byChiPs, 255, sizeof(byChiPs));//吃牌的数组
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//出牌数据
		byNo = THING_CHI_PAI;//事件的代号
	}
};
///碰牌牌事件
struct tagPengPai
{
	uchar	byUser;//碰牌玩家位置
	uchar	byBePeng;//被碰牌玩家位置
	uchar	byPs;	//碰的牌值

	void Clear()
	{
		byUser	=255;//吃牌玩家位置
		byBePeng	=255;//被碰牌玩家位置
		byPs		=255;//碰的牌值
	}
};
///坎牌事件
struct tagKanPai
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//碰牌玩家位置
	uchar	byPs;	//碰的牌值
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//吃牌玩家位置
		byPs		=255;//碰的牌值
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		byNo = THING_KAN_PAI;//事件的代号
	}
};
///扫虎事件（查虎麻将专用）
struct tagSaoHu
{
	uchar	byDo;		//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;		//玩家位置
	uchar	byPs;		//碰的牌值
	bool	bSaohu[4];	//扫虎的玩家
	bool	bHitSaohu[4];	//刚点击了扫虎
	uchar    byOutData[4];//单钓扫虎玩家出的牌
	uchar    byHandData[4];//单钓扫虎玩家留在手中的牌
	uchar    byBaoPai;	//包牌玩家位置
	uchar	byNo;		//事件的代号
	uchar	byNext;		//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//吃牌玩家位置
		byPs	=255;//碰的牌值
		byBaoPai=255;	//包牌玩家位置
		memset(bSaohu, 0, sizeof(bSaohu));//	
		memset(bHitSaohu, 0, sizeof(bHitSaohu));//	
		memset(byOutData, 255, sizeof(byOutData));//	
		memset(byHandData, 255, sizeof(byHandData));//	
		byNo = THING_SAO_HU;//事件的代号
	}
};
///杠牌牌事件
struct tagGangPai
{
	uchar	byUser;//杠牌玩家位置
	uchar	byBeGang;//被杠牌玩家位置
	uchar	byType;//杠牌的类型
	uchar	byPs;	//杠的牌值

	void Clear()
	{
		byUser	=255;//杠牌玩家位置
		byBeGang	=255;//被杠牌玩家位置
		byType	=255;//杠牌的类型
		byPs		=255;	//杠的牌值
	}
};
///听牌牌事件
struct tagTingPai
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//听牌玩家位置
	uchar	byTing;//出牌玩家位置
	uchar	byPs;	//听牌出的牌值
	uchar	byNo;	//事件的代号
	bool	bHitTing[4];//玩家点击了听牌
	uchar	byNext;//下一个事件的代号
	void Clear()
	{
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//听牌玩家位置
		byTing	=255;//出牌玩家位置
		byPs		=255;	//听牌出的牌值	
		memset(bHitTing,0,sizeof(bHitTing));//玩家点击了听牌
		byNo = THING_TING_PAI;//事件的代号
	}
};


///糊牌事件
struct tagHuPai
{
	uchar	byUser;				//糊牌玩家位置
	uchar	byDianPao;			//点炮玩家位置
	uchar	byPs;				//胡牌的牌值
	uchar	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//手牌数据
	uchar	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];//出牌不超过160张，0为结束标志
	uchar	byUserNum;			//糊牌玩家的个数
	bool    bZimo;				//是否自摸
	uchar   byHuType[MAX_HUPAI_TYPE];//糊牌类型	

	void Clear()
	{
		byUser		=255;//糊牌玩家位置
		byDianPao	=255;//点炮玩家位置
		byPs		=255;	//胡牌的牌值
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//出牌数据
		byUserNum = 0;//糊牌玩家的个数
		bZimo = false;   //是否自摸
		memset(byHuType, 255, sizeof(byHuType));//出牌数据
	}
};
///算分事件
struct tagCountFen
{
	uchar	byNt;//庄家位置
	bool	bHu[PLAY_COUNT];//那些玩家糊牌
	__int64 iZongFen[PLAY_COUNT];//各个玩家的总分
	int 	iHuFen[PLAY_COUNT];//各个玩家的糊牌分
	uchar	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//手牌数据
	uchar   byHuType[PLAY_COUNT][MAX_HUPAI_TYPE];//各个玩家的糊牌类型	


	void Clear()
	{

		byNt	=255;//庄家位置
		memset(bHu,0,sizeof(bHu));
		memset(iZongFen,0,sizeof(iZongFen));
		memset(iHuFen,0,sizeof(iHuFen));
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据	
		memset(byHuType, 255, sizeof(byHuType));//各个玩家的糊牌类型
	}
};
///游戏结束处理事件
struct tagEndHandle
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//糊牌玩家位置
	uchar	byHu;//点炮玩家位置
	uchar	byNextNt;//下局庄家位置
	uchar	byPs;	//点炮的牌值
	bool	bDH;//一炮多响
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	tagEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//糊牌玩家位置
		byHu	=255;//点炮玩家位置
		byNextNt=255;//下局庄家位置
		byPs		=255;	//点炮的牌值
		bDH = 0;	
		byNo = THING_ENG_HANDLE;//事件的代号
	}
};
///非正常结束处理事件
struct tagUnnormalEndHandle
{
	uchar	byDo;//0未发生，10已结束，中间状态为正在发生
	uchar	byUser;//糊牌玩家位置
	int		iFen[8];//得分
	int		iMoney[8];//金币
	bool    bForce;//是否强退结束
	uchar	byNo;	//事件的代号
	uchar	byNext;//下一个事件的代号
	tagUnnormalEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byDo	=255;//0未发生，10已结束，中间状态为正在发生
		byUser	=255;//糊牌玩家位置
		memset(iFen, 0, sizeof(iFen));
		memset(iMoney, 0, sizeof(iMoney));
		bForce = false;//是否安全结束
		byNo = THING_ENG_UN_NORMAL;//事件的代号
	}
};

//总结榜数据结构
struct	CalculateBoard
{
	bool	bClear;
	bool	bBestFire;				//最佳炮手
	bool	bWinner;				//大赢家
	int		iZimo;					//自摸次数
	int		iJiePao;				//接炮次数
	int		iDianPao;				//点炮次数
	int		iAnGang;				//暗杠次数
	int		iMingGang;				//明杠次数
	int     iBuGang;				//补杠次数
	int		iPaiTou;				//中牌头次数
	__int64	i64WinMoney;			//输赢金币
	CalculateBoard()
	{
		memset(this,0,sizeof(CalculateBoard));
	}
};

//保存游戏信息，用于重新玩(基本只是用于大结算)
struct	GameInfForReplay
{
	int		iZimo;					//自摸次数
	int		iJiePao;				//接炮次数
	int		iDianPao;				//点炮次数
	int		iAnGan;					//暗干次数
	int		iMingGan;				//明干次数
	__int64	i64WinMoney;			//输赢金币

	bool    isBanker;
	GameInfForReplay()
	{
		memset(this,0,sizeof(GameInfForReplay));
	}
};

#define INIT_MSG(type, var, val, dataToSend, dataSize)\
	type var;\
	memset(&var, val, sizeof(var));\
	var.Clear();\
	type *dataToSend = &var;\
	int dataSize = sizeof(var);

#define EXTRACT_REQUEST(type, inputData, inputDataSize, req)\
	type *req = (type *)inputData;\
	if(sizeof(type) != inputDataSize)\
	req = NULL;

#pragma pack(pop)
///////////////////////////////////////////////////
#endif