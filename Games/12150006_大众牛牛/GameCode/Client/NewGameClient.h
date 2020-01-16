#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
//#include "CommonDefine.h"
#include<deque>
using namespace irrklang;

#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A

const   int  GAME_WAIT_STATION    = 1 ;
const   int  GAME_BEGIN_STATION   = 2 ; 
const   int  GAME_FINISH_STATION  = 3 ; 
//�˿���
#define CARD_NUMS					5

//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_WAIT_NEXT				101					//��ʼ��ʱ��
#define ID_OUT_CARD					102					//���Ƶȴ�ʱ��
#define ID_LOOK_BACK				103					//������
#define ID_CHANGE_MUSIC			    104					//����������
#define ID_NOTE                     105                 //�ȴ��м���ע
#define ID_OPEN_CARD                106                 //�ȴ��мҳ���
#define ID_CALL_SCORE_TIME			107					//�зֶ�ʱ��

//��Ϸ��Ч
#define SOUND_BEGIN_GAME			  0x01			//��Ϸ��ʼ
#define SOUND_TIME_0				  0x02			//ʱ����ʾ��
#define SOUND_TIME_1			      0x03
#define SOUND_TIME_2			      0x04
#define SOUND_TIME_3			      0x05
#define SOUND_TIME_4			      0x06
#define SOUND_TIME_5			      0x07
#define SOUND_WIN				      0x08			//ʤ��
#define SOUND_LOSE				      0x09			//ʧ��
#define SOUND_CHONGPAI			      0x0A			//����
#define SOUND_XIAZHU			      0x0B			//��ע
#define SOUND_TIP			          0x0C			//��ʾ
#define SOUND_TANPAI			      0x0D			//̯��
#define SOUND_CLICKCARD			      0x0E			//�����				
#define SOUND_HIT_BEGIN				  0x0F			//�����ʼ��ť
#define SOUND_HIT_LEAVE				  0x11			//�뿪
#define SOUND_JIAOZHUANG			  0x12			//��ׯ	
#define SOUND_GIVEUP			      0x13			//�в���ׯ

/*��ţ-ţ��*/
#define SOUND_NIUNIU_0				  0x3F			
#define SOUND_NIUNIU_1				  0x4F	
#define SOUND_NIUNIU_2			      0x5F
#define SOUND_NIUNIU_3			      0x6F
#define SOUND_NIUNIU_4			      0x7F
#define SOUND_NIUNIU_5			      0x8F
#define SOUND_NIUNIU_6			      0x9F
#define SOUND_NIUNIU_7				  0xAF			
#define SOUND_NIUNIU_8				  0xBF			
#define SOUND_NIUNIU_9			      0xCF			
#define SOUND_NIUNIU_10			      0xDF				//ţţ
			


#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\

class IGameFrame;
interface IUserInterface;
class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);//�����캯����һ�ε��ã�
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);


private:
	
	IGameFrame							*m_pGameFrame;									///�����Ϸָ��
	IUserInterface						*m_pUI;											///������Ϸָ��
	ISoundEngine						*m_SoundEngineGame;								//��Ϸ��Ч
	ISoundEngine						*m_SoundEnginBg ;								//������Ч

	bool                m_bSoundPlay ;		//�Ƿ���Բ�������
	bool                m_bSoundBgPlay ;	//������Ч
		
	CUpGradeGameLogic					m_Logic;										//�ͻ����߼�
public:
	//�̶���Ϣ
	//=====================ʱ�����Ϣ===================
	BYTE					m_iThinkTime;				//������Ϸ����
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iSendCardTime;			//��һ����ʱ��(�����ٶ�)
	BYTE					m_iCallScoreTime;			//��Ϸ��ʼʱ��
	BYTE					m_iXiaZhuTime;				//��עʱ��
	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�
	int						m_iVersion;					//��Ϸ�汾
	int						m_iVersion2;				//��Ϸ�����汾
	int						m_iMyDeskStation ;			//����Լ�����Ϸλ��
	int						m_iHaveCattleTip;			//��ţ��ʾʱ��
	BYTE					m_byGameStation ;			//��ǰ����Ϸ״̬
	BYTE					m_iPalyCardCount;			//��Ϸ�˿���Ŀ
	BYTE					m_iCurrentCardCount;		//��ǰҪ�����Ƶ���Ŀ
	BYTE					m_iCardCount;				//������Ϸ���˿���Ŀ
	__int64					m_iLimitNote[PLAY_COUNT];				//����޶�ע
	__int64					m_i64UserNoteLimite[PLAY_COUNT][4];	//����ĸ���ע����
	__int64					m_iThisTurnLimit;			//������ע
	//������Ϣ
	int						m_iUserStation[PLAY_COUNT];						//��¼�û�״̬(��ʶ����Ƿ���;����� ����һֱ�������)
	BYTE					m_DeskReadyCard[PLAY_COUNT][SH_USER_CARD];		//����˿�������(�߼�λ��)
	BYTE					m_iDeskCardCount[PLAY_COUNT];					//����˿�������(�߼�λ��)

	BYTE                    m_iUpBullCard[PLAY_COUNT][2];//�����ţ��
	BYTE                    m_bSendCard[PLAY_COUNT];		//��������
	BYTE					m_bUpCard[2];				//�������
	BYTE                    m_bUnderCard[3];			//�������
	BYTE					m_byNtStation;										//ׯ��λ��

	//������Ϣ
	BYTE				    	m_bCurrentOperationStation;		//�F�ڽз���
	BYTE						m_bTimeOutCount;			//��ʱ
	BYTE						m_bShowAnimalStation;		//������ʾλ��
	int					    	m_iNowOutPeople;			//���ڳ�����
	int						    m_iFirstOutPeople;			//��һ��������
	int					    	m_iNowBigNote;				//��ǰ���Ѻע��
//	__int64						m_iThisGameNote[PLAY_COUNT];			//������Ϸ��ע��(��ע)
	__int64						m_iTotalGameNote[PLAY_COUNT];			//������Ϸ��ע��(ʵ��)
	int						    m_iUpGradePeople;					//ׯ��λ��
	int                         m_iNote;                    //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	BOOL						m_bBgSound;					//��������
	int                         m_iAfterWaitAction;
	int							m_bCurrentClockLabel;			//��ǰʱ�ӱ�ǩ
	int							m_iCurrentClockControlID;		//��ǰ���Ƶ�ʱ��ID

	bool                        m_bAutoBull;                ///�Ƿ��Զ���ţ
	BYTE                        m_iSendAnimaCardPos;     ///���ƶ���λ��
	int                         m_iSendCardTotle;         ///���Ƶ�����
	BOOL                        m_bIsSuper;               ///�Ƿ񳬶�
	BOOL                        m_bPlayer;                ///�Ƿ���ʵ��ң�2011-8-17 ��ƽ̨�޸Ĺ�������ҿ��Խ�����Ϸ������;�������ӵĲ�����ʵ���
	BOOL						m_bIsHitTip;			//�Ƿ������ʾ��ť,����ţ��

	//�������
	deque<BYTE>                 m_sendQueue;			//����˳������
	BYTE						m_bySendCardPos;		//��ǰ����λ��
	BYTE						m_bySendCardCount;		//���ƴ���
	BYTE						m_bySendCount[PLAY_COUNT];		//�Ѿ����˶�������
	//��Ϸ����
	SuperUserSetData			m_SuperSetData;				//��������
	
private:
	
