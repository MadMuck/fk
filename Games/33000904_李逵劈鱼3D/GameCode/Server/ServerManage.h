#pragma once

#include "StdAfx.h"

#include <vector>

struct FishTraceInfo //鱼轨迹信息结构体
{
	FishKind fish_kind;
	FishKind fish_kind_ex_;
	int fish_id;
	DWORD build_tick;
};

struct SweepFishInfo //鱼轨迹信息结构体
{
	FishKind fish_kind;
	FishKind fish_kind_ex_;
	int fish_id;
	BulletKind bullet_kind;
	int bullet_mulriple;
};

struct ServerBulletInfo //服务端子弹信息结构体
{
	bool		bActive;
	int			ibullet_id;
	int			ibullet_mulriple;
	DWORD		dwBirthTime;		//创建时间
	BulletKind	ibullet_kind;

	ServerBulletInfo()
	{
		memset(this,0,sizeof(ServerBulletInfo));
	}
};

struct SuperManInfo//超端用户数据
{
	int iUserID;//用户ID
	int iCellScore;//单位分数
};

struct SpecialUserInfo//特殊用户数据
{
	int iUserID;//用户ID
	double dProbability;//概率基数
};


#define SEND_FISH_NUM 4

static std::vector<DWORD> g_balck_list_;
static std::vector<DWORD> g_white_list_;

struct Fish20Config 
{
	DWORD game_id;
	int catch_count;
	double catch_probability;
};
static std::vector<Fish20Config> g_fish20_config_;

#define G_iBulletIonTimer					20

#define G_iLockTimer						31
#define G_iClearTraceTimer					32		//清理所有鱼计时器
#define G_iBuildSmallFishTraceTimer			33		//小鱼计时器		1-10		
#define G_iBuildMediumFishTraceTimer		34		//中等鱼计时器		11 - 16
#define G_iBuildBigFishTraceTimer			35		//中等鱼计时器		17 - 22
#define G_iBuildFish23TraceTimer			36		//23号鱼计时器	水草
#define G_iBuildFish24TraceTimer			37		//24号鱼计时器	炸弹
#define G_iBuildFishJCTraceTimer			38		//金蟾计时器	
#define G_iBuildFishYWTraceTimer			39		//30-31号鱼计时器	鱼王
#define G_iBuildFishDSYTraceTimer			40		//大三元
#define G_iBuildFishDSXTraceTimer			41		//大三元和大四喜计时器	直线
#define G_iBuildLineFishTraceTimer			42		//直线鱼群计时器

#define G_iBuildCircleFishTraceTimer0		43		//环形鱼群计时器0
#define G_iBuildCircleFishTraceTimer1		44		//环形鱼群计时器1


#define G_iSwitchSceneTimer					45		//鱼阵计时器
#define G_iSceneEndTimer					46		//鱼阵结束计时器
#define G_iGameFinishTimer					47		//游戏结束计时器
#define G_iAutoFireTimer					48		//玩家自动开火计时器
#define G_iCheckFinishTimer					49		//检查是否可以结束定时器


#define AutoFireTime  0.5f					//自动开火间隔时间（秒）


const int G_iFishAliveTime = 60000;	//鱼的生命周期



const int kLKScoreTimer					=		57;
const int kLKScoreElasped				=	1;




static SCORE g_stock_score_ = 0;
static SCORE g_revenue_score = 0;

static const float kFireAngle12[20] = { 4.8073f, 4.9137, 4.95414, 5.13784, 5.386, 5.6318, 5.99, 6.1709, 6.3124, 4.862,
	4.8073f+M_PI_2, 4.9137+M_PI_2, 4.95414+M_PI_2, 5.13784+M_PI_2, 5.386+M_PI_2, 5.6318+M_PI_2, 5.99+M_PI_2, 6.1709+M_PI_2, 6.3124+M_PI_2, 4.862+M_PI_2};
static const float kFireAngle34[20] = { 4.5705f, 4.3526, 4.2376, 4.1574, 4.04445, 3.92, 3.8050, 3.695, 3.5835, 3.3133,
	4.5705f-M_PI_2, 4.3526-M_PI_2, 4.2376-M_PI_2, 4.1574-M_PI_2, 4.04445-M_PI_2, 3.92-M_PI_2, 3.8050-M_PI_2, 3.695-M_PI_2, 3.5835-M_PI_2, 3.3133-M_PI_2};

