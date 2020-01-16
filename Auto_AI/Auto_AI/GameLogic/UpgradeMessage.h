#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#pragma pack( 1 )
//�ļ����ֶ���

#define GAMENAME						TEXT( "����ţţ" )
#define NAME_ID							12150006						//���� ID
#define PLAY_COUNT						6								//��Ϸ����
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable"
//��Ϸ��Ϣ


#define GET_STRING( NUM )                 #NUM
#define INT_TO_STR( NUM )                 GET_STRING( NUM )
#define CLIENT_DLL_NAME                 TEXT( INT_TO_STR( NAME_ID )##".ico" )        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT( INT_TO_STR( NAME_ID ) )                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT( "CardSkin" )		                 // ����ͼƬ�ļ���


//��Ϸ�����汾
#define DEV_HEIGHT_VERSION				3								//�ڲ������߰汾��
#define DEV_LOW_VERSION					10 								//�ڲ������Ͱ汾��

//��Ϸ״̬����
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_ROB_NT						20              //��ׯ״̬
#define GS_NOTE                         21				//�µ�ע״̬
#define GS_SEND_CARD					22				//����״̬
#define GS_OPEN_CARD					23				//��ţ״̬
#define GS_GAME_BEGIN					24				//��Ϸ��ʼ״̬
#define GS_WAIT_NEXT					25				//�ȴ���һ�̿�ʼ 

#define GS_OPEN_CUO_PAI					1				//�û�����˴���
#define GS_OPEN_FAN_PAI					2				//�û�����˷���
#define GS_OPEN_TI_SHI					3				//�û��������ʾ

//��ע����
#define STATE_NULL					0x00			//��״̬-����״̬-
#define STATE_PLAY_GAME				0x01			//������Ϸ��
#define STATE_HAVE_NOTE				0x02			//�Ѿ���ע״̬
#define STATE_OPEN_CARD				0x03			//�Ѿ���ţ��
#define STATE_HAVE_ROBNT			0x04			//�Ѿ���ׯ״̬


#define MAX_SHAPE_COUNT                 0x15            //������������
#define SH_USER_CARD					5               //�Ƶ�����


//��Ϸ�淨
enum Em_Play_Mode
{
	Mode_QZ_ZYQZ					=	0x00000001, 				//��ׯ-������ׯ
	Mode_QZ_NNSZ					=	0x00000002, 				//��ׯ-ţţ��ׯ
	Mode_QZ_GDZJ					=	0x00000004, 				//��ׯ-�̶�ׯ��
	Mode_QZ_MPQZ					=	0x00000008, 				//��ׯ-������ׯ
	Mode_QZ_TBZZ					=	0x00000010, 				//��ׯ-ͨ��ţţ

	Mode_DF_CHOOSE1					=	0x00000020, 				//�׷�-ѡ��1
	Mode_DF_CHOOSE2					=	0x00000040, 				//�׷�-ѡ��2
	Mode_DF_CHOOSE3					=	0x00000080, 				//�׷�-ѡ��3
	Mode_DF_CHOOSE4					=	0x00000100, 				//�׷�-ѡ��4

	Mode_SHAPE_SHUN_ZI				=	0x00000200, 				//����-˳��ţ
	Mode_SHAPE_WU_HUA				=	0x00000400, 				//����-�廨ţ
	Mode_SHAPE_TONG_HUA				=	0x00000800, 				//����-ͬ��ţ
	Mode_SHAPE_HU_LU				=	0x00001000, 				//����-��«ţ
	Mode_SHAPE_ZHA_DAN				=	0x00002000, 				//����-ը��ţ

	Mode_MAX_QZ_CHOOSE1				=	0x00004000, 				//�����ׯ-1��
	Mode_MAX_QZ_CHOOSE2				=	0x00008000, 				//�����ׯ-2��
	Mode_MAX_QZ_CHOOSE3				=	0x00010000, 				//�����ׯ-3��
	Mode_MAX_QZ_CHOOSE4				=	0x00020000, 				//�����ׯ-4��

	Mode_TZ_CHOOSE1					=	0x00040000, 				//��ע-0��
	Mode_TZ_CHOOSE2					=	0x00080000, 				//��ע-5��
	Mode_TZ_CHOOSE3					=	0x00100000, 				//��ע-10��
	Mode_TZ_CHOOSE4					=	0x00200000, 				//��ע-15��

	Mode_GJ_NOT_IN					=	0x00400000, 				//�߼�-��ֹ����
	Mode_GJ_NO_CUO_PAI				=	0x00800000, 				//�߼�-��ֹ����
	Mode_GJ_NOTE_LIMIT				=	0x01000000, 				//�߼�-��ע����

	Mode_SHAPE_TONG_HUA_SHUN		=	0x02000000, 				//����-ͬ��˳

	Mode_ZF_FANG_ZHU				=	0x04000000, 				//֧��-����Ϊ1����֧������ȻΪAA֧��

	Mode_FAN_BEI_322				=	0x08000000, 				//����-����Ϊ322ģʽ����ȻΪ4322ģʽ

	Mode_SZ_0						=	0x10000000,					//��ׯ����-0��
	Mode_SZ_100						=	0x20000000,					//��ׯ����-100��
	Mode_SZ_150						=	0x40000000,					//��ׯ����-150��
	Mode_SZ_200						=	0x80000000,					//��ׯ����-200��
};

//����ţţ���ͣ����������
enum  Em_NN_Type
{
	NN_ZhaDan		= 0, 			//ը��ţ
	NN_HuLu			= 1, 			//��«ţ
	NN_TongHua		= 2, 			//ͬ��ţ
	NN_WuHua		= 3, 			//�廨ţ
	NN_ShunZi		= 4, 			//˳��ţ
	NN_TongHuaShun	= 5				//ͬ��˳ţ
};

const int INVALID = 255;

/********************************************************************************/

//��Ϸ���ݰ�
/********************************************************************************/
/*------------------------------------------------------------------------------*/
/// ֪ͨ�ͻ����Ƿ�Ϊ�����ͻ���״̬��Ϣ�ṹ
typedef struct SuperUserState
{
	BYTE byDeskStation;      /**< ���λ�� */
	bool bEnable;           /**< �Ƿ񳬶� */
	SuperUserState( )
	{
		memset( this, 0, sizeof( SuperUserState ) );
		byDeskStation = 255;
	}
}SUPERSTATE;
/*------------------------------------------------------------------------------*/
struct SuperUserSetData
{
	BYTE	byDeskStation;      //���λ�� 
	bool	bSetSuccess;		//�Ƿ����óɹ���
	int		iShape;				//�������õ�����
	SuperUserSetData( )
	{
		memset( this, 0, sizeof( SuperUserSetData ) );
		byDeskStation = INVALID;
	}
};


