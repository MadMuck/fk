#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#pragma pack(1)
//�ļ����ֶ���
#define GAMENAME						TEXT("��������3D")


//�汾����
#define GAME_MAX_VER					1							//������߰汾
#define GAME_LESS_VER					1							//������Ͱ汾
#define GAME_CHANGE_VER					0							//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable_gdw"

//��Ϸ��Ϣ
#define NAME_ID							33000904							//���� ID
#define CLIENT_DLL_NAME					TEXT("33000904.ico")
#define SERVER_DLL_NAME					TEXT("33000904.dll")

#define PLAY_COUNT						4							//��Ϸ����

//��Ϸ�ڲ���ʼ�汾�˶�
#define DEV_HEIGHT_VERSION				1							//�ڲ������߰汾��
#define DEV_LOW_VERSION					1							//�ڲ������Ͱ汾��

#define SKIN_FOLDER						TEXT("33000904")			//ͼƬ�ļ���
#define SKIN_CARD						TEXT("CardSkin")            //��ͼƬ�ļ���

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0							//�ȴ���������״̬
#define GS_WAIT_ARGEE					1							//�ȴ�ͬ������
#define GS_SEND_CARD					20							//����״̬
#define GS_CONTRIBUTE                   21                          //����״̬
#define GS_BACKCONTRIBUT                22                          //�˹�״̬
#define GS_WAIT_BACK					23							//�ȴ���ѹ����(��һ����Ϊ�ȴ��з�״̬)
#define GS_PLAY_GAME					24							//��Ϸ��״̬
#define GS_WAIT_NEXT					25							//�ȴ���һ�̿�ʼ 

/*------------------------------------------------------------------------------*/
#define DEF_SCREEN_WIDTH	  1434		// Ĭ������
#define DEF_SCREEN_HEIGHT	  872			// Ĭ������

/*------------------------------------------------------------------------*/
//��Ϸ���ڴ�С
static float screen_width_ = DEF_SCREEN_WIDTH;
static float screen_height_ = DEF_SCREEN_HEIGHT;
//������ʾ����С
static int screen_width_PC = DEF_SCREEN_WIDTH;
static int screen_height_PC = DEF_SCREEN_HEIGHT;

static	float g_hscale = 1.0f;
static	float g_vscale = 1.0f;
/*------------------------------------------------------------------------------*/

/********************************************************************************/

//��Ϸ���ݰ�
/********************************************************************************/


//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ
};
//////////////////////////////////////////////////////////////////////////////
//////////////////������Ϸ��Ϣ�����������������//////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	int					iUpgradeStation;				//ׯ��λ��
	BYTE				iStation[2];					//���Ƶ�λ��
	BYTE                iBeenPlayGame;                 
	            
};
//��Ϸ׼�����ݰ�
struct ReadyStruct
{
	BYTE				bDeskStation;					//��ǰ������
};

//�û��������ݰ� ������ͻ��ˣ�
struct OutCardMsg
{
	int					iNextDeskStation;				//��һ������
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iCardList[39];					//�˿���Ϣ
};
//���������ݰ�
struct OutCardFinishStruct
{
	BYTE bFriendDesk;									//������λ��
	BYTE bDeskStation;									//λ��
	BYTE bSequence;										
};

//����
struct AwardPointStruct
{
	BYTE	iAwardPoint;
	BYTE	bDeskStation;
};


struct SendCardMsgStruct
{
	BYTE				bDeskStation;						//ׯ��λ��
//	BYTE				bCardFriend[4];					//��������
};

//�������ݰ�
struct SendCardStruct
{
	int					bDesk;							//���
	BYTE				iCard;							//�Ʊ��
};

//�l�������Ɣ���
struct SendAllStruct
{
	BYTE iUserCardCount[4];									//��������
	BYTE iUserCardList[216];								//���ƶ���
};

//�з����ݰ�
struct JiaofenStruct
{
	int			iStyle;									//��������(�зַ���)
	int         bDeskStation;							//��ǰ�з���
	BOOL		bFlag;									//�з��������ֱ��
};

//����
struct GainstContributeStruct
{

};

//�Ϲ�
struct ContributeStruct
{
	BYTE bLoseDesk1;								//��һ���
	BYTE bLoseCard1;								//��һ����Ϲ���
	BYTE bLoseDesk2;								//�ڶ����
	BYTE bLoseCard2;								//�ڶ�����Ϲ�����

	BYTE bWinerDesk1;								//��һӮ��
	BYTE bWinerCard1;								//��һӮ���õ�����
	BYTE bWinerDesk2;								//�ڶ�Ӯ��
	BYTE bWinerCard2;								//�ڶ�Ӯ���õ�����

	BYTE PlayCount;                                 //�Ϲ�������;
};
//ѡ�ƽ��
struct ChoiceCardStruct 
{
	BYTE bDeskStation;								//ѡ������λ��
	BYTE bCard;										//��
	BYTE bDest;										//ѡ����λ�ú�
};

