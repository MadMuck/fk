#pragma once
//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//�汾����
#define GAME_MAX_VER					1					//������߰汾
#define GAME_LESS_VER					1					//������Ͱ汾
#define GAME_CHANGE_VER					0					//�޸İ汾
//�ļ����ֶ���
#define GAMENAME						TEXT("��Ϻз")
//���� ID
#define NAME_ID							31009000								
//��Ϸ��Ϣ
#define PLAY_COUNT						11					//��Ϸ����
#define	BET_ARES						47					//��ע������
#define BETSTYLE						5					//��������
#define RECORD_COUNT					20					//��󿪽���¼
#define BET_INFO_MAX					10					//���η�����ע��Ϣ��󳤶�
#define NUO_COUNT						3					//������ҵ������Ų����
//��ע��������
static	const	int		G_iArePeiLv[BET_ARES] = {5,5,5,5,5,5,6,6,6,6,6,6,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

//static  const   int     G_iNoToPoint[8]={1,2,3,6,6,5,4,1};

//1�� 2��« 3�� 4з 5Ϻ 6��
static  const   int     G_iNoToPoint[6]={1,2,3,4,5,6};			//��ѹ,��ע����0~5

static  const   int     G_iBaoZiPoint[6]={1,2,3,4,5,6};			//��ѹ����,��ע����6~11

//��ѡ2, C 6 2 = 15, ��ע����12~26
static  const   int     G_iChooseTwo[15][2]={{1,2},{1,3},{1,4},{1,5},{1,6},{2,3},{2,4},{2,5},{2,6},{3,4},{3,5},{3,6},{4,5},{4,6},{5,6}};		

//��ѡ3, C 6 3 = 20����ע����27~46
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

//��Ϸ״̬
#define	GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define	GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define	GS_ROB_NT						19				//��ׯ�׶�
#define	GS_FREE_STATUS					20				//��Ϸ���н׶�
#define	GS_BET_STATUS					21				//��Ϸ��ע�׶�
#define GS_ROLL_STATUS                  22              //��Ϸҡ���׶�
#define	GS_SEND_STATUS					23				//���ƽ׶�
#define	GS_WAIT_NEXT					24				//�ȴ���һ�̿�ʼ�׶�

//��ϢID
#define ASS_GAME_FREE					53				//������Ϣ					BeginFreeMsg
#define ASS_BEGIN_BET					54				//��ʼ��ע					BeginBetMsg
#define ASS_ROLL_DICE                   55              //��ʼҡ��					RollDiceMsg
#define ASS_SHOW_DICE					56				//��ʼ����					BeginSendCardMsg
#define ASS_UPDATE_BET					57				//������ע					TUpdateHistoryBet(��Ϊgamestation�Ĳ�����Ϣ)
#define ASS_APPLY_NT					58				//������ׯ��Ϣ				ApplyNtMsg
#define ASS_APPLY_RESULT				59				//������ׯ�����Ϣ			ApplyNtResultMsg
#define ASS_APPLY_LEAVE					60				//������ׯ��Ϣ				ApplyLiveNtMsg
#define ASS_LEAVE_RESULT				61				//������ׯ�����Ϣ			ApplyLiveNtResultMsg
#define ASS_USER_BET					62				//�����ע��Ϣ				UserBetMsg
#define ASS_BET_RESULT					63				//�����ע�����Ϣ			UserBetResultMsg
#define ASS_GAME_OPEN                   64              //��������					BeginAward
#define ASS_SUPER_RESULT				65				//�����û����ƽ����Ϣ��	SuperUserControlReslut,�ĵ�û��˵��,�ݲ�ʹ��
#define ASS_SUPER_SEND					66				//�����û���ͷ��˷��ͽ��	SuperUserControlStart,�ĵ�û��˵��,�ݲ�ʹ��
#define ASS_SUPER_USER					67				//�����û���Ϣ				SuperUserState,�ĵ�û��˵��,�ݲ�ʹ��

#define ASS_WAITE_NT		            69              //��ׯ�ȴ���Ϣ				WaiteNtMsg
#define ASS_CONTINUE_BET                70              //��ѹ��Ϣ					CS_ContinueBetMsg,�ĵ�û��˵��,�ݲ�ʹ��
#define ASS_CONTINUE_BET_RESULT         71              //��ѹ���					CS_ContinueBetResultMsg,�ĵ�û��˵��,�ݲ�ʹ��
#define ASS_CLEAR_BET                   72              //�����Ϣ					��ʹ��
#define ASS_CLEAR_BET_RESULT            73              //��ս��					��ʹ��

#define S_C_AWARD_RECORD_NTF            74              //��ʷ������¼				S_C_AwardRecord
#define S_C_ROB_NT_NTF					75				//��ׯ֪ͨ					NULL	
#define C_S_BET_INFO_REQ				76				//�����ע��Ϣ����			TMSG_BET_INFO_REQ
#define S_C_BET_INFO_RSP				77				//�����ע��Ϣ�ظ�			TMSG_BET_INFO_RSP	
#define C_S_CHANGE_BET_REQ				78				//Ųע����					TMSG_CHANGE_BET_REQ
#define S_C_CHANGE_BET_RSP				79				//Ųע�ظ�					TMSG_CHANGE_BET_RSP
#define S_C_UPDATE_BET_NTF				80				//������ע��Ϣ				TMSG_UPDATE_BET_NTF
#define S_C_GAME_COUNT_NTF				81				//ʣ�����֪ͨ				TMSG_GAME_COUNT_NTF
#define S_C_CALCULATE_BOARD				82				//�����					TMSG_CALCULATE_BOARD	

#define S_C_NUO_INFO_RSP				83				//���Ų��Ϣ�ظ�			TMSG_NUO_INFO_RSP	

#pragma pack(1)

//������¼�ṹ��
struct STruOpenAward
{
	BYTE byWinArea[3];			//���ֿ�������

	STruOpenAward()
	{
		memset(byWinArea, 255, sizeof(byWinArea));
	}
};

//������¼��Ϣ��
struct S_C_AwardRecord
{
	STruOpenAward stAward[RECORD_COUNT];          //��ʷ������¼

	S_C_AwardRecord()
	{
		memset(stAward, 255, sizeof(stAward));
	}
};

//������ׯ��Ϣ
struct	ApplyNtMsg
{
	BYTE	byDeskStation;		//������λ��
	ApplyNtMsg()
	{
		byDeskStation = 255;
	}
};

//�����û����ƵĽ��
struct	SuperUserControlReslut
{
	BYTE	bSuperDeskStation;	//�����û���λ��
	BYTE    bySuperReslut[3];	//�����û����ƵĽ��
	BYTE    bShowRobotBet;		//�����û����ƻ�������ע��ʾ
	SuperUserControlReslut()
	{
		bSuperDeskStation = 255;
		memset(bySuperReslut, 255,sizeof(bySuperReslut));
		bShowRobotBet = 0;
	}
};

//�����û���ͷ��˷��ͽ��
struct	SuperUserControlStart
{
	BYTE	byDeskStation;		//�����û���λ��
	BYTE    bySuperStart[3];	//�����û����ƵĽ��
	SuperUserControlStart()
	{
		byDeskStation = 255;
		memset(bySuperStart, 255,sizeof(bySuperStart));
	}
};

// ֪ͨ�ͻ����Ƿ�Ϊ�����ͻ���״̬��Ϣ�ṹ
struct SuperUserState
{
	BYTE byDeskStation;      /**< ���λ�� */
	bool bEnable;           /**< �Ƿ�ͨ */
	SuperUserState()
	{
		memset(this,0,sizeof(SuperUserState));
	}
};

//-------------------------------------------------------------------------------
//��ׯ�ȴ�
struct WaiteNtMsg
{
	bool	bWait;
	WaiteNtMsg()
	{
		memset(this,0,sizeof(WaiteNtMsg));
	}
};
//֪ͨ��ע��Ϣ��
struct BeginBetMsg
{
	BYTE	byNtStation;	//ׯ��λ��
	BYTE    byNtCount;     //ׯ����ׯ����
	BYTE    byGameStation;//��Ϸ״̬
	BYTE    byNtWaitList[PLAY_COUNT];//ׯ���б�
	long long i64NtScore;//ׯ�ҳɼ�
	BeginBetMsg()
	{
		memset(this,0,sizeof(BeginBetMsg));
		memset(byNtWaitList,255,sizeof(byNtWaitList));
		byNtStation=255;
	}
};

//��ѹ��Ϣ��
struct CS_ContinueBetMsg
{
	BYTE byDeskStation;
	CS_ContinueBetMsg()
	{
		byDeskStation=255;
	}
};

//��ѹ���(����ע)
struct	CS_ContinueBetResultMsg
{
    bool          bFlag;               //�Ƿ�����ѹ��ʶ
	BYTE	      byDeskStation;		//��ע��λ��
	CS_ContinueBetResultMsg()
	{
		byDeskStation = 255;
		memset(this,0,sizeof(CS_ContinueBetResultMsg));
	}
};

//�����Ϣ��
struct CS_CleartBetMsg
{
	BYTE byDeskStation;
	CS_CleartBetMsg()
	{
		byDeskStation = 255;
	}
};
//���Ѻע���
struct SC_ClearBetResultMsg
{
	BYTE	      byDeskStation;		//��ע��λ��
	long long     i64SumAllBet;       //�������ע
	long long     i64UserAllBet;      //��ǰ�������ע
	long long     i64BeforeClearBet;  //���֮ǰ������ע
	long long     i64UserBetCount[BET_ARES]; //�м���ÿ���������ע���
	long long	  i64AreaBetAllSum[BET_ARES];		//�����������ע
	SC_ClearBetResultMsg()
	{
		memset(this,0,sizeof(i64AreaBetAllSum));
	}
};
//������Ϣ��
struct BeginAward
{
	long long     i64PlayerGrade[PLAY_COUNT];//��ҳɼ�
	long long     i64PlayCoin[PLAY_COUNT];//�����Ӯ���
    long long     i64AreaGain[BET_ARES];//�����ÿ���������Ӯ���(�����flog)
	long long     i64ReturnMoney[PLAY_COUNT];//�����Ľ���� 
	BeginAward()
	{
		memset(this,0,sizeof(BeginAward));
	}
};

//������Ϣ��
struct BeginSendCardMsg
{
	BYTE byGameStation;				//��Ϸ״̬
	BYTE byWinPrize[3];				//����λ��flog

	BeginSendCardMsg()
	{
		byGameStation = 0;
		memset(byWinPrize, 255, sizeof(byWinPrize));//flog
	}
};

//ҡ����Ϣ��
struct RollDiceMsg
{
	BYTE  byGameStation;
	RollDiceMsg()
	{
		byGameStation = 255;
	}
};

//������ʷ��ע
struct TUpdateHistoryBet
{
	long long	i64AreaBetAllSum[BET_ARES];	//������Ҹ����������ע
	long long	i64UserBetSum[BET_ARES];	//��ǰ��Ҹ����������ע
	long long	i64SumAllBet;				//�����������ע��
	long long   i64UserAllBet;			    //��ǰ�������ע
	int			iChouMaCount[6][BETSTYLE];	//ÿ��������ע��������ֻ��ʾ6����ѹ����

	TUpdateHistoryBet()
	{
		memset(this,0,sizeof(i64AreaBetAllSum));
	}
};

//������Ϣ��
struct BeginFreeMsg
{
	bool	bFlag;			//���б�ʶ
	BYTE	byGameStation ;         ///��ǰ����Ϸ״̬
	BYTE	byFreeTime;		//����ʱ��
	BYTE    byNtStation;//ׯ��λ��
	BYTE    byNtcount;//ׯ����ׯ����
	BYTE    byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	long long i64NtScore;//ׯ�ҳɼ�
	BeginFreeMsg()
	{
		memset(this,0,sizeof(BeginFreeMsg));
		byNtStation = 255;
		memset(byWaitNtList, 255, sizeof(byWaitNtList));
	}
};

//������ׯ�����Ϣ
struct	ApplyNtResultMsg
{
	bool    bNtFlag;			//��û��ׯ�ҵ��������ׯ�ı�־
	bool	bApplay;			//�Ƿ���Ϊ���û�дﵽ��ׯ��Ҷ���ׯ���ɹ�
	BYTE	byDeskStation;		//������λ��
	BYTE	byWaitNtList[PLAY_COUNT];	//������ׯ�б�
	BYTE	byNtStation;		//ׯ�ҵ�λ��
	BYTE    byNtcount;//ׯ����ׯ����
	BYTE    bGameStation;
	long long i64NtScore;//ׯ�ҳɼ�
	ApplyNtResultMsg()
	{
		memset(this,0,sizeof(ApplyNtResultMsg));
		memset(byWaitNtList,255,sizeof(byWaitNtList));
		byNtStation=255;
		bApplay=false;
		bNtFlag=false;
	}
};

//������ׯ��Ϣ��
struct	ApplyLiveNtMsg
{
	BYTE	byDeskStation;		//������λ��
	ApplyLiveNtMsg()
	{
		byDeskStation = 255;
	}
};

//������ׯ�����Ϣ��
struct	ApplyLiveNtResultMsg
{
	BYTE    byNtStation;//ׯ��λ��
	BYTE	byDeskStation;		//������λ��
	BYTE    byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	BYTE    byNtcount;//ׯ����ׯ����
	long long i64NtScore;//ׯ�ҳɼ�
	ApplyLiveNtResultMsg()
	{
		memset(this, 0, sizeof(ApplyLiveNtResultMsg));
		memset(byWaitNtList,255,sizeof(byWaitNtList));
		byNtStation=255;
		byDeskStation=255;
	}
};

//�����ע
struct	UserBetMsg
{
	BYTE	byDeskStation;		//��ע��λ��
	BYTE	byBetArea;			//��ע����
	BYTE	byChouMaType;		//��ע��������
	UserBetMsg()
	{
		memset(this,0,sizeof(UserBetMsg));
	}
};

//��ѡ�淨��־λ
enum PM_FLAGS
{
	None = 0,
	PM_MEM_CHUAN = 1,			//�Ŵ�
	PM_BAO_ZI = 1 << 1,		//����
	PM_NUO = 1 << 2,		//Ų
	PM_NUO_NEEDBET = 1 << 3,		//����ע��Ų
	PM_2DICE_CHUAN = 1 << 4,		//˫��������1:8(�޴�λΪ1:10)
	PM_2DICE_BAOZI = 1 << 5,		//˫���ӱ���1:15(�޴�λΪ1:10)
	PM_3DICE_GETTWO = 1 << 6,		//���������ж�1:2 1:5(�޴�λΪ1:2.5 1:5)
	PM_3DICE_BAOZI = 1 << 7,		//�����ӱ���1:25(�޴�λΪ1:30)
	PM_FREE_ROBNT = 1 << 8,		//������ׯ(�޴�λΪ�̶�ׯ��)
};


//���������淨
struct tagPM
{
	int  iPlayingMethod;	//�淨
	int  iBetLimit;			//��ע����
	BYTE byDiceNum;			//������
	BYTE byReadyTime;		//׼��ʱ��
	BYTE byRobNtTime;		//��ׯʱ��
	BYTE byBetTime;			//��עʱ��
	BYTE byNuoLimit;		//Ų����(�ٷֱ�)

	tagPM()
	{
		memset(this, 0, sizeof(tagPM));
	}
};

//��Ϸ״̬�������ݰ�
struct	GameStation_Base
{
    bool        bShowChouMa;                //�Ƿ���ʾ����
	BYTE		bGameStation;		        //��Ϸ״̬
	BYTE		byRobNTtime;				//��ׯʱ��
	BYTE		byFreeTime;					//����ʱ��
	BYTE		byBetTime;			        //��עʱ��
	BYTE        byRolldiceTime;             //ҡ��ʱ��
	BYTE		bySendTime;	                //����ʱ��
	BYTE		byRemaindTime;				//ʣ��ʱ��
	BYTE		byNtStation;				//ׯ��λ��
	BYTE		byMaxNtPeople;				//�����ׯ����
	BYTE		byBetWaitTime;				//����ע�׶η��Ƶȴ�ʱ��
	BYTE		byOpenAwardWaitTime;		//�������׶η��Ƶȴ�ʱ��
	BYTE		byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	int			iNtPlayCount;		        //ׯ����ׯ����
	int			iApplyNtLimite;		        //��ׯ����
	long long	i64NtGrade;					//ׯ�ҳɼ�
	long long   i64PlayerGrade;				//�мҳɼ�
	long long   lSingleNoteLimit;           //ʣ����ע��
	int			iChouMaValue[BETSTYLE];		//����ֵ
	tagPM	    playMode;					//�淨

	GameStation_Base()
	{
		memset(this,0, sizeof(GameStation_Base));
	}
};

//��ע���(����ע)
struct	UserBetResultMsg
{
	bool		bDynamicBet;		//�Ƿ�̬��ע
	BYTE	    byDeskStation;		//��ע��λ��
	BYTE		byBetArea;			//��ע����
	BYTE		byChouMaType;		//��ע��������
	long long   i64SumAllBet;       //����ע
	long long   i64UserAllBet;      //��ǰ�������ע
	long long   i64UserBetCount[BET_ARES]; //�м���ÿ���������ע���
	long long	i64AreaBetAllSum[BET_ARES];		//�����������ע
	long long   i64RemainedNotes;                 //ʣ����ע��(�ǵø���ls)
	BYTE		bErrorCode;			//0�ɹ� 1:����������ע���� 2:Ǯ���� 3:ׯ�Ҳ�����ע 4:������������ 5:�������� 6:ׯ�Ҳ�����

	UserBetResultMsg()
	{
		memset(this,0,sizeof(UserBetResultMsg));
	}
};

//֪ͨ����
struct TMSG_GAME_COUNT_NTF
{
	int iRemainCount;			// ʣ�����
	int iTotalCount;		// �ܾ���

	TMSG_GAME_COUNT_NTF()
	{
		iRemainCount = 0;
		iTotalCount = 0;
	}
};
			
//�����ע��Ϣ����		
struct TMSG_BET_INFO_REQ
{
	BYTE byBetStation;						//�����ĸ���ҵ�λ��

	TMSG_BET_INFO_REQ()
	{
		byBetStation = 255;
	}
};	



struct UserBetInfo
{
	BYTE byBetStation;
	int  i64UserBetCount[BET_ARES]; //���������ÿ���������ע���

	UserBetInfo()
	{
		byBetStation = 255;
		memset(i64UserBetCount, 0, sizeof(i64UserBetCount));
	}
};

//�����ע��Ϣ�ظ�	
//�������i64UserBetInfos����Ϊ�����Ϸ����11�������Ϣ����С�������η�������(MAX_SEND_SIZE 2044)
//��������Ϊ�˷������������i64UserBetInfos����Ϊ10��BET_INFO_MAX�������Ϸ����11 - 1����Ϊׯ�Ҳ�����עû����ע��Ϣ��
//���UserBetInfo��i64UserBetCount����Ҳ��Ϊ��int�����������ָ�ֵ����int��ȡֵ��Χ����ֻ�ܸĻ�longlong���ͣ���������ʹ���
struct TMSG_BET_INFO_RSP
{
	UserBetInfo   i64UserBetInfos[BET_INFO_MAX]; //���������ÿ���������ע���(��ׯ��)

	TMSG_BET_INFO_RSP()
	{

	}
};

struct UserNuoData
{
	BYTE byBefore;			//���ĸ�����Ų
	BYTE byAfter;			//Ų���ĸ�����
	int  iNuoMoney;			//Ų����

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

//��Ϊ���������ע��Ϣ�Ĳ�����Ϣ
struct TMSG_NUO_INFO_RSP
{
	UserNuoInfo   sUserNuoInfos[BET_INFO_MAX];			//���������Ų���(��ׯ��)

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


//Ų����
struct TMSG_CHANGE_BET_REQ
{
	BYTE byOriBetArea;				//ԭ��ע����
	BYTE byNewBetArea;				//ҪŲ������ע����
	int  byChangeCount[BETSTYLE];	//Ųע������(ÿ��Ų����)

	TMSG_CHANGE_BET_REQ()
	{
		byOriBetArea = 255;
		byNewBetArea = 255;
		memset(byChangeCount, 0, sizeof(byChangeCount));
	}
};

//Ų�ظ�
struct TMSG_CHANGE_BET_RSP
{
	BYTE byErrorCode;			//0:�ɹ� 1:����ԭ��ע������עֵ 2:������������ 3:����Ų���ƴ��� 4:ѡ��������� 5:�������ʹ��� 6:��Ų��Ҵ��� 

	TMSG_CHANGE_BET_RSP()
	{
		byErrorCode = 0;
	}
};

//Ų��֮��������ݸ���֪ͨ
struct TMSG_UPDATE_BET_NTF
{
	long long   i64UserBetCount[BET_ARES];		//�Լ���ÿ���������ע���
	long long	i64AreaBetAllSum[BET_ARES];		//�����������ע

	TMSG_UPDATE_BET_NTF()
	{
		memset(this, 0, sizeof(TMSG_UPDATE_BET_NTF));
	}
};

//�����
struct TMSG_CALCULATE_BOARD
{
	long long i64WinMoney[PLAY_COUNT];

	TMSG_CALCULATE_BOARD()
	{
		memset(i64WinMoney, 0, sizeof(i64WinMoney));
	}
};

#pragma pack()