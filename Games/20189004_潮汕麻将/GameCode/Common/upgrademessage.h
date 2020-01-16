#pragma once

#include <memory.h>
#include <stddef.h>

typedef unsigned char uchar;

///������汾
#define DEV_LIB_VER				1									///������汾
//֧������
///֧�����Ͷ���
#define SUP_NORMAL_GAME			0x01								///��ͨ��Ϸ
#define SUP_MATCH_GAME			0x02								///������Ϸ
#define SUP_MONEY_GAME			0x04								///�����Ϸ
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

////////////////////////////////////////////////////////////////////////////////
#define GFF_FORCE_FINISH			0										///< ǿ�н��
#define GF_NORMAL               10                                  /// ��Ϸ��������
#define GF_SAFE                 11                                  /// ��Ϸ��ȫ����

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE		
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

#define AI_FAST							// �����˿��ٳ���
#ifndef AI_MACHINE					// ȡ�����ٻ�����
#undef AI_FAST
#endif

// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					TEXT("MatchTable_BSMJ")

// �汾����
#define GAME_MAX_VER					2									// ������߰汾
#define GAME_LESS_VER					2									// ������Ͱ汾
#define GAME_CHANGE_VER					0									// �޸İ汾		
#define PLAY_COUNT						4									// ��Ϸ����	

// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0				// �ȴ���������״̬
#define GS_WAIT_ARGEE					1				// �ȴ�ͬ������
#define GS_DICE							20				// ������״̬
#define GS_SEND_CARD					21				// ����״̬
#define GS_PLAY_GAME					22				// ��Ϸ��״̬
#define GS_WAIT_NEXT					23				// �ȴ���һ�̿�ʼ 


//////�������ͼ���������////////////////////
/********************************************************************************/
#define ACTION_NO				0				//û�ж���
#define ACTION_CHI				1				//���ƶ���
#define ACTION_PENG				2				//���ƶ���
#define ACTION_KAN				3				//�����ƶ���
#define ACTION_AN_GANG			4				//���ܶ���
#define ACTION_BU_GANG			5				//���ܶ���
#define ACTION_MING_GANG		6				//���ܶ���
#define ACTION_CHI_TNG			7				//���ԣ��Ժ����ƣ�
#define ACTION_TING				8				//������
#define ACTION_HU				10				//������
#define ACTION_YANG_MA			11				//����

#define TYPE_JIANG_PAI			15				//��������
#define TYPE_JINGDIAO_JIANG		16				//������������
#define TYPE_SHUN_ZI			17				//˳������
#define TYPE_AN_KE				18				//��������
#define TYPE_JING_KE			19				//�����������

///�����Ƶ�����
#define HAND_CARD_NUM			14				//�����������
#define OUT_CARD_NUM			60				//��������
#define HUA_CARD_NUM			20				//��������
#define MEN_CARD_NUM			40				//��������
#define MAX_HUPAI_TYPE			10				//��������
#define MAX_HAND_PAI			14				//������
#define MAX_PAI_TOU				16				//�����ͷ��
#define MAX_TING_NUM			25				//���������
#pragma pack(push,1)

///�����������ݽṹ
struct GCPStruct
{
	uchar  byType;    //����������
	uchar  iOutpai;   //���˳�����
	uchar  byData[4]; //������������
	uchar  iStation;  //���������λ��
	uchar  iBeStation;//�����������λ��
	GCPStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byData,255,sizeof(byData));
		byType = 255;    //����������
		iOutpai = 255;   //���˳�����
		iStation = 255;  //���������λ��
		iBeStation = 255;//�����������λ��
	};
};
//
///�����ݽṹ��������ֵ��������
struct PaiStruct
{
	uchar  pai;   //������
	uchar  count; //������
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

	bool bFengDing;			//�Ƿ�ⶥ
	bool bFanMa;				//�Ƿ���
	uchar FengDingMulti;	//�ⶥ����
	uchar FanMaNum;		//�������
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
	PM_KeJiePaoHu		=1<<0,//�ɽ��ں�
	PM_GangBao			=1<<1,//�ܰ�����
	PM_BiHu					=1<<2,//�ƺ�

