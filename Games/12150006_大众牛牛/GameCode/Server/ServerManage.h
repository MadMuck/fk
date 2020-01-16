#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"
#include "..\GameMessage\UpgradeMessage.h"
#include<vector>
#include <time.h>
//��ʱ�� ID
#define TIME_NOTE                   31              //��ע��ʱ��
#define TIME_NOTE_FINISH			32				//��ע��ɶ�ʱ��
#define TIME_SEND_CARD_FINISH		33				//���ƽ�����ʱ��
#define TIME_AUTO_BULL				34				//�Զ���ţ��ʱ��
#define TIME_GAME_FINISH			35				//��Ϸ������ʱ��
#define TIME_CALL_ROBNT				36				//��ׯ��ʱ��
#define TIME_ROBNT_FINISH		    37				//��ׯ������ʱ��
#define TIME_GAME_BEGIN				38				//��Ϸ��ʼ��ʱ��
#define TIME_AUTO_NEXT				39				//�Զ���ʼ��һ�ֶ�ʱ��
#define TIME_AUTO_BEGIN				40				//��Ϸ�Զ���ʼ��ʱ��
#define TIME_OPEN_DELAY				41				//ׯ������ʾ��Чʱ��

#define TIME_AUTO_OPERATOR			48				//�й��Զ�����
#define TIMER_ID_KICK				49				//�޳���Ҷ�ʱ��

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������


#define MODE_SET_CARD_DEBUG				TRUE//������ģʽ|����������


struct TAIControl
{
	__int64 iMoney;
	int		iLucky;
	int		iMinBull;
	int		iMaxBull;
};


static __int64					G_iAIHaveWinMoney;	//������ӮǮ��20121126	dwj ��������Ϊ��̬ȫ�ֱ�������Ϊ���������Ҫ��ʱ�޸ĸñ���
static __int64					G_iReSetAIHaveWinMoney;	/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  �ñ���ҲҪ����Ϊ��̬ȫ�ֱ��� ����ÿ�����Ӷ���ȥ����*/


typedef BYTE PlayerStation;

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
private:
	
	bool					m_bAICtrl;			//�Ƿ��������˿���
	vector<TAIControl>		m_vAIContrl;		//�����˿���ϸ��

	bool					m_bAIRobAndNote;	//�Ƿ������������ׯ��ע
	int						m_iAIRobNTShape;	//��������ׯ����
	int						m_iAINoteShape;		//��������ע����

private:
	int		m_iSitTime[PLAY_COUNT];				//�������ʱ��
	int		m_iTickTime;						//����ʱ��
	bool	m_bAuto[PLAY_COUNT];				//����Ƿ��й�
	bool	m_bNormalFirst;						//�Ƿ��һ��
	bool	m_bHaveBeginButton;					//�Ƿ��п�ʼ��ť
	int		m_iBasePoint;						//��Ϸ�׷�
	int		m_RoomId;							//����id
public:


	void PrepareCard();

	//֪ͨAi����
	void NotifyAiCardShape();

	void GameBeginInit();

	void SendGameBegin();

	void MakeGoodCard();

	//��ȡ����ʵ������
	int GetRealShape(int iMinBull, int iMaxBull);

	//�Ƿ�����Ч��ţţ����
	bool IsValidBull(int iShape);

	//�Ƿ���Ҫ�����˿���
	bool NeedAIControl();

	//��Ϸ������Ϸ���ݣ������������ݿ�
	//szUserGameInfo �������
	//iSize ���ݳ���
	virtual void ReleaseRoomBefore() ;
	//�ָ����䷢����Ϸ���ݵ���Ϸ������
	//iCount����������
	virtual void SetReturnGameInfo(int UserID,void* szUserGameInfo,int iSize,int iCount);
	// �������
	virtual BYTE UserSitDesk( MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo );
public:
	//�Զ���ׯ
	void AutoRobNT(BYTE byUser);

	//�Զ���ע
	void AutoNote(BYTE byUser);

	//�Զ�̯��
	bool AutoTanPai(BYTE byUser);

	//�Զ�����
	void AutoAction(BYTE byUser);

	//����й�����
	void HandleAuto(BYTE byUser, bool bAuto);

	//��ȡĬ���淨
	void LoadPlayMode();

	//����ʼ������
	void InitLastData();
	//��¼��һ����Ϣ
	void RecoderLastGame(__int64 iChangeMoney[], const int iSize );
	//��������
	bool FindShape(BYTE bUser, BYTE byRandCards[], int iShape);
	//���ó�������
	void SetSuPerUserCards();
	//��ȡ�����ע
	int GetMaxTuiZhu( );
	//�����ܹ���ע�ķ���
	void SetCanNote( vector<PlayerStation> &vRandLoser = vector<PlayerStation>( ) );
	//��ȡ�׷�
	int GetBasePoint( );
	//��ȡ�����ׯ����
	int GetMaxRobNT( );
	//�Ƿ���ĳ���淨
	bool IsPlayMode( int iMode );
	//�Ƿ�����Ч���
	bool IsValidPlayer( int iNum );
	//��ȡ������עֵ
	int GetLimitNote( BYTE byUser, bool bMin );
	//��ȡ�Ѿ�������Ϸ�����
	int GetPlayingPlayerCount( );
	//������Ч������ţţ
	void SetValidNN( );
	//����ţ�ı���
	void SetNNPoint( );
	///���ʤ�����
	BYTE GetWinPeo( );
	///�������ţţ���
	BYTE MaxNNUser( );
	//�����ʾ����
	void HandleTiShi(BYTE bDeskStation, TMSG_TI_SHI_REQ* tTiShiReq);
	//����ׯ�ҿ�ʼ��Ϸ
	void HandleMasterStart(BYTE bDeskStation, TMSG_MASTER_START_GAME_REQ* pMasterStart);


	bool SendGameDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	bool SendWatchDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);

	bool SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	bool SendWatchDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
