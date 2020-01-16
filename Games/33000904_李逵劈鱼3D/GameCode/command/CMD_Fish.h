
#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// 服务定义

#define KIND_ID               2010
#define GAME_NAME             TEXT("渔夫码头")
#define GAME_PLAYER           4



#define MAX_BULLET_COUNT		99	//每个人的最大子弹限制个数
#define MAX_COIN_COUNT			200	//最大金币个数

const int	G_iFish_Count = 300;	//屏幕中最多容纳多少条鱼

// 简单的版本检测
// 如果客户端有更新应该更改这个值.如果只修改EXE的版本而网络数据包没修改的话，更新客户端是没什么意义的，因为EXE的版本是可以直接被修改的
// 只要保存以前的EXE版本,更新完之后再换回去，如果有BUG的话等于还是没修复。
#define GAME_VERSION          30

#ifndef SCORE
#define SCORE __int64/*LONGLONG*/
#endif

struct Position {
	float x;
	float y;
};

const Position kMuzzleOffset = { 41.f, 50.f };

const int kResolutionWidth = 1440;
const int kResolutionHeight = 900;


#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif
//const float kChairDefaultAngle[GAME_PLAYER] = { M_PI, M_PI, M_PI, 0, 0, 0, -M_PI_2, M_PI_2 };

enum TraceType {
  TRACE_LINEAR = 0,
  TRACE_BEZIER
};

struct FPoint {
  float x;
  float y;
};

struct FPointAngle {
  float x;
  float y;
  float angle;
};
struct VectorForward
{
	float x;
	float y;
};


//////////////////////////////////////////////////////////////////////////
// 游戏定义

/*
// 座位号
-------------
    0   1   2 
7               3                
    6   5   4
-------------
*/

enum SceneKind {
  SCENE_KIND_1 = 0,
  SCENE_KIND_2,
  SCENE_KIND_3,
  SCENE_KIND_4,
  SCENE_KIND_5,

  SCENE_KIND_COUNT
};

enum SceneStyle
{
	SCENE_STYLE_1 = 0,
	SCENE_STYLE_2,
	SCENE_STYLE_3,

	SCENE_STYLE_COUNT
};


enum FishKind 
{
	FISH_KIND_1 = 0 ,		//蜗牛鱼
	FISH_KIND_2,			//绿草鱼
	FISH_KIND_3,			//黄草鱼
	FISH_KIND_4,			//大眼鱼
	FISH_KIND_5,			//黄扁鱼
	FISH_KIND_6,			//小丑鱼
	FISH_KIND_7,			//小刺鱼
	FISH_KIND_8,			//蓝鱼
	FISH_KIND_9,			//红灯笼鱼
	FISH_KIND_10,			//蓝灯笼鱼
	FISH_KIND_11,			//海龟
	FISH_KIND_12,			//花瓣鱼
	FISH_KIND_13,			//蝴蝶鱼
	FISH_KIND_14,			//孔雀鱼
	FISH_KIND_15,			//剑鱼
	FISH_KIND_16,			//蝙蝠鱼
	FISH_KIND_17,			//银鲨鱼
	FISH_KIND_18,			//金鲨鱼
	FISH_KIND_19,			//金箍棒
	FISH_KIND_20,			//霸王鲸
 	FISH_KIND_21,			//美人鱼
 	FISH_KIND_22,			//金龙
 	FISH_KIND_23,			//李逵
// 	FISH_KIND_24,			//全屏炸弹
// 	FISH_KIND_25,			//金色金蟾
// 	FISH_KIND_26,			//绿色金蟾
// 
// 
// 	FISH_KIND_27,			//三角大三元
// 	FISH_KIND_28,			//直线大三元
// 	FISH_KIND_29,			//四角大四喜
// 	FISH_KIND_30,			//直线大四喜
// 
// 	FISH_KIND_31,			//红鱼--鱼王
// 	FISH_KIND_32,			//带盘子的鱼-鱼王
	FISH_KIND_COUNT
};