	PM_QiangGangHu	=1<<3,//���ܺ� 2
	PM_QiXiaoDui			=1<<4,//��С�� 3
	PM_HunYiSe				=1<<5,//��һɫ 2
	PM_QingYiSe			=1<<6,//��һɫ 3
	PM_GangKai				=1<<7,//���Ͽ��� 2
	PM_PengPengHU		=1<<8,//������ 2
	PM_ShiSanYao			=1<<9,//ʮ���� 10
	PM_TianDiHu			=1<<10,//��غ� 10
	PM_18LuoHan			=1<<11,//18�޺� 10
	PM_SuperQiDui		=1<<12,//�����߶� 4
	PM_DoubleSpQiDui	=1<<13,//˫�����߶� 6
	PM_TripleSpQiDui		=1<<14,//�������߶� 8
	PM_YaoJiu				=1<<15,//�۾� 2
	PM_SmalSiXi				=1<<16,//С��ϲ4
	PM_BigSiXi				=1<<17,//����ϲ 6
	PM_SmallSanYuan	=1<<18,//С��Ԫ 4
	PM_BigSanYuan		=1<<19,//����Ԫ 6
	
	PM_MaGenGang		=1<<20,//�����
	PM_MaGenDiFen		=1<<21,//����׷�
	//////////////////////////////////////////////////
	//PM_HaveJoker = 1 << 1,				//�й�
	//PM_NoJoker = 1 << 2,					//�й�
	//PM_QiangGang_Double = 1 << 3,		//����˫����ͷ
	//PM_DanDiao_Joker = 1 << 4,				//12�ŵ�����
	//PM_YaoJiu_One_Or_Nine = 1 << 5,		//�۾ź�1��9
	//PM_TianDiHu_Double = 1 << 6,			//��غ�˫����ͷ
	//PM_ForeJoker_ZiMo = 1 << 7,				//�Ĺ�����
	//PM_TimeOut_Auto = 1 << 8,				//��ʱ�йܣ�3���ӣ�
};


// ��Ϸ���ݰ�
/********************************************************************************/
//ֻ��һ�����ݰ�����������ݿ��Բ�ʹ��,��������ʹ��
struct GameStationData
{
	//������Ϣ
	int					iVersion;						// �����汾
	int					iVersion2;
	bool				bShowUserId;					// �Ƿ���ʾ������ҵ�id
	//��Ϸ��Ϣ
	uchar				byNtStation;					// ׯ��λ��
	bool				m_bistuoguan[PLAY_COUNT];		// �Ƿ��й�
	uchar				m_byNowOutStation;				// ���ڳ����û�
	bool				m_bTing[PLAY_COUNT];			// �Ƿ����� 

	int					Stationpara;
	uchar               ucAgree;                        //����λ���Ƿ�ͬ��
	//������
	uchar				m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//���Ʋ�����20�ţ�0Ϊ������־
	uchar				m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//���Ʋ�����160�ţ�0Ϊ������־
	uchar				m_iArMenPai[PLAY_COUNT][MEN_CARD_NUM];	//���Ʋ�����40��
	//��Ϸʱ��
	uchar				byAutoOutTime;		//�Զ�����ʱ��
	uchar				byOutTime;			//����ʱ��
	uchar				byThinkTime;		//˼��ʱ��
	uchar				byBeginTime;		//��ʼ�ȴ�ʱ��
	bool				bAutoBegin;			//ʱ�䵽���Ƿ��Զ���ʼ

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

		byAutoOutTime = 1;		//�Զ�����ʱ��
		byOutTime = 20;			//����ʱ��
		byThinkTime = 15;		//˼��ʱ��
		byBeginTime = 30;		//��ʼ�ȴ�ʱ��
		bAutoBegin = 0;			//ʱ�䵽���Ƿ��Զ���ʼ
	}
};

struct GameStationData_Base
{
	//������Ϣ
	int					Stationpara;		// ��Ϸ״̬
	int					iVersion;			// �����汾
	int					iVersion2;
	uchar               ucAgree;            //����λ���Ƿ�ͬ��
	uchar				byAutoOutTime;		//�Զ�����ʱ��
	uchar				byOutTime;			//����ʱ��
	uchar				byThinkTime;		//˼��ʱ��
	uchar				byBeginTime;		//��ʼ�ȴ�ʱ��
	tagPM				m_tagPM;			//�淨 
	uchar               m_byWindDes[PLAY_COUNT];
	uchar				m_BasePoint;	//�׷�
	GameStationData_Base()
	{
		memset(this, 0, sizeof(GameStationData_Base));	
		memset(m_byWindDes, 255, sizeof(m_byWindDes));
	};
};

