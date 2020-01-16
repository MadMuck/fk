#include "StdAfx.h"
#include "ClientGameDlg.h"
#include ".\clientgamedlg.h"


BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
		


	ON_WM_TIMER()
	//	ON_WM_CLOSE()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{

}

//��������
CClientGameDlg::~CClientGameDlg()
{

}

//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClass::DoDataExchange(pDX);
//	DoDataExchangeWebVirFace(pDX);
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();

	return TRUE;
}

//�Ƿ���ĳ���淨
bool CClientGameDlg::IsPlayMode( int iMode )
{
	if ( ( m_tBaseInfo.iPlayMode & iMode ) > 0 )
	{
		return true;
	}
	return false;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	GameStation_Base *pGameStation = (GameStation_Base *)pStationData;
	if (NULL == pGameStation) return true;

	memcpy(&m_tBaseInfo, pGameStation, sizeof(GameStation_Base));
	SetStationParameter(pGameStation->byGameStation);

	switch (GetStationParameter())
	{
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ�̿�ʼ
	case GS_GAME_BEGIN:
		{	
			GameStation_WaiteAgree *pGameStation = (GameStation_WaiteAgree *)pStationData;
			if( uDataSize != sizeof(GameStation_WaiteAgree) ) return true;

			int iTime = ( rand()+ GetMeUserInfo()->bDeskStation ) % m_tBaseInfo.iBeginTime ;

			//������Ŷӳ��Ͳ���ȥ���׼����
			if (!(m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME))
			{
				SetGameTimer(0,(iTime+1),ID_BEGIN_TIME);
			}
			return true;
		}
	case GS_ROB_NT:		//��ׯ״̬
		{
			if( uDataSize != sizeof(GameStation_RobNt) ) return true;
			return true;
		}
	case GS_NOTE:		//��ע״̬
		{
			if( uDataSize != sizeof(GameStation_Note) ) return true;
			GameStation_Note *pNoteNtf = (GameStation_Note *)pStationData;
			memcpy(m_iCanNote, pNoteNtf->iCanNote, sizeof(m_iCanNote));
			return true;
		}
	case GS_SEND_CARD:	//����״̬
		{
			if( uDataSize != sizeof(GameStation_SendCard) ) return true;
			return true;
		}
	case GS_OPEN_CARD:	//��Ϸ������
		{
			if (uDataSize != sizeof(GameStation_OpenCard)) return true;
			return true;
		}
	default:
		return false;
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		if(pNetData == NULL) return true;
		switch (pNetHead->bAssistantID)
		{
		case MSG_AI_CARD_SHAPE:
			{
				if(uDataSize != sizeof(TMSG_AI_CARD_SHAPE)) 
					return true;
				TMSG_AI_CARD_SHAPE* pCardShape = (TMSG_AI_CARD_SHAPE*) pNetData;
				if (pCardShape->byUser != GetMeUserInfo()->bDeskStation)
					return true;

				m_tCardCfg = *pCardShape;
				return true;
			}
		case ASS_CALL_SEND_CARD:
			{
				if(uDataSize != sizeof(SendAllCardStruct)) return true;
				SetStationParameter(GS_SEND_CARD);
				return true;
			}
		case ASS_CALL_ROBNT:
			{
				if(uDataSize != sizeof(TMSG_ROB_NT_NTF)) return true;
				SetStationParameter(GS_ROB_NT);
				//
				SetGameTimer(0, rand() % m_tBaseInfo.iRobNTTime, ID_ROB_NT_TIMER);
				return true;
			}
		case ASS_CALL_NOTE:
			{
				if(uDataSize != sizeof(TMSG_NOTE_NFT)) return true;
				TMSG_NOTE_NFT* pNoteNtf = (TMSG_NOTE_NFT*) pNetData;
				SetStationParameter(GS_NOTE);
				//
				memcpy(m_iCanNote, pNoteNtf->iCanNote[GetMeUserInfo()->bDeskStation], sizeof(m_iCanNote));
				SetGameTimer(0, rand() % m_tBaseInfo.iXiaZhuTime, ID_NOTE_TIMER);
				return true;
			}
		case ASS_CALL_OPEN:
			{
				if(uDataSize != sizeof(TNoticeOpenCard)) return true;
				SetStationParameter(GS_OPEN_CARD);
				//
				SetGameTimer(0, rand() % m_tBaseInfo.iThinkTime, ID_OPEN_CARD_TIMER);
				return true;
			}
		case ASS_CONTINUE_END:
			{
				if(uDataSize != sizeof(GameEndStruct)) return true;
				SetStationParameter(GS_WAIT_NEXT);
				//
				SetGameTimer(0, rand() % m_tBaseInfo.iBeginTime, ID_BEGIN_TIME);
				return true;
			}
		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	memset(m_iCanNote, 0, sizeof(m_iCanNote));
	m_tCardCfg.Clear();
	return __super::ResetGameFrame();
}

//��ʼ��ť����
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
	OnControlHitBegin();
	return 0;
}

bool CClientGameDlg::OnControlHitBegin()
{
	if ( ( (GetStationParameter()==GS_WAIT_NEXT) || (GetStationParameter()==GS_WAIT_ARGEE) ) && (!(m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME) ) )
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillGameTimer(0);
	ResetGameStation(0);
	return true;;
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{

	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_ROB_NT_TIMER);
	KillTimer(ID_NOTE_TIMER);
	KillTimer(ID_OPEN_CARD_TIMER);

	return;
}

//��ȡ�����ׯ����
int CClientGameDlg::GetMaxRobNT( )
{
	int iMaxNote = 1;
	if ( IsPlayMode( Mode_QZ_MPQZ ) )
	{
		if ( IsPlayMode( Mode_MAX_QZ_CHOOSE1 ) )
		{
			iMaxNote = 1;
		}
		else if ( IsPlayMode( Mode_MAX_QZ_CHOOSE2 ) )
		{
			iMaxNote = 2;
		}
		else if ( IsPlayMode( Mode_MAX_QZ_CHOOSE3 ) )
		{
			iMaxNote = 3;
		}
		else if ( IsPlayMode( Mode_MAX_QZ_CHOOSE4 ) )
		{
			iMaxNote = 4;
		}
	}
	else if ( IsPlayMode( Mode_QZ_ZYQZ ) )
	{
		iMaxNote = 1;
	}
	return iMaxNote;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	
		{
			KillAllTimer();
			m_tCardCfg.Clear();
			if( GetStationParameter() == GS_WAIT_ARGEE || GetStationParameter() == GS_WAIT_NEXT)
			{
				OnHitBegin(0,0);
			}			
			return true;
		}
	case ID_ROB_NT_TIMER:	//��ׯ��ʱ��
		{
			KillAllTimer();
			if(GetStationParameter() == GS_ROB_NT)
			{
				TMSG_ROB_NT_REQ tRobNT;
				if(m_tCardCfg.byUser != 255)
				{
					//���ܿ��ƿ�����
					if( m_tCardCfg.iShape >= m_tCardCfg.iRobNT)
					{
						tRobNT.iValue = rand() % GetMaxRobNT( ) + 1 ;				//�����ׯ
					}
					else
					{
						tRobNT.iValue = 0;											//����ׯ
					}
				}
				else
				{
					tRobNT.iValue = rand() % ( GetMaxRobNT( ) + 1 );				//���
				}
				SendGameData(&tRobNT, sizeof(tRobNT), MDM_GM_GAME_NOTIFY, ASS_USER_ROBNT, 0);
			}
			return true;
		}
	case ID_NOTE_TIMER:	//��ע
		{
			KillAllTimer();
			if(GetStationParameter() == GS_NOTE)
			{
				int iCount = 0;
				for(int i = 0; i < 3; i++)
				{
					if(m_iCanNote[i] != 0)
					{
						iCount++;
					}
				}
				if( 0 == iCount) return true;
				TMSG_NOTE_REQ userBet;
				if(m_tCardCfg.byUser != 255)
				{
					if(m_tCardCfg.iShape < m_tCardCfg.iNote)
					{
						userBet.iNoteType = m_iCanNote[0];						//��Сע
					}
					else
					{
						userBet.iNoteType= m_iCanNote[iCount - 1];				//���ע
					}
				}
				else
				{
					userBet.iNoteType= m_iCanNote[rand() % iCount];				//�����ע
				}
				SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);
			}
			return true;
		}
	case ID_OPEN_CARD_TIMER:
		{
			KillAllTimer();
			if (GetStationParameter() == GS_OPEN_CARD)
			{
				SendGameData(MDM_GM_GAME_NOTIFY, ASS_USER_OPEN, 0);
			}
			return true;
		}
	}
	return true;
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	memset(m_iCanNote, 0, sizeof(m_iCanNote));
	return;
}

void CClientGameDlg::OnGameSetting()
{
	AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{

}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	
	if (GetStationParameter()>GS_WAIT_ARGEE)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			if (m_pUserInfo[i]!=NULL) 
			{
				m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
			}
		}
	}

	//��ǰ����뿪��������Ƶ����Ҳ��֮�뿪����
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&pUserItem!=NULL&&!bWatchUser==TRUE)
	{
	
		OnCancel();

	}
	return true;

}

void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	CLoveSendClass::OnTimer(nIDEvent);
}


void CClientGameDlg::OnCancel()
{
	if(GetStationParameter()>=GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT)		//��ѯ״̬
	{
		return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	KillAllTimer();

	AFCCloseFrame();

	SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);		//����������Ϣ

	__super::OnCancel();
}