enum Fish_Coin_Count
{
	FISH_1_COIN_COUNT = 2,
	FISH_2_COIN_COUNT = 3,
	FISH_3_COIN_COUNT = 4,
	FISH_4_COIN_COUNT = 5,
	FISH_5_COIN_COUNT = 6,
	FISH_6_COIN_COUNT = 7,
	FISH_7_COIN_COUNT = 8,
	FISH_8_COIN_COUNT = 9,
	FISH_9_COIN_COUNT = 10,
	FISH_10_COIN_COUNT = 11,
	FISH_11_COIN_COUNT = 12,
	FISH_12_COIN_COUNT = 13,
	FISH_13_COIN_COUNT = 14,
	FISH_14_COIN_COUNT = 15,
	FISH_15_COIN_COUNT = 16,
	FISH_16_COIN_COUNT = 17,
	FISH_17_COIN_COUNT = 18,
	FISH_18_COIN_COUNT = 19,
	FISH_19_COIN_COUNT = 20,
	FISH_20_COIN_COUNT = 21,
 	FISH_21_COIN_COUNT = 22,
 	FISH_22_COIN_COUNT = 23,
 	FISH_23_COIN_COUNT = 24,
// 	FISH_24_COIN_COUNT = 25,
// 	FISH_25_COIN_COUNT = 26,
// 	FISH_26_COIN_COUNT = 27,
// 	FISH_27_COIN_COUNT = 28,
// 	FISH_28_COIN_COUNT = 29,
// 	FISH_29_COIN_COUNT = 30,
// 	FISH_30_COIN_COUNT = 31,
// 	FISH_31_COIN_COUNT = 32,
};

//打死指定鱼 生成多少个鱼币个数
static unsigned int	G_Fish_Coin_Count[FISH_KIND_COUNT] = 
{
	FISH_1_COIN_COUNT,
	FISH_2_COIN_COUNT,
	FISH_3_COIN_COUNT,
	FISH_4_COIN_COUNT,
	FISH_5_COIN_COUNT,
	FISH_6_COIN_COUNT,
	FISH_7_COIN_COUNT,
	FISH_8_COIN_COUNT,
	FISH_9_COIN_COUNT,
	FISH_10_COIN_COUNT,
	FISH_11_COIN_COUNT,
	FISH_12_COIN_COUNT,
	FISH_13_COIN_COUNT,
	FISH_14_COIN_COUNT,
	FISH_15_COIN_COUNT,
	FISH_16_COIN_COUNT,
	FISH_17_COIN_COUNT,
	FISH_18_COIN_COUNT,
	FISH_19_COIN_COUNT,
	FISH_20_COIN_COUNT,
 	FISH_21_COIN_COUNT,
 	FISH_22_COIN_COUNT,
 	FISH_23_COIN_COUNT,
// 	FISH_24_COIN_COUNT,
// 	FISH_25_COIN_COUNT,
// 	FISH_26_COIN_COUNT,
// 	FISH_27_COIN_COUNT,
// 	FISH_28_COIN_COUNT,
// 	FISH_29_COIN_COUNT,
// 	FISH_30_COIN_COUNT,
// 	FISH_31_COIN_COUNT,
};

//环形渔政个数
//static	int G_Fish_Circle_Count[FISH_KIND_COUNT] = {60,60,50,35,30,35,30,25,20,20,10,8,0,0,0,0,0,0,0,0,0,};

enum BulletKind 
{
  BULLET_KIND_1_NORMAL = 0,
  BULLET_KIND_2_NORMAL,
  BULLET_KIND_3_NORMAL,
  BULLET_KIND_4_NORMAL,
  BULLET_KIND_1_ION,
  BULLET_KIND_2_ION,
  BULLET_KIND_3_ION,
  BULLET_KIND_4_ION,

  BULLET_KIND_COUNT
};




const int kMaxCatchFishCount = 2;

// 限制客户端的鱼种类数量
//const int kFishKindCount[FISH_KIND_COUNT] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