//�û��й����ݰ�
struct Usertouguan
{						
	uchar			desk;				// ˭�й�
	bool			enable;				// �й�״̬
	bool			bTuoGuan[4];		// ��������й�״̬
	void Clear()
	{}
};
/////�û�ͬ��ṹ
//struct MSG_GR_R_UserAgree
//{
//    uchar			bDeskNO;							///��Ϸ����
//    uchar			bDeskStation;						///λ�ú���
//    uchar			bAgreeGame;							///ͬ���־
//};
//�û�ͬ����Ϸ
struct UserArgeeGame
{
	uchar			byPlayCount;				// ��Ϸ����
	uchar			byCardCount;				// ����Ŀ
};

// ��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	uchar				bDeskStation;					// �˳�λ��
	int					iTurePoint[4];					// ׯ�ҵ÷�
	int					iChangeMoney[8];				// �������
};

struct HaveThingStruct		// �����뿪
{
	uchar	pos;
	char	szMessage[61];
};

struct LeaveResultStruct	// �����뿪���
{
	uchar	bLeaveStation; //��Ҫ�뿪��ҵ�λ��
	uchar	bDeskStation;
	int		bArgeeLeave;
};

struct MessageStruct		// �ͻ��˵�������,��Ϣ
{
	char Message[200];
};

///////////////////////////////////////////////////////////////////////////

#define MAX_HEPAI_TYPES     256 ///����������

///�齫�ƣ��������ƣ������㣬��ǰ���Ʒ���
struct MjMenPaiData 
{
	uchar    byMenPai[300];				//�齫����������Ҹ�����������������ÿ������2���� 
	uchar    byNt;						//ׯ��λ��
	uchar	byStation;					//��ǰץ����ҵ�λ��
	uchar	byDir;						//��ǰץ�Ʒ���(ץ�Ǹ������ǰ����)
	uchar	byGetPaiDir;				//�������Ʒ���
	uchar	byRemainDun;				//ʣ�����
	uchar	byGetPai;				    //���ƶ���
	uchar    byMenPaiDun[4];             //ÿ�������ǰ�Ķ���
	uchar    byStartIndex;				//������ʼ����		
	uchar    byEndIndex;				    //���ƽ�������		
	uchar    byCaiShenIndex;				//����λ������		
	uchar    byAllPaiNum;				//�����Ƶ�����		

