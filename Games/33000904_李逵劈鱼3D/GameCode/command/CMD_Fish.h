
#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// ������

#define KIND_ID               2010
#define GAME_NAME             TEXT("�����ͷ")
#define GAME_PLAYER           4



#define MAX_BULLET_COUNT		99	//ÿ���˵�����ӵ����Ƹ���
#define MAX_COIN_COUNT			200	//����Ҹ���

const int	G_iFish_Count = 300;	//��Ļ��������ɶ�������

// �򵥵İ汾���
// ����ͻ����и���Ӧ�ø������ֵ.���ֻ�޸�EXE�İ汾���������ݰ�û�޸ĵĻ������¿ͻ�����ûʲô����ģ���ΪEXE�İ汾�ǿ���ֱ�ӱ��޸ĵ�
// ֻҪ������ǰ��EXE�汾,������֮���ٻ���ȥ�������BUG�Ļ����ڻ���û�޸���
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
// ��Ϸ����

/*
// ��λ��
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
	FISH_KIND_1 = 0 ,		//��ţ��
	FISH_KIND_2,			//�̲���
	FISH_KIND_3,			//�Ʋ���
	FISH_KIND_4,			//������
	FISH_KIND_5,			//�Ʊ���
	FISH_KIND_6,			//С����
	FISH_KIND_7,			//С����
	FISH_KIND_8,			//����
	FISH_KIND_9,			//�������
	FISH_KIND_10,			//��������
	FISH_KIND_11,			//����
	FISH_KIND_12,			//������
	FISH_KIND_13,			//������
	FISH_KIND_14,			//��ȸ��
	FISH_KIND_15,			//����
	FISH_KIND_16,			//������
	FISH_KIND_17,			//������
	FISH_KIND_18,			//������
	FISH_KIND_19,			//�𹿰�
	FISH_KIND_20,			//������
 	FISH_KIND_21,			//������
 	FISH_KIND_22,			//����
 	FISH_KIND_23,			//����
// 	FISH_KIND_24,			//ȫ��ը��
// 	FISH_KIND_25,			//��ɫ���
// 	FISH_KIND_26,			//��ɫ���
// 
// 
// 	FISH_KIND_27,			//���Ǵ���Ԫ
// 	FISH_KIND_28,			//ֱ�ߴ���Ԫ
// 	FISH_KIND_29,			//�ĽǴ���ϲ
// 	FISH_KIND_30,			//ֱ�ߴ���ϲ
// 
// 	FISH_KIND_31,			//����--����
// 	FISH_KIND_32,			//�����ӵ���-����
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

//����ָ���� ���ɶ��ٸ���Ҹ���
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

//������������
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

// ���ƿͻ��˵�����������
//const int kFishKindCount[FISH_KIND_COUNT] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

//////////////////////////////////////////////////////////////////////////
// ���������
#define SUB_S_CHOICE_TEAM_NTF				97 //֪ͨѡ����Ӫ
#define SUB_S_CHOICE_TEAM_RES				98 //ѡ����Ӫ���
#define SUB_S_GAME_BEGIN					99 //��Ϸ��ʼ֪ͨ
#define SUB_S_GAME_CONFIG                   100
#define SUB_S_FISH_TRACE                    101//��������Ĺ켣
#define SUB_S_FISH_GROUP_TRACE              102//������Ⱥ�Ĺ켣
#define SUB_S_EXCHANGE_FISHSCORE            103
#define SUB_S_USER_FIRE                     104//������
#define SUB_S_CATCH_FISH                    105
#define SUB_S_BULLET_ION_TIMEOUT            106//�ӵ���ΧʧЧ
#define SUB_S_LOCK_TIMEOUT                  107//������ʧЧʱ��
#define SUB_S_CATCH_SWEEP_FISH              108
#define SUB_S_CATCH_SWEEP_FISH_RESULT       109
#define SUB_S_HIT_FISH_LK                   110
#define SUB_S_SWITCH_SCENE                  111
#define SUB_S_STOCK_OPERATE_RESULT          112
#define SUB_S_SCENE_END                     113

#define SUB_S_CATCH_SWEEP_FISH_RESULT_EX    115//�㲥֪ͨ������Ҳ�������

#define SUB_S_GAMEOUT						114//�����ǿ���ÿͻ����˳�

#define SUB_S_USER_COME	                    120//��ҽ���

#define S_C_UPDATE_CONTROL_INFO_SIG			121//���¿�����Ϣ
#define S_C_UPDATE_SPECIAL_INFO_SIG			122//���¿�����Ϣ
#define S_C_UPDATE_ONLINEUSER_INFO_SIG		123//�������������Ϣ
#define S_C_SWITCH_INFO_SIG					124
#define S_C_UPDATE_FISH_NAME_SIG			125
#define S_C_SUPER_CONFIG_SIG				126
#define S_C_END_UPDATE_ONLINEUSER_INFO_SIG	127

#define S_C_GAME_FINISH_SIG					130//һ����Ϸ����С����
#define S_C_UPDATE_CALCULATE_BOARD_SIG		131//���´����
#define	S_C_UPDATE_REMAIN_INFO_SIG			132//����ʣ�������ʣ��ʱ��

/// ʣ�����������
struct S_C_RemainTime
{
	int remainCount;						//// ʣ�����
	int totalCount;							//// �ܾ���
	int remainTime;                         //// ʣ��ʱ��
	int totalTime;							//// ��ʱ��

	S_C_RemainTime()
	{
		memset(this, 0, sizeof(this));
	}
};

enum GameResult
{
	WIN = 0,	//ʤ��
	LOSE = 1,	//ʧ��
	DEUCE = 2	//ƽ��
};

/// �����
struct S_C_Calculate_Board
{
	SCORE iuserPoint[PLAY_COUNT];           //// ��ҵ÷�
	BYTE  isWin[PLAY_COUNT];				//��Ӯ���

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

/// С����
struct S_C_GameFinish
{
	SCORE iuserPoint[PLAY_COUNT];           //// ��ҵ÷�
	BYTE  isWin[PLAY_COUNT];				//��Ӯ���

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
	BYTE byteamInfo[PLAY_COUNT];		//��Ӫ��Ϣ

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
	BYTE byDeskStation;				//ѡ����Ӫ���
	bool bsuccess;					//ѡ���Ƿ�ɹ�
	BYTE byteamInfo[PLAY_COUNT];	//��Ӫ��Ϣ
};

struct S_C_GAME_BEGIN
{
	SCORE PointsInit[PLAY_COUNT];	//��ʼ����--
	int   ShootInit;	//��ʼ����
};

struct S_C_SUPER_CONFIG
{
	bool bSuperUser;
};

struct CMD_S_GameStatus 
{
  DWORD game_version;
  SceneStyle Scene_Style_Kind_;			//��������ID
  int	iUserFireBase[GAME_PLAYER];		//����ڻ���
  SCORE fish_score[GAME_PLAYER];		//��ҷ���
  SCORE exchange_fish_score[GAME_PLAYER];
  int	iRemainShootCount[GAME_PLAYER];	//���ʣ������
  BYTE  byteamInfo[PLAY_COUNT];			//�����Ӫ	
  int   iPlayMode;						//��Ϸ�淨
  int   iPointsInit[PLAY_COUNT];	//��ʼ����
  int   iShootInit;					    	//��ʼ����
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

struct GameStation//�����ػ��õ����ݰ�sdp
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	BYTE				game_status;//״̬��ʶ

	CMD_S_GameStatus	GameStatus;//״̬����
	CMD_S_GameConfig	GameConfig;//��������

	BYTE GameMessage[MAX_PATH];//��Ϸ��Ϣ
};

//��ҽ���  ��Ҫ�ǰ���ҵĽ�������͹���
struct CMD_S_GameUserCome
{
	DWORD	dwDeskStation;
	__int64 i64fish_score;				//��ҷ���
	int	iRemainShootCount;				//���ʣ������

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
	FishKind fish_kind;			//���������
	FishKind fish_kind_ex[5];	//��ĸ������� 
	int fish_id;
	TraceType trace_type;
	BYTE TraceID;			//�켣ID
	BYTE SceneFish;			//��������

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
  SCORE swap_fish_score;//��ǰ���ʵ������� ����������
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
  int iRemainShootCount;		//���ʣ������
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
  __int64	i64UserFishScore_;	//��ҷ���
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
  __int64	i64UserFishScore_;	//��ҷ���
};

struct CMD_S_CatchSweepFishResultEx 
{
	char    cRoomName[61];//��������
	int		exchange_ratio_userscore_;	//����������û����
	int		exchange_ratio_fishscore_;	//������������
	int		iDeskID;			//����ID
	char	szNickName[61]; //�ǳ�
	int		fish_kind;		//������ID
	SCORE	fish_score;		//������׬����Ǯ
	char    cFishName[MAX_PATH];//�������
};

struct CMD_S_HitFishLK {
  WORD chair_id;
  int fish_id;
  int fish_mulriple;
};

struct CMD_S_SwitchScene 
{
  SceneKind scene_kind;
  SceneStyle Scene_Style_;	//����ID
  int fish_count;
  int fish_id[245];
  FishKind fish_kind[245];
};

struct CMD_S_GroupFishTrace
{
	BYTE byIndex;
	int fish_count;
	FishKind fish_kind[250];
	FishKind fish_red_kind[5];		//������Ǻ�ɫ����
	int fish_id[250];
};


struct CMD_S_StockOperateResult {
  unsigned char operate_code;
  SCORE stock_score;
};

//////////////////////////////////////////////////////////////////////////
// �ͻ�������

//#define SUB_C_EXCHANGE_FISHSCORE            1
#define SUB_C_USER_FIRE                     2//��ҿ���
#define SUB_C_CATCH_FISH                    3
#define SUB_C_CATCH_SWEEP_FISH              4
#define SUB_C_HIT_FISH_I                    5
#define SUB_C_STOCK_OPERATE                 6
#define SUB_C_USER_FILTER                   7
#define SUB_C_ANDROID_STAND_UP              8
#define SUB_C_FISH20_CONFIG                 9
#define SUB_C_ANDROID_BULLET_MUL            10//�������ӵ���Ŀ

#define C_S_UPDATE_SUPER_INFO_SIG			11
#define C_S_ADD_CONTROL_INFO_SIG			12
#define C_S_DELETE_CONTROL_INFO_SIG			13
#define C_S_ADD_SPECIAL_SIG					14
#define C_S_DELETE_SPECIAL_SIG				15
#define C_S_SWITCH_CONTROL_SIG				16
#define C_S_SWITCH_SPECIAL_SIG				17


#define C_S_CLEAR_FISH_SIG					18	//�����

#define SUB_C_USER_CHOICE_TEAM				19 //���ѡ����Ӫ

struct CMD_C_CHOICE_TEAM
{
	BYTE byDeskStation;				//ѡ����Ӫ���
	BYTE bychoiceTeam;				//ѡ�����Ӫ
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
	unsigned char operate_code; // 0��ѯ 1 ��� 2 ���� 3 ��ѯ��ˮ
};

struct CMD_C_UserFilter
{
	DWORD game_id;
	unsigned char operate_code; // 0 ������ 1 ������ 2 ���
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

//��������
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