//////////////////////////////////////////////////////////////////////////
// 服务端命令
#define SUB_S_CHOICE_TEAM_NTF				97 //通知选择阵营
#define SUB_S_CHOICE_TEAM_RES				98 //选择阵营结果
#define SUB_S_GAME_BEGIN					99 //游戏开始通知
#define SUB_S_GAME_CONFIG                   100
#define SUB_S_FISH_TRACE                    101//生成新鱼的轨迹
#define SUB_S_FISH_GROUP_TRACE              102//生成鱼群的轨迹
#define SUB_S_EXCHANGE_FISHSCORE            103
#define SUB_S_USER_FIRE                     104//玩家射击
#define SUB_S_CATCH_FISH                    105
#define SUB_S_BULLET_ION_TIMEOUT            106//子弹范围失效
#define SUB_S_LOCK_TIMEOUT                  107//锁定鱼失效时间
#define SUB_S_CATCH_SWEEP_FISH              108
#define SUB_S_CATCH_SWEEP_FISH_RESULT       109
#define SUB_S_HIT_FISH_LK                   110
#define SUB_S_SWITCH_SCENE                  111
#define SUB_S_STOCK_OPERATE_RESULT          112
#define SUB_S_SCENE_END                     113

#define SUB_S_CATCH_SWEEP_FISH_RESULT_EX    115//广播通知所有玩家捕到打鱼

#define SUB_S_GAMEOUT						114//服务端强行让客户端退出

#define SUB_S_USER_COME	                    120//玩家进入

#define S_C_UPDATE_CONTROL_INFO_SIG			121//更新控制信息
#define S_C_UPDATE_SPECIAL_INFO_SIG			122//更新控制信息
#define S_C_UPDATE_ONLINEUSER_INFO_SIG		123//更新在线玩家信息
#define S_C_SWITCH_INFO_SIG					124
#define S_C_UPDATE_FISH_NAME_SIG			125
#define S_C_SUPER_CONFIG_SIG				126
#define S_C_END_UPDATE_ONLINEUSER_INFO_SIG	127

#define S_C_GAME_FINISH_SIG					130//一局游戏结束小结算
#define S_C_UPDATE_CALCULATE_BOARD_SIG		131//更新大结算
#define	S_C_UPDATE_REMAIN_INFO_SIG			132//更新剩余局数，剩余时间

/// 剩余局数，秒数
struct S_C_RemainTime
{
	int remainCount;						//// 剩余局数
	int totalCount;							//// 总局数
	int remainTime;                         //// 剩余时间
	int totalTime;							//// 总时间

	S_C_RemainTime()
	{
		memset(this, 0, sizeof(this));
	}
};

enum GameResult
{
	WIN = 0,	//胜利
	LOSE = 1,	//失败
	DEUCE = 2	//平手
};

/// 大结算
struct S_C_Calculate_Board
{
	SCORE iuserPoint[PLAY_COUNT];           //// 玩家得分
	BYTE  isWin[PLAY_COUNT];				//输赢情况

	S_C_Calculate_Board()
	{
		clear();
	}

	void clear()
	{
		memset(iuserPoint,0,sizeof(iuserPoint));
		memset(isWin,false,sizeof(isWin));
	}
};

/// 小结算
struct S_C_GameFinish
{
	SCORE iuserPoint[PLAY_COUNT];           //// 玩家得分
	BYTE  isWin[PLAY_COUNT];				//输赢情况

	void clear()
	{
		memset(iuserPoint,0,sizeof(iuserPoint));
		memset(isWin,false,sizeof(isWin));
	}

	S_C_GameFinish()
	{
		clear();
	}
};

enum team
{
	TEAM_NULL = 0,
	TEAM_A = 1,
	TEAM_B = 2
};

struct S_C_CHOICE_TEAM_NTF
{
	BYTE byteamInfo[PLAY_COUNT];		//阵营信息

	S_C_CHOICE_TEAM_NTF()
	{
		clear();
	}

	void clear()
	{
		memset(byteamInfo, 0, sizeof(byteamInfo));

	}
};

struct S_C_CHOICE_TEAM_RES
{
	BYTE byDeskStation;				//选择阵营玩家
	bool bsuccess;					//选择是否成功
	BYTE byteamInfo[PLAY_COUNT];	//阵营信息
};