#define GF_NORMAL					10								/**< 游戏正常结束 */
#define GF_SALE						11								/**< 游戏安全结束 */

static bool		g_special_switch = false;
static bool		g_control_switch = false;

static vector<int>				g_AdminiList;

static vector<ControlInfo>				g_vecControlList;
static vector<SpecialUser>				g_vecSpecialList;



struct UserDataInfo
{
	DWORD		dwUserID;
	__int64		i64UserMoney;
	UserDataInfo()
	{
		memset(this,0,sizeof(UserDataInfo));
	}
};
//#define MAX_USER_COUNT 180
static	vector<UserDataInfo> G_vecUserInfo;

//游戏桌类
class CServerGameDesk : public CGameDesk
{
//函数定义
public:
	//构造函数
	CServerGameDesk(); 
	//析构函数
	virtual ~CServerGameDesk();

	//重载函数
public:
	//初始化游戏逻辑
	virtual bool InitDeskGameStation();
	//游戏开始
	virtual bool GameBegin(BYTE bBeginFlag);
	//游戏结束
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//判断是否正在游戏
	virtual bool IsPlayGame(BYTE bDeskStation);
	///判断此游戏桌是否开始游戏
	virtual BOOL	IsPlayingByGameStation();
	//游戏数据包处理函数
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//拦截框架消息
	virtual	bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);;
	//用户离开游戏桌
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//断线重回
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	///用户重新进入
	virtual  bool UserReCome(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	///用户坐到游戏桌
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);

	//重载纯虚函数
public:
	//获取游戏状态信息
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//重置游戏状态
	virtual bool ReSetGameState(BYTE bLastStation);
	//定时器消息
	virtual bool OnTimer(UINT uTimerID);
	//处理函数