	MjMenPaiData()
	{
		Init();
	};
	void Init()
	{
		memset(byMenPai,255,sizeof(byMenPai));
		memset(byMenPaiDun,255,sizeof(byMenPaiDun));//�����ǰ�Ķ���
		byNt = 255;						//ׯ��λ��
		byStation = 255;					//��ǰץ����ҵ�λ��
		byDir = 255;						//��ǰץ�Ʒ���(ץ�Ǹ������ǰ����)
		byGetPaiDir = 255;				//�������Ʒ���
		byRemainDun = 255;				//ʣ�����
		byGetPai = 255;				    //���ƶ���
		byStartIndex = 255;				//������ʼ����
		byEndIndex = 255;				    //���ƽ�������
		byAllPaiNum = 255;				    //�����Ƶ�����
		byCaiShenIndex = 255;				//����λ������
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
	bool ChangAnMenPai(uchar oldpai,uchar newpai)// ������Ҫ�������� oldpai,���������� newpai
	{
		if(oldpai == 255 || newpai == 255)
			return false;
		for(int i=0;i<300;++i)
		{
			if(byMenPai[i] == oldpai)
			{
				byMenPai[i] = newpai;//���Ƴɹ�
				return true;
			}
		}
		return false;
	};
};

/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
// #define MDM_GM_GAME_NOTIFY				180				///��Ϣ���ͣ���Ϸ֪ͨ��Ϣ
// #define ASS_GM_GAME_STATION				2				///��Ϸ״̬
// #define ASS_GM_AGREE_GAME               1               ///�û�ͬ��

#define ASS_UG_USER_SET					70				//�û�������Ϸ
#define ASS_SEZIFINISH					72				//ɫ�ӽ���
#define ASS_AUTOCARD					74				//�й�
#define ASS_PASS_CHECK					75				//"��"
#define ASS_HAVE_THING					76				//�����뿪
#define ASS_LEFT_RESULT					77				//�뿪���

#define ASS_SUPER						78				//�����ͻ��ˣ�������
#define ASS_HUPAI_TEST					79				//�����ͻ��ˣ����Ʋ���

#define ASS_MJ_RULE						89				//�齫��������
#define ASS_EXIT						90				//�˳�
#define ASS_MESSAGE						91				//��Ϣ
#define	ASS_SET_STATE					92				//���ڳ����˵��������󷢸�4���ͻ��˵ļ�ʱ���ź�
#define ASS_GM_SHORT_STOP               93              //��ͣ��Ϣ
#define ASS_NET_CAUSE_END               94              //����ԭ������Ϸ����

//#define ASS_HUPAI_TEST					159			//�����ͻ��ˣ����Ʋ���
#define ASS_CHANGE_PAI					160				//�û����ƣ�������
#define ASS_SET_MOPAI					161				//�û�����׼��Ҫ������
#define ASS_HUANGZHUANG					162				//ǿ�ƻ�ׯ

/********************************************************************************/

//////////////////////�¼���////////////////////////////////////////////////////////////////////
//����ǰ�¼�
enum MJTHINGS
{
	THING_BEGIN				=10,			//��Ϸ��ʼ�¼�					        
	THING_DONG_NT			,			//�׾��Զ�Ϊׯ�¼�					         
	THING_2SEZI_NT			,			//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	THING_2SEZI_NT_GP		,			//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	THING_TURN_2SEZI_NT		,			//�׾�������2��ɫ�Ӷ�ׯ���¼�		
	THING_2SEZI_DIR			,			//��2��ɫ���¼������Ʒ����¼�		
	THING_2SEZI_DIR_GP		,			//��2��ɫ���¼������Ʒ�������Ƶ��¼�
	THING_2SEZI_GP			,			//��2��ɫ���¼������Ƶ��¼�		
	THING_2SEZI_NT_DIR_GP	,			//��2��ɫ���¼������Ƶ��¼�	
	//����ǰ�¼�            
	THING_SEND_PAI			,			//�����¼�							
	THING_TIAO_PAI			,			//�����¼�							
	THING_2SEZI_JING		,			//��2��ɫ�Ӷ������¼�			
	THING_1SEZI_JIN			,			//��1��ɫ�Ӷ������¼�
	THING_ALL_BUHUA		    ,			//Ϊ������Ҳ����¼�				
	THING_ONE_BUHUA		    ,			//������Ҳ����¼�					
	//���������¼�          
	THING_BEGIN_OUT			,			//ׯ�ҿ�ʼ����֪ͨ�¼�
	THING_DING_QUE          ,           //��ȱ�¼�
	THING_OUT_PAI			,			//�����¼�
	THING_ZHUA_PAI			,			//ץ���¼�
	THING_CPG_NOTIFY		,			//�ܳ���֪ͨ�¼��¼�
	THING_CHI_PAI			,			//�����¼�
	THING_PENG_PAI			,			//�����¼�
	THING_KAN_PAI			,			//�����¼�
	THING_SAO_HU			,			//ɨ���¼�
	THING_GANG_PAI			,			//�����¼�
	THING_TING_PAI			,			//�����¼�
	THING_WAIT_OUT			,			//�ȴ������¼�
	//���������¼�          

	THING_CHANGE_NOTIFY		,			//֪ͨ���ѡ��
	THING_CHANGE_SEND_CARD	,			//���·���

	//���Ƽ����ƺ��¼�
	THING_HU_PAI			,			//���¼�	
	THING_COUNT_FEN			,			//����¼�	
	THING_ENG_HANDLE		,			//���������¼�
	THING_ENG_UN_NORMAL		,			//���������������¼�
	THING_HAVE_ACTION		,			//�ж���   

	S_C_UPDATE_CALCULATE_BOARD_SIG,		//���½����
	S_C_UPDATE_REMAIN_JUSHU_SIG,		//����ʣ�����
	S_C_UPDATE_PLAY_METHOD,				//��Ϸ�淨֪ͨ

	THING_PAI_TOU			,			//��ͷ�¼�

	THING_MAX				                //����¼�id
};                              
							
//////////////////////�¼����ݽṹ/////////////

///��Ϸ��ʼ�¼�
struct tagBegin
{
	uchar	byNt;		//ׯ��λ��
	uchar	byRemainingCards;	// ʣ������
	uchar	m_byWindDes[PLAY_COUNT]; //��ҷ�λ