private:
	//���ÿؼ��Ƿ�ɼ�
	void SetControlVisible(const int iControlID,bool bFlag);

	//���ð�ť�Ƿ����
	void	SetBtEnable(const int iButtonID, bool bFlag);

	//���ð�ť�Ƿ�ɼ�
	void	SetBtVisible(const int iButtonID, bool bFlag);

	//����ʱ�����ʾ����ʱ
	void	SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime = 0);

	//����ͼƬ�ؼ���λ��
	void	SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag);

	//����ͼƬ�ؼ��Ƿ�ɼ�
	void	SetImageVisible(const int iImageID, bool bFlag);

	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void	SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);

	//���������ؼ��ɼ�
	void	SetContainVisible(const int iContainID,bool bFlag);

	//���������ؼ�����Ӧ�κ����ʱ��
	void  SetContainEnable(const int iContainID,bool bFlag);

	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//�����ı��ؼ���ʾ������Ϣ
	void	SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag);
	//�������ֿռ���ʾ����
	void	SetInumInfo(const int , __int64 ,bool );

	//�������ֿռ���ʾ����(�����Ӻźͼ��ż�����)
	void SetInumInfo_1(const int iNumID, __int64 i64Num, bool bFlag);

	//���ó���ؼ���ʾֵ
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);

	//���ÿؼ�λ��
	void   SetControlPos(const int iControlID,bool bFlag,int x,int y);

	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void	SetRButtonSelected(const int iRButtonID,bool bSelected,bool bShow = true);

	//��ȡ��ѡ��ؼ��Ƿ�ѡ��
	bool	GetRButtonSelected(const int iRButtonID);

	//���ÿɲ����ƿؼ�����
	void	SetOperateCardInfo(const int iOperateCardID, BYTE byCardList[],BYTE byCardCount, bool bShow);
	//���ò��ɲ����ƿؼ�����
	void	SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount, bool bShow);

