#pragma once
#include "../Common/UpgradeMessage.h"
#pragma pack(push,1)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#define MAIN_VERSION					710		// 主版本号,?年?月
#define ASS_VERSION						15		// 辅助版本号,?号
            

//文件名字定义
#define GAMENAME						TEXT("潮汕麻将") 
#define NAME_ID						    20189004  		// 名字 ID


static char szTempStr[MAX_PATH] =		{0};			/** 暂存字符串，用来获取 dll_name 等的时候使用 */
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))
///////////////////////////////////////////////////////////////////////////////////////////////////

#define ROOM_ID                         1

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")  // 客户端ico名字
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))          // 客户端目录
#define SKIN_CARD						TEXT("CardSkin")		           // 牌共用图片文件夹
#define SKIN_MJ							TEXT("MjSkin")					   // 麻将共用图片文件夹

	

#define MAX_HAND_PAI					14				//手牌数


///冒泡排序(从大到小排序)
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

///冒泡排序(从大到小排序)
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

//冒泡排序(从大到小)
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

///吃碰杠牌数据结构
struct GCPStructEx:GCPStruct
{
	uchar  byBigGang[4][2]; //特殊杠牌数据（长春麻将）
	bool  bIsBigGang;      //是否大杠
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



///事件数据包,重载得来，可以添加自己的特殊数据
///游戏开始事件
struct tagBeginEx:tagBegin
{
	void Clear()
	{
		tagBegin::Clear();
	}
};

///以东为庄事件
struct tagDongNtEx:tagDongNt
{
	void Clear()
	{
		tagDongNt::Clear();
	}
};

///掷2颗色子的点数和为庄事件
struct tagTwoSeziNtEx:tagTwoSeziNt
{
	void Clear()
	{
		tagTwoSeziNt::Clear();
	}
};

///掷2颗色子确定庄家和起牌点位置事件
struct tagTwoSeziNtAndGetPaiEx:tagTwoSeziNtAndGetPai
{
	void Clear()
	{
		tagTwoSeziNtAndGetPai::Clear();
	}
};

///轮流掷2颗色子确定庄家
struct tagTurnSeziNtEx:tagTurnSeziNt
{
	void Clear()
	{
		tagTurnSeziNt::Clear();
	}
};

///掷2颗色子确定起牌位置事件
struct tagTwoSeziDirEx:tagTwoSeziDir
{
	void Clear()
	{
		tagTwoSeziDir::Clear();
	}
};

///掷2颗色子确定起牌位置（点数和）和起牌点（最小点）事件
struct tagTwoSeziDirAndGetPaiEx:tagTwoSeziDirAndGetPai
{
	void Clear()
	{
		tagTwoSeziDirAndGetPai::Clear();
	}
};

///掷2颗色子确定起牌敦数事件
struct tagTwoSeziGetPaiEx:tagTwoSeziGetPai
{
	void Clear()
	{
		tagTwoSeziGetPai::Clear();
	}
};

///发牌事件
struct tagSendPaiEx:tagSendPai
{
	void Clear()
	{
		tagSendPai::Clear();
	}
};
///跳牌事件
struct tagTiaoPaiEx:tagTiaoPai
{
	void Clear()
	{
		tagTiaoPai::Clear();
	}
};


///所有玩家补花事件
struct tagAllBuHuaEx:tagAllBuHua
{
	void Clear()
	{
		tagAllBuHua::Clear();
	}
};

///单个玩家补花事件
struct tagOneBuHuaEx:tagOneBuHua
{
	void Clear()
	{
		tagOneBuHua::Clear();
	}
};
///开始出牌通知事件
struct tagBeginOutPaiEx:tagBeginOutPai
{
	void Clear()
	{
		tagBeginOutPai::Clear();
	}
};
///出牌通知
struct tagOutPai_NTF
{
	uchar bDeskStation;		//出牌玩家位置