	void Clear()
	{
		byNt	=255;		//ׯ��λ��
		byRemainingCards = 0;
		memset(m_byWindDes, 255, sizeof(m_byWindDes));
	}
};

///�Զ�Ϊׯ�¼�
struct tagDongNt
{
	uchar   byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar   byNt;		//ׯ��λ��
	uchar   byNo;		//�¼��Ĵ���
	uchar   byNext;	//��һ���¼��Ĵ���
	void Clear()
	{	
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byNt	=255;//ׯ��λ��
		byNo = THING_BEGIN;//�¼��Ĵ���
	}
};

///��2��ɫ�ӵĵ�����Ϊׯ�¼�
struct tagTwoSeziNt
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//��ɫ�����
	uchar	byNt;//ׯ��λ��
	uchar	bySezi0;//ɫ��0����
	uchar	bySezi1;//ɫ��1����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	uchar	byNo;//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//��ɫ�����
		byNt	=255;//ׯ��λ��
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����	
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_NT;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
struct tagTwoSeziNtAndGetPai
{
	uchar   byDo;	//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar   byUser;	//��ɫ�����
	uchar   byNt;	//ׯ��λ��
	uchar   bySezi0;	//ɫ��0����
	uchar   bySezi1;	//ɫ��1����
	uchar   byGetPai;//����λ��
	bool   bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	uchar   byNo;//�¼��Ĵ���
	uchar   byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//��ɫ�����
		byNt	=255;//ׯ��λ��
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����
		byGetPai=255;//����λ��
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_NT_GP;//�¼��Ĵ���
	}
};

///������2��ɫ��ȷ��ׯ��
struct tagTurnSeziNt
{

	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byNt;//ׯ��λ��
	uchar	byUser;//��ǰ��ɫ�ӵ����
	uchar	bySezi0;//ɫ��0����
	uchar	bySezi1;//ɫ��1����
	uchar	bySezi[4];//���0ɫ������
	uchar	byMaxUser;//����������Ҹ���
	uchar	byMaxDian;//������ֵ
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	bool	bDoFinish[4];//��¼�Ѿ�����ɫ�����
	bool	bDoSezi[4];//����Ҫ��ɫ�ӵ����
	bool	bFirst;//�Ƿ��һ����ɫ��
	uchar	byNo;//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byNt	=255;//ׯ��λ��
		byUser	=255;//��ǰ��ɫ�ӵ����
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����
		memset(bySezi,255,sizeof(bySezi));//���0ɫ������
		byMaxUser	=255;//����������Ҹ���
		byMaxDian	=255;//������ֵ
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		memset(bDoFinish,0,sizeof(bDoFinish));
		memset(bDoSezi,0,sizeof(bDoSezi));
		bFirst = true;//�Ƿ��һ����ɫ��
		byNo = THING_TURN_2SEZI_NT;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ������λ���¼�
struct tagTwoSeziDir
{
	uchar	byDo;			//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;			//��ɫ�����
	uchar	byNt;			//ׯ��
	uchar	bySezi0;		//ɫ��0����
	uchar	bySezi1;		//ɫ��1����
	uchar	byDirection;	//���Ʒ�λ
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	uchar	byNo;			//�¼��Ĵ���
	uchar	byNext;		//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;			//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;		//��ɫ�����
		bySezi0	=255;		//ɫ��0����
		bySezi1	=255;		//ɫ��1����
		byNt = 255;			//ׯ��
		byDirection	=255;	//���Ʒ�λ
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_DIR;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
struct tagTwoSeziDirAndGetPai
{
	uchar	byUser;	//��ɫ�����
	uchar	bySezi0;	//ɫ��0����
	uchar	bySezi1;	//ɫ��1����
	uchar	byDirection;//���Ʒ�λ
	uchar	byGetPai;	//���Ƶ�

	void Clear()
	{
		byUser	=255;	//��ɫ�����
		bySezi0	=255;	//ɫ��0����
		bySezi1	=255;	//ɫ��1����
		byDirection	=255;//���Ʒ�λ
		byGetPai	=255;	//���Ƶ�
	}
};

///��2��ɫ��ȷ�����ƶ����¼�
struct tagTwoSeziGetPai
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//��ɫ�����
	uchar	bySezi0;//ɫ��0����
	uchar	bySezi1;//ɫ��1����
	uchar	byGetPai;//���Ƶ�
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;	//��ɫ�����
		bySezi0	=255;	//ɫ��0����
		bySezi1	=255;	//ɫ��1����
		byGetPai	=255;	//���Ƶ�
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_GP;//�¼��Ĵ���
	}
};

///�����¼�
struct tagSendPai
{
	uchar	byRemainPai;//ʣ������
	uchar	byAllPaiNum;//������
	uchar	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	uchar   m_byFanCard;			//����������
	uchar   m_byJoker;				//������

	void Clear()
	{
		byRemainPai	=255;//ʣ������
		byAllPaiNum	=255;//������
		m_byFanCard = 255;
		m_byJoker = 255;
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
	}
};
///�����¼�
struct tagTiaoPai
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	uchar	byNt;//ׯ��λ��
	uchar	byIndex0;//����λ��0
	uchar	byIndex1;//����λ��1
	uchar	byRemainPai;//ʣ������
	uchar	bySendPai;//��������
	uchar	byGetPai;//���Ƶ�
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//��������
	uchar	byMenPai[4][MEN_CARD_NUM];//����
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//�������λ��
		byNt		=255;//ׯ��λ��
		byIndex0	=255;//����λ��0
		byIndex1	=255;//����λ��1
		byRemainPai	=255;//ʣ������
		bySendPai	=255;//��������
		byGetPai	=255;//���Ƶ�
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byNo = THING_TIAO_PAI;//�¼��Ĵ���
	}
};
///��ɫ��2��ɫ�Ӷ������¼�
struct tagTwoSeziJing
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//��ɫ�����λ��
	uchar	byNt;//ׯ��λ��
	uchar	byFanPai;//���Ƶ�
	uchar	bySezi0;//ɫ��0����
	uchar	bySezi1;//ɫ��1����
	uchar	byMainJing;//������ֵ
	uchar	byLessJing;//��������ֵ
	uchar	byStartIndex;///��ʼ����
	uchar	byEndIndex;  ///��������
	uchar	byCaiShenIndex; ///��������
	uchar	byMenPai[4][MEN_CARD_NUM];//����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//��ɫ�����λ��
		byNt		=255;//ׯ��λ��
		byFanPai	=255;//���Ƶ�
		bySezi0		=255;//ɫ��0����
		bySezi1		=255;//ɫ��1����
		byMainJing	=255;//������ֵ
		byLessJing	=255;//��������ֵ
		byStartIndex	=255;///��ʼ����
		byEndIndex	=255;  ///��������
		byCaiShenIndex=255; ///��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_JING;//�¼��Ĵ���
	}
};
///��ɫ��1��ɫ�Ӷ������¼�
struct tagOneSeziJin
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//��ɫ�����λ��
	uchar	byNt;//ׯ��λ��
	uchar	byFanPai;//���Ƶ�
	uchar	bySezi0;//ɫ��0����
	uchar	byMainJing;//������ֵ
	uchar	byLessJing;//��������ֵ
	uchar	byStartIndex;///��ʼ����
	uchar	byEndIndex;  ///��������
	uchar	byCaiShenIndex; ///��������
	uchar	byMenPai[4][MEN_CARD_NUM];//����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//��ɫ�����λ��
		byNt		=255;//ׯ��λ��
		byFanPai	=255;//���Ƶ�
		bySezi0		=255;//ɫ��0����
		byMainJing	=255;//������ֵ
		byLessJing	=255;//��������ֵ
		byStartIndex	=255;///��ʼ����
		byEndIndex	=255;  ///��������
		byCaiShenIndex=255; ///��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_1SEZI_JIN;//�¼��Ĵ���
	}
};

///������Ҳ����¼�
struct tagAllBuHua
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser[4];//��Ҫ�������λ��
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//��������
	uchar	byMenPai[4][MEN_CARD_NUM];//����
	uchar	m_byArHuaPai[4][HUA_CARD_NUM];//��������
	bool	bBuHuaFinish[4];//��¼�Ѿ��Ѿ�������ɵ����
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo = 255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		memset(byUser, 255, sizeof(byUser));	//��Ҫ�������λ��
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������	
		memset(bBuHuaFinish,0,sizeof(bBuHuaFinish));
		byNo = THING_ALL_BUHUA;//�¼��Ĵ���
	}
};

///������Ҳ����¼�
struct tagOneBuHua
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	uchar	byHua;	  //ץ���û���
	uchar	byPs;	  //��������
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//��������
	uchar	byMenPai[4][MEN_CARD_NUM];//����
	uchar	m_byArHuaPai[4][HUA_CARD_NUM];//
	bool	bFinish;//�Ƿ񲹻�����
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byHua	=255;	  //ץ���û���
		byPs	=255;	  //��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������
		bFinish=false;
		byNo = THING_ONE_BUHUA;//�¼��Ĵ���
	}
};
///��ʼ����֪ͨ�¼�
struct tagBeginOutPai
{
	uchar	byUser;	//�״γ������λ��
	uchar	byNt;	//ׯ��λ��

	void Clear()
	{
		byUser	=255;//�״γ������λ��
		byNt	=255;//ׯ��λ��
	}
};
///�����¼�
struct tagOutPai
{
	uchar	byUser;				//�������λ��
	uchar	byPs;				//���Ƶ�ֵ
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//��������
	uchar	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־

	void Clear()
	{
		byUser	=255;				//�������λ��
		byPs		=255;				//���Ƶ�ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������	
	}
};
///ץ�����¼�
struct tagZhuaPai
{
	uchar	byUser;		//ץ�����λ��
	uchar	byPs;		//ץ�Ƶ�ֵ
	bool	bHead;		//ץ�Ƶķ���
	uchar	byRemainingCards; // ʣ������

	void Clear()
	{
		byUser	=255;		//ץ�����λ��
		byPs		=255;		//ץ�Ƶ�ֵ
		bHead = true;
		byRemainingCards = 0;
	}
};
///�����ܺ�֪ͨ�¼��¼�
struct tagCPGNotify
{
	uchar	byUser;				//�������λ��
	uchar	byPs;				//���Ƶ�ֵ
	bool	bZhuaPai;			//�Ƿ��Լ�ץ��
	bool	bChi;				//�Ƿ��ܳ�
	bool	bPeng;				//�Ƿ�����
	bool	bGang;				//�Ƿ��ܸ�
	bool	bTing;				//��ǰ�ܷ�����
	bool	bHu;				//�Ƿ��ܺ�
	bool    bCanAction;			//����Ƿ��ж�������
	uchar	m_byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	uchar	m_iChi[3][3];		//�ܳԵ���
	uchar	m_iGangData[4][2];	//�ܸܵ���	
	uchar   m_iHuPs;			//������
	
	void Clear()
	{
		byUser	=255;			//�������λ��
		byPs	=255;			//���Ƶ�ֵ
		bZhuaPai=false;			//�Ƿ��Լ�ץ��
		bChi	=false;			//�Ƿ��ܳ�
		bPeng	=false;			//�Ƿ�����
		bGang	=false;			//�Ƿ��ܸ�
		bTing	=false;			//��ǰ�ܷ�����
		bHu		=false;			//�Ƿ��ܺ�
		bCanAction = false;		//����Ƿ��ж�������
		memset(m_byTingCanOut,255,sizeof(m_byTingCanOut));///�����ܴ����
		memset(m_iChi,255,sizeof(m_iChi));//�ܳԵ���
		memset(m_iGangData,255,sizeof(m_iGangData));//�ܸܵ���	
		m_iHuPs = 255;
	}
};
///�������¼�
struct tagChiPai
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	uchar	byBeChi;//���������λ��
	uchar	byType;//���Ƶ����ͣ�0�Դ�1���м䣬2��С�������ƣ�
	uchar	byPs;	//�Ե���ֵ
	uchar	byChiPs[3];//���Ƶ�����
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//��������
	uchar	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byBeChi	=255;//���������λ��
		byType	=255;//���Ƶ����ͣ�0�Դ�1���м䣬2��С�������ƣ�
		byPs		=255;	//�Ե���ֵ
		memset(byChiPs, 255, sizeof(byChiPs));//���Ƶ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		byNo = THING_CHI_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagPengPai
{
	uchar	byUser;//�������λ��
	uchar	byBePeng;//���������λ��
	uchar	byPs;	//������ֵ

	void Clear()
	{
		byUser	=255;//�������λ��
		byBePeng	=255;//���������λ��
		byPs		=255;//������ֵ
	}
};
///�����¼�
struct tagKanPai
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	uchar	byPs;	//������ֵ
	uchar	m_byArHandPai[4][HAND_CARD_NUM];//��������
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byPs		=255;//������ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		byNo = THING_KAN_PAI;//�¼��Ĵ���
	}
};
///ɨ���¼����黢�齫ר�ã�
struct tagSaoHu
{
	uchar	byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;		//���λ��
	uchar	byPs;		//������ֵ
	bool	bSaohu[4];	//ɨ�������
	bool	bHitSaohu[4];	//�յ����ɨ��
	uchar    byOutData[4];//����ɨ����ҳ�����
	uchar    byHandData[4];//����ɨ������������е���
	uchar    byBaoPai;	//�������λ��
	uchar	byNo;		//�¼��Ĵ���
	uchar	byNext;		//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byPs	=255;//������ֵ
		byBaoPai=255;	//�������λ��
		memset(bSaohu, 0, sizeof(bSaohu));//	
		memset(bHitSaohu, 0, sizeof(bHitSaohu));//	
		memset(byOutData, 255, sizeof(byOutData));//	
		memset(byHandData, 255, sizeof(byHandData));//	
		byNo = THING_SAO_HU;//�¼��Ĵ���
	}
};
///�������¼�
struct tagGangPai
{
	uchar	byUser;//�������λ��
	uchar	byBeGang;//���������λ��
	uchar	byType;//���Ƶ�����
	uchar	byPs;	//�ܵ���ֵ