struct S_C_GAME_BEGIN
{
	SCORE PointsInit[PLAY_COUNT];	//初始分数--
	int   ShootInit;	//初始炮数
};

struct S_C_SUPER_CONFIG
{
	bool bSuperUser;
};

struct CMD_S_GameStatus 
{
  DWORD game_version;
  SceneStyle Scene_Style_Kind_;			//场景背景ID
  int	iUserFireBase[GAME_PLAYER];		//玩家炮火倍数
  SCORE fish_score[GAME_PLAYER];		//玩家分数
  SCORE exchange_fish_score[GAME_PLAYER];
  int	iRemainShootCount[GAME_PLAYER];	//玩家剩余炮数
  BYTE  byteamInfo[PLAY_COUNT];			//玩家阵营	
  int   iPlayMode;						//游戏玩法
  int   iPointsInit[PLAY_COUNT];	//初始分数
  int   iShootInit;					    	//初始炮数
};

struct CMD_S_GameConfig {
  int exchange_ratio_userscore;
  int exchange_ratio_fishscore;
  int exchange_count;

  int min_bullet_multiple;
  int max_bullet_multiple;

  int bomb_range_width;
  int bomb_range_height;

  int fish_multiple_min[FISH_KIND_COUNT];
  int fish_multiple_max[FISH_KIND_COUNT];
  double fish_speed[FISH_KIND_COUNT];
  int fish_bounding_box_width[FISH_KIND_COUNT];
  int fish_bounding_box_height[FISH_KIND_COUNT];
  int fish_hit_radius[FISH_KIND_COUNT];

  int bullet_speed[BULLET_KIND_COUNT];
  int net_radius[BULLET_KIND_COUNT];
};

struct GameStation//断线重回用的数据包sdp
{
	//游戏版本
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	BYTE				game_status;//状态标识

	CMD_S_GameStatus	GameStatus;//状态数据
	CMD_S_GameConfig	GameConfig;//配置数据

	BYTE GameMessage[MAX_PATH];//游戏消息
};

//玩家进入  主要是把玩家的金币数发送过来
struct CMD_S_GameUserCome
{
	DWORD	dwDeskStation;
	__int64 i64fish_score;				//玩家分数
	int	iRemainShootCount;				//玩家剩余炮数

	CMD_S_GameUserCome()
	{
		memset(this,0,sizeof(CMD_S_GameUserCome));
		dwDeskStation = 255;
	}
};

struct OnLineUserInfo
{
	BYTE	deskNo;
	char	nickName[61];
	int		i64UserMoney;
	DWORD	userID;
	OnLineUserInfo()
	{
		memset(this,0,sizeof(OnLineUserInfo));
	}
};

struct S_C_SWITCH_INFO
{
	bool special_switch_;
	bool control_switch_;
	S_C_SWITCH_INFO()
	{
		memset(this,0,sizeof(S_C_SWITCH_INFO));
	}
};


struct S_C_FISH_NAME
{
	BYTE	byFishKind;
	char	cFishName[4][MAX_PATH];
};

struct S_C_ON_LINE_USER
{	

	bool	bStart;
	OnLineUserInfo user_info_;
	S_C_ON_LINE_USER()
	{
		memset(this,0,sizeof(S_C_ON_LINE_USER));
	}
};

struct CMD_S_FishTrace 
{
	FPoint init_pos[5];
	int init_count;
	FishKind fish_kind;			//鱼的主类型
	FishKind fish_kind_ex[5];	//鱼的辅助类型 
	int fish_id;
	TraceType trace_type;
	BYTE TraceID;			//轨迹ID
	BYTE SceneFish;			//鱼阵类型

	void init()
	{
		memset(this,0,sizeof(CMD_S_FishTrace));
	}
};

struct	CMD_S_CirleFishGroup
{
	bool		bHave;
	FishKind	iFishKind;
	int			iIndex;
	int			iCount;
	int			iRedIndex;
	int			iRedIndexIndex;

