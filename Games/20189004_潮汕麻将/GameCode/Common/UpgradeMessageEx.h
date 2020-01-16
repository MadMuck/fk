#pragma once
#include "../Common/UpgradeMessage.h"
#pragma pack(push,1)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#define MAIN_VERSION					710		// ���汾��,?��?��
#define ASS_VERSION						15		// �����汾��,?��
            

//�ļ����ֶ���
#define GAMENAME						TEXT("�����齫") 
#define NAME_ID						    20189004  		// ���� ID


static char szTempStr[MAX_PATH] =		{0};			/** �ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ�� */
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))
///////////////////////////////////////////////////////////////////////////////////////////////////

#define ROOM_ID                         1

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")  // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))          // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("CardSkin")		           // �ƹ���ͼƬ�ļ���
#define SKIN_MJ							TEXT("MjSkin")					   // �齫����ͼƬ�ļ���

	

#define MAX_HAND_PAI					14				//������


///ð������(�Ӵ�С����)
static void MaoPaoSort(int a[], int count,bool big)
{
	if(a == NULL)
        return ;
	for (int i =0; i < count - 1; i++ )
	{
	    for(int j = 0; j < count - i -1; j++)
		{
			if(big)
			{
				if(a[j] < a[j+1])
				{
					int iTempBig = a[j];
					a[j] = a[j+1];
					a[j+1] = iTempBig;		
				}
			}
			else
			{
				if(a[j] > a[j+1])
				{
					int iTempBig = a[j];
					a[j] = a[j+1];
					a[j+1] = iTempBig;		
				}
			}
		}	
	}
}

///ð������(�Ӵ�С����)
static void MaoPaoSort(uchar a[], int count,bool big)
{
	if(a == NULL)
        return ;
	for (int i =0; i < count - 1; ++i )
	{
	    for(int j = i+1; j < count; ++j)
		{
			if(big)
			{
				if(a[j] > a[i])
				{
					uchar iTempBig = a[j];
					a[j] = a[i];
					a[i] = iTempBig;		
				}
			}
			else
			{
				if(a[j] < a[i])
				{
					uchar iTempBig = a[j];
					a[j] = a[i];
					a[i] = iTempBig;		
				}
			}
		}	
	}
}

//ð������(�Ӵ�С)
static void MaoPaoSort(PaiStruct a[], int count)
{
	if(a == NULL)
        return ;
	PaiStruct iTempBig ;
	for (int i =0; i < count - 1; i++ )
	{
	    for(int j = i; j < count; j++)
		{
			if(a[i].count < a[j].count)
			{
				iTempBig.count= a[i].count;
				iTempBig.pai= a[i].pai;

				a[i].count = a[j].count;
				a[i].pai = a[j].pai;

				a[j].count = iTempBig.count;	
				a[j].pai = iTempBig.pai;	
			}
		}	
	}
}

///�����������ݽṹ
struct GCPStructEx:GCPStruct
{
	uchar  byBigGang[4][2]; //����������ݣ������齫��
	bool  bIsBigGang;      //�Ƿ���
	GCPStructEx()
	{
		Init();
	}
	void Init()
	{
		GCPStruct::Init();
		bIsBigGang = false;
		memset(byBigGang,255,sizeof(byBigGang));
	};
};



///�¼����ݰ�,���ص�������������Լ�����������
///��Ϸ��ʼ�¼�
struct tagBeginEx:tagBegin
{
	void Clear()
	{
		tagBegin::Clear();
	}
};

///�Զ�Ϊׯ�¼�
struct tagDongNtEx:tagDongNt
{
	void Clear()
	{
		tagDongNt::Clear();
	}
};

///��2��ɫ�ӵĵ�����Ϊׯ�¼�
struct tagTwoSeziNtEx:tagTwoSeziNt
{
	void Clear()
	{
		tagTwoSeziNt::Clear();
	}
};

///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
struct tagTwoSeziNtAndGetPaiEx:tagTwoSeziNtAndGetPai
{
	void Clear()
	{
		tagTwoSeziNtAndGetPai::Clear();
	}
};

///������2��ɫ��ȷ��ׯ��
struct tagTurnSeziNtEx:tagTurnSeziNt
{
	void Clear()
	{
		tagTurnSeziNt::Clear();
	}
};