public:
	//��һ����������������ִ�д˺���
	void HandleStartStation(void *buffer);
	//��ׯ״̬
	void	HandleRobNtStation(void *buffer);
	//��ע״̬
	void	HandleNoteStation(void *buffer);
	//����״̬
	void	HandleSendCardStation(void *buffer);
	//��Ϸ������
	void	HandleOpenCardStation(void *buffer);
	
	///��������
	void	ResetGameData();

	//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
	void SetPicSize(const int iButtonID, CString sImagSrc,bool bFlag,int x,int y,int w,int h);

	
 public:
    /// ��ʼ�����ڶ��ε��ã�
    virtual int Initial();
    /// ��ʼ��UI�������ε��ã�
    virtual int InitUI();
    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);
    /// ��������״̬
    virtual int ResetGameFrame(void);
    /// ��ҽ��루���Ĵε��ã�
    virtual int GameUserCome(void);
    /// ����뿪
    virtual int GameUserLeft(int nIndex);
    /// ��Ҷ���
    virtual int GameUserOffLine(void);
    // �Ƿ�������Ϸ������Ϸʵ��
    // �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool IsPlayingGame();          
    /// ��ʱ����Ϣ
    virtual void OnTimer(int nTimerID);
    /// ��״̬
    virtual void SetStationParameter(BYTE bGameStation);
    ///������Ϸ״̬
	virtual BYTE GetStationParameter();
	///��Ϸ��������Ϸ�˳�ʱ���ã�
	virtual bool OnGameQuiting();
	
	//����UI����
	void	ResetGameDialog();
	///��ʼ����Ϸ�еı���
	void	InitialGameData();  
	//��ȡ��Ϸ����
	void	LoadGameSet();
	//������Ϸ����
	void	SaveGameSet();
	
	///ɾ�����ж�ʱ��
	void	KillAllGameTime() ;

	//���ű�����Ч
	void	PlayBgSound(bool bISLoop);

	//������Ϸ��Ч
	void	PlayGameSound(int SoundID,bool bISLoop = false, BYTE byDeskStation = 0);
	
	///��ҽ�����Ϸ
	void	SetGameStation(void * pBuffer,int nLen);



	//��ť
	//��ʼ��ť
	void OnControlHitBeginBt();

	
	//��ע��ť
	void OnHandleStartBet(int iHitButton);
	//��ׯ��ť
	void OnHandleHitNtBt();
	//������ׯ��ť
	void OnHandleHitGiveUpNtBt();
	//����/̯�ư�ť
	void OnHandleIsShowCardBt(int iHitButton);
	//����ɲ�����
	void OnOperateCardBt();
	//����������뿪��ť
	void OnClickContinueBt(int iHitButton);

	//���������Ϣ
	void OnSendCardFinish();

	//�ͷ��˴���������Ϣ
	//�����ׯ
	void OnHandleCallScoreMessage(void *buffer);
	//�����ׯ���
	void OnHandleCallScoreResultMessage(void *buffer);
	//�����ׯ������Ϣ
	void OnHandleCallScoreFinishMessage(void *buffer);
	//����ͬ����Ϸ��Ϣ
	void OnAgreeGame(void *buffer);
	//������Ϸ��ʼ��ע��Ϣ
	void OnHandleBetBegin(void *buffer);
	//������ע���
	void OnHandleBetResult(void *buffer);
	//�����ƽ��
	void OnHandleSendCardBegin(void *buffer);
	//�����ƽ�����Ϣ
	void OnHandleSendFinish();
	//������Ϸ��ʼ
	void OnHandleGameBegin(void *buffer);
	//��Ϸ����
	void OnGameFinish(void *buffer);
	//����̯����Ϣ
	void OnHandleOpenCard(void *buffer);
	//��������˳���Ϣ
	void OnHandlePlayerCut(void *buffer);
	//������������˳���Ϣ
	void OnHandlePlayerSafeEnd(void *buffer);
	//����������������ĳ����û���Ϣ
	void	HandleSuperMessage(void *buffer);

	//������Ӧ��ׯ��ʱ��
	void OnStartClock(int viewStation,BYTE bCallScoreTime);
	
	//��ʾ��Ϸ����
	void	ShowGameSet(bool bShow);
	//�����ׯ/����ׯͼƬ
	void CLearScratchNt();
	//�������ʱ��
	void ClearAllClock();
	//��ֹ��ע
	void ClearAllBet();

	//��ʾ��ע
	void DisplayBet(__int64 i64MaxBetCount);
	//��עѡ������
	void BetOptionSet(__int64 i64Num,int iOption,bool IsUse);
	//���Լ�����
	void ShowCurrentCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//�����˵���
	void ShowCurrentNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//չʾ̯����ҵ���
	void ShowOpenCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//������ʾ������
	void ShowUpCard(const int iControlID1,const int iControlID2,BYTE bUnderCardList[],BYTE bUnderCardCount,BYTE bUpCardList[],BYTE bUpCardCount);
	//�������ֿռ���ʾ����(����������:0:��ʾ�Ӻţ����ţ��Ͷ��ţ�1��ֻ��ʾ���ţ�2��������������)
	void	SetInumPro(const int , __int64 , BYTE ,bool );
	//��ȡ�������
	void GetUpCard(const int iControlID,BYTE bCardList[]);
	//��ȡ������Ƶ�����
	int GetUpCardNum(const int iControlID);
	//��ʾ�����
	void	ShowResultDlg(GameEndStruct *pGameFinish,bool bShow);
	//��ʾ���˽������
	void	ShowSuperSet(bool bFlag);
	//���ͳ�����������
	void	SuperUserSet();

	//ʱ�侯ʾ��
	void TimeWarning(int iTime); 
	//��ʾʱ�ӵ���ʱ
	void	ShowTimer(const int , bool, BYTE byMax, BYTE byMin=0 );

	//���ֶ���
	//������ţ����
	void ShowHaveCattleAction(const int iImageID,int iCardShape,bool bShow,BYTE byDeskStation);
	//������������
	void	PlayShapeSound(int iCardShape,bool bShow,BYTE byDeskStation);
	//���ŷ��ƶ���
	void CNewGameClient::PlaySendCard(BYTE Station);
	//���ŷ��ƶ���
	void ShowSendCardAction(const int ACTION_ID,const int END_ID);
	//���û�����
	void SendCard(BYTE bCardValue,BYTE bSendPeople);
	//���㷢��˳��
	void SendQueue(int iStart);
	//���㵱ǰ�Ƶ�ӵ����
	BYTE GetSendStation(BYTE (*BCardList)[5],BYTE bCardValue,int iCardCount);
	//��ǰ�������Ƿ���Ч
	BOOL IsUser(BYTE bCardValue);	

};