public:
	
	//读取静态变量
	static BOOL LoadIni();	//准备发牌工作
	//读取静态变量
	static BOOL LoadAdminIni();	

	BOOL LoadExtIni(UINT uRoomID);   //根据房间ID加载配置文件

	//清所有计时器
	void KillAllTimer();

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////游戏辅助功能/////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//请求提前结束
	BOOL UserStop(BYTE bDeskStation,BOOL bAgree= false);
	//统计游戏人数
	BYTE CountPlayer();

	//------------------------------------------------------------------------------
	//捕鱼函数----------------------------------------------------------------------
	//------------------------------------------------------------------------------

	//算分
	void CalcScore(BYTE bDeskStation);
	void	RecoderStock();
	//启动所有计时器
	void StartAllGameTimer();
	//设置游戏状态
	void SetGameStatus(BYTE game_status) {m_bGameStation =  game_status;}
	//获取游戏状态
	BYTE GetGameStatus() const { return m_bGameStation; }
	//获得鱼ID
	int GetNewFishID();
	//获得子弹ID
	int GetBulletID(WORD chairid);
	//发送配置信息
	bool SendGameConfig(BYTE bDeskStation, GameStation& cmd);
	//加载配置文件
	bool LoadConfig();

	FishTraceInfo* ActiveFishTrace();
	bool FreeFishTrace(FishTraceInfo* fish_trace_info);
	void FreeAllFishTrace();
	FishTraceInfo* GetFishTraceInfo(int fish_id);


	void SaveSweepFish(FishKind fish_kind, FishKind fish_kind_ex_,int fish_id, BulletKind bullet_kind, int bullet_mulriple);
	bool FreeSweepFish(int fish_id);
	SweepFishInfo* GetSweepFish(int fish_id);

	//清除指定玩家指定子弹信息，并将清除的数据保存到仓库中
	bool	FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info);
	//清理指定玩家所有子弹信息
	void	FreeAllBulletInfo(WORD chairid);
	//清理过时的子弹
	void	FreeTimeOutBullet();
	//获取子弹信息
	ServerBulletInfo* GetBulletInfo(WORD chairid, int bullet_id);

	//计时器-----------------------------------------------------------------
	bool	OnTimerBuildSmallFishTrace();	//小鱼 1-10
	bool	OnTimerBuildMediumFishTrace();	//中型鱼 11-16
	bool	OnTimerBuildBigFishTrace();	//大鱼 17-22
	bool	OnTimerBuildFish23Trace();		//水草
	bool	OnTimerBuildFishBombTrace();	//炸弹
	bool	OnTimerBuildFishJCTrace();		//金蟾
	bool	OnTimerBuildFishYWTrace();		//鱼王
	
	



	//检测是否特殊玩家
	bool	IsSpecialUser(BYTE bDeskStation,double & dProperty);

	bool OnTimerBuildFishLKTrace();



	/*--------------------金币数实时更新------------------*/
	void UpDateRealMoney(DWORD dwUserID,__int64 i64Money);
	void InsertNewUser(DWORD dwUserID,__int64 i64UserMoney);
	void DeleteNullUser(DWORD dwUserID);
	__int64 GetUserRealMoney(DWORD dwUserID);
	/*--------------------金币数实时更新------------------*/

	bool	UpdateControllerMoney(BYTE bDeskStation,__int64 i64ChangeMoney,bool bReset = false);

	bool	ChangeCatchProperty(BYTE bDeskStation,double &fProperty,int fish_kind);

	bool	UpdateSpecialData(BYTE bDeskStation,int fish_kind);
	void	TidySpecialData();
	void	TidyControlData();

	bool	InsertSpecialFish(SpecialUser &TSpecialUser,int fish_kind,int catch_rate,int left_num);
	bool	CheckArrayEnd(int index,int max_lenth);
	void	SendSuperConfig(BYTE bDeskStation);



	bool OnAddSpecial(BYTE bDeskStation,void *pData);
	bool OnDeleteSpecial(BYTE bDeskStation,void *pData);
	bool OnAddControl(BYTE bDeskStation,void *pData);
	bool OnDeleteControl(BYTE bDeskStation,void *pData);
	
	bool FindControlInfo(int user_id_,int &index);
	void UpdateControlInfo(BYTE bDeskStation);


	void UpdateOnLineUserInfo(BYTE bDeskStation);

	void UpdateSwitchInfo(BYTE bDeskStation);

	void UpdateFishName(BYTE bDeskStation);

	void UpdateSpecialInfo(BYTE bDeskStation);

	bool IsSpecialFish(int fish_kind);




	bool OnTimerClearTrace();
	bool OnTimerBulletIonTimeout(WPARAM bind_param);
	bool OnTimerLockTimeout();
	//计时器_转换场景
	bool OnTimerSwitchScene();
	//计时器_转换场景结束
	bool OnTimerSceneEnd();
	bool OnTimerLKScore();

	//收到消息-----------------------------------------------------------------
	//换算捕鱼币
	bool OnSubUserChoiceTeam(BYTE bDeskStation, BYTE choice);
	bool OnSubUserFire(BYTE bDeskStation, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid,int iBullet_id,float vector_x,float vector_y,bool bIsRobot = false,bool bAutoShoot = false);
	bool OnSubCatchFish(BYTE bDeskStation, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul, bool IsCatch);
	bool OnSubCatchSweepFish(BYTE bDeskStation, int fish_id, int* catch_fish_id, int catch_fish_count);
	bool OnSubHitFishLK(BYTE bDeskStation, int fish_id);
	bool OnSubStockOperate(BYTE bDeskStation, unsigned char operate_code);
	bool OnSubUserFilter(BYTE bDeskStation, DWORD game_id, unsigned char operate_code);
	bool OnSubFish20Config(BYTE bDeskStation, DWORD game_id, int catch_count, double catch_probability);

	//事件函数-----------------------------------------------------------------
	//游戏开始事件

	bool OnEventSendGameScene(WORD chair_id, BYTE game_status, GameStation& cmd);

	//鱼阵轨迹-----------------------------------------------------------------
	//创建轨迹
	void BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type);
	//创建N条鱼同一条轨迹
	void BuildInitLineTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type,DWORD &dwChar_id);
	//创建N条鱼的圈轨迹
	void BuildInitCircleTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type,int fish_count,int iIndex);
	

	//
	void BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,FishKind fish_kind_start_ex = FISH_KIND_1, FishKind fish_kind_end_ex =FISH_KIND_12);
	//创建N条鱼的直线鱼轨迹
	void BuildFishLineTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end);
	//创建N条鱼圆形轨迹
	CMD_S_CirleFishGroup	m_TCirleFishGroup[2];
	void BuildFishCircleTrace0(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,bool bHaveRed = false);
	void BuildFishCircleTrace1(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,bool bHaveRed = false);
	//清理轨迹
	void ClearFishTrace(bool force = false);

	//创建场景-----------------------------------------------------------------
	void BuildSceneKind1();
	void BuildSceneKind2();
	void BuildSceneKind3();
	void BuildSceneKind4();
	void BuildSceneKind5();



	//发送消息方法重写-----------------------------------------------------------------
	//发给所有人
	void SendDataToAllPlayers(void * pData, UINT uSize, BYTE bAssID);
	//发给指定玩家
	void SendDataToPlayer(BYTE bDeskStation, void * pData, UINT uSize, BYTE bAssID);

	//是否是超端用户
	//bool IsSuperMan(BYTE bDesk);
	//获取该玩家的单位分数
	int GetSuperCellscore(BYTE bDesk);
	//存在真人
	bool ExistUnAI();

	//字符转换（UTF_8->GB2312）
	void UTF_8ToUnicode(wchar_t* pOut,char *pText);
	void UnicodeToGB2312(char* pOut,wchar_t uData);
	void UTF_8ToGB2312(char*pOut, char *pText, int pLen);
	//修改奖池数据1(平台->游戏)
	/// @param bAIWinAndLostAutoCtrl 机器人控制输赢开关
	/// @return 是否成功
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//修改奖池数据2(平台->游戏)
	/// @param m_iAIWantWinMoney 机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
	/// @param m_iAIWinLuckyAt 机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
	/// @return 是否成功
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//修改奖池数据3(平台->游戏)
	/// @param iReSetAIHaveWinMoney 机器人输赢控制：重置机器人已经赢钱的数目
	/// @return 是否成功
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

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
	virtual void UpDataRoomPond(__int64 iAIHaveWinMoney);


	void SaveSetting();
	bool ExamSuperUser(BYTE bDesk);

	void IALockFUN_(BYTE byDeskStation,int &lock_fish_id_);

	int	 GetLockCount(int fish_id);

	void ClearLockFish(int fish_id_);