///��2��ɫ��ȷ������λ���¼�
struct tagTwoSeziDirEx:tagTwoSeziDir
{
	void Clear()
	{
		tagTwoSeziDir::Clear();
	}
};

///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
struct tagTwoSeziDirAndGetPaiEx:tagTwoSeziDirAndGetPai
{
	void Clear()
	{
		tagTwoSeziDirAndGetPai::Clear();
	}
};

///��2��ɫ��ȷ�����ƶ����¼�
struct tagTwoSeziGetPaiEx:tagTwoSeziGetPai
{
	void Clear()
	{
		tagTwoSeziGetPai::Clear();
	}
};

///�����¼�
struct tagSendPaiEx:tagSendPai
{
	void Clear()
	{
		tagSendPai::Clear();
	}
};
///�����¼�
struct tagTiaoPaiEx:tagTiaoPai
{
	void Clear()
	{
		tagTiaoPai::Clear();
	}
};


///������Ҳ����¼�
struct tagAllBuHuaEx:tagAllBuHua
{
	void Clear()
	{
		tagAllBuHua::Clear();
	}
};

///������Ҳ����¼�
struct tagOneBuHuaEx:tagOneBuHua
{
	void Clear()
	{
		tagOneBuHua::Clear();
	}
};
///��ʼ����֪ͨ�¼�
struct tagBeginOutPaiEx:tagBeginOutPai
{
	void Clear()
	{
		tagBeginOutPai::Clear();
	}
};
///����֪ͨ
struct tagOutPai_NTF
{
	uchar bDeskStation;		//�������λ��

	void Clear()
	{
		bDeskStation = 255;
	}
};
///�����¼�
struct tagOutPaiEx:tagOutPai
{
	void Clear()
	{
		tagOutPai::Clear();
	}
};
///ץ�����¼�
struct tagZhuaPaiEx:tagZhuaPai
{
	void Clear()
	{
		tagZhuaPai::Clear();
	}
};
///�����ܺ�֪ͨ�¼��¼�
struct tagCPGNotifyEx:tagCPGNotify
{
	void Clear()
	{
		tagCPGNotify::Clear();
	}
};
///�������¼�
struct tagChiPaiEx:tagChiPai
{
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	void Clear()
	{
		tagChiPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};
///�������¼�
struct tagPengPaiEx:tagPengPai
{
	void Clear()
	{
		tagPengPai::Clear();
	}
};
///�����¼�
struct tagKanPaiEx:tagKanPai
{
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	void Clear()
	{
		tagKanPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���	
	}
};
///ɨ���¼����黢�齫ר�ã�
struct tagSaoHuEx:tagSaoHu
{
	void Clear()
	{
		tagSaoHu::Clear();
	}
};
///�������¼�
struct tagGangPaiEx:tagGangPai
{
	int			iGangFen[PLAY_COUNT];	//���θ��Ƶĵ÷����

	void Clear()
	{
		tagGangPai::Clear();
		memset(iGangFen, 0, sizeof(iGangFen));//���θ��Ƶĵ÷����
	}
};
///�������¼�
struct tagTingPaiEx:tagTingPai
{
	void Clear()
	{
		tagTingPai::Clear();
	}
};
///�������¼�
struct tagHuPaiEx:tagHuPai
{
	void Clear()
	{
		tagHuPai::Clear();
	}
};
///����¼�
struct tagCountFenEx:tagCountFen
{
	int 	iGangFen[4];//������ҵĸܷ�
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���

	bool    m_bDianPao[PLAY_COUNT];//�Ƿ����
	bool    m_bIsZiMo[PLAY_COUNT];//�Ƿ���������

	int		m_iZongFan[PLAY_COUNT];//�����ܷ���
	int		iTax;//����ѣ���˰��
	uchar	m_byPs[PLAY_COUNT]; // ������ֵ, 255��Ч

	int     m_iPaiTouCount[PLAY_COUNT];	//����ͷ��

	int     m_iPaiTouFen[PLAY_COUNT];	//��ͷ��

	int iDianGangFen[PLAY_COUNT];//��ܷ�
	int iMingGangFen[PLAY_COUNT];//���ܷ�
	int iAnGangFen[PLAY_COUNT];//���ܷ� 
	