public:
	unsigned int					m_iPlayMode;							//�淨
	vector<PlayerStation>			m_vPlayingPlayer;						//��������Ϸ�����
	int								m_iCanNote[PLAY_COUNT][3];				//�ܹ���ע�ķ���
	int								m_iLastNote[PLAY_COUNT];				//��һ�������ע����
	int								m_iLastWinPoint[PLAY_COUNT];			//��һ����ҵ÷ַ���
	bool							m_bNNValid[6];							//��Ч������ţţ
	BYTE							m_byLastGradePeople;					//��һ��ׯ��λ��	
	BYTE							m_bySonStation[PLAY_COUNT];				//̯���ڼ����״̬
	bool							m_bMasterStartGame;						//���������ʼ��Ϸ
	bool							m_bFirstLoad;							//��һ�μ���
	bool							m_bSystemOperate;						//�Ƿ�ϵͳ����
	TMSG_LAST_GAME_RSP				m_tLastGameInfo;						//��һ����Ϣ
	int							m_PlayerCount;						//������Ϸ����

protected:
	/*---------------------------��Ϸ��������----------------------------------*/
	CUpGradeGameLogic		m_Logic;						//��Ϸ�߼�
	
	BYTE					m_iAllCardCount;							//��Ϸ���õ�����
	BYTE					m_iTotalCard[NUM_CARD_ARRAY_WITH_FACE];		//�ܵ���

	/*---------------------------��Ϸ��������ÿ����Ҫ���õ�����----------------*/
	SuperUserSetData		m_SuperSetData[PLAY_COUNT];					//�������õ�����
	bool                    m_bUserReady[PLAY_COUNT] ;			///���׼��״̬
	BYTE					m_byUpGradePeople;					//ׯ��λ��	
	BYTE                    m_iFirstJiao;						//��һ����ׯ���λ��	

	BYTE					m_byUserStation[PLAY_COUNT];			//��¼�û�״̬	DWJ
	BYTE					m_iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�
	int						m_iRobNTBase[PLAY_COUNT];				//�����ׯ״̬ -1��ʾ��δ������0��ʾ����ׯ 1��ʾ��ׯ��������ģʽ��1-4��ʾ��ׯ����	DWJ
	int						m_iNTBase;								//��ׯ����/ׯ�ұ���
	int						m_iUserBase[PLAY_COUNT];				//�мұ���

	int						m_iOpenShape[PLAY_COUNT];
	BYTE					m_byOpenUnderCount[PLAY_COUNT];		//��������
	BYTE					m_byOpenUnderCard[PLAY_COUNT][3];	//���Ƶ�������
	BYTE					m_byOpenUpCard[PLAY_COUNT][2];		//��������2����
	BYTE                    m_byBullCard[PLAY_COUNT][3];          //��Ұڵ�ţ�� 


	/*---------------------------��Ϸ�����ò���----------------------------------*/
	BYTE				m_iBeginTime;			//��Ϸ��ʼʱ��
	BYTE				m_iThinkTime;			//��Ϸ��ţ˼��ʱ��
	BYTE				m_iXiaZhuTime;			//��עʱ��
	BYTE				m_iRobNTTime;		    //��ׯʱ��( ������ģʽ )
	BYTE				m_bCardShapeBase[MAX_SHAPE_COUNT];//0-10������ţ��ţţ��ı���, 11-15Ϊ�����淨���͵ı���
	//���˲���
	vector<long>		m_vlSuperID;				//���泬����ҵ�����

	//���²������ǿ����õ� ����û��д�������ļ��� ���Ƿ�ֹ�ͻ������������
	BYTE				m_iSendCardTime;		//����ʱ��-���ƿͻ��˷�һ���Ƶ��ٶ� 
	BYTE				m_iRandomNTTime;		//���ׯ�Ҷ���ʱ����( ms )
	BYTE				m_iRandomNTRound;		//���ׯ�Ҷ�������Ȧ��
	BOOL				m_bTurnRule;			//��Ϸ˳�� 0-˳ʱ�� 1-��ʱ��
	int					m_iGoodCard;			//�����ƻ���ţ������
	bool				m_bHaveKing;			//�Ƿ�����

	time_t				tRobNTTime;
	time_t				tShowCardTime;
	time_t				tNoteTime;
	
	CalculateBoard			m_TCalculateBoard;