private:
	
public:
	bool m_bExitWatcher[PLAY_COUNT];//是否存在旁观玩家 flase为 不存在， true为 存 序号为玩家逻辑ID

	char m_cFishName[FISH_KIND_COUNT][MAX_PATH];

private:
	bool			m_bIsplaying;				//游戏进行中

	SceneKind		next_scene_kind_;			//下一个场景渔震ID
	SceneStyle		Scene_Style_Kind_;			//场景背景ID
	bool			special_scene_;				//特殊场景

	WORD			android_chairid_;			//机器人座位

	int				current_fish_lk_multiple_;	//当前李逵的倍数

	int				fish_id_;					//新鱼序列号
	int				fish_id_LK;					//当前李逵序列号
	int				m_iUserFireBase[GAME_PLAYER];	//玩家炮火倍数
	SCORE			exchange_fish_score_[GAME_PLAYER];//兑换了的分数
//	__int64			m_i64fish_score_[GAME_PLAYER];	//玩家获得的分数
	bool			m_bUserHavePlay[GAME_PLAYER];	//标记玩家是否玩了游戏

	int				m_iBullet_id_[GAME_PLAYER];

	int				m_iCellScore;

	SCORE base_score_;

	int stock_crucial_score_[10];
	double stock_increase_probability_[10];
	int stock_crucial_count_;

	int				exchange_ratio_userscore_;	//换算比例中用户金币
	int				exchange_ratio_fishscore_;	//换算比例中鱼币
	int				exchange_count_;			//一次兑换的数目（配置项）

	int				min_bullet_multiple_;		//子弹最小倍数
	int				max_bullet_multiple_;		//子弹最大倍数
	int				King_bomb_stock_;			//鱼王炸弹库存
	//int				bomb_stock_;				//炸弹库存
	int				super_bomb_stock_;			//超级炸弹库存
	double			fish_speed_[FISH_KIND_COUNT];//各种鱼的速度
	int				fish_multiple_min[FISH_KIND_COUNT];//各种鱼的分数
	int				fish_multiple_max[FISH_KIND_COUNT];//各种鱼的分数
	int				fish23_max_multiple_;//海草最大倍数
	int				fish25_max_multiple_;//金金蟾最大倍数
	int				fish26_max_multiple_;//绿金蟾最大倍数
	int				fish27_max_multiple_;//三角大三元最大倍数
	int				fish28_max_multiple_;//直线大三元最大倍数
	int				fish29_max_multiple_;//四角大四喜最大倍数
	int				fish30_max_multiple_;//直线大四喜最大倍数
	
	
	int				fishLK_max_multiple_;//李逵最大倍数
	int				fish_bounding_box_width_[FISH_KIND_COUNT];//各种鱼边框的宽
	int				fish_bounding_box_height_[FISH_KIND_COUNT];//各种鱼边框的高
	int				fish_hit_radius_[FISH_KIND_COUNT];//各种鱼伤害范围
	double			fish_capture_probability_[FISH_KIND_COUNT];//各种鱼被捕获的概率
	int				bomb_range_width_;//炸弹范围的宽
	int				bomb_range_height_;//炸弹范围的高

	int				bullet_speed_[BULLET_KIND_COUNT];//各种子弹速度
	int				net_radius_[BULLET_KIND_COUNT];//各种子弹产生鱼网的大小
	int				net_bounding_box_width_[BULLET_KIND_COUNT];//各种子弹产生鱼网的宽
	int				net_bounding_box_height_[BULLET_KIND_COUNT];//各种子弹产生鱼网的高

	//鱼的轨迹数据
	typedef std::vector<FishTraceInfo*> FishTraceInfoVecor;
	FishTraceInfoVecor active_fish_trace_vector_;//当前活动的鱼的轨迹
	FishTraceInfoVecor storage_fish_trace_vector_;//鱼的轨迹数据仓库

	std::vector<SweepFishInfo> sweep_fish_info_vector_;

	//服务端的子弹数据
	ServerBulletInfo	m_tServerBulletInfo[PLAY_COUNT][MAX_BULLET_COUNT];

	//当前总鱼数
	int m_iFishNum;



	//超端用户数据
	//std::vector<SuperManInfo> m_SuperMan;
	//特殊用户数据
	std::vector<SpecialUserInfo> m_vSpecialUser;

	int		m_iBuildSmallFishTime;		//小鱼出现时长0
	int		m_iBuildMediumFishTime;		//中等鱼出现时长1
	int		m_iBuildBigFishTime;		//大鱼出现的时长
	int		m_iBuild23FishTime;			//水草的时长
	int		m_iBuild24FishTime;			//炸弹
	int		m_iBuildJCFishTime;			//金蟾
	int		m_iBuildYWFishTime;			//鱼王
	int		m_iBuildDSYFishTime;		//大三元
	int		m_iBuildDSXFishTime;		//大四喜

	int		m_iSwitchSceneTime;			//鱼阵时长
	int		m_iClearTraceElasped;
	int		m_iSceneEndTime;			//鱼阵结束时间
	int		m_iLockTime ;				//定屏时长
	int		m_iBulletIonTime;			//超级炮弹的时间长

	int		m_iLockFish[PLAY_COUNT];

	int		m_iSmallFishCatchRate;
	int		m_iSmallFishKind;

	int		m_iPointsInit;							//初始积分
	int		m_iShootInit;							//初始炮数

	int		m_iGameTime;							//游戏时间/分钟
	int		m_iPlayingMethod;						//赛制 & 结算比例   1<<1 1V1, 1<<2 2V2, 1<<3 四人对战, 1<<4 1:1, 1<<5 1:10

	std::map<int,int> userPoint;	//玩家分数
	std::map<int,int> userShoot;	//玩家炮数
	std::map<int,int> userTeam;		//玩家阵营
	std::map<int,int> teamNum;		//阵营人数（key阵营，value玩家数）
	S_C_Calculate_Board m_CalculateBoard;	//大结算数据
	DWORD m_lStartTime;				// 开始时间
	std::map<int,int> userAutoFireId;	//玩家自动开火子弹ID
	std::map<int,float> userLeftTime;		//已离线玩家的离线时间

	void InitGameRule();			//设置游戏规则

	int GetCurPlayerCount();		//获取当前桌子有多少玩家

	void SendReaminInfo(bool isFinish = false);			//更新剩余局数剩余时间

	void SendScoreBoard();			//发送大结算

	void DissMissRoomBefore();		//提前解散房间

	void AutoFire(BYTE bDeskStation);			//玩家离开或断线自动射击

	bool CanBeginGame();

	bool CheckFinish();
};

/******************************************************************************************************/