	void Clear()
	{
		memset(iGangFen, 0, sizeof(iGangFen));//������ҵĸܷ�
		tagCountFen::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
		memset(m_bDianPao, 0, sizeof(m_bDianPao));	//�������λ��
		memset(m_bIsZiMo, 0, sizeof(m_bIsZiMo));	//�Ƿ���������
		memset(m_iZongFan, 0, sizeof(m_iZongFan));	//�����ܷ���
		iTax = 0;//����ѣ���˰��
		memset(m_byPs, 255, sizeof(m_byPs));
		memset(m_iPaiTouCount, 0, sizeof(m_iPaiTouCount));
		memset(m_iPaiTouFen, 0, sizeof(m_iPaiTouFen));
		memset(iMingGangFen,0,sizeof(iDianGangFen));
		memset(iAnGangFen,0,sizeof(iAnGangFen));
		memset(iDianGangFen,0,sizeof(iDianGangFen));
	}
};
///��Ϸ���������¼�
struct tagEndHandleEx:tagEndHandle
{
	void Clear()
	{
		tagEndHandle::Clear();
	}
};
///���������������¼�
struct tagUnnormalEndHandleEx:tagUnnormalEndHandle
{
	void Clear()
	{
		tagUnnormalEndHandle::Clear();
	}
};
///////////////////////////////////////////////////

///�����¼����ݰ�
struct tagChangePsEx
{
	uchar byUser;		//�������
	uchar byChangePs[3];	//�ṩ����
	
	bool bNotifyFinish;	//�Ƿ�����ṩ����

	bool bNotify;					//�Ƿ�֪ͨ���������Ҫ׼���ṩ3����ֵ

	tagChangePsEx()
	{
		Clear();
	}

	void Clear()
	{
		byUser = 255;
		memset(byChangePs,255,sizeof(byChangePs));
		bNotifyFinish = false;
		bNotify = false;

	}
	
};

///��ȱ�¼����ݰ�
struct tagDingQueEx
{
	uchar byUser; //��ȱ���
	uchar byQue; //ȱ����
	uchar byQuePai[PLAY_COUNT]; //ȱ����
	bool  bNotify; // �Ƿ��������ȱ֪ͨ
	bool  bFinish[PLAY_COUNT];//�Ƿ��Ѿ���ɶ�ȱ
	uchar m_byArHandPai[4][HAND_CARD_NUM];//��������
	tagDingQueEx()
	{
		Clear();
	}
	void Clear()
	{
		byUser = 255; //��ȱ���
		byQue = 255; //ȱ����
		memset(byQuePai, 255, sizeof(byQuePai));//
		memset(bFinish, 0, sizeof(bFinish));////�Ƿ��Ѿ���ɶ�ȱ
		bNotify = false; // �Ƿ��������ȱ֪ͨ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������
	}
};

///�����������ݰ�
struct GameStationDataEx:GameStationData
{
	GCPStructEx			m_UserGCPData[4][5];			//��ҳ�������	 
    uchar               bAutoBegin;     
	uchar				m_byFengDing;//�ⶥ����Ĭ��3��
	bool				m_bShowTax;//�Ƿ���ʾ̨��
	int					m_iBaseFen;//��Ϸ�����֣�Ĭ��Ϊ1
	bool				m_bForceFinish;//�Ƿ�ǿ�˽����汾
	bool				m_bChengBao;//�ֳа��ƺͼҼ���
	uchar				m_RemainingCards; //ʣ������
	uchar				byDirection;	//���Ʒ�λ
	uchar				byGetPai;		//���Ƶ�
	uchar				bySezi0;	//ɫ��0����
	uchar				bySezi1;	//ɫ��1����
	tagPM				m_tagPM;	//�淨

	GameStationDataEx()
	{		
		Init();
	};
	void Init()
	{
		memset(this, 0, sizeof(GameStationDataEx));	
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		memset(m_byArOutPai,255,sizeof(m_byArOutPai));
		memset(m_iArMenPai,255,sizeof(m_iArMenPai));
		memset(m_UserGCPData,255,sizeof(m_UserGCPData));

		m_byFengDing = 20;//�ⶥ����Ĭ��3��
		m_bShowTax = 1;//�Ƿ���ʾ̨��
		m_iBaseFen = 1;//��Ϸ�����֣�Ĭ��Ϊ1
		m_bForceFinish = false;//�Ƿ�ǿ�˽����汾
		m_bChengBao = true;//�ֳа��ƺͼҼ���
		byDirection = 255;
		byGetPai = 255;
		bySezi0 = 255;
		bySezi1 = 255;
	}
};

struct GameStationData_Dice : GameStationData_Base
{
	uchar				byNtStation;	//ׯ��λ��
	uchar				byDirection;	//���Ʒ�λ
	uchar				byGetPai;		//���Ƶ�
	uchar				bySezi0;		//ɫ��0����
	uchar				bySezi1;		//ɫ��1����

	GameStationData_Dice()
	{
		byNtStation = 255;
		byDirection = 255;
		byGetPai = 255;
		bySezi0 = 255;
		bySezi1 = 255;
	};
};

struct GameStationData_SendCard : GameStationData_Dice
{
	uchar				m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//���Ʋ�����20�ţ�0Ϊ������־
	uchar				m_RemainingCards; // ʣ������
	uchar				m_byFanCard;			//����������
	uchar				m_byJoker;				//������

	GameStationData_SendCard()
	{
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		m_RemainingCards = 0;
		m_byFanCard = 255;
		m_byJoker   = 255;
	}
};

struct GameStationData_Playing : GameStationData_SendCard
{
	bool				m_bistuoguan[PLAY_COUNT];		// �Ƿ��й�
	uchar				m_byNowOutStation;				// ���ڳ����û�
	uchar				m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//���Ʋ�����160�ţ�0Ϊ������־
	GCPStructEx			m_UserGCPData[PLAY_COUNT][5];			//��ҳ�������	
	bool				m_bIsHu[PLAY_COUNT];			//�Ƿ����
	uchar				m_byHuCard;						//������

	GameStationData_Playing()
	{
		memset(m_bistuoguan,false,sizeof(m_bistuoguan));
		m_byNowOutStation = 255;
		memset(m_byArOutPai,0,sizeof(m_byArOutPai));
		memset(m_bIsHu,false,sizeof(m_bIsHu));
		m_byHuCard = 255;
	}
};

///�����������ݰ�
struct GangFenRecordEx
{
	int fen[5][PLAY_COUNT+1];
	int count;//����
	GangFenRecordEx()
	{
		Init();
	};
	void Init()
	{
		memset(this,0,sizeof(GangFenRecordEx));
	}
};

//֪ͨ�ж���
struct SC_NotifyHaveAction
{
	uchar byDeskStation;//����λ��
	void Clear()
	{
		byDeskStation=255;
	}
};

struct SC_RemainingGameCount
{
	int remaingameCount;	// ʣ�����
	int total;				// �ܾ���
};

struct TingInfo
{
	uchar Value;	//����ֵ
	uchar Count;	//ʣ������
	uchar Fan;		//����

	void Clear()
	{
		Value = 255;
		Count = 0;
		Fan   = 0;
	}
};

struct WaitOutCard
{
	uchar     OutCard;			//���ƿ��Գ�����ֵ
	TingInfo  TingCards[MAX_TING_NUM];	//������Ϣ

	void Clear()
	{
		OutCard	 = 255;
		for(int i = 0; i < sizeof(TingCards) / sizeof(TingInfo); ++i)
		{
			TingCards[i].Clear();
		}
	}
};

struct WaitOutCards
{
	WaitOutCard OutCards[HAND_CARD_NUM];

	WaitOutCards()
	{
		Clear();
	}

	void Clear()
	{
		for(int i = 0; i < sizeof(OutCards) / sizeof(WaitOutCard); ++i)
		{
			OutCards[i].Clear();
		}
	}
};

struct HuanZhangFinish
{
	uchar m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM]; //��������
	uchar m_byHuanZhangData[PLAY_COUNT][3];			//��һ�3�ŵõ�����
	uchar m_bySelectData[PLAY_COUNT][3];			//��һ�3��ѡ�����

	void Clear()
	{
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
		memset(m_byHuanZhangData, 255, sizeof(m_byHuanZhangData));
		memset(m_bySelectData, 255, sizeof(m_bySelectData));
	}
};

struct PaiTou
{
	uchar m_byPaiTou[MAX_PAI_TOU];				//��������ͷ
	uchar m_byZhongPaiTou[PLAY_COUNT][MAX_PAI_TOU];			//�е���ͷ

	PaiTou()
	{
		memset(m_byPaiTou, 255, sizeof(m_byPaiTou));
		memset(m_byZhongPaiTou, 255, sizeof(m_byZhongPaiTou));
	}
};

#pragma pack(pop)