public:
	//���캯��
	CServerGameDesk( ); 
	//��������
	virtual ~CServerGameDesk( );

	//���غ���
public:
	//��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation( );
	//��Ϸ��ʼ
	virtual bool	GameBegin( BYTE bBeginFlag );

	//��Ϸ����
	virtual bool	GameFinish( BYTE bDeskStation, BYTE bCloseFlag );
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame( BYTE bDeskStation );

	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGameEx( BYTE bDeskStation );

	//�����Ϣ������
	virtual bool HandleFrameMessage( BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser );
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage( BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser );
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk( BYTE bDeskStation, CGameUserInfo * pUserInfo );
	//
	virtual bool UserNetCut( BYTE bDeskStation, CGameUserInfo *pLostUserInfo );
	//�û���������
	virtual bool UserReCome( BYTE bDeskStation, CGameUserInfo * pNewUserInfo );
	//��;�����
	virtual void DissMissRoomBefore( );

	///�Ƿ���Կ�ʼ��Ϸ
	virtual bool CanBeginGame();

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation( BYTE bDeskStation, UINT uSocketID, bool bWatchUser );
	//������Ϸ״̬
	virtual bool ReSetGameState( BYTE bLastStation );
	//��¼��������Ӯֵ
	void RecordAiHaveWinMoney( GameEndStruct *GameEnd );
	//��ʱ����Ϣ
	virtual bool OnTimer( UINT uTimerID );
	//������
private:
	//�����Ϸ������Ϣ
	void GetGameBaseStat( GameStation_Base& TGameStation );
public:
	//��ȡ��̬����
	BOOL	LoadIni( );			
	//�����û���֤
	void	SuperUserExamine( BYTE bDeskStation );
	//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
	bool	IsSuperUser( BYTE byDeskStation );
	//ͳ�Ƶ�ǰ��δ��ע����Ҹ���
	BYTE	CountNoNotePeople( );
	//���䱶��
	int		GetRoomBasePoint( );
	//������ҷ���
	int		ComputePoint( BYTE DeskStation );

	//����
	BOOL	ChangeCard( BYTE bDestStation, BYTE bWinStation );
	//����ָ��������ҵ�����
	void	ChangeTwoUserCard( BYTE byFirstDesk, BYTE bySecondDesk );
	
	//��ȡ��һ�����λ��
	BYTE	GetNextDeskStation( BYTE bDeskStation );
	//֪ͨ��ׯ
	BOOL	NotifyRobNT( );
	//��ׯ
	BOOL	HandleUserRobNT( BYTE bDeskStation, int iVal );
	//�Ƿ���ׯ����
	BOOL IsRobNTFinish( );
	//�зֽ���, 
	BOOL	RobNTFinish( bool bShowDing, int iPersonCount );
	//֪ͨ�û���ע
	BOOL	NoticeUserNote( );
	//�����û���ע���
	bool	HandleUserNoteResult( BYTE bDeskStation, int iNoteType = 0 );
	//�����˿˸��û�
	BOOL	NotifySendCard( );
	//���ƽ���
	BOOL	NotifySendCardFinish( );
	//����
	BOOL	BeginOpenCard( );
	//bClient: �ͻ���true( ���ͻ������ݺϷ��Լ�ţ����ȷ�� ), �����false
	BOOL	UserOpenCard( BYTE byDeskStation, void * pData );
private:
	void GetBullCard( BYTE byDeskStation );
public:
	//����Ƿ����
	BOOL	CheckFinish( );

	int GetCurPlayerCount( );

	//�Զ���ʼ��Ϸ
	void AutoBegin( );
	
	///�������м�ʱ��
	void	KillAllTimer( );

	///��ȡ��ǰ�����������ͣ�
	int		GetMaxCardShape( BYTE iCard[], int iCardCount );
private:
	//�޸Ľ�������1( ƽ̨->��Ϸ )
	/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPond( bool	bAIWinAndLostAutoCtrl );

	//�޸Ľ�������2( ƽ̨->��Ϸ )
	/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1, 2, 3 4���öϵ�
	/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1, 2, 3, 4ӮǮ�ĸ���
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx( __int64	iAIWantWinMoney[], int	iAIWinLuckyAt[] );

	//�޸Ľ�������3( ƽ̨->��Ϸ )
	/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx( __int64	iReSetAIHaveWinMoney );

	//����ʤ��
	/// �ж�
	virtual BOOL Judge( );
	/// ʤ
	virtual BOOL JudgeWiner( );
	/// ��
	virtual BOOL JudgeLoser( );
	//GRM����
	//���½�������( ��Ϸ->ƽ̨ )
	/// @return ������ӮǮ��
	virtual void UpDataRoomPond( __int64 iAIHaveWinMoney );

	void UpdateCalculateBoard( ); //���½����

#if MODE_SET_CARD_DEBUG
private:
	//��ȡ��������
	void LoadSetCard( );	
	//���ƺ���
	void ChangeCard2Test( );
private:
	bool m_bTestSwitch;
	BYTE m_byCardArr[PLAY_COUNT][SH_USER_CARD];
#endif // MODE_SET_CARD_DEBUG

};

/******************************************************************************************************/