	void Clear()
	{
		bDeskStation = 255;
	}
};
///出牌事件
struct tagOutPaiEx:tagOutPai
{
	void Clear()
	{
		tagOutPai::Clear();
	}
};
///抓牌牌事件
struct tagZhuaPaiEx:tagZhuaPai
{
	void Clear()
	{
		tagZhuaPai::Clear();
	}
};
///吃碰杠糊通知事件事件
struct tagCPGNotifyEx:tagCPGNotify
{
	void Clear()
	{
		tagCPGNotify::Clear();
	}
};
///吃牌牌事件
struct tagChiPaiEx:tagChiPai
{
	GCPStructEx m_UserGCPData[4][5];     //吃碰杠杠牌
	void Clear()
	{
		tagChiPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//吃碰杠杠牌
	}
};
///碰牌牌事件
struct tagPengPaiEx:tagPengPai
{
	void Clear()
	{
		tagPengPai::Clear();
	}
};
///坎牌事件
struct tagKanPaiEx:tagKanPai
{
	GCPStructEx m_UserGCPData[4][5];     //吃碰杠杠牌
	void Clear()
	{
		tagKanPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//吃碰杠杠牌	
	}
};
///扫虎事件（查虎麻将专用）
struct tagSaoHuEx:tagSaoHu
{
	void Clear()
	{
		tagSaoHu::Clear();
	}
};
///杠牌牌事件
struct tagGangPaiEx:tagGangPai
{
	int			iGangFen[PLAY_COUNT];	//本次杠牌的得分情况

	void Clear()
	{
		tagGangPai::Clear();
		memset(iGangFen, 0, sizeof(iGangFen));//本次杠牌的得分情况
	}
};
///听牌牌事件
struct tagTingPaiEx:tagTingPai
{
	void Clear()
	{
		tagTingPai::Clear();
	}
};
///糊牌牌事件
struct tagHuPaiEx:tagHuPai
{
	void Clear()
	{
		tagHuPai::Clear();
	}
};
///算分事件
struct tagCountFenEx:tagCountFen
{
	int 	iGangFen[4];//各个玩家的杠分
	GCPStructEx m_UserGCPData[4][5];     //吃碰杠杠牌

	bool    m_bDianPao[PLAY_COUNT];//是否点炮
	bool    m_bIsZiMo[PLAY_COUNT];//是否自摸胡牌

	int		m_iZongFan[PLAY_COUNT];//胡牌总番数
	int		iTax;//服务费（收税）
	uchar	m_byPs[PLAY_COUNT]; // 胡牌牌值, 255无效

	int     m_iPaiTouCount[PLAY_COUNT];	//中牌头数

	int     m_iPaiTouFen[PLAY_COUNT];	//牌头分

	int iDianGangFen[PLAY_COUNT];//点杠分
	int iMingGangFen[PLAY_COUNT];//明杠分
	int iAnGangFen[PLAY_COUNT];//暗杠分 
	

	void Clear()
	{
		memset(iGangFen, 0, sizeof(iGangFen));//各个玩家的杠分
		tagCountFen::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//吃碰杠杠牌
		memset(m_bDianPao, 0, sizeof(m_bDianPao));	//点炮玩家位置
		memset(m_bIsZiMo, 0, sizeof(m_bIsZiMo));	//是否自摸胡牌
		memset(m_iZongFan, 0, sizeof(m_iZongFan));	//胡牌总番数
		iTax = 0;//服务费（收税）
		memset(m_byPs, 255, sizeof(m_byPs));
		memset(m_iPaiTouCount, 0, sizeof(m_iPaiTouCount));
		memset(m_iPaiTouFen, 0, sizeof(m_iPaiTouFen));
		memset(iMingGangFen,0,sizeof(iDianGangFen));
		memset(iAnGangFen,0,sizeof(iAnGangFen));
		memset(iDianGangFen,0,sizeof(iDianGangFen));
	}
};
///游戏结束处理事件
struct tagEndHandleEx:tagEndHandle
{
	void Clear()
	{
		tagEndHandle::Clear();
	}
};
///非正常结束处理事件
struct tagUnnormalEndHandleEx:tagUnnormalEndHandle
{
	void Clear()
	{
		tagUnnormalEndHandle::Clear();
	}
};
///////////////////////////////////////////////////

///换牌事件数据包
struct tagChangePsEx
{
	uchar byUser;		//换牌玩家
	uchar byChangePs[3];	//提供换牌
	
	bool bNotifyFinish;	//是否完成提供换牌

