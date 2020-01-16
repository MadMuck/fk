#pragma once

#include "StdAfx.h"

#include <vector>

struct FishTraceInfo //��켣��Ϣ�ṹ��
{
	FishKind fish_kind;
	FishKind fish_kind_ex_;
	int fish_id;
	DWORD build_tick;
};

struct SweepFishInfo //��켣��Ϣ�ṹ��
{
	FishKind fish_kind;
	FishKind fish_kind_ex_;
	int fish_id;
	BulletKind bullet_kind;
	int bullet_mulriple;
};

struct ServerBulletInfo //������ӵ���Ϣ�ṹ��
{
	bool		bActive;
	int			ibullet_id;
	int			ibullet_mulriple;
	DWORD		dwBirthTime;		//����ʱ��
	BulletKind	ibullet_kind;

	ServerBulletInfo()
	{
		memset(this,0,sizeof(ServerBulletInfo));
	}
};

struct SuperManInfo//�����û�����
{
	int iUserID;//�û�ID
	int iCellScore;//��λ����
};

struct SpecialUserInfo//�����û�����
{
	int iUserID;//�û�ID
	double dProbability;//���ʻ���
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
#define G_iClearTraceTimer					32		//�����������ʱ��
#define G_iBuildSmallFishTraceTimer			33		//С���ʱ��		1-10		
#define G_iBuildMediumFishTraceTimer		34		//�е����ʱ��		11 - 16
#define G_iBuildBigFishTraceTimer			35		//�е����ʱ��		17 - 22
#define G_iBuildFish23TraceTimer			36		//23�����ʱ��	ˮ��
#define G_iBuildFish24TraceTimer			37		//24�����ʱ��	ը��
#define G_iBuildFishJCTraceTimer			38		//����ʱ��	
#define G_iBuildFishYWTraceTimer			39		//30-31�����ʱ��	����
#define G_iBuildFishDSYTraceTimer			40		//����Ԫ
#define G_iBuildFishDSXTraceTimer			41		//����Ԫ�ʹ���ϲ��ʱ��	ֱ��
#define G_iBuildLineFishTraceTimer			42		//ֱ����Ⱥ��ʱ��

#define G_iBuildCircleFishTraceTimer0		43		//������Ⱥ��ʱ��0
#define G_iBuildCircleFishTraceTimer1		44		//������Ⱥ��ʱ��1


#define G_iSwitchSceneTimer					45		//�����ʱ��
#define G_iSceneEndTimer					46		//���������ʱ��
#define G_iGameFinishTimer					47		//��Ϸ������ʱ��
#define G_iAutoFireTimer					48		//����Զ������ʱ��
#define G_iCheckFinishTimer					49		//����Ƿ���Խ�����ʱ��


#define AutoFireTime  0.5f					//�Զ�������ʱ�䣨�룩


const int G_iFishAliveTime = 60000;	//�����������



const int kLKScoreTimer					=		57;
const int kLKScoreElasped				=	1;




static SCORE g_stock_score_ = 0;
static SCORE g_revenue_score = 0;

static const float kFireAngle12[20] = { 4.8073f, 4.9137, 4.95414, 5.13784, 5.386, 5.6318, 5.99, 6.1709, 6.3124, 4.862,
	4.8073f+M_PI_2, 4.9137+M_PI_2, 4.95414+M_PI_2, 5.13784+M_PI_2, 5.386+M_PI_2, 5.6318+M_PI_2, 5.99+M_PI_2, 6.1709+M_PI_2, 6.3124+M_PI_2, 4.862+M_PI_2};
static const float kFireAngle34[20] = { 4.5705f, 4.3526, 4.2376, 4.1574, 4.04445, 3.92, 3.8050, 3.695, 3.5835, 3.3133,
	4.5705f-M_PI_2, 4.3526-M_PI_2, 4.2376-M_PI_2, 4.1574-M_PI_2, 4.04445-M_PI_2, 3.92-M_PI_2, 3.8050-M_PI_2, 3.695-M_PI_2, 3.5835-M_PI_2, 3.3133-M_PI_2};

#define GF_NORMAL					10								/**< ��Ϸ�������� */
#define GF_SALE						11								/**< ��Ϸ��ȫ���� */

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

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
//��������
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation();
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	///�жϴ���Ϸ���Ƿ�ʼ��Ϸ
	virtual BOOL	IsPlayingByGameStation();
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//���ؿ����Ϣ
	virtual	bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);;
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//�����ػ�
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	///�û����½���
	virtual  bool UserReCome(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	///�û�������Ϸ��
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
public:
	
	//��ȡ��̬����
	static BOOL LoadIni();	//׼�����ƹ���
	//��ȡ��̬����
	static BOOL LoadAdminIni();	

	BOOL LoadExtIni(UINT uRoomID);   //���ݷ���ID���������ļ�

	//�����м�ʱ��
	void KillAllTimer();

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////��Ϸ��������/////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//������ǰ����
	BOOL UserStop(BYTE bDeskStation,BOOL bAgree= false);
	//ͳ����Ϸ����
	BYTE CountPlayer();

	//------------------------------------------------------------------------------
	//���㺯��----------------------------------------------------------------------
	//------------------------------------------------------------------------------

	//���
	void CalcScore(BYTE bDeskStation);
	void	RecoderStock();
	//�������м�ʱ��
	void StartAllGameTimer();
	//������Ϸ״̬
	void SetGameStatus(BYTE game_status) {m_bGameStation =  game_status;}
	//��ȡ��Ϸ״̬
	BYTE GetGameStatus() const { return m_bGameStation; }
	//�����ID
	int GetNewFishID();
	//����ӵ�ID
	int GetBulletID(WORD chairid);
	//����������Ϣ
	bool SendGameConfig(BYTE bDeskStation, GameStation& cmd);
	//���������ļ�
	bool LoadConfig();

	FishTraceInfo* ActiveFishTrace();
	bool FreeFishTrace(FishTraceInfo* fish_trace_info);
	void FreeAllFishTrace();
	FishTraceInfo* GetFishTraceInfo(int fish_id);


	void SaveSweepFish(FishKind fish_kind, FishKind fish_kind_ex_,int fish_id, BulletKind bullet_kind, int bullet_mulriple);
	bool FreeSweepFish(int fish_id);
	SweepFishInfo* GetSweepFish(int fish_id);

	//���ָ�����ָ���ӵ���Ϣ��������������ݱ��浽�ֿ���
	bool	FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info);
	//����ָ����������ӵ���Ϣ
	void	FreeAllBulletInfo(WORD chairid);
	//�����ʱ���ӵ�
	void	FreeTimeOutBullet();
	//��ȡ�ӵ���Ϣ
	ServerBulletInfo* GetBulletInfo(WORD chairid, int bullet_id);

	//��ʱ��-----------------------------------------------------------------
	bool	OnTimerBuildSmallFishTrace();	//С�� 1-10
	bool	OnTimerBuildMediumFishTrace();	//������ 11-16
	bool	OnTimerBuildBigFishTrace();	//���� 17-22
	bool	OnTimerBuildFish23Trace();		//ˮ��
	bool	OnTimerBuildFishBombTrace();	//ը��
	bool	OnTimerBuildFishJCTrace();		//���
	bool	OnTimerBuildFishYWTrace();		//����
	
	



	//����Ƿ��������
	bool	IsSpecialUser(BYTE bDeskStation,double & dProperty);

	bool OnTimerBuildFishLKTrace();



	/*--------------------�����ʵʱ����------------------*/
	void UpDateRealMoney(DWORD dwUserID,__int64 i64Money);
	void InsertNewUser(DWORD dwUserID,__int64 i64UserMoney);
	void DeleteNullUser(DWORD dwUserID);
	__int64 GetUserRealMoney(DWORD dwUserID);
	/*--------------------�����ʵʱ����------------------*/

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
	//��ʱ��_ת������
	bool OnTimerSwitchScene();
	//��ʱ��_ת����������
	bool OnTimerSceneEnd();
	bool OnTimerLKScore();

	//�յ���Ϣ-----------------------------------------------------------------
	//���㲶���
	bool OnSubUserChoiceTeam(BYTE bDeskStation, BYTE choice);
	bool OnSubUserFire(BYTE bDeskStation, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid,int iBullet_id,float vector_x,float vector_y,bool bIsRobot = false,bool bAutoShoot = false);
	bool OnSubCatchFish(BYTE bDeskStation, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul, bool IsCatch);
	bool OnSubCatchSweepFish(BYTE bDeskStation, int fish_id, int* catch_fish_id, int catch_fish_count);
	bool OnSubHitFishLK(BYTE bDeskStation, int fish_id);
	bool OnSubStockOperate(BYTE bDeskStation, unsigned char operate_code);
	bool OnSubUserFilter(BYTE bDeskStation, DWORD game_id, unsigned char operate_code);
	bool OnSubFish20Config(BYTE bDeskStation, DWORD game_id, int catch_count, double catch_probability);

	//�¼�����-----------------------------------------------------------------
	//��Ϸ��ʼ�¼�

	bool OnEventSendGameScene(WORD chair_id, BYTE game_status, GameStation& cmd);

	//����켣-----------------------------------------------------------------
	//�����켣
	void BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type);
	//����N����ͬһ���켣
	void BuildInitLineTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type,DWORD &dwChar_id);
	//����N�����Ȧ�켣
	void BuildInitCircleTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type,int fish_count,int iIndex);
	

	//
	void BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,FishKind fish_kind_start_ex = FISH_KIND_1, FishKind fish_kind_end_ex =FISH_KIND_12);
	//����N�����ֱ����켣
	void BuildFishLineTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end);
	//����N����Բ�ι켣
	CMD_S_CirleFishGroup	m_TCirleFishGroup[2];
	void BuildFishCircleTrace0(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,bool bHaveRed = false);
	void BuildFishCircleTrace1(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,bool bHaveRed = false);
	//����켣
	void ClearFishTrace(bool force = false);

	//��������-----------------------------------------------------------------
	void BuildSceneKind1();
	void BuildSceneKind2();
	void BuildSceneKind3();
	void BuildSceneKind4();
	void BuildSceneKind5();



	//������Ϣ������д-----------------------------------------------------------------
	//����������
	void SendDataToAllPlayers(void * pData, UINT uSize, BYTE bAssID);
	//����ָ�����
	void SendDataToPlayer(BYTE bDeskStation, void * pData, UINT uSize, BYTE bAssID);

	//�Ƿ��ǳ����û�
	//bool IsSuperMan(BYTE bDesk);
	//��ȡ����ҵĵ�λ����
	int GetSuperCellscore(BYTE bDesk);
	//��������
	bool ExistUnAI();

	//�ַ�ת����UTF_8->GB2312��
	void UTF_8ToUnicode(wchar_t* pOut,char *pText);
	void UnicodeToGB2312(char* pOut,wchar_t uData);
	void UTF_8ToGB2312(char*pOut, char *pText, int pLen);
	//�޸Ľ�������1(ƽ̨->��Ϸ)
	/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//�޸Ľ�������2(ƽ̨->��Ϸ)
	/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
	/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//�޸Ľ�������3(ƽ̨->��Ϸ)
	/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

	//����ʤ��
	/// �ж�
	virtual BOOL Judge();
	/// ʤ
	virtual BOOL JudgeWiner();
	/// ��
	virtual BOOL JudgeLoser();
	//GRM����
	//���½�������(��Ϸ->ƽ̨)
	/// @return ������ӮǮ��
	virtual void UpDataRoomPond(__int64 iAIHaveWinMoney);


	void SaveSetting();
	bool ExamSuperUser(BYTE bDesk);

	void IALockFUN_(BYTE byDeskStation,int &lock_fish_id_);

	int	 GetLockCount(int fish_id);

	void ClearLockFish(int fish_id_);