/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameStation_Base
{
	BYTE				byGameStation;					//��Ϸ״̬
	unsigned int		iPlayMode;						//����ѡ�� 
	int					iVipGameCount;					//�������Ӿ���
	int					iRunGameCount;					//��Ϸ���еľ���

	//��Ϸ��Ϣ
	BYTE				iThinkTime;						//̯��ʱ��
	BYTE				iBeginTime;						//׼��ʱ��
	BYTE				iSendCardTime;					//����ʱ��( ms )
	BYTE				iXiaZhuTime;					//��עʱ��
	BYTE				iRobNTTime;					    //��ׯʱ��
	BYTE				iTickTime;						//����ʱ��

	BYTE				iAllCardCount;					//�˿���Ŀ

	int					iRoomBasePoint;					//���䱶��
	int					iBaseNote;						//��ע

	bool				bSystemOperate;					//�Ƿ�ϵͳ����
	bool				bAuto[PLAY_COUNT];				//����Ƿ��й�

	GameStation_Base( )
	{
		memset( this, 0, sizeof( GameStation_Base ) );
		bSystemOperate = true;

		iBeginTime = 5;
		iThinkTime = 10;
		iXiaZhuTime = 15;
		iRobNTTime = 5;
		iTickTime = 15;
		iSendCardTime = 5;
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ & ��Ϸ��ʼ��
struct GameStation_WaiteAgree : public GameStation_Base
{
	bool                bUserReady[PLAY_COUNT] ;        //����Ƿ���׼��
	BYTE				byNTStation;					//ׯ��λ�ã�255����ʾ

	GameStation_WaiteAgree( )
	{
		memset( this, 0, sizeof( GameStation_WaiteAgree ) );
		byNTStation = 0xFF;
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ��Ϸ��ʼ �� // Not Used
struct GameStation_GameBegin : public GameStation_Base
{
	bool                bUserReady[PLAY_COUNT] ;        //����Ƿ���׼��
	GameStation_GameBegin( )
	{
		memset( this, 0, sizeof( GameStation_GameBegin ) );
	}
};

/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ��ׯ״̬ ��
struct GameStation_RobNt : public GameStation_Base
{
	BYTE			byUserRobNT[PLAY_COUNT];				//�������ׯ���	-1-��ʾ��û���� 0-��ʾ���� 1-4��ʾ�Ѿ���ׯ�ı���
	int				iUserStation[PLAY_COUNT];				//�����״̬ ����Ƿ���;�����
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�
	BYTE			byRemainTime;							//ʣ��ʱ��
	
	GameStation_RobNt( )
	{
		memset( this, 0, sizeof( GameStation_RobNt ) );
		memset( byUserRobNT, INVALID, sizeof( byUserRobNT ) );
	}
};

/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ��ע״̬ ��
struct GameStation_Note : public GameStation_Base
{
	BYTE			iStartPos;								//ׯ��λ��
	int				iUserStation[PLAY_COUNT];				//�����״̬
	int				iUserBase[PLAY_COUNT];					//�û���ע������ׯ��ׯ�Ҷ�Ӧֵ������ׯ����
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�
	BYTE			byRemainTime;							//ʣ��ʱ��
	int				iCanNote[3];							//����ע�ķ���
	
	GameStation_Note( )
	{
		memset( this, 0, sizeof( GameStation_Note ) );
		memset( iUserStation, -1, sizeof( iUserStation ) );
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ����״̬ ��
struct GameStation_SendCard : public GameStation_Base
{
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�
	int				iUserStation[PLAY_COUNT];				//�����״̬ ����Ƿ���;�����
	int				iUserBase[PLAY_COUNT];					//�û�ÿ��ѹ��ע����ׯ��ׯ�Ҷ�Ӧֵ������ׯ����
	BYTE			byNTStation;							//ׯ��λ��
	GameStation_SendCard( )
	{
		memset( this, 0, sizeof( GameStation_SendCard ) );
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ̯��״̬ ��
struct GameStation_OpenCard : public GameStation_Base
{
	BYTE			byNtStation;							//ׯ��λ��
	int				iUserStation[PLAY_COUNT];				//�����״̬
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�

	BYTE			byOpenUnderCard[PLAY_COUNT][3];			//���Ƶ�������
	BYTE			byOpenUpCard[PLAY_COUNT][2];			//��������2����
	int				iOpenShape[PLAY_COUNT];					//��ţ����
	int				imultiple[PLAY_COUNT];					//��ţ����

	int				iUserBase[PLAY_COUNT];					//�û���������ׯ��ׯ�Ҷ�Ӧֵ������ׯ����

	BYTE			byRemainTime;							//ʣ��ʱ��

	BYTE			bySonStation;							//̯���ڼ����״̬���û��Ƿ����˴��ƺͷ���
	
	GameStation_OpenCard( )
	{
		memset( this, 0, sizeof( GameStation_OpenCard ) );
		bySonStation = INVALID;
	}
};
/*------------------------------------------------------------------------------*/

//֪ͨ�ͻ�����ע
struct TMSG_NOTE_NFT
{
	int                 iNt;                            //ׯ��
	int					iCanNote[PLAY_COUNT][3];		//����ע�ķ�����0ΪĬ��ֵ������ʾ
	int					iNtRobMul;						//ׯ����ׯ����
	TMSG_NOTE_NFT()
	{
		memset(this, 0, sizeof(*this));
		iNt = INVALID;
		iNtRobMul = 1;
	}
};

/*-------------------------------------------------------------------------------*/

/// �������ݰ���һ�ν��˿�ȫ�������ͻ���
struct SendAllCardStruct
{
	BYTE      iStartPos;                  //������ʼλ��
	BYTE      iUserCard[PLAY_COUNT][SH_USER_CARD];   //�û��˿�
	BYTE      iUserCardCount[PLAY_COUNT];

	SendAllCardStruct( )
	{
		memset( iUserCard, 0, sizeof( iUserCard ) );
		memset( iUserCardCount, 0, sizeof( iUserCardCount ) );
		iStartPos = INVALID;
	}
};
/*-------------------------------------------------------------------------------*/
//֪̯ͨ�����ݰ�
struct TNoticeOpenCard
{
	BYTE				byDeskStation;							//֪̯ͨ����ҵ�λ��
	BYTE				byUserState[PLAY_COUNT];				//����ҵ�״̬( ����Ƿ���;����� ����һֱ����Ϸ���е���� )

	TNoticeOpenCard( )
	{
		memset( this, 0, sizeof( TNoticeOpenCard ) );
		byDeskStation = INVALID;
		memset(byUserState, INVALID, sizeof(byUserState));
	}
};

/*
//�û�׼�����ݰ�
struct UserAgreeStruct
{
	BYTE				bDeskStation;					//λ��
	bool				bUserAgree[PLAY_COUNT];			//�û�ͬ��

	UserAgreeStruct( )
	{
		bDeskStation = INVALID;
		memset( bUserAgree, false, sizeof( bUserAgree ) );
	}
};
*/
//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	int					iUserState[PLAY_COUNT];					//���״̬( ��ǰ����, ������ )
	__int64				iChangeMoney[PLAY_COUNT];				//��Ӯ����
};

//֪ͨ�ͻ�����ׯ
struct TMSG_ROB_NT_NTF
{
	BYTE byStation;			//֪ͨ��ׯ���
	TMSG_ROB_NT_NTF()
	{
		byStation = INVALID;
	}
};
//�ͻ���������ׯ
struct TMSG_ROB_NT_REQ
{
	int	iValue;									//��ׯ���ͣ�0����ׯ, 1-4��ׯ������
	TMSG_ROB_NT_REQ()
	{
		iValue = 0;
	}
};
//����˻ظ���ׯ���
struct TMSG_ROB_NT_RSP
{
	BYTE	byStation;								//��ׯ���
	int		iValue;									//��ׯ���ͣ�0����ׯ, 1-4��ׯ������
	int		iSuccess;								//�Ƿ�ɹ���ׯ��0��ʾ�ɹ���
	TMSG_ROB_NT_RSP()
	{
		memset(this, 0, sizeof(*this));
		byStation = INVALID;
	}
};


//��ׯ�������ݰ�
struct RobNTResultStruct
{	
	BYTE				bNTDeskStation;							//ׯ��λ��
	BYTE				byUserState[PLAY_COUNT];				//����ҵ�״̬( ����Ƿ���;����� ����һֱ����Ϸ���е���� )
	BYTE				bCatchNum[PLAY_COUNT];					//�������ׯ����
	bool				bShowDing;								//�Ƿ���������ֵ

	RobNTResultStruct( )
	{
		memset( this, 0, sizeof( RobNTResultStruct ) );
		memset( byUserState, INVALID, sizeof( byUserState ) );
		memset( bCatchNum, INVALID, sizeof( bCatchNum ) );
		bShowDing = false;
	}
};
/*----------------------------------------------------------*/
//���̯�ƽṹ��
struct UserTanPai
{
	BYTE	byDeskStation;		//̯����ҵ�λ��
	BYTE	byUnderCard[3];		//���Ƶ�������
	BYTE	byUpCard[2];		//��������2����
	int		iShape;				//��ţ����
	int		imultiple;			//��ţ����

	UserTanPai( )
	{
		memset( this, 0, sizeof( UserTanPai ) );
		byDeskStation = INVALID;
		imultiple = 1;
	}
};

//�û���ע����
struct TMSG_NOTE_REQ
{
	int  iNoteType;				//��ע��
	TMSG_NOTE_REQ( )
	{
		memset( this, 0, sizeof( *this ) );
	}
};

//��ע�ظ�
struct TMSG_NOTE_RSP
{
	BYTE	byStation;			//��ע���
	int		iNoteValue;			//��עֵ
	TMSG_NOTE_RSP()
	{
		memset(this, 0, sizeof(*this));
		byStation = INVALID;
	}
};

struct CalculateBoardItem
{
	bool	bWinner;				//�Ƿ��Ӯ��
	bool	bLoser;					//�Ƿ�Ϊ����
	char	chUserName[64];			//�û��ǳ�
	int		iUserID;				//���ID
	int		iZuoZhuangCount;		//��ׯ����
	int		iQiangZhuangCount;		//��ׯ����
	int		iTuiZhuCount;			//��ע����
	__int64	i64WinMoney;			//�ܳɼ�
	CalculateBoardItem()
	{
		memset( this, 0, sizeof(*this) );
	}
};

struct	CalculateBoard
{
	CalculateBoardItem tItem[PLAY_COUNT];
	CalculateBoard( )
	{
		memset( this, 0, sizeof( *this ) );
	}
};

//��Ҵ�������
struct TMSG_CUO_PAI_REQ
{
	BYTE byUser;
	TMSG_CUO_PAI_REQ( )
	{
		memset( this, 0, sizeof( *this ) );
		byUser = INVALID;
	}
};

//��ҷ�������
struct TMSG_FAN_PAI_REQ
{
	BYTE byUser;
	TMSG_FAN_PAI_REQ( )
	{
		memset( this, 0, sizeof( *this ) );
		byUser = INVALID;
	}
};

//��Ҵ��ƻظ�
struct TMSG_CUO_PAI_RSP
{
	BYTE byUser;
	TMSG_CUO_PAI_RSP( )
	{
		memset( this, 0, sizeof( *this ) );
		byUser = INVALID;
	}
};

//��ҷ��ƻظ�
struct TMSG_FAN_PAI_RSP
{
	BYTE byUser;
	TMSG_FAN_PAI_RSP( )
	{
		memset( this, 0, sizeof( *this ) );
		byUser = INVALID;
	}
};

//�ͻ���������ʾţţ����
struct TMSG_TI_SHI_REQ
{
	BYTE byStation;				//�������
	TMSG_TI_SHI_REQ()
	{
		byStation = INVALID;
	}
};
//�������ظ�ţţ����
struct TMSG_TI_SHI_RSP
{
	BYTE	byStation;			//�ظ����
	BYTE	byUnderCard[3];		//���Ƶ�������
	BYTE	byUpCard[2];		//��������2����
	int		iShape;				//��ţ����
	TMSG_TI_SHI_RSP()
	{
		memset(this, 0, sizeof(*this));
		byStation = INVALID;
		memset(byUpCard, INVALID, sizeof(byUpCard));
		memset(byUnderCard, INVALID, sizeof(byUnderCard));
	}
};
//�������̯�ƽ���֪ͨ�����������ʾׯ������
struct TMSG_ALL_TAN_PAI_FINISH_NTF
{
	BYTE	byNTDeskStation;	//ׯ�ҵ�λ��
	BYTE	byUnderCard[3];		//ׯ�ҵ��Ƶ�������
	BYTE	byUpCard[2];		//ׯ����������2����
	int		iShape;				//ׯ�Ұ�ţ����
	int		imultiple;			//��ţ����
	TMSG_ALL_TAN_PAI_FINISH_NTF()
	{
		memset(this, 0, sizeof(*this));
		byNTDeskStation = INVALID;
		memset(byUnderCard, INVALID, sizeof(byUnderCard));
		memset(byUpCard, INVALID, sizeof(byUpCard));
		iShape = 0;
	}
};
//��������ʼ��Ϸ
struct TMSG_MASTER_START_GAME_REQ
{
	bool bCanBegin;					//�ܷ�ʼ��Ϸ
	TMSG_MASTER_START_GAME_REQ()
	{
		memset(this, 0, sizeof(*this));
	}
};
//���������Ƿ�ɹ�
struct TMSG_MASTER__RSP
{
	int	iSuccess;					//Ĭ��0��ʾ�ɹ����Ƿ���������쳣���ش���
	TMSG_MASTER__RSP()
	{
		memset(this, 0, sizeof(*this));
	}
};
		
//��һ����Ϸ����
struct TMSG_LAST_GAME_REQ
{
	BYTE byUser;					//�������
	TMSG_LAST_GAME_REQ()
	{
		memset(this, 0, sizeof(*this));
		byUser = 255;
	}
};
//��һ����Ϸ�ظ�
struct TMSG_LAST_GAME_RSP
{
	int					iSuccess;								//Ĭ��0��ʾ�ɹ�������ǵ�һ�ֵ��쳣���ش���
	BYTE				byNTDeskStation;						//ׯ�ҵ�λ��
	BYTE				byUnderCard[PLAY_COUNT][3];				//���Ƶ�������
	BYTE				byUpCard[PLAY_COUNT][2];				//��������2����
	int					iShape[PLAY_COUNT];						//��ţ����
	int					imultiple[PLAY_COUNT];					//��ţ����
	__int64				iChangeMoney[PLAY_COUNT];				//��Ӯ����
	int					iNoteValue[PLAY_COUNT];					//��עֵ
	TMSG_LAST_GAME_RSP()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0, sizeof(*this));
		byNTDeskStation = INVALID;
		memset(byUnderCard, INVALID, sizeof(byUnderCard));
		memset(byUpCard, INVALID, sizeof(byUpCard));
	}
};

//������������ͻظ�
struct TMSG_GIFT_REQ_AND_RSP
{
	BYTE bySender;				//���ͷ�
	BYTE byReceiver;			//���շ�
	bool bSendAll;				//�Ƿ������������
	int iGiftNo;				//������
	TMSG_GIFT_REQ_AND_RSP()
	{
		memset(this, 0, sizeof(*this));
		bySender = INVALID;
		byReceiver = INVALID;
	}
};

//�й�����
struct TMSG_AUTO_REQ
{
	BYTE	byUser;							//�������
	bool	bAuto;							//�Ƿ��й�
	TMSG_AUTO_REQ()
	{
		memset(this, 0, sizeof(*this));
		byUser = INVALID;
	}
};
//�йܻظ�
struct TMSG_AUTO_RSP
{
	BYTE	byUser;							 //�������
	bool	bAuto;							 //�Ƿ��й�
	TMSG_AUTO_RSP()
	{
		memset(this, 0, sizeof(*this));
		byUser = INVALID;
	}
};

//AI����֪ͨ
struct TMSG_AI_CARD_SHAPE
{
	BYTE	byUser;							//�������
	int		iRobNT;							//��ׯ
	int		iNote;							//��ע
	int		iShape;							//����
	TMSG_AI_CARD_SHAPE()
	{
		Clear();
	}
	void Clear()
	{
		memset(this, 0, sizeof(*this));
		byUser = INVALID;
	}
};

//-------------------------------------------------------------------------------


/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
#define ASS_CALL_NOTE					55				//֪ͨ��ע		
#define ASS_USER_NOTE					56				//�����ע		
#define ASS_CALL_NOTE_RESULT			57				//֪ͨ��ע���	
#define ASS_CALL_SEND_CARD				58				//֪ͨ������Ϣ	
#define ASS_CALL_SEND_FINISH			59				//���귢����		
#define ASS_CALL_OPEN					60				//֪ͨ���ư�ţ	
#define ASS_USER_OPEN					61				//��Ұ�ţ		
#define ASS_CALL_OPEN_RESULT			62				//��Ұ�ţ���	
#define ASS_CALL_ROBNT			        63				//֪ͨ��ׯ	    
#define ASS_USER_ROBNT			        64				//�����ׯ	    

#define ASS_CONTINUE_END				65				//��Ϸ����

#define	S_C_UPDATE_CALCULATE_BOARD_SIG	66
#define	S_C_UPDATE_CURRENT_JUSHU_SIG	67

#define ASS_CALL_ROBNT_RESULT			68				//֪ͨ��ׯ���	
#define ASS_CALL_ROBNT_FINISH			69				//֪ͨ��ׯ����	

const int MSG_GAME_BEGIN_NTF		=	70;				//��Ϸ��ʼ֪ͨ
//#define ASS_GAME_ZHUNBEI				71				//�û�׼��
#define ASS_GAME_AUTO_BEGIN				72				//�Զ���ʼ֪ͨ

#define ASS_SUPER_USER					73				//֪ͨ�Ƿ�Ϊ�������
#define ASS_SUPER_USER_SET				74				//�����û���������
#define ASS_SUPER_USER_SET_RESULT		75				//�������ý����Ϣ

const int MSG_CUO_PAI_REQ			=	76;				//�ͻ��˴�������	
const int MSG_CUO_PAI_RSP =	MSG_CUO_PAI_REQ;			//����˴��ƻظ�	
const int MSG_FAN_PAI_REQ			=	77;				//�ͻ��˷�������	
const int MSG_FAN_PAI_RSP =	MSG_FAN_PAI_REQ;			//����˷��ƻظ�
const int MSG_TI_SHI_REQ			=	78;				//�ͻ���������ʾţţ����
const int MSG_TI_SHI_RSP = MSG_TI_SHI_REQ;				//�������ظ�ţţ����

const int MSG_ALL_TAN_PAI_FINISH_NTF =	79;				//�������̯�ƽ���֪ͨ
const int MSG_MASTER_START_GAME_REQ	 =	80;				//��������ʼ��Ϸ
const int MSG_MASTER__RSP = MSG_MASTER_START_GAME_REQ;	//���������Ƿ�ɹ�

const int MSG_LAST_GAME_REQ			=	81;				//��һ����Ϸ����
const int MSG_LAST_GAME_RSP	= MSG_LAST_GAME_REQ;		//��һ����Ϸ�ظ�

const int MSG_SEND_GIFT_REQ			=	82;				//������������
const int MSG_SEND_GIFT_RSP	=	MSG_SEND_GIFT_REQ;		//��������ظ�

const int MSG_AUTO_REQ		=			83;				//�й�����
const int MSG_AUTO_RSP		= MSG_AUTO_REQ;				//�йܻظ�

const int MSG_AI_CARD_SHAPE =			84;				//AI����֪ͨ

#pragma pack( )
/********************************************************************************/
#endif