	void Clear()
	{
		byUser	=255;//�������λ��
		byBeGang	=255;//���������λ��
		byType	=255;//���Ƶ�����
		byPs		=255;	//�ܵ���ֵ
	}
};
///�������¼�
struct tagTingPai
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	uchar	byTing;//�������λ��
	uchar	byPs;	//���Ƴ�����ֵ
	uchar	byNo;	//�¼��Ĵ���
	bool	bHitTing[4];//��ҵ��������
	uchar	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byTing	=255;//�������λ��
		byPs		=255;	//���Ƴ�����ֵ	
		memset(bHitTing,0,sizeof(bHitTing));//��ҵ��������
		byNo = THING_TING_PAI;//�¼��Ĵ���
	}
};


///�����¼�
struct tagHuPai
{
	uchar	byUser;				//�������λ��
	uchar	byDianPao;			//�������λ��
	uchar	byPs;				//���Ƶ���ֵ
	uchar	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	uchar	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	uchar	byUserNum;			//������ҵĸ���
	bool    bZimo;				//�Ƿ�����
	uchar   byHuType[MAX_HUPAI_TYPE];//��������	

	void Clear()
	{
		byUser		=255;//�������λ��
		byDianPao	=255;//�������λ��
		byPs		=255;	//���Ƶ���ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		byUserNum = 0;//������ҵĸ���
		bZimo = false;   //�Ƿ�����
		memset(byHuType, 255, sizeof(byHuType));//��������
	}
};
///����¼�
struct tagCountFen
{
	uchar	byNt;//ׯ��λ��
	bool	bHu[PLAY_COUNT];//��Щ��Һ���
	__int64 iZongFen[PLAY_COUNT];//������ҵ��ܷ�
	int 	iHuFen[PLAY_COUNT];//������ҵĺ��Ʒ�
	uchar	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	uchar   byHuType[PLAY_COUNT][MAX_HUPAI_TYPE];//������ҵĺ�������	


	void Clear()
	{

		byNt	=255;//ׯ��λ��
		memset(bHu,0,sizeof(bHu));
		memset(iZongFen,0,sizeof(iZongFen));
		memset(iHuFen,0,sizeof(iHuFen));
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(byHuType, 255, sizeof(byHuType));//������ҵĺ�������
	}
};
///��Ϸ���������¼�
struct tagEndHandle
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	uchar	byHu;//�������λ��
	uchar	byNextNt;//�¾�ׯ��λ��
	uchar	byPs;	//���ڵ���ֵ
	bool	bDH;//һ�ڶ���
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	tagEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byHu	=255;//�������λ��
		byNextNt=255;//�¾�ׯ��λ��
		byPs		=255;	//���ڵ���ֵ
		bDH = 0;	
		byNo = THING_ENG_HANDLE;//�¼��Ĵ���
	}
};
///���������������¼�
struct tagUnnormalEndHandle
{
	uchar	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	uchar	byUser;//�������λ��
	int		iFen[8];//�÷�
	int		iMoney[8];//���
	bool    bForce;//�Ƿ�ǿ�˽���
	uchar	byNo;	//�¼��Ĵ���
	uchar	byNext;//��һ���¼��Ĵ���
	tagUnnormalEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		memset(iFen, 0, sizeof(iFen));
		memset(iMoney, 0, sizeof(iMoney));
		bForce = false;//�Ƿ�ȫ����
		byNo = THING_ENG_UN_NORMAL;//�¼��Ĵ���
	}
};

//�ܽ�����ݽṹ
struct	CalculateBoard
{
	bool	bClear;
	bool	bBestFire;				//�������
	bool	bWinner;				//��Ӯ��
	int		iZimo;					//��������
	int		iJiePao;				//���ڴ���
	int		iDianPao;				//���ڴ���
	int		iAnGang;				//���ܴ���
	int		iMingGang;				//���ܴ���
	int     iBuGang;				//���ܴ���
	int		iPaiTou;				//����ͷ����
	__int64	i64WinMoney;			//��Ӯ���
	CalculateBoard()
	{
		memset(this,0,sizeof(CalculateBoard));
	}
};

//������Ϸ��Ϣ������������(����ֻ�����ڴ����)
struct	GameInfForReplay
{
	int		iZimo;					//��������
	int		iJiePao;				//���ڴ���
	int		iDianPao;				//���ڴ���
	int		iAnGan;					//���ɴ���
	int		iMingGan;				//���ɴ���
	__int64	i64WinMoney;			//��Ӯ���

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