	bool bNotify;					//是否通知各个玩家需要准备提供3张牌值

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

///定缺事件数据包
struct tagDingQueEx
{
	uchar byUser; //定缺玩家
	uchar byQue; //缺的牌
	uchar byQuePai[PLAY_COUNT]; //缺的牌
	bool  bNotify; // 是否服务器定缺通知
	bool  bFinish[PLAY_COUNT];//是否已经完成定缺
	uchar m_byArHandPai[4][HAND_CARD_NUM];//手牌数据
	tagDingQueEx()
	{
		Clear();
	}
	void Clear()
	{
		byUser = 255; //定缺玩家
		byQue = 255; //缺的牌
		memset(byQuePai, 255, sizeof(byQuePai));//
		memset(bFinish, 0, sizeof(bFinish));////是否已经完成定缺
		bNotify = false; // 是否服务器定缺通知
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//手牌数据
	}
};

///断线重连数据包
struct GameStationDataEx:GameStationData
{
	GCPStructEx			m_UserGCPData[4][5];			//玩家吃碰杠牌	 
    uchar               bAutoBegin;     
	uchar				m_byFengDing;//封顶配置默认3番
	bool				m_bShowTax;//是否显示台费
	int					m_iBaseFen;//游戏基本分，默认为1
	bool				m_bForceFinish;//是否强退结束版本
	bool				m_bChengBao;//分承包制和家家有
	uchar				m_RemainingCards; //剩余牌数
	uchar				byDirection;	//起牌方位
	uchar				byGetPai;		//起牌点
	uchar				bySezi0;	//色子0数据
	uchar				bySezi1;	//色子1数据
	tagPM				m_tagPM;	//玩法

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

		m_byFengDing = 20;//封顶配置默认3番
		m_bShowTax = 1;//是否显示台费
		m_iBaseFen = 1;//游戏基本分，默认为1
		m_bForceFinish = false;//是否强退结束版本
		m_bChengBao = true;//分承包制和家家有
		byDirection = 255;
		byGetPai = 255;
		bySezi0 = 255;
		bySezi1 = 255;
	}
};

struct GameStationData_Dice : GameStationData_Base
{
	uchar				byNtStation;	//庄家位置
	uchar				byDirection;	//起牌方位
	uchar				byGetPai;		//起牌点
	uchar				bySezi0;		//色子0数据
	uchar				bySezi1;		//色子1数据

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
	uchar				m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//手牌不超过20张，0为结束标志
	uchar				m_RemainingCards; // 剩余牌数
	uchar				m_byFanCard;			//翻出来的牌
	uchar				m_byJoker;				//万能牌

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
	bool				m_bistuoguan[PLAY_COUNT];		// 是否托管
	uchar				m_byNowOutStation;				// 现在出牌用户
	uchar				m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//出牌不超过160张，0为结束标志
	GCPStructEx			m_UserGCPData[PLAY_COUNT][5];			//玩家吃碰杠牌	
	bool				m_bIsHu[PLAY_COUNT];			//是否胡牌
	uchar				m_byHuCard;						//胡的牌

	GameStationData_Playing()
	{
		memset(m_bistuoguan,false,sizeof(m_bistuoguan));
		m_byNowOutStation = 255;
		memset(m_byArOutPai,0,sizeof(m_byArOutPai));
		memset(m_bIsHu,false,sizeof(m_bIsHu));
		m_byHuCard = 255;
	}
};

///断线重连数据包
struct GangFenRecordEx
{
	int fen[5][PLAY_COUNT+1];
	int count;//数量
	GangFenRecordEx()
	{
		Init();
	};
	void Init()
	{
		memset(this,0,sizeof(GangFenRecordEx));
	}
};

//通知有动作
struct SC_NotifyHaveAction
{
	uchar byDeskStation;//出牌位置
	void Clear()
	{
		byDeskStation=255;
	}
};

struct SC_RemainingGameCount
{
	int remaingameCount;	// 剩余局数
	int total;				// 总局数
};

struct TingInfo
{
	uchar Value;	//听牌值
	uchar Count;	//剩余张数
	uchar Fan;		//胡番

	void Clear()
	{
		Value = 255;
		Count = 0;
		Fan   = 0;
	}
};

struct WaitOutCard
{
	uchar     OutCard;			//听牌可以出的牌值
	TingInfo  TingCards[MAX_TING_NUM];	//听牌信息

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
	uchar m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM]; //手牌数据
	uchar m_byHuanZhangData[PLAY_COUNT][3];			//玩家换3张得到的牌
	uchar m_bySelectData[PLAY_COUNT][3];			//玩家换3张选择的牌

	void Clear()
	{
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
		memset(m_byHuanZhangData, 255, sizeof(m_byHuanZhangData));
		memset(m_bySelectData, 255, sizeof(m_bySelectData));
	}
};

struct PaiTou
{
	uchar m_byPaiTou[MAX_PAI_TOU];				//翻出的牌头
	uchar m_byZhongPaiTou[PLAY_COUNT][MAX_PAI_TOU];			//中的牌头

	PaiTou()
	{
		memset(m_byPaiTou, 255, sizeof(m_byPaiTou));
		memset(m_byZhongPaiTou, 255, sizeof(m_byZhongPaiTou));
	}
};

#pragma pack(pop)