private:
	
public:
	bool m_bExitWatcher[PLAY_COUNT];//�Ƿ�����Թ���� flaseΪ �����ڣ� trueΪ �� ���Ϊ����߼�ID

	char m_cFishName[FISH_KIND_COUNT][MAX_PATH];

private:
	bool			m_bIsplaying;				//��Ϸ������

	SceneKind		next_scene_kind_;			//��һ����������ID
	SceneStyle		Scene_Style_Kind_;			//��������ID
	bool			special_scene_;				//���ⳡ��

	WORD			android_chairid_;			//��������λ

	int				current_fish_lk_multiple_;	//��ǰ���ӵı���

	int				fish_id_;					//�������к�
	int				fish_id_LK;					//��ǰ�������к�
	int				m_iUserFireBase[GAME_PLAYER];	//����ڻ���
	SCORE			exchange_fish_score_[GAME_PLAYER];//�һ��˵ķ���
//	__int64			m_i64fish_score_[GAME_PLAYER];	//��һ�õķ���
	bool			m_bUserHavePlay[GAME_PLAYER];	//�������Ƿ�������Ϸ

	int				m_iBullet_id_[GAME_PLAYER];

	int				m_iCellScore;

	SCORE base_score_;

	int stock_crucial_score_[10];
	double stock_increase_probability_[10];
	int stock_crucial_count_;

	int				exchange_ratio_userscore_;	//����������û����
	int				exchange_ratio_fishscore_;	//������������
	int				exchange_count_;			//һ�ζһ�����Ŀ�������

	int				min_bullet_multiple_;		//�ӵ���С����
	int				max_bullet_multiple_;		//�ӵ������
	int				King_bomb_stock_;			//����ը�����
	//int				bomb_stock_;				//ը�����
	int				super_bomb_stock_;			//����ը�����
	double			fish_speed_[FISH_KIND_COUNT];//��������ٶ�
	int				fish_multiple_min[FISH_KIND_COUNT];//������ķ���
	int				fish_multiple_max[FISH_KIND_COUNT];//������ķ���
	int				fish23_max_multiple_;//���������
	int				fish25_max_multiple_;//���������
	int				fish26_max_multiple_;//�̽�������
	int				fish27_max_multiple_;//���Ǵ���Ԫ�����
	int				fish28_max_multiple_;//ֱ�ߴ���Ԫ�����
	int				fish29_max_multiple_;//�ĽǴ���ϲ�����
	int				fish30_max_multiple_;//ֱ�ߴ���ϲ�����
	
	
	int				fishLK_max_multiple_;//���������
	int				fish_bounding_box_width_[FISH_KIND_COUNT];//������߿�Ŀ�
	int				fish_bounding_box_height_[FISH_KIND_COUNT];//������߿�ĸ�
	int				fish_hit_radius_[FISH_KIND_COUNT];//�������˺���Χ
	double			fish_capture_probability_[FISH_KIND_COUNT];//�����㱻����ĸ���
	int				bomb_range_width_;//ը����Χ�Ŀ�
	int				bomb_range_height_;//ը����Χ�ĸ�

	int				bullet_speed_[BULLET_KIND_COUNT];//�����ӵ��ٶ�
	int				net_radius_[BULLET_KIND_COUNT];//�����ӵ����������Ĵ�С
	int				net_bounding_box_width_[BULLET_KIND_COUNT];//�����ӵ����������Ŀ�
	int				net_bounding_box_height_[BULLET_KIND_COUNT];//�����ӵ����������ĸ�

	//��Ĺ켣����
	typedef std::vector<FishTraceInfo*> FishTraceInfoVecor;
	FishTraceInfoVecor active_fish_trace_vector_;//��ǰ�����Ĺ켣
	FishTraceInfoVecor storage_fish_trace_vector_;//��Ĺ켣���ݲֿ�

	std::vector<SweepFishInfo> sweep_fish_info_vector_;

	//����˵��ӵ�����
	ServerBulletInfo	m_tServerBulletInfo[PLAY_COUNT][MAX_BULLET_COUNT];

	//��ǰ������
	int m_iFishNum;



	//�����û�����
	//std::vector<SuperManInfo> m_SuperMan;
	//�����û�����
	std::vector<SpecialUserInfo> m_vSpecialUser;

	int		m_iBuildSmallFishTime;		//С�����ʱ��0
	int		m_iBuildMediumFishTime;		//�е������ʱ��1
	int		m_iBuildBigFishTime;		//������ֵ�ʱ��
	int		m_iBuild23FishTime;			//ˮ�ݵ�ʱ��
	int		m_iBuild24FishTime;			//ը��
	int		m_iBuildJCFishTime;			//���
	int		m_iBuildYWFishTime;			//����
	int		m_iBuildDSYFishTime;		//����Ԫ
	int		m_iBuildDSXFishTime;		//����ϲ

	int		m_iSwitchSceneTime;			//����ʱ��
	int		m_iClearTraceElasped;
	int		m_iSceneEndTime;			//�������ʱ��
	int		m_iLockTime ;				//����ʱ��
	int		m_iBulletIonTime;			//�����ڵ���ʱ�䳤

	int		m_iLockFish[PLAY_COUNT];

	int		m_iSmallFishCatchRate;
	int		m_iSmallFishKind;

	int		m_iPointsInit;							//��ʼ����
	int		m_iShootInit;							//��ʼ����

	int		m_iGameTime;							//��Ϸʱ��/����
	int		m_iPlayingMethod;						//���� & �������   1<<1 1V1, 1<<2 2V2, 1<<3 ���˶�ս, 1<<4 1:1, 1<<5 1:10

	std::map<int,int> userPoint;	//��ҷ���
	std::map<int,int> userShoot;	//�������
	std::map<int,int> userTeam;		//�����Ӫ
	std::map<int,int> teamNum;		//��Ӫ������key��Ӫ��value�������
	S_C_Calculate_Board m_CalculateBoard;	//���������
	DWORD m_lStartTime;				// ��ʼʱ��
	std::map<int,int> userAutoFireId;	//����Զ������ӵ�ID
	std::map<int,float> userLeftTime;		//��������ҵ�����ʱ��

	void InitGameRule();			//������Ϸ����

	int GetCurPlayerCount();		//��ȡ��ǰ�����ж������

	void SendReaminInfo(bool isFinish = false);			//����ʣ�����ʣ��ʱ��

	void SendScoreBoard();			//���ʹ����

	void DissMissRoomBefore();		//��ǰ��ɢ����

	void AutoFire(BYTE bDeskStation);			//����뿪������Զ����

	bool CanBeginGame();

	bool CheckFinish();
};

/******************************************************************************************************/