	CMD_S_CirleFishGroup()
	{
		bHave = false;
		iCount = 0;
		iFishKind = FISH_KIND_COUNT;
		iIndex = -1;
		iRedIndex = 0;
		iRedIndexIndex = 0;
	}
};



struct CMD_S_ExchangeFishScore 
{
  WORD chair_id;
  SCORE swap_fish_score;//当前玩家实际鱼币数 而不是增量
  SCORE exchange_fish_score;
};

struct CMD_S_UserFire 
{
  BulletKind bullet_kind;
  int bullet_id;
  WORD chair_id;
  WORD android_chairid;
  float vector_x;
  float vector_y;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
  SCORE fish_score;
  bool bIsRobot;
  int iRemainShootCount;		//玩家剩余炮数
  bool bAutoShoot;
 // VectorForward vectorForward;
};

struct CMD_S_CatchFish 
{
  WORD chair_id;
  int fish_id;
  FishKind fish_kind;
  bool bullet_ion;
  SCORE fish_score;
  __int64	i64UserFishScore_;	//玩家分数
};

struct CMD_S_BulletIonTimeout {
  WORD chair_id;
};

struct CMD_S_CatchSweepFish {
	BYTE	byIndex;
	WORD chair_id;
	int fish_id;
	__int64 i64FishSocre;
	CMD_S_CatchSweepFish()
	{
		memset(this,0,sizeof(CMD_S_CatchSweepFish));
	}	
};

struct CMD_S_CatchSweepFishResult 
{
  WORD		chair_id;
  int		fish_id;
  int		catch_fish_count;
  int		catch_fish_id[200];
  int		fish_score[200];
  SCORE		fish_score_Sum;
  __int64	i64UserFishScore_;	//玩家分数
};

struct CMD_S_CatchSweepFishResultEx 
{
	char    cRoomName[61];//房间名字
	int		exchange_ratio_userscore_;	//换算比例中用户金币
	int		exchange_ratio_fishscore_;	//换算比例中鱼币
	int		iDeskID;			//桌子ID
	char	szNickName[61]; //昵称
	int		fish_kind;		//鱼类型ID
	SCORE	fish_score;		//打中鱼赚多少钱
	char    cFishName[MAX_PATH];//鱼的名字
};

struct CMD_S_HitFishLK {
  WORD chair_id;
  int fish_id;
  int fish_mulriple;
};

struct CMD_S_SwitchScene 
{
  SceneKind scene_kind;
  SceneStyle Scene_Style_;	//场景ID
  int fish_count;
  int fish_id[245];
  FishKind fish_kind[245];
};

struct CMD_S_GroupFishTrace
{
	BYTE byIndex;
	int fish_count;
	FishKind fish_kind[250];
	FishKind fish_red_kind[5];		//单独标记红色鱼王
	int fish_id[250];
};


struct CMD_S_StockOperateResult {
  unsigned char operate_code;
  SCORE stock_score;
};

//////////////////////////////////////////////////////////////////////////
// 客户端命令

//#define SUB_C_EXCHANGE_FISHSCORE            1
#define SUB_C_USER_FIRE                     2//玩家开火
#define SUB_C_CATCH_FISH                    3
#define SUB_C_CATCH_SWEEP_FISH              4
#define SUB_C_HIT_FISH_I                    5
#define SUB_C_STOCK_OPERATE                 6
#define SUB_C_USER_FILTER                   7
#define SUB_C_ANDROID_STAND_UP              8
#define SUB_C_FISH20_CONFIG                 9
#define SUB_C_ANDROID_BULLET_MUL            10//机器人子弹数目

#define C_S_UPDATE_SUPER_INFO_SIG			11
#define C_S_ADD_CONTROL_INFO_SIG			12
#define C_S_DELETE_CONTROL_INFO_SIG			13
#define C_S_ADD_SPECIAL_SIG					14
#define C_S_DELETE_SPECIAL_SIG				15
#define C_S_SWITCH_CONTROL_SIG				16
#define C_S_SWITCH_SPECIAL_SIG				17