//�˹�
struct MoveBackContributeStruct
{
	BYTE bDeskStation;								//�˹�λ��
	BYTE bCard;										//�˹���
};

//�˹����
struct ChangeCardStruct
{
	BYTE bDeskFrom;									//˭����
	BYTE bCard;										//������
	BYTE bDeskTo;									//������˭
	BYTE bDest;                                      //ѡ��ڼ���ȷ��
};

//���������ݰ�  �������������
struct ClientNtStruct
{
	BYTE				bDeskStation;					//����λ��
	BYTE				iNtCard;						//������
	BYTE				iCardCount;						//������Ŀ
};

//���������ݰ�  ������ͻ��ˣ�
struct ServerNtStruct
{
	BYTE				bDeskStation;					//����λ��
	BYTE				iNtCard;						//���÷���
	BYTE				iCardCount;						//��������
};

//�������ݰ�
struct BackCardStruct
{
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[8];					//��������
};

//�������ݰ�
struct BackCardExStruct
{
	BYTE				iNtHua;							//���ƻ�ɫ
	BYTE				iGiveBackPeople;				//�������
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[8];					//��������
};

//������״̬���ݰ�
struct BackStationStruct
{
	BOOL				bOtherCanSee;					//��������Ƿ�ɼ�
//	BYTE				iNtPeople;						//����λ��	
	BYTE				iGiveBackStation;				//����λ��
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[12];					//��������
};

//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
	BYTE                iUserCard[PLAY_COUNT][54];      //��ҵ���
	BYTE                iUserCardCount[PLAY_COUNT];
	BeginPlayStruct()
	{
		memset(iUserCard,0,sizeof(iUserCard));
		memset(iUserCardCount,0,sizeof(iUserCardCount));
	}
};

//�û��������ݰ� �������������
struct OutCardStruct
{
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[39];					//�˿���Ϣ
	OutCardStruct()
	{
		memset(this, 0 ,sizeof(OutCardStruct));
	}
};


//�û�˦��ʧ�����ݰ� ������ͻ��ˣ�
struct OutShowCardStruct
{
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iResultCount;					//����˿���Ŀ
	BYTE				iCardList[78];					//�˿���Ϣ
};

//�û��������ݰ� ������ͻ��ˣ�(���һ���ƣ�
struct BatchOutCardMsg
{
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				iCardList[156];					//�˿���Ϣ
};

//��һ�ֳ���
struct NewTurnStruct
{
	BYTE bDeskStation;									//�������
};


//��ҵķ����ݰ�
struct GamePointStruct
{
	bool				bAdd;							//δ��(���Ϊtrue��ʾiDesk���ץ�÷�,false��ʾ��Ϊ������ʱ��)
	BYTE 				iDesk;							//�ĸ���ҵ÷�
	int					iPoint;							//�÷���Ŀ
	int					iTaxis;							//�����Ⱥ�
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					iRoomBasePoint;						//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����

//	int					iBasePoint;						//�Α򱶔�
//	int					iDeskBasePoint;						//�Α򱶔�
	int					bDeskStation;					//�˳�λ��
	__int64				iTurePoint[4];					//ׯ�ҵ÷�
	__int64				iChangeMoney[8];				//��ҽ��
};

//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	int					iRoomBasePoint;						//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����

	BYTE				iUpGradeStation;				//ׯ��λ��
	int					iGrade2;						//ׯ������
	BYTE				iAward[4];						//���Ͷ���
	BYTE				iContinueWin[4];				//��ʤ
	BYTE				 iContinueLose[4];				//����
	int					iAwardPoint[4];					//���ͷ����ͼӷ�
	BYTE				iSequence[4];
	BYTE				itype;							//����,��ͨ;����
	int					iTurePoint[4];					//��ҵ÷�
	int					iSystemPoint[4];				//ϵͳ�ӷ�
	__int64				iGamePoint[4];					//�Α���ݔ�A��r
	__int64				iChangeMoney[8];				//��ҽ��
	int                 iWinSequence[4][2];             ///ͷδ�����
};

//��Ϸ����
struct GameFinishNotify
{
	int					iRoomBasePoint;					//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����
	TCHAR				name[4][21];
	BYTE				iUpGradePeople;
	BYTE				iGrade2;
	BYTE				iStyle;							//��Ϸ�����Ƿ�Ϊǿ��,������������
	BYTE				iContinueWin[4];				//��ʤ
	BYTE				iContinueLose[4];				//����
	BYTE				iAward[4];						//���͔���
	int					iAwardPoint[4];					//���ͷ�
	__int64				iWardPoint[4];				//Ӯ����
	int					iGamePoint[4];					//�Α�ݔ�A
	BYTE				iSequence[4];					//������˳��

	__int64				iMoney[4];
	
};
/////////////////////////////////////////////////////////
//////////////������Ϸ�ṹ///////////////////////////////
/////////////////////////////////////////////////////////

//�й�
struct AutoCardStruct
{
	BYTE bDeskStation;
	bool bAutoCard;
};

//����Ҫ��
struct HaveThingStruct
{
	BYTE pos;
	char szMessage[61];
};

//ͬ���뿪
struct LeaveResultStruct
{
	BYTE bDeskStation;
	int bArgeeLeave;
};

//������Ϣ
struct MessageStruct	//�ͻ��˵�������
{
	char Message[200];
};

//��ǰ����
struct StopResultStruct
{
	bool bArgee;
};

//ͬ��Ͷ��
struct LoseResultStruct
{
	BYTE bDeskStation;
	bool bArgee;
};

//����
struct SuperClientStruct
{
	int iCardListNum;
	BYTE bCardList[100];
};

struct OutCardErrorStruct
{
	int iCardCount;
	BYTE bCard[45];
	OutCardErrorStruct()
	{
		iCardCount = 0;
		memset(bCard,0,sizeof(bCard));

	}
};

struct tagPM
{
	int iPlayingMethod;		//��Ϸ�淨��PM_FLAGS��
 	BYTE byGameTime;		//��Ϸʱ�䣨���ӣ�
};

enum PM_FLAGS
{
	None = 0,
	OneToOneFight = 1 << 1,		//1V1
	TwoToTwoFight = 1 << 2,		//2V2	
	FourFight = 1 << 3,			//���˶�ս
	OneRatioOne = 1 << 4,		//����1:1
	OneRatioTen = 1 << 5,		//����1:10
};

#pragma pack()
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
////��Ϸ�������
//#define ASS_UG_USER_SET					50				//�û�������Ϸ
//#define ASS_BEGIN_UPGRADE				51				//������Ϸ��ʼ
//#define ASS_READY						52				//׼������
//#define ASS_SEND_CARD					53				//������Ϣ(һ��һ�ŷ�)
//#define ASS_SEND_ALL_CARD				54				//����������(һ�·���ȫ��)
//#define ASS_SEND_CARD_MSG				55				//�����г��ֵ���Ϣ(ץ����һ����Ϊׯ�һ��ߵ�һ������)
//#define ASS_SEND_FINISH					58				//�������
//
////�����߼�
//#define ASS_GAINST_CONTRIBUTE			62				//����
//#define ASS_CONTRIBUTE					63				//����Ϲ�
//#define ASS_CONTRIBUTE_RESLULT			64				//����˹����
//#define ASS_CONTRIBUTE_FINISH			65				//�˹����
//#define ASS_MOVE_BACK_CONTRIBUTE		66				//�˹�
//#define ASS_MOVE_BACK_RESULT			67				//�˹����
//#define ASS_MOVE_BACK_FINISH			68				//�˹�����
//#define ASS_CHANGE_CARD_RESULT			69				//�������
//#define ASS_CHOICE_CARD					70				//ѡ��
//#define ASS_CHOICE_CARD_RESULT			71				//ѡ�ƽ��
////��Ϸ��ʼ����Ϣ����
//#define ASS_GAME_PLAY					77				//��ʼ��Ϸ
//#define ASS_OUT_CARD_RESULT				78				//�û����ƽ��
//#define ASS_OUT_CARD					80			//�û�����
//#define ASS_SUPERSEDE_OUT_CARD			81				//�������
//
//#define ASS_AWARD_POINT					83				//�û�����
//#define ASS_PUNISH_POINT				84				//����
//#define ASS_GAME_POINT					85				//�Ƿ��з�
//
//#define ASS_LAST_CARD_REMIND			86				//���һ��������
//#define ASS_OUT_CARD_FINISH				87				//�û�������
//#define ASS_NT_TWO						88				//��ʾ������ѵ���Ϣ
//#define ASS_NEW_TURN					89				//��һ�ֿ�ʼ
////˦��(������ʹ��)
//#define ASS_SHOW_CARD					90				//˦��ʧ��
//#define ASS_BATCH_OUT					91				//��������
//
//#define ASS_CONTINUE_END				95				//��Ϸ����
//#define ASS_NO_CONTINUE_END				96				//��Ϸ����
//#define ASS_CUT_END						97				//�û�ǿ���뿪
//#define ASS_SALE_END					98				//��Ϸ��ȫ����
//
////����
//#define ASS_MESSAGE						100				//�ӷ���˷���������Ϣ
//#define ASS_AUTO						101				//�й�
//#define ASS_HAVE_THING					102				//����Ҫ��
//#define ASS_LEFT_RESULT					103				//����Ҫ�߽��
//#define ASS_STOP_THING					104				//��ǰ����
//#define ASS_AGREE_STOP					105				//��ǰ�������
//#define ASS_LOSE						106				//Ͷ��
//#define ASS_LOSE_RESULT					107				//Ͷ�����
//#define ASS_BUY_PROP					108				//�����
//#define ASS_SUPER_CLIENT				109				//�����ͻ���
//#define ASS_OUT_CARD_ERROR              110             //�ͻ��˳��ƴ���
/********************************************************************************/

#endif