#define C_S_CLEAR_FISH_SIG					18	//清除鱼

#define SUB_C_USER_CHOICE_TEAM				19 //玩家选择阵营

struct CMD_C_CHOICE_TEAM
{
	BYTE byDeskStation;				//选择阵营玩家
	BYTE bychoiceTeam;				//选择的阵营
};

struct C_S_ClearFish
{
	int fish_id_;
	C_S_ClearFish()
	{
		memset(this,0,sizeof(C_S_ClearFish));
	}
};

struct SpecialFish
{
	BYTE fish_kind_;
	int left_num_;
	int catch_rate_;
	SpecialFish()
	{
		memset(this,0,sizeof(SpecialFish));
	}
};

struct SpecialUser
{
	SpecialFish special_fish_[4];
	long int user_id_;
	SpecialUser()
	{
		memset(this,0,sizeof(SpecialUser));
		user_id_ = -1;
	}
};

struct ControlInfo
{
	bool win_or_lose_;
	int user_id_;
	int catch_rate_;
	SCORE limit_score_;
	ControlInfo()
	{
		memset(this,0,sizeof(ControlInfo));
		user_id_ = -1;
	}
};

struct C_S_DELETE_CONTROL_INFO 
{
	int user_id_;
};

struct C_S_ADD_SPECIAL
{
	BYTE fish_kind_;
	int user_id_;
	int left_num_;
	int catch_rate_;

	C_S_ADD_SPECIAL()
	{
		memset(this,0,sizeof(C_S_ADD_SPECIAL));
	}
};

struct C_S_DELETE_SPECIAL
{
	int user_id_;
	int fish_kind_;
	C_S_DELETE_SPECIAL()
	{
		memset(this,0,sizeof(C_S_DELETE_SPECIAL));
	}
};

struct S_C_UPDATE_CONTROL_INFO
{
	ControlInfo control_infos_[40];
	S_C_UPDATE_CONTROL_INFO()
	{
		memset(this,0,sizeof(S_C_UPDATE_CONTROL_INFO));
	}
};

struct S_C_UPDATE_SPECIAL_INFO
{
	SpecialUser special_infos_[20];
	S_C_UPDATE_SPECIAL_INFO()
	{
		memset(this,0,sizeof(S_C_UPDATE_SPECIAL_INFO));
	}
};

struct CMD_C_ExchangeFishScore 
{
  bool increase;
};

struct CMD_C_UserFire 
{
	BulletKind bullet_kind;
	float vector_x;
	float vector_y;
	float angle;
	int bullet_mulriple;
	int lock_fishid;
	int bullet_id;
	bool bIsRobot;
	//VectorForward vectorForward;
	
};


struct CMD_C_CatchFish {
  bool bIsCatch;
  WORD chair_id;
  int fish_id;
  BulletKind bullet_kind;
  int bullet_id;
  int bullet_mulriple;
};

struct CMD_C_CatchSweepFish 
{
  WORD chair_id;
  int fish_id;
  int catch_fish_count;
  int catch_fish_id[300];
};

struct CMD_C_HitFishLK
{
	int fish_id;
};

struct CMD_C_StockOperate 
{
	unsigned char operate_code; // 0查询 1 清除 2 增加 3 查询抽水
};

struct CMD_C_UserFilter
{
	DWORD game_id;
	unsigned char operate_code; // 0 黑名单 1 白名单 2 清除
};

struct CMD_C_Fish20Config 
{
	DWORD game_id;
	int catch_count;
	double catch_probability;
};

struct CMD_C_AndroidBulletMul 
{
	WORD chair_id;
	int bullet_id;
	int bullet_mulriple;
	BulletKind bullet_kind;
};

//连锁闪电
struct CMD_C_LineInfo
{
	FPoint	fpStart;
	FPoint	fpEnd;
	float	fChange;
	DWORD	dwTime;
	
	CMD_C_LineInfo()
	{
		memset(this,0,sizeof(CMD_C_LineInfo));
	}
};

#pragma pack()

#endif // CMD_FISH_H_