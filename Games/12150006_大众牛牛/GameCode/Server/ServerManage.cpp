#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"
#include "LogModule.h"
#include "WylLog.h"

CString		g_outString;
__int64		g_i64AIJiangChi;
__int64		g_i64ResetAIJiangChi;

void SetAIJiangChi(__int64 NowValue, bool bReset = false)
{
	if(!bReset)
	{
		g_i64AIJiangChi = NowValue;
	}
	else
	{
		g_i64ResetAIJiangChi = NowValue;
	}
}

__int64 GetAIJiangChi( bool bReset = false)
{
	if(!bReset)
	{
		return g_i64AIJiangChi;
	}
	else
	{
		return g_i64ResetAIJiangChi;
	}
}


//���캯��
CServerGameDesk::CServerGameDesk( void ):CGameDesk( ALL_ARGEE )
{
	m_bGameStation=GS_WAIT_ARGEE;//��Ϸ״̬

	m_iAllCardCount		= 52;
	m_byUpGradePeople	= 255;	
	m_iFirstJiao		= 255;
	m_iNTBase			= 1;

	memset( m_iTotalCard, 0, sizeof( m_iTotalCard ) );
	memset( m_iUserCard, 0, sizeof( m_iUserCard ) );
	memset( m_byOpenUnderCard, 0, sizeof( m_byOpenUnderCard ) );
	memset( m_byOpenUpCard, 0, sizeof( m_byOpenUpCard ) );
	memset( m_byBullCard, 0, sizeof( m_byBullCard ) );

	for( int i=0; i<PLAY_COUNT; i++ )
	{
		m_bUserReady[i]			= false;
		m_byUserStation[i]		= STATE_NULL;
		m_iUserCardCount[i]		= 0;
		m_iRobNTBase[i]			= -1;
		m_iOpenShape[i]			= UG_UN_OPEN;
		m_byOpenUnderCount[i]	= 0;
		m_iUserBase[i]			= 0;
	}

	memset( m_iCanNote, 0, sizeof( m_iCanNote ) );
	memset(m_bySonStation, 255, sizeof(m_bySonStation));
	m_bMasterStartGame = false;
	m_byLastGradePeople = 255;
	m_bFirstLoad = true;
	memset(m_SuperSetData, 0, sizeof(m_SuperSetData));

	for (int i = 0; i < MAX_SHAPE_COUNT; ++i)
	{
		m_bCardShapeBase[i] = 1;
	}
	memset(m_bAuto, 0, sizeof(m_bAuto));
	m_bNormalFirst = true;
	m_RoomId = 123456;
}
//��ʼ��
bool CServerGameDesk::InitDeskGameStation( )
{
	//���������ļ�
	LoadIni( );
	memset( &m_TCalculateBoard, 0, sizeof( m_TCalculateBoard ) );

	return true;
}
//����ini�����ļ�
BOOL	CServerGameDesk::LoadIni( )
{
	CString s = CINIFile::GetAppPath ( );/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini" );
	CString key = TEXT( "game" );
	m_iBeginTime	= f.GetKeyVal( key, "begintime", 5 );
	if( m_iBeginTime<5 )
	{
		m_iBeginTime=5;
	}
	m_iThinkTime	= f.GetKeyVal( key, "thinktime", 10 );
	if( m_iThinkTime<10 )
	{
		m_iThinkTime=10;
	}
	m_iXiaZhuTime	= f.GetKeyVal( key, "XiaZhutime", 15 );
	if( m_iXiaZhuTime<10 )
	{
		m_iXiaZhuTime=10;
	}
	m_iRobNTTime    = f.GetKeyVal( key, "RobNTtime", 5 );
	if( m_iRobNTTime<5 )
	{
		m_iRobNTTime=5;
	}

	m_iTickTime = f.GetKeyVal( key, "TickTime", 15);

	m_bSystemOperate = f.GetKeyVal( key, "SystemOperate", 1) > 0 ? true : false;

	m_bHaveBeginButton = f.GetKeyVal( key, "HaveBeginButton", 0) > 0 ? true : false;

	m_iBasePoint = f.GetKeyVal( key, "BasePoint", 1);

	//�������ò�Ҫд�������ļ���
	m_iGoodCard		= f.GetKeyVal( key, "iGoodCard", 5 );
	m_bHaveKing		= f.GetKeyVal( key, "HaveKing", 0 ) > 0 ? true : false;				//�Ƿ�����
	m_iSendCardTime = f.GetKeyVal( key, "Sendcardtime", 100 );			
	m_bTurnRule		= f.GetKeyVal( key, "TurnRule", 0 );
	m_iRandomNTTime = f.GetKeyVal( key, "RandomNTTime", 230 );
	m_iRandomNTRound= f.GetKeyVal( key, "RandomNTRound", 3 );

	//��������
	key = TEXT( "SuperSet" );
	int iSuperCount = f.GetKeyVal( key, "SuperUserCount", 0 );
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear( );
	for ( int j=0; j<iSuperCount; j++ )
	{
		strText.Format( "SuperUserID[%d]", j );
		lUserID = f.GetKeyVal( key, strText, 0 );
		//�����г���ID ���浽����������
		m_vlSuperID.push_back( lUserID );
	}

	key = TEXT( "AIControl" );
	m_vAIContrl.clear();
	int iAICount = f.GetKeyVal( key, "AICount", 0 );
	m_bAICtrl = (f.GetKeyVal( key, "AISwitch", 0 ) == 1);

	for ( int j=0; j<iAICount; j++ )
	{
		TAIControl tAI;
		strText.Format( "Money[%d]", j );
		tAI.iMoney = f.GetKeyVal( key, strText, 0 );
		if(tAI.iMoney  < 0)//����-1���������
		{
			tAI.iMoney = (int)pow(2.0, 32);
		}
		strText.Format( "Lucky[%d]", j );
		tAI.iLucky = f.GetKeyVal( key, strText, 50 );

		strText.Format( "MinBull[%d]", j);
		tAI.iMinBull = f.GetKeyVal( key, strText, 0 );

		strText.Format( "MaxBull[%d]", j);
		tAI.iMaxBull = f.GetKeyVal( key, strText, 0 );
		m_vAIContrl.push_back( tAI );
	}

	SetAIJiangChi( f.GetKeyVal(key, "AIHaveWinMoney", 0));
	SetAIJiangChi( f.GetKeyVal(key, "ReSetAIHaveWinMoney", 0), true);

	m_bAIRobAndNote = f.GetKeyVal( key, "AIRobAndNote", 1 );
	m_iAIRobNTShape = f.GetKeyVal( key, "AIRobNTShape", 8 );
	m_iAINoteShape = f.GetKeyVal( key, "AINoteShape", 8 );	
	return true;
}

//��������
CServerGameDesk::~CServerGameDesk( void )
{
}
//�Զ���ע
void CServerGameDesk::AutoNote(BYTE byUser)
{
	if(!IsValidPlayer(byUser)) return;
	if(byUser == m_byUpGradePeople) return;
	
	TMSG_NOTE_REQ userBet;
	userBet.iNoteType = m_iCanNote[byUser][0];			
	HandleUserNoteResult( byUser, userBet.iNoteType );
	
}
//�Զ���ׯ
void CServerGameDesk::AutoRobNT(BYTE byUser)
{
	HandleUserRobNT( byUser, 0 );
}

//�Զ�����
void CServerGameDesk::AutoAction(BYTE byUser)
{
	switch(m_bGameStation)
	{
	case GS_NOTE:
		AutoNote(byUser);
		break;
	case GS_OPEN_CARD:
		AutoTanPai(byUser);
		break;
	case GS_ROB_NT:
		AutoRobNT(byUser);
		break;
	default:
		break;
	}
}

//�Զ�̯��
bool CServerGameDesk::AutoTanPai(BYTE byUser)
{
	//��Ч��Ҳ�����
	if(!IsValidPlayer(byUser)) return false;
	if(m_byUserStation[byUser] == STATE_OPEN_CARD) return false;

	GetBullCard( byUser );

	UserTanPai TUserTanPai;
	memcpy( TUserTanPai.byUnderCard, m_byBullCard[byUser], sizeof( TUserTanPai.byUnderCard ) );

	UserOpenCard( byUser, &TUserTanPai );
	return true;
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer( UINT uTimerID )
{
	g_outString.Format("dznn::��ʱ����Ϣ[%d]", uTimerID);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	switch( uTimerID )
	{
	case TIME_AUTO_OPERATOR:
		{
			KillTimer(TIME_AUTO_OPERATOR);
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(!IsValidPlayer(i)) continue;
				if(!m_bAuto[i]) continue;
				AutoAction(i);
			}
			return true;
		}
	case TIMER_ID_KICK:
		{
			if(m_pDataManage != NULL && m_pDataManage->IsQueueGameRoom())
			{
				KillTimer(TIMER_ID_KICK);
				return true;
			}
			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				if (NULL == m_pUserInfo[i]) continue;
				if(m_bGameStation != GS_WAIT_ARGEE && m_bGameStation != GS_WAIT_NEXT) continue;
				m_iSitTime[i]++;
				if (m_iSitTime[i] >= m_iTickTime && m_pUserInfo[i]->m_UserData.bUserState != USER_ARGEE)
				{
					UserLeftDesk(i, m_pUserInfo[i]);
					InitLastData();
				}
			}
			//��Ҷ��߳����رն�ʱ��
			if( GetDeskPlayerNum() == 0)
			{
				KillTimer(TIMER_ID_KICK);
			}
			return true;
		}
	case TIME_OPEN_DELAY:
		{
			KillTimer(TIME_OPEN_DELAY);
			TMSG_ALL_TAN_PAI_FINISH_NTF tTanPaiFinish;
			tTanPaiFinish.byNTDeskStation = m_byUpGradePeople;
			memcpy( tTanPaiFinish.byUnderCard, m_byOpenUnderCard[m_byUpGradePeople], sizeof(tTanPaiFinish.byUnderCard) );
			memcpy( tTanPaiFinish.byUpCard, m_byOpenUpCard[m_byUpGradePeople], sizeof(tTanPaiFinish.byUpCard) );
			tTanPaiFinish.iShape = m_iOpenShape[m_byUpGradePeople];
			tTanPaiFinish.imultiple = m_bCardShapeBase[tTanPaiFinish.iShape];
			for( int i = 0;i < PLAY_COUNT; ++i )
			{
				if( m_pUserInfo[i] )
				{
					SendGameDataEx( i, &tTanPaiFinish, sizeof( tTanPaiFinish ), MDM_GM_GAME_NOTIFY, MSG_ALL_TAN_PAI_FINISH_NTF, 0 );
				}
			}
			SendWatchDataEx( m_bMaxPeople, &tTanPaiFinish, sizeof( tTanPaiFinish ), MDM_GM_GAME_NOTIFY, MSG_ALL_TAN_PAI_FINISH_NTF, 0 );
		
			//����
			SetTimer( TIME_GAME_FINISH, 1000 );
			return true;
		}
	case TIME_GAME_BEGIN: 
		{
			KillTimer( TIME_GAME_BEGIN );
			if ( GS_GAME_BEGIN != m_bGameStation ) return true;

			if ( IsPlayMode( Mode_QZ_ZYQZ ) )
			{
				NotifyRobNT( );
			}
			else if ( IsPlayMode( Mode_QZ_NNSZ ) )
			{
				m_bGameStation = GS_ROB_NT;		//û��֪ͨ��ׯ�׶Σ���������һ��Сʱ��ĵȴ����˴���һ����Ϸ״̬��
				if ( (0 == m_iRunGameCount && m_bIsBuy) || (!m_bIsBuy && m_bNormalFirst))
				{
					int iValidPlayers = GetPlayingPlayerCount( );
					int iRandNum = rand( ) % iValidPlayers;
					m_byUpGradePeople = m_vPlayingPlayer[iRandNum];
					SetCanNote( );
					RobNTFinish( true, iValidPlayers );
				}
				else
				{
					//�����ţţ��ׯ��һ����Ϸ��������ׯ�Ҹ�ֵ;
					SetCanNote( );
					RobNTFinish( false, 1 );
				}
			}
			else if ( IsPlayMode( Mode_QZ_GDZJ ) )
			{
				m_bGameStation = GS_ROB_NT;
				//��һ��ׯ��Ϊ����
				for( int i = 0; i < PLAY_COUNT; i ++ )
				{
					if( m_pUserInfo[i]
					&& m_byUserStation[i] != STATE_NULL
						&& m_pUserInfo[i]->m_UserData.dwUserID == m_iDeskMaster )
					{
						m_byUpGradePeople = i;
						break;
					}
				}

				//Ԥ����������������
				if ( INVALID == m_byUpGradePeople )
				{
					m_byUpGradePeople = GetNextDeskStation( rand( )%PLAY_COUNT );
				}
				SetCanNote( );
				RobNTFinish( false, 1 );
			}
			else if ( IsPlayMode( Mode_QZ_MPQZ ) )
			{
				NotifySendCard( );
			}
			else if ( IsPlayMode( Mode_QZ_TBZZ ) )
			{
				m_bGameStation = GS_NOTE;
				SetCanNote( );
				for ( int i=0; i<PLAY_COUNT; i++ )
				{
					if ( IsValidPlayer( i ) )
					{
						TMSG_NOTE_REQ userBet;
						userBet.iNoteType = m_iCanNote[i][0];
						HandleUserNoteResult( i, userBet.iNoteType );
					}
				}
			}
			return TRUE;
		}
	case TIME_AUTO_NEXT: 
		{
			KillTimer( TIME_AUTO_NEXT );
			if(!m_bSystemOperate) break;

			if ( GS_WAIT_NEXT == m_bGameStation )
			{
				AutoBegin( );
			}

			return TRUE;
		}
	case TIME_AUTO_BEGIN:
		{
			KillTimer( TIME_AUTO_BEGIN );
			if( GS_WAIT_NEXT == m_bGameStation )
			{
				if ( GetCurPlayerCount( ) > 1 )
				{
					GameBegin( true );
				} 
				else
				{
					m_bGameStation = GS_WAIT_ARGEE;
					ReSetGameState( GF_NORMAL );
				}
			}	
		}
		return TRUE;
	case TIME_CALL_ROBNT:	//��ׯ
		{
			KillTimer( TIME_CALL_ROBNT );
			if(!m_bSystemOperate) break;

			if ( m_bGameStation == GS_ROB_NT )
			{
				for ( int i=0; i<PLAY_COUNT; i++ )
				{
					if ( m_pUserInfo[i] != NULL && m_byUserStation[i] == STATE_PLAY_GAME )
					{
						HandleUserRobNT( i, 0 );
					}
				}
			}
			return TRUE;
		}
	case TIME_ROBNT_FINISH:
		{
			KillTimer( TIME_ROBNT_FINISH );
			if( m_bGameStation == GS_NOTE )//++nm
			{
				NoticeUserNote( );
			}	
			return TRUE;
		}
	case TIME_NOTE:	//��ע��ʱ��
		{
			KillTimer( TIME_NOTE );
			if(!m_bSystemOperate) break;
			if ( m_bGameStation == GS_NOTE )
			{
				for ( int i=0; i<PLAY_COUNT; i++ )
				{
					if ( m_pUserInfo[i] != NULL 
						&& i != m_byUpGradePeople )
					{
						TMSG_NOTE_REQ userBet;
						userBet.iNoteType = m_iCanNote[i][0];			
						HandleUserNoteResult( i, userBet.iNoteType );
					}
				}
			}
			return TRUE;
		}
	case TIME_NOTE_FINISH:	//��ע��ɶ�ʱ��
		{
			KillTimer( TIME_NOTE_FINISH );
			if ( m_bGameStation==GS_NOTE ) 
			{
				if ( IsPlayMode( Mode_QZ_MPQZ ) )
				{
					BeginOpenCard( );
				}
				else
				{
					NotifySendCard( );
				}

			}
			return TRUE;
		}
	case TIME_SEND_CARD_FINISH:	//���ƽ�����ʱ��
		{
			KillTimer( TIME_SEND_CARD_FINISH );
			if ( m_bGameStation == GS_SEND_CARD )
			{
				NotifySendCardFinish( );
			}
			return TRUE;
		}
	case TIME_AUTO_BULL:	//�Զ���ţ��ʱ��
		{
			KillTimer( TIME_AUTO_BULL );
			if(!m_bSystemOperate) break;
			// ��ţǰ���ߣ���ׯ�����������û��עǰ���ߣ���ϵͳ�Զ������ţ��
			//if( m_bIsBuy )
			{
				for ( BYTE i=0; i<PLAY_COUNT; i++ )
				{
					if ( NULL!=m_pUserInfo[i] && m_byUserStation[i] != STATE_NULL && m_byUserStation[i] != STATE_OPEN_CARD )
					{
						GetBullCard( i );

						UserTanPai TUserTanPai;
						memcpy( TUserTanPai.byUnderCard, m_byBullCard[i], sizeof( TUserTanPai.byUnderCard ) );

						UserOpenCard( i, &TUserTanPai );
					}
				}
			}
			return TRUE;
		}
	case TIME_GAME_FINISH:	//������ʱ��
		{
			KillTimer( TIME_GAME_FINISH );
			if ( m_bGameStation==GS_OPEN_CARD ) 
			{
				GameFinish( 0, GF_NORMAL );
			}
			return TRUE;
		}
	}
	return __super::OnTimer( uTimerID );
}

//�����Ϣ������
bool CServerGameDesk::HandleFrameMessage( BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser )
{
	switch( pNetHead->bAssistantID )
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{

			char cKey[10];
			CString sPath=CBcfFile::GetAppPath( );
			int iResult = 1;
			if ( CBcfFile::IsFileExist( sPath + "SpecialRule.bcf" ) )
			{
				CBcfFile fsr( sPath + "SpecialRule.bcf" );
				sprintf( cKey, "%d", NAME_ID );
				iResult = fsr.GetKeyVal ( _T( "ForceQuitAsAuto" ), cKey, 0 );
			}

			if ( iResult == 0 )
			{
				return __super::HandleFrameMessage( bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser ); 
			}
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser );
}

//�Ƿ�����Ч��ţţ����
bool CServerGameDesk::IsValidBull(int iShape)
{
	if( ( iShape >= 0 && iShape <= UG_BULL_BULL ) || ( iShape >= UG_BULL_ShunZi && iShape <= UG_BULL_TongHuaShun ) )
	{
		return true;
	}
	return false;
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage( BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser )
{
	if ( m_pUserInfo[bDeskStation] == NULL )
	{
		return false;
	}

	g_outString.Format("dznn::�յ����[%d][%s]��Ϣ[%d]", bDeskStation, m_pUserInfo[bDeskStation]->m_UserData.nickName, pNetHead->bAssistantID);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	switch ( pNetHead->bAssistantID )
	{
	case MSG_AUTO_REQ:
		{
			if (uSize != sizeof(TMSG_AUTO_REQ)) return true;
			if( NULL == pData) return true;
			TMSG_AUTO_REQ * pAuto = (TMSG_AUTO_REQ *)pData;
			HandleAuto(bDeskStation, pAuto->bAuto);
			return true;
		}
	case MSG_SEND_GIFT_REQ:
		{
			if (uSize != sizeof(TMSG_GIFT_REQ_AND_RSP)) return true;
			if( NULL == pData) return true;
			TMSG_GIFT_REQ_AND_RSP * pGiftRsp = (TMSG_GIFT_REQ_AND_RSP *)pData;
			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL == m_pUserInfo[i]) continue;
				SendGameDataEx(i, pGiftRsp, sizeof(TMSG_GIFT_REQ_AND_RSP), MDM_GM_GAME_NOTIFY, MSG_SEND_GIFT_RSP, 0);
			}
			return true;
		}
	case MSG_LAST_GAME_REQ:
		{
			if (uSize != sizeof(TMSG_LAST_GAME_REQ)) return true;
			if (NULL == pData) return true;
			TMSG_LAST_GAME_RSP tGameRsp;
			if (0 == m_iRunGameCount || m_bNormalFirst)
			{
				tGameRsp.iSuccess = -1;
			}
			else
			{
				memcpy(&tGameRsp, &m_tLastGameInfo, sizeof(m_tLastGameInfo));
				tGameRsp.iSuccess = 0;
			}
			SendGameDataEx(bDeskStation, &tGameRsp, sizeof(tGameRsp), MDM_GM_GAME_NOTIFY, MSG_LAST_GAME_RSP, 0);
			return true;
		}
	case MSG_MASTER_START_GAME_REQ:
		{
			if (uSize != sizeof(TMSG_MASTER_START_GAME_REQ)) return true;
			if (NULL == pData) return true;
			TMSG_MASTER_START_GAME_REQ* pMasterStart = (TMSG_MASTER_START_GAME_REQ*) pData;
			HandleMasterStart(bDeskStation, pMasterStart);
			return true;
		}
	case MSG_TI_SHI_REQ:
		{
			if ( GS_OPEN_CARD != m_bGameStation) return true;
			if ( uSize != sizeof(TMSG_TI_SHI_REQ) ) return true;
			if ( NULL == pData) return true;
			
			TMSG_TI_SHI_REQ* tTiShiReq = (TMSG_TI_SHI_REQ*) pData;
			HandleTiShi(bDeskStation, tTiShiReq);
			return true;
		}
	case MSG_CUO_PAI_REQ:
		{
			if ( GS_OPEN_CARD != m_bGameStation) return true;
			m_bySonStation[bDeskStation] = GS_OPEN_CUO_PAI;
			TMSG_CUO_PAI_RSP tCuoPai;
			tCuoPai.byUser = bDeskStation;
			SendGameDataEx( bDeskStation, &tCuoPai, sizeof( tCuoPai ), MDM_GM_GAME_NOTIFY, MSG_CUO_PAI_RSP, 0 );
			return true;
		}
	case MSG_FAN_PAI_REQ:
		{
			if ( GS_OPEN_CARD != m_bGameStation) return true;
			m_bySonStation[bDeskStation] = GS_OPEN_FAN_PAI;
			TMSG_FAN_PAI_RSP tFanPai;
			tFanPai.byUser = bDeskStation;
			SendGameDataEx( bDeskStation, &tFanPai, sizeof( tFanPai ), MDM_GM_GAME_NOTIFY, MSG_FAN_PAI_RSP, 0 );
			return true;
		}
	case ASS_SUPER_USER_SET:	//�������������Ϣ
		{
			if( bWatchUser )
			{
				return true;
			}
			SuperUserSetData * pSuperSet= ( SuperUserSetData* )pData;
			if( NULL == pSuperSet ) 
			{
				return true;
			}
			if (pSuperSet->byDeskStation < 0 || pSuperSet->byDeskStation >= PLAY_COUNT)
			{
				return true;
			}

			if ( IsSuperUser( bDeskStation ) )
			{
				// ţ1��ţţ ���� ����ţ
				if( IsValidBull(pSuperSet->iShape) )
				{
					m_SuperSetData[pSuperSet->byDeskStation].bSetSuccess = true;
				}
				else
				{
					m_SuperSetData[pSuperSet->byDeskStation].bSetSuccess = false;
				}
				m_SuperSetData[pSuperSet->byDeskStation].iShape =pSuperSet->iShape;

				SendGameDataEx( bDeskStation, &m_SuperSetData[pSuperSet->byDeskStation], sizeof( SuperUserSetData ), MDM_GM_GAME_NOTIFY, ASS_SUPER_USER_SET_RESULT, 0 );
			}
			return true;
		}

	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if ( bWatchUser )
				return true;
			if( m_bUserReady[bDeskStation] )
			{
				return true; 
			}

			if ( m_bGameStation != GS_WAIT_ARGEE && m_bGameStation != GS_WAIT_NEXT )
			{
				return true;
			}

			m_bUserReady[bDeskStation] = true ; 
			return __super::HandleNotifyMessage( bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser );
		}
	case ASS_USER_OPEN:		//�û���ţ
		{
			if( bWatchUser )
			{
				return true;
			}
			if( m_bGameStation!=GS_OPEN_CARD )
			{
				return true;
			}
			if ( m_byUserStation[bDeskStation] == STATE_NULL )
			{
				return true;
			}

			GetBullCard( bDeskStation );

			UserTanPai TUserTanPai;
			memcpy( TUserTanPai.byUnderCard, m_byBullCard[bDeskStation], sizeof( TUserTanPai.byUnderCard ) );

			UserOpenCard( bDeskStation, &TUserTanPai);
			return true;
		}
	case ASS_USER_ROBNT:	//�����ׯ
		{
			if ( ( bWatchUser )||( uSize!=sizeof( TMSG_ROB_NT_REQ ) ) ) 
				return true;
			if ( m_byUserStation[bDeskStation] == STATE_NULL )
			{
				return true;
			}
			TMSG_ROB_NT_REQ * pRobNT=( TMSG_ROB_NT_REQ * )pData;
			if ( NULL == pRobNT )
			{
				return true;
			}
			if( m_bGameStation!=GS_ROB_NT )
			{
				return true;
			}
			//LOG_MSG_RECV( bDeskStation, ASS_USER_ROBNT, *pRobNT );
			HandleUserRobNT( bDeskStation, pRobNT->iValue );
			return true;
		}
	case ASS_USER_NOTE:	//�����ע
		{
			if( bWatchUser )
				return true;
			if ( m_byUserStation[bDeskStation] == STATE_HAVE_NOTE || m_byUserStation[bDeskStation] == STATE_NULL )
			{
				return true;
			}

			if ( m_bGameStation != GS_NOTE )
			{
				return true;
			}
			TMSG_NOTE_REQ *pVerb = ( TMSG_NOTE_REQ * ) pData;
			if ( NULL == pVerb )
			{
				return true;
			}
			//LOG_MSG_RECV( bDeskStation, ASS_USER_NOTE, *pVerb );
			return HandleUserNoteResult( bDeskStation, pVerb->iNoteType );

			return TRUE;
		}
	}
	return __super::HandleNotifyMessage( bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser );
}

int CServerGameDesk::GetCurPlayerCount( )
{
	int CurPlayerCount = 0;
	for( int i = 0; i < PLAY_COUNT; ++i )
	{
		if ( m_pUserInfo[i] != NULL )
		{
			++CurPlayerCount;
		}
	}
	return CurPlayerCount;
}

void CServerGameDesk::AutoBegin( )
{
	//if( m_bIsBuy )
	{
		int CurPlayerCount = GetCurPlayerCount( );
		if( CurPlayerCount > 1 )
		{
			KillTimer( TIME_AUTO_BEGIN );
			SetTimer( TIME_AUTO_BEGIN, m_iBeginTime * 1000 + 500 );
			for( int i = 0; i < PLAY_COUNT; ++i )
			{
				if ( m_pUserInfo[i] != NULL )
				{
					SendGameDataEx( i, NULL, 0, MDM_GM_GAME_NOTIFY, ASS_GAME_AUTO_BEGIN, 0 );
				}	
			}
			SendWatchDataEx( m_bMaxPeople, NULL, 0, MDM_GM_GAME_NOTIFY, ASS_GAME_AUTO_BEGIN, 0 );
		}			
	}
}

//�趨��Ϸ������Ϣ
void CServerGameDesk::GetGameBaseStat( GameStation_Base& TGameStation )
{
	//��Ϸ������Ϣ
	TGameStation.byGameStation	= m_bGameStation;		//��Ϸ״̬
	TGameStation.iPlayMode		= m_iPlayMode;

	TGameStation.iVipGameCount	= m_iVipGameCount;		//�������Ӿ���
	TGameStation.iRunGameCount	= m_iRunGameCount + 1;	//��Ϸ���еľ���
	TGameStation.iThinkTime		= m_iThinkTime;			//̯��ʱ��
	TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
	TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
	TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
	TGameStation.iAllCardCount	= m_iAllCardCount;		//�˿���Ŀ
	TGameStation.iRobNTTime     = m_iRobNTTime;         //��ׯʱ��
	TGameStation.iTickTime		= m_iTickTime;			//����ʱ��
	TGameStation.iRoomBasePoint = GetRoomBasePoint( );	//���䱶��
	TGameStation.bSystemOperate = m_bSystemOperate;		//ϵͳ����
	memcpy(TGameStation.bAuto, m_bAuto, sizeof(m_bAuto));//�Ƿ��й�
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation( BYTE bDeskStation, UINT uSocketID, bool bWatchUser )
{
	m_RoomId = m_pDataManage->m_InitData.uRoomID;
	g_outString.Format("dznn::���%d������������Ϸ״̬%d", bDeskStation, m_bGameStation);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	//����ȡ���й�
	if(!m_bIsBuy)
	{
		HandleAuto(bDeskStation, false);
	}

	//��Ϸ�淨��ֵ

	if (m_bIsBuy && m_bFirstLoad)
	{
		int* pMode = reinterpret_cast<int*>(m_szDeskConfig);
		m_iPlayMode = *pMode;
		/*
		if(GetCreateType() == 1)
		{
			LoadPlayMode();
		}*/
		m_bFirstLoad = false;
	}
	else if (!m_bIsBuy)
	{
		LoadPlayMode();
	}

	time_t CurTime = time( NULL );
	// ��֤�����û�Ȩ��
	SuperUserExamine( bDeskStation );

	bool bRetValue = false;

	switch ( m_bGameStation )
	{
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ����Ϸ��ʼ
	case GS_GAME_BEGIN:			//��Ϸ��ʼ״̬
		{
			GameStation_WaiteAgree TGameStation;
			::memset( &TGameStation, 0, sizeof( TGameStation ) );
			GetGameBaseStat( TGameStation );

			TGameStation.byNTStation = m_byUpGradePeople;	//ׯ��λ��
			memcpy( TGameStation.bUserReady , m_bUserReady , sizeof( TGameStation.bUserReady ) ) ;
			SendGameStation( bDeskStation, uSocketID, bWatchUser, &TGameStation, sizeof( TGameStation ) );
			bRetValue = true;
			break;
		}

	case GS_ROB_NT:		//��ׯ״̬
		{
			GameStation_RobNt TGameStation;
			::memset( &TGameStation, 0, sizeof( TGameStation ) );
			GetGameBaseStat( TGameStation );

			TGameStation.byRemainTime = m_iRobNTTime - ( CurTime - tRobNTTime );	//ʣ��ʱ��

			memcpy( TGameStation.iUserCard, m_iUserCard, sizeof( TGameStation.iUserCard ) );

			for ( int i=0; i<PLAY_COUNT; i++ )
			{
				//�û������˿���Ŀ
				TGameStation.iUserCardCount[i]		= m_iUserCardCount[i];			
				TGameStation.byUserRobNT[i] = m_iRobNTBase[i];
				//����ҵ�״̬ ����Ƿ���;�����
				TGameStation.iUserStation[i] = m_byUserStation[i];
			}

			SendGameStation( bDeskStation, uSocketID, bWatchUser, &TGameStation, sizeof( TGameStation ) );
			bRetValue = true;
			break;
		}
	case GS_NOTE:		//��ע״̬
		{
			GameStation_Note TGameStation;
			GetGameBaseStat( TGameStation );

			TGameStation.iStartPos		= m_byUpGradePeople;	//ׯ��λ��

			TGameStation.byRemainTime = m_iXiaZhuTime - ( CurTime - tNoteTime );	//ʣ��ʱ��

			//�û����ϵ��˿�
			memcpy( TGameStation.iUserCard, m_iUserCard, sizeof( TGameStation.iUserCard ) );

			for( int i=0;i<PLAY_COUNT;i++ )
			{
				TGameStation.iUserCardCount[i]		= m_iUserCardCount[i];			//�û������˿���Ŀ
				TGameStation.iUserStation[i]		= m_byUserStation[i];			//�����״̬
				TGameStation.iUserBase[i]			= m_iUserBase[i];				//�û���ע���
			}

			if (IsValidPlayer(m_byUpGradePeople))
			{
				TGameStation.iUserBase[m_byUpGradePeople] = m_iRobNTBase[m_byUpGradePeople];//ׯ����ׯ����
			}

			memcpy( TGameStation.iCanNote, m_iCanNote[bDeskStation], sizeof(TGameStation.iCanNote) );
			
			SendGameStation( bDeskStation, uSocketID, bWatchUser, &TGameStation, sizeof( TGameStation ) );
			bRetValue = true;
			break;
		}
	case GS_SEND_CARD:	
		{
			GameStation_SendCard TGameStation;
			GetGameBaseStat( TGameStation );

			//�û����ϵ��˿�
			memcpy( TGameStation.iUserCard, m_iUserCard, sizeof( TGameStation.iUserCard ) );

			for( int i=0; i<PLAY_COUNT; i++ )
			{
				TGameStation.iUserCardCount[i]		= m_iUserCardCount[i];			//�û������˿���Ŀ
				TGameStation.iUserBase[i]	= m_iUserBase[i];						//�û�ÿ��ע���
				TGameStation.iUserStation[i] = m_byUserStation[i];					//����ҵ�״̬ ����Ƿ���;�����
			}

			if (IsValidPlayer(m_byUpGradePeople))
			{
				TGameStation.iUserBase[m_byUpGradePeople] = m_iRobNTBase[m_byUpGradePeople];//ׯ����ׯ����
			}

			TGameStation.byNTStation = m_byUpGradePeople;	//ׯ��λ��

			SendGameStation( bDeskStation, uSocketID, bWatchUser, &TGameStation, sizeof( TGameStation ) );
			bRetValue = true;
			break;
		}
	case GS_OPEN_CARD:	//��Ϸ��״̬
		{
			GameStation_OpenCard TGameStation;
			GetGameBaseStat( TGameStation );

			TGameStation.byNtStation	= m_byUpGradePeople;	//ׯ��λ��

			TGameStation.byRemainTime	= m_iThinkTime - ( CurTime - tShowCardTime );//ʣ��ʱ��-

			//�û�������
			::memcpy( TGameStation.iUserCardCount, m_iUserCardCount, sizeof( TGameStation.iUserCardCount ) );
			::memcpy( TGameStation.iUserCard, m_iUserCard, sizeof( TGameStation.iUserCard ) );

			
			memcpy( TGameStation.iOpenShape, m_iOpenShape, sizeof( TGameStation.iOpenShape ) );
			memcpy( TGameStation.byOpenUnderCard, m_byOpenUnderCard, sizeof( TGameStation.byOpenUnderCard ) );
			memcpy( TGameStation.byOpenUpCard, m_byOpenUpCard, sizeof( TGameStation.byOpenUpCard ) );

			for( int i=0;i<PLAY_COUNT;i++ )
			{
				TGameStation.iUserBase[i]			= m_iUserBase[i];				//�û�����
				TGameStation.imultiple[i] = m_bCardShapeBase[m_iOpenShape[i]];
				TGameStation.iUserStation[i] = m_byUserStation[i];
			}

			if (IsValidPlayer(m_byUpGradePeople))
			{
				TGameStation.iUserBase[m_byUpGradePeople] = m_iRobNTBase[m_byUpGradePeople];//ׯ����ׯ����
			}

			TGameStation.bySonStation = m_bySonStation[bDeskStation];

			SendGameStation( bDeskStation, uSocketID, bWatchUser, &TGameStation, sizeof( TGameStation ) );

			bRetValue = true;
			break;
		}
	}

	NotifyAiCardShape();

	return bRetValue;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState( BYTE bLastStation )
{
	KillAllTimer( );
	m_iFirstJiao		= 255;
	m_iNTBase			= 1;
	memset( m_iTotalCard, 0, sizeof( m_iTotalCard ) );
	memset( m_iUserCard, 0, sizeof( m_iUserCard ) );
	memset( m_byOpenUnderCard, 0, sizeof( m_byOpenUnderCard ) );
	memset( m_byOpenUpCard, 0, sizeof( m_byOpenUpCard ) );
	memset( m_byBullCard, 0, sizeof( m_byBullCard ) );

	for( int i=0; i<PLAY_COUNT; i++ )
	{
		m_bUserReady[i]			= false;
		m_byUserStation[i]		= STATE_NULL;
		m_iUserCardCount[i]		= 0;
		m_iRobNTBase[i]			= -1;
		m_iOpenShape[i]			= UG_UN_OPEN;
		m_byOpenUnderCount[i]	= 0;
		m_iUserBase[i]			= 0;
	}

	memset( m_iCanNote, 0, sizeof( m_iCanNote ) );
	memset(m_bySonStation, 255, sizeof(m_bySonStation));
	m_vPlayingPlayer.clear();
	memset(m_bAuto, 0, sizeof(m_bAuto));

	return TRUE;
}

void CServerGameDesk::SetValidNN( )
{
	//��������ţ�Ƿ���Ч
	memset( m_bNNValid, 0, sizeof( m_bNNValid ) );
	if ( IsPlayMode( Mode_SHAPE_ZHA_DAN ) )
	{
		m_bNNValid[NN_ZhaDan] = true;
	}
	if ( IsPlayMode( Mode_SHAPE_HU_LU ) )
	{
		m_bNNValid[NN_HuLu] = true;
	}
	if ( IsPlayMode( Mode_SHAPE_WU_HUA ) )
	{
		m_bNNValid[NN_WuHua] = true;
	}
	if ( IsPlayMode( Mode_SHAPE_TONG_HUA ) )
	{
		m_bNNValid[NN_TongHua] = true;
	}
	if ( IsPlayMode( Mode_SHAPE_SHUN_ZI ) )
	{
		m_bNNValid[NN_ShunZi] = true;
	}
	if ( IsPlayMode( Mode_SHAPE_TONG_HUA_SHUN ) )
	{
		m_bNNValid[NN_TongHuaShun] = true;
	}
}

void CServerGameDesk::SetNNPoint( )
{
	if ( IsPlayMode( Mode_FAN_BEI_322 ) )
	{
		m_bCardShapeBase[UG_BULL_SEVEN] = 1;
		m_bCardShapeBase[UG_BULL_EIGHT] = 2;
		m_bCardShapeBase[UG_BULL_NINE] = 2;
		m_bCardShapeBase[UG_BULL_BULL] = 3;
	}
	else
	{
		m_bCardShapeBase[UG_BULL_SEVEN] = 2;
		m_bCardShapeBase[UG_BULL_EIGHT] = 2;
		m_bCardShapeBase[UG_BULL_NINE] = 3;
		m_bCardShapeBase[UG_BULL_BULL] = 4;
	}


	m_bCardShapeBase[UG_BULL_ShunZi] = 5;
	m_bCardShapeBase[UG_BULL_WuHua] = 5;
	m_bCardShapeBase[UG_BULL_TongHua] = 6;
	m_bCardShapeBase[UG_BULL_HuLu] = 7;
	m_bCardShapeBase[UG_BULL_BOMB] = 8;
	m_bCardShapeBase[UG_BULL_TongHuaShun] = 8;
}
//��������
bool CServerGameDesk::FindShape(BYTE bUser, BYTE byRandCards[], int iShape)
{
	//��һ������Ʋ��ܸı���һ��
	for (int i = bUser * SH_USER_CARD; i < m_iAllCardCount; i++)
	{
		for (int j = i + 1; j < m_iAllCardCount; j++)
		{
			for (int k = j + 1; k < m_iAllCardCount; k++ )
			{
				for (int m = k + 1; m < m_iAllCardCount; m++)
				{
					for (int n = m + 1; n < m_iAllCardCount; n++)
					{
						byRandCards[0] = m_iTotalCard[i];
						byRandCards[1] = m_iTotalCard[j];
						byRandCards[2] = m_iTotalCard[k];
						byRandCards[3] = m_iTotalCard[m];
						byRandCards[4] = m_iTotalCard[n];
						if (m_Logic.GetShape(byRandCards, SH_USER_CARD, m_bNNValid) == iShape)
						{
							//ƥ�䵽��֮�󽻻�
							BYTE temp = m_iTotalCard[bUser * SH_USER_CARD + 0];
							m_iTotalCard[bUser * SH_USER_CARD + 0] = m_iTotalCard[i];
							m_iTotalCard[i] = temp;

							temp = m_iTotalCard[bUser * SH_USER_CARD + 1];
							m_iTotalCard[bUser * SH_USER_CARD + 1] = m_iTotalCard[j];
							m_iTotalCard[j] = temp;

							temp = m_iTotalCard[bUser * SH_USER_CARD + 2];
							m_iTotalCard[bUser * SH_USER_CARD + 2] = m_iTotalCard[k];
							m_iTotalCard[k] = temp;

							temp = m_iTotalCard[bUser * SH_USER_CARD + 3];
							m_iTotalCard[bUser * SH_USER_CARD + 3] = m_iTotalCard[m];
							m_iTotalCard[m] = temp;

							temp = m_iTotalCard[bUser * SH_USER_CARD + 4];
							m_iTotalCard[bUser * SH_USER_CARD + 4] = m_iTotalCard[n];
							m_iTotalCard[n] = temp;

							//�ҵ���ȷ��RandCards�����˳�ѭ��
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

//��ȡ����ʵ������
int CServerGameDesk::GetRealShape(int iMinBull, int iMaxBull)
{
	vector<int> vBull;
	for(int i = iMinBull; i <= iMaxBull; i++)
	{
		if(!IsValidBull(i)) continue;
		if( i >= 0 && i < UG_BULL_BULL )
		{
			for(int j = 0; j < 4; j++)
			{
				vBull.push_back(i);
			}
		}
		else
		{
			vBull.push_back(i);
		}
	}
	random_shuffle(vBull.begin(), vBull.end());
	return vBull.front();
}

// ֪ͨ����������
void CServerGameDesk::NotifyAiCardShape()
{
	if(!m_bAIRobAndNote) return;
	int iCount = 0;
	for(int i = 0; i < 3; i++)
	{
		if(m_iCanNote[i] != 0)
		{
			iCount++;
		}
	}

	for( int i=0;i<PLAY_COUNT;i++ )
	{
		if(!m_pUserInfo[i]) continue;
		if (!m_pUserInfo[i]->m_UserData.isVirtual) continue;
		if(!IsValidPlayer(i)) continue;

		TMSG_AI_CARD_SHAPE data;
		data.byUser = i;
		data.iRobNT = m_iAIRobNTShape;
		data.iNote = m_iAINoteShape;
		data.iShape = m_Logic.GetShape( m_iUserCard[i], SH_USER_CARD, m_bNNValid, m_byBullCard[i] );

		SendGameDataEx( i, &data, sizeof( data ), MDM_GM_GAME_NOTIFY, MSG_AI_CARD_SHAPE, 0 );
	}
}

//���ó�������
void CServerGameDesk::SetSuPerUserCards( )
{
	//if(NeedAIControl())
	{
		int iResult = rand( )%100 + 1;
		__int64 iAIJiangChi = GetAIJiangChi();

		bool bControl = false;
		int	iIndex = 0;
		for(int i = 0; i < (int)m_vAIContrl.size(); ++i)
		{
			__int64 iMoney = m_vAIContrl.at(i).iMoney;
			int iLucky = m_vAIContrl.at(i).iLucky;
			if ( iAIJiangChi <= iMoney )
			{
				if(iResult <= iLucky)
				{
					bControl = true;
					iIndex = i;
				}
				break;
			}
		}
		
		if(bControl)
		{
			//�����˳��˿��ƵĻ����˿��Ʋ���Ӱ��
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(m_pUserInfo[i] == nullptr) continue;
				if(!m_SuperSetData[i].bSetSuccess && m_pUserInfo[i]->m_UserData.isVirtual)		//û�г��˿��ƵĻ�����
				{
					m_SuperSetData[i].bSetSuccess = true;
					m_SuperSetData[i].byDeskStation = i;
					//��ƽ�������������ţ�ĸ��ʻ������СһЩ
					m_SuperSetData[i].iShape = GetRealShape(m_vAIContrl[iIndex].iMinBull, m_vAIContrl[iIndex].iMaxBull);

					g_outString.Format("dznn::�����˿��ƣ��������=%d����������%d����ǰ����%I64d", i, m_SuperSetData[i].iShape, GetAIJiangChi());
					WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);
				}
			}
		}
	}

	BYTE bySavedCard[NUM_CARD_ARRAY_WITH_FACE];	//���ݵ��������
	memset(bySavedCard, 255, sizeof(bySavedCard));

	BYTE bUser = 0;		//ʵ���ҵ����������
	int iShape = 0;
	for (int x = 0; x < PLAY_COUNT; ++x)
	{
		if (m_SuperSetData[x].bSetSuccess)
		{
			BYTE byRandCards[SH_USER_CARD];	//����ҵ�������
			memset(byRandCards, 255, sizeof(byRandCards));

			iShape = m_SuperSetData[x].iShape;
			bool bFind = FindShape(bUser, byRandCards, iShape);
			if (bFind)
			{
				//������ȷ����
				for (int y = 0; y < SH_USER_CARD; ++y)
				{
					bySavedCard[x * SH_USER_CARD + y] = byRandCards[y];
				}
				bUser++;	
			}
		}
	}
	//��ȷ���Ʒ�ǰ�棬����������ʣ�����
	for (int i = bUser * SH_USER_CARD; i < NUM_CARD_ARRAY_WITH_FACE; ++i)
	{
		for (int j = 0; j < NUM_CARD_ARRAY_WITH_FACE; ++j)
		{
			if (bySavedCard[j] == INVALID)
			{
				bySavedCard[j] = m_iTotalCard[i];
				break;
			}
		}
	}
	//������Ҫ����
	memcpy( m_iTotalCard, bySavedCard, sizeof(bySavedCard) );

	//ֻ��Чһ��
	memset(m_SuperSetData, 0, sizeof(m_SuperSetData));
}

BYTE CServerGameDesk::UserSitDesk( MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo )
{
	BYTE result = __super::UserSitDesk( pUserSit, pUserInfo );
	if (result == ERR_GR_SIT_SUCCESS && !m_bIsBuy)
	{
		m_iSitTime[pUserSit->bDeskStation] = 0;
		//����Ϸ����;����
		if(m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_NEXT )
		{
			SetTimer(TIMER_ID_KICK, 1000);
		}
	}
	return result;
}


/*---------------------------------------------------------------------------------*/
//��Ϸ��ʼ
bool	CServerGameDesk::GameBegin( BYTE bBeginFlag )
{
	KillTimer(TIMER_ID_KICK);											//�ر����˶�ʱ��
	KillTimer( TIME_AUTO_BEGIN );										//�ر��Զ���ʼ��ʱ��
	if ( __super::GameBegin( bBeginFlag )==false ) 
	{
		GameFinish( 0, GFF_SAFE_FINISH );
		return false;
	}
	m_PlayerCount = 0;
	for(int i =0 ; i<m_bMaxPeople ; i++)
	{
		if(m_pUserInfo[i]!=NULL) 
			m_PlayerCount++;  
	}

	g_outString.Format("dznn::��Ϸ��ʼ����Ϸ�淨%d����ǰ���о���%d�������%s", m_iPlayMode, m_iRunGameCount, m_szDeskPassWord);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	LoadIni( );															//��������
	m_bGameStation = GS_GAME_BEGIN;										//��Ϸ״̬
	GameBeginInit();													//��Ϸ��ʼ��ʼ��
	m_Logic.RandCard( m_iTotalCard, m_iAllCardCount, m_bHaveKing );		//�ַ��˿�
	MakeGoodCard();														//���ɺ���	
	SetSuPerUserCards();												//�������� && �����˿���
	PrepareCard();															//׼���÷�����
	SendGameBegin();													//������Ϸ��ʼ
	NotifyAiCardShape();												//֪ͨ����������

	SetTimer( TIME_GAME_BEGIN, 2000 );
	return true;
}
/*---------------------------------------------------------------------------------*/
//֪ͨ�û���ע
BOOL	CServerGameDesk::NoticeUserNote( )
{
	m_bGameStation = GS_NOTE;

	TMSG_NOTE_NFT TUserNote;
	TUserNote.iNt				= m_byUpGradePeople;	
	memcpy( TUserNote.iCanNote, m_iCanNote, sizeof( m_iCanNote ) );
	if (IsValidPlayer(m_byUpGradePeople))
	{
		TUserNote.iNtRobMul	= m_iRobNTBase[m_byUpGradePeople];
	}
	

	for ( int i=0;i<PLAY_COUNT;i++ ) 
	{
		if ( NULL == m_pUserInfo[i] || m_byUserStation[i] ==STATE_NULL )
		{
			continue;
		}
		SendGameDataEx( i, &TUserNote, sizeof( TUserNote ), MDM_GM_GAME_NOTIFY, ASS_CALL_NOTE, 0 );	
	}
	SendWatchDataEx( m_bMaxPeople, &TUserNote, sizeof( TUserNote ), MDM_GM_GAME_NOTIFY, ASS_CALL_NOTE, 0 );

	SetTimer( TIME_NOTE, m_iXiaZhuTime * 1000 );
	SetTimer( TIME_AUTO_OPERATOR, 1000);
	tNoteTime = time( NULL );//��¼��ʼ��ׯʱ��

	return TRUE;
}

/*---------------------------------------------------------------------------------*/
//�û���ע
bool	CServerGameDesk::HandleUserNoteResult( BYTE bDeskStation, int iNoteType )
{
	if ( STATE_NULL == m_byUserStation[bDeskStation] ) return true;
	if( iNoteType < GetLimitNote( bDeskStation, true ) || iNoteType > GetLimitNote( bDeskStation, false ) ) return true;
	if(m_byUserStation[bDeskStation] == STATE_HAVE_NOTE) return true;
	if(m_bGameStation != GS_NOTE) return true;

	if (iNoteType > 2 * GetBasePoint())
	{
		m_TCalculateBoard.tItem[bDeskStation].iTuiZhuCount++;
	}

	m_iUserBase[bDeskStation]		= iNoteType;
	WylDebugPrintf(m_RoomId, m_bDeskIndex, "�����˿��ƣ����%d��ע%d��", bDeskStation, iNoteType);

	//��Ǹ�����Ѿ���ע��
	m_byUserStation[bDeskStation] = STATE_HAVE_NOTE;

	m_iLastNote[bDeskStation]	= iNoteType;

	TMSG_NOTE_RSP TNoteResult;					
	TNoteResult.byStation = bDeskStation;
	TNoteResult.iNoteValue = iNoteType;
	for ( int i = 0; i < PLAY_COUNT; i++ )
	{
		if( m_pUserInfo[i] )
		{
			SendGameDataEx( i, &TNoteResult, sizeof( TNoteResult ), MDM_GM_GAME_NOTIFY, ASS_CALL_NOTE_RESULT, 0 );
		}		
	}
	SendWatchDataEx( m_bMaxPeople, &TNoteResult, sizeof( TNoteResult ), MDM_GM_GAME_NOTIFY, ASS_CALL_NOTE_RESULT, 0 );

	//û����ע������
	int iNoCount = CountNoNotePeople();
	if ( ( iNoCount == 1 && !IsPlayMode(Mode_QZ_TBZZ) ) || ( iNoCount == 0 && IsPlayMode(Mode_QZ_TBZZ) ) )
	{
		KillTimer( TIME_NOTE );
		SetTimer( TIME_NOTE_FINISH, 500 );
		return true;
	}
	return true;
}
/*---------------------------------------------------------------------------------*/
//�����˿˸��û�
BOOL	CServerGameDesk::NotifySendCard( )
{
	m_bGameStation = GS_SEND_CARD;
	if ( IsPlayMode( Mode_QZ_MPQZ ) || IsPlayMode( Mode_QZ_TBZZ ))
	{
		m_iFirstJiao = GetNextDeskStation( rand( ) % PLAY_COUNT );
	}
	else
	{
		m_iFirstJiao = m_byUpGradePeople;
	}

	for( int i=0;i<PLAY_COUNT;i++ )
	{
		if( m_pUserInfo[i] )
		{
			SendAllCardStruct TSendAllCard;
			TSendAllCard.iStartPos = m_iFirstJiao;
			memcpy( TSendAllCard.iUserCardCount, m_iUserCardCount, sizeof( TSendAllCard.iUserCardCount ) );	
			memcpy( TSendAllCard.iUserCard[i], m_iUserCard[i], sizeof( TSendAllCard.iUserCard[i] ) );
			SendGameDataEx( i, &TSendAllCard, sizeof( TSendAllCard ), MDM_GM_GAME_NOTIFY, ASS_CALL_SEND_CARD, 0 );
			SendWatchDataEx( i, &TSendAllCard, sizeof( TSendAllCard ), MDM_GM_GAME_NOTIFY, ASS_CALL_SEND_CARD, 0 );
		}
	}	

	///����ʱ��
	SetTimer( TIME_SEND_CARD_FINISH, ( GetPlayingPlayerCount( ) * ( m_iSendCardTime*SH_USER_CARD ) )+500 );
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//���ƽ���
BOOL	CServerGameDesk::NotifySendCardFinish( )
{
	for ( int i=0;i<PLAY_COUNT;i++ ) 
	{
		if( m_pUserInfo[i] )
		{
			SendGameDataEx( i, MDM_GM_GAME_NOTIFY, ASS_CALL_SEND_FINISH, 0 );
		}
	}
	SendWatchDataEx( m_bMaxPeople, MDM_GM_GAME_NOTIFY, ASS_CALL_SEND_FINISH, 0 );

	if ( IsPlayMode( Mode_QZ_MPQZ ) )
	{
		NotifyRobNT( );
	} 
	else
	{
		BeginOpenCard( );
	}

	return true;
}
/*---------------------------------------------------------------------------------*/
//��Ϸ��ʼ
BOOL	 CServerGameDesk::BeginOpenCard( )
{
	//������Ϸ״̬
	m_bGameStation=GS_OPEN_CARD;

	//������Ϸ��ʼ��Ϣ
	TNoticeOpenCard TBegin;
	for ( int i=0;i<PLAY_COUNT;i++ )
	{
		memcpy(TBegin.byUserState, m_byUserStation, sizeof(m_byUserStation));
		if ( m_pUserInfo[i] && STATE_NULL != m_byUserStation[i] )
		{
			TBegin.byDeskStation = i;
			SendGameDataEx( i, &TBegin, sizeof( TBegin ), MDM_GM_GAME_NOTIFY, ASS_CALL_OPEN, 0 );
		}
	}
	SendWatchDataEx( m_bMaxPeople, &TBegin, sizeof( TBegin ), MDM_GM_GAME_NOTIFY, ASS_CALL_OPEN, 0 );

	// ����ʱ��ţ
	SetTimer( TIME_AUTO_BULL, m_iThinkTime * 1000 + 200 );
	SetTimer( TIME_AUTO_OPERATOR, 1000);
	tShowCardTime=time( NULL );
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//��Ұ�ţ
BOOL	CServerGameDesk::UserOpenCard( BYTE byDeskStation, void * pData )
{
	UserTanPai *pUserTanPai = ( UserTanPai * ) pData;
	if ( NULL == pUserTanPai || byDeskStation >= PLAY_COUNT || STATE_OPEN_CARD == m_byUserStation[byDeskStation] ) return TRUE;
	if(STATE_NULL == m_byUserStation[byDeskStation] ) return TRUE;
	if(m_bGameStation != GS_OPEN_CARD) return TRUE;

	//������������������
	BYTE byUpCard[2];
	memset( byUpCard, 255, sizeof( byUpCard ) );

	bool bFlagGetBullInfo = false;
	int iShape = UG_NO_POINT;


	//Set Up Card
	{
		BYTE byTmpCard[SH_USER_CARD];
		memcpy( byTmpCard, m_iUserCard[byDeskStation], sizeof( byTmpCard ) );

		int iCountSameUnderCard = 0;// �������ţ���Ƿ����������
		for ( int i=0; i<SH_USER_CARD; i++ )
		{
			for( int j=0;j<3;j++ )
			{
				if ( m_byBullCard[byDeskStation][j] == byTmpCard[i] )
				{
					byTmpCard[i] = 255;
					iCountSameUnderCard ++;
					break;
				}
			}
		}

		int iIndex = 0;
		for ( int i=0; i<SH_USER_CARD; i++ )
		{
			if ( 255 != byTmpCard[i] )
			{
				byUpCard[iIndex] = byTmpCard[i];
				iIndex++;
				if ( iIndex >=2 )
				{
					break;
				}
			}
		}
	}

	iShape = m_Logic.GetShape( m_iUserCard[byDeskStation], SH_USER_CARD, m_bNNValid, m_byBullCard[byDeskStation] );
	if (iShape > UG_BULL_BULL)
	{
		BYTE byCards[SH_USER_CARD];
		memcpy(byCards, m_iUserCard[byDeskStation], sizeof(byCards));
		m_Logic.SortByValue(byCards, SH_USER_CARD, iShape);
		for (int i = 0; i < SH_USER_CARD; ++i)
		{
			if (i < 3)
			{
				m_byBullCard[byDeskStation][i] = byCards[i];
			}
			else
			{
				byUpCard[i - 3] = byCards[i];				
			}
		}
	}

	UserTanPai TUserTanPaiResult;
	TUserTanPaiResult.iShape = iShape;
	TUserTanPaiResult.imultiple = m_bCardShapeBase[iShape];
	TUserTanPaiResult.byDeskStation = byDeskStation;
	//�����������
	memcpy( TUserTanPaiResult.byUpCard, byUpCard, sizeof( TUserTanPaiResult.byUpCard ) );
	memcpy( TUserTanPaiResult.byUnderCard, m_byBullCard[byDeskStation], sizeof( m_byBullCard[byDeskStation] ) );

	m_byUserStation[byDeskStation] = STATE_OPEN_CARD;

	//��ţ����
	m_iOpenShape[byDeskStation] = iShape;
	//��������
	m_byOpenUnderCount[byDeskStation] = 3;	
	//���Ƶ�������
	memcpy( m_byOpenUnderCard[byDeskStation], TUserTanPaiResult.byUnderCard, sizeof( m_byOpenUnderCard[byDeskStation] ) );
	//��������2����
	memcpy( m_byOpenUpCard[byDeskStation], TUserTanPaiResult.byUpCard, sizeof( m_byOpenUpCard[byDeskStation] ) );

	for( int i = 0;i < PLAY_COUNT; ++i )
	{
		if( m_pUserInfo[i] )
		{
			SendGameDataEx( i, &TUserTanPaiResult, sizeof( TUserTanPaiResult ), MDM_GM_GAME_NOTIFY, ASS_CALL_OPEN_RESULT, 0 );
		}
	}
	SendWatchDataEx( m_bMaxPeople, &TUserTanPaiResult, sizeof( TUserTanPaiResult ), MDM_GM_GAME_NOTIFY, ASS_CALL_OPEN_RESULT, 0 );

	//����Ƿ񶼰�ţ��
	if ( CheckFinish( ) )
	{
		KillTimer( TIME_AUTO_BULL );
		if (m_byUpGradePeople != INVALID)
		{
			SetTimer(TIME_OPEN_DELAY, 1000);
			return TRUE;
		}
		
		//����
		SetTimer( TIME_GAME_FINISH, 1000 );
	}
	return TRUE;
}

//���ţ����
void CServerGameDesk::GetBullCard( BYTE byDeskStation )
{
	BYTE bResult[3];
	memset( bResult, 0, sizeof( bResult ) );
	if ( m_Logic.GetBull( m_iUserCard[byDeskStation], SH_USER_CARD, bResult ) )
	{
		::CopyMemory( m_byBullCard[byDeskStation], bResult, sizeof( bResult ) );
	}
	else
	{
		memset( m_byBullCard[byDeskStation], 0, sizeof( m_byBullCard[byDeskStation] ) );
		m_byBullCard[byDeskStation][0] = m_iUserCard[byDeskStation][0];
		m_byBullCard[byDeskStation][1] = m_iUserCard[byDeskStation][1];
		m_byBullCard[byDeskStation][2] = m_iUserCard[byDeskStation][2];
	}
}

/*---------------------------------------------------------------------------------*/
//����Ƿ����
BOOL CServerGameDesk::CheckFinish( )
{
	int iOpenCount = 0;
	for ( int i = 0;i < PLAY_COUNT;i++ )
	{
		if( !m_pUserInfo[i] )
		{
			continue;
		}
		if ( m_byUserStation[i] == STATE_OPEN_CARD )
		{
			iOpenCount++;
		}
	}

	if( iOpenCount == GetPlayingPlayerCount( ) )			
	{
		return TRUE;
	}
	return FALSE;
}

BYTE CServerGameDesk::CountNoNotePeople( )
{
	int iCount=0;
	for( int i=0;i<PLAY_COUNT;i++ )
	{
		if( !IsValidPlayer( i ) ) continue;
		if( m_byUserStation[i] == STATE_HAVE_NOTE ) 
		{
			iCount++;
		}
	}

	return GetPlayingPlayerCount( ) - iCount;
}
//��¼��һ����Ϣ
void CServerGameDesk::RecoderLastGame( __int64 iChangeMoney[], const int iSize )
{
	m_tLastGameInfo.byNTDeskStation = m_byUpGradePeople;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if(!IsValidPlayer(i)) continue;
		memcpy(m_tLastGameInfo.byUnderCard[i], m_byOpenUnderCard[i], sizeof(m_byOpenUnderCard[i]) );
		memcpy(m_tLastGameInfo.byUpCard[i], m_byOpenUpCard[i], sizeof(m_byOpenUpCard[i]) );
		m_tLastGameInfo.iShape[i] = m_iOpenShape[i];
		m_tLastGameInfo.imultiple[i] =	m_bCardShapeBase[m_iOpenShape[i]];
		m_tLastGameInfo.iChangeMoney[i] = iChangeMoney[i];
		m_tLastGameInfo.iNoteValue[i] = m_iUserBase[i];
	}
}

//��Ϸ����
bool CServerGameDesk::GameFinish( BYTE bDeskStation, BYTE bCloseFlag )
{
	g_outString.Format("dznn::��Ϸ�������������%d", bCloseFlag);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	KillTimer( TIME_AUTO_BULL );
	KillAllTimer( );

	switch ( bCloseFlag )
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			m_bGameStation=GS_WAIT_NEXT;
			m_byLastGradePeople = m_byUpGradePeople;
			m_bNormalFirst = false;

			GameEndStruct TGameEnd;
			::memset( &TGameEnd, 0, sizeof( TGameEnd ) );

			for ( int i = 0;i < PLAY_COUNT;i++ )
			{
				if ( !IsValidPlayer( i ) ) continue;
				TGameEnd.iUserState[i] = m_byUserStation[i];
			}

			//ͨ��ţţ�ҳ�������Ҽٶ�Ϊׯ������ͳһ�������
			if ( IsPlayMode( Mode_QZ_TBZZ ) )
			{
				m_byUpGradePeople = GetWinPeo( );
			}

			__int64 iNtPoint = 0;
			for ( int i = 0;i < PLAY_COUNT;i++ )
			{
				if ( !IsValidPlayer( i ) ) continue;
				if( i != m_byUpGradePeople )
				{
					TGameEnd.iChangeMoney[i] = GetRoomBasePoint() * ComputePoint( i )*m_iUserBase[i]*m_iNTBase;

					iNtPoint += TGameEnd.iChangeMoney[i];
				}
			}
			if (IsValidPlayer(m_byUpGradePeople) )
			{
				TGameEnd.iChangeMoney[m_byUpGradePeople] = iNtPoint * -1;
			}
			

			__int64 temp_point[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset( temp_cut, 0, sizeof( temp_cut ) );
			for ( int i=0;i<PLAY_COUNT;i++ )
			{
				temp_point[i]=TGameEnd.iChangeMoney[i];
				m_iLastWinPoint[i] = (int)TGameEnd.iChangeMoney[i] / GetRoomBasePoint( );
			}
			RecordAiHaveWinMoney( &TGameEnd );

			ChangeUserPointint64( temp_point, temp_cut );

			//ȫ�ǻ����˾Ͳ���¼
			bool bHaveUser = false;
			for( int i=0; i<PLAY_COUNT; i++ )
			{
				if ( NULL == m_pUserInfo[i] )
				{
					continue;
				}

				if ( 0 == m_pUserInfo[i]->m_UserData.isVirtual )
				{
					bHaveUser = true;
				}
			}

			if ( bHaveUser )
			{
				__super::RecoderGameInfo( TGameEnd.iChangeMoney );
			}

			RecoderLastGame(TGameEnd.iChangeMoney, PLAY_COUNT);

			for ( int i=0; i < PLAY_COUNT; i++ ) 
			{
				if ( m_pUserInfo[i]!=NULL ) 
				{
					SendGameDataEx( i, &TGameEnd, sizeof( TGameEnd ), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0 );
				}
			}
			SendWatchDataEx( m_bMaxPeople, &TGameEnd, sizeof( TGameEnd ), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0 );
			for ( int i = 0; i < PLAY_COUNT; i++ )
			{
				m_TCalculateBoard.tItem[i].i64WinMoney += TGameEnd.iChangeMoney[i];
			}

			if ( m_bIsBuy ) //������
			{
				for ( int i = 0; i < PLAY_COUNT; i++ )
				{
					if(IsPlayMode(Mode_QZ_TBZZ)) continue;	//ͨ��ţţ��ͳ����ׯ����
					if ( m_pUserInfo[i]==NULL || m_byUserStation[i] != STATE_OPEN_CARD ) 
						continue;
					//ׯ�Ҿ���/ʤ������/ʧ�ܾ���
					if ( m_byUpGradePeople == i )
					{
						m_TCalculateBoard.tItem[i].iZuoZhuangCount++;
					} 
				}
				if( m_iVipGameCount <= m_iRunGameCount ) //����������
				{
					m_bGameStation = GS_WAIT_ARGEE;
					UpdateCalculateBoard( );
					ReSetGameState( bCloseFlag );
					__super::GameFinish( bDeskStation, bCloseFlag );
					return true;
				}
			}

			//�����ԭ
			if ( IsPlayMode( Mode_QZ_TBZZ ) )
			{
				m_byUpGradePeople = INVALID;
			}
			else if (IsPlayMode(Mode_QZ_MPQZ) || IsPlayMode(Mode_QZ_ZYQZ))
			{
				m_byUpGradePeople = INVALID;
			}
			else if ( IsPlayMode( Mode_QZ_NNSZ ) )
			{
				BYTE byNNUser = MaxNNUser( );
				if ( byNNUser != INVALID )
				{
					m_byUpGradePeople = byNNUser;
				}
			}

			ReSetGameState( bCloseFlag );
			__super::GameFinish( bDeskStation, bCloseFlag );
			
			if (!m_bIsBuy)
			{
				SetTimer(TIMER_ID_KICK, 1000);
				for (int i = 0; i < PLAY_COUNT; ++i)
				{
					m_iSitTime[i] = 0;
				}
			}
			else if(m_bSystemOperate)
			{
				SetTimer( TIME_AUTO_NEXT, 3000 );
			}
			return true;
		}
	case GFF_SAFE_FINISH:
		{
			m_bGameStation = GS_WAIT_ARGEE;
			GameEndStruct TGameEnd;
			::memset( &TGameEnd, 0, sizeof( TGameEnd ) );
			for ( int i=0; i < PLAY_COUNT; i++ ) 
			{
				if ( m_pUserInfo[i]!=NULL ) 
				{
					SendGameDataEx( i, &TGameEnd, sizeof( TGameEnd ), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0 );
				}
			}
			SendWatchDataEx( m_bMaxPeople, &TGameEnd, sizeof( TGameEnd ), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0 );
			ReSetGameState( bCloseFlag );
			__super::GameFinish( bDeskStation, bCloseFlag );
			return true;
		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			m_bGameStation=GS_WAIT_ARGEE;

			GameEndStruct CutEnd;
			memset( &CutEnd, 0, sizeof( CutEnd ) );
			int total = 0;
			for ( int i=0; i<PLAY_COUNT; i++ )
			{
				total+=m_iUserBase[i];
			}

			if ( bDeskStation == m_byUpGradePeople )
			{
				CutEnd.iChangeMoney[bDeskStation]= -total*5 * GetRoomBasePoint( );
			}
			else
			{
				CutEnd.iChangeMoney[bDeskStation]= -m_iUserBase[bDeskStation]*5 * GetRoomBasePoint( );		//�۳�ǿ�����ȫ����ע
			}

			bool temp_cut[PLAY_COUNT];
			memset( temp_cut, 0, sizeof( temp_cut ) );
			int i=0;
			for ( i=0; i<PLAY_COUNT; ++i ) 
			{ 
				temp_cut[i] = ( bDeskStation == i )?true:false; 
			} 
			if (!m_bIsBuy)
			{
				ChangeUserPointint64( CutEnd.iChangeMoney, temp_cut );
				//ȫ�ǻ����˾Ͳ���¼
				bool bHaveUser = false;
				for( int i=0; i<PLAY_COUNT; i++ )
				{
					if ( NULL == m_pUserInfo[i] )
					{
						continue;
					}

					if (!m_pUserInfo[i]->m_UserData.isVirtual )
					{
						bHaveUser = true;
						break;
					}
				}

				if ( bHaveUser )
				{
					__super::RecoderGameInfo( CutEnd.iChangeMoney );
				}
			}
			
			for ( int i=0;i<m_bMaxPeople;i++ )
				SendGameDataEx( i, &CutEnd, sizeof( CutEnd ), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0 );
			SendWatchDataEx( m_bMaxPeople, &CutEnd, sizeof( CutEnd ), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0 );

			ReSetGameState( bCloseFlag );
			__super::GameFinish( bDeskStation, bCloseFlag );
			return true;
		}
	case GFF_DISSMISS_FINISH:
		{
			DissMissRoomBefore( );
			m_bGameStation = GS_WAIT_ARGEE;
			ReSetGameState( bCloseFlag );
			__super::GameFinish( bDeskStation, bCloseFlag );
			return true;

		}
	default:
		break;
	}

	//��������
	ReSetGameState( bCloseFlag );
	__super::GameFinish( bDeskStation, bCloseFlag );
	return true;
}
//����ʼ������
void CServerGameDesk::InitLastData()
{
	m_byUpGradePeople = 255;
	m_bMasterStartGame = false;
	memset( &m_TCalculateBoard, 0, sizeof( m_TCalculateBoard ) );
	m_bFirstLoad = true;
	memset(m_SuperSetData, 0, sizeof(m_SuperSetData));

	m_byLastGradePeople = 255;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		m_iLastNote[i]			= ( numeric_limits<int>::max )( );			//�㹻�󣬱�֤��һ�ֲ��ܹ���ע
		m_iLastWinPoint[i]		= 0;										//û��Ӯ����֤��һ�ֲ��ܹ���ע}
	}
	m_tLastGameInfo.Init();
	m_bNormalFirst = true;
}

void CServerGameDesk::UpdateCalculateBoard( )
{
	if (IsPlayGameEx(0) )
	{
		int iBigWinnerStation = 0;	
		int iBigLoserStation = 0;

		for ( int i = 0; i < PLAY_COUNT; i++ )
		{
			if ( m_pUserInfo[i] != NULL )
			{
				m_TCalculateBoard.tItem[i].iUserID = m_pUserInfo[i]->m_UserData.dwUserID;
				strcpy(m_TCalculateBoard.tItem[i].chUserName, m_pUserInfo[i]->m_UserData.nickName);
			}
			//��Ӯ��
			if( m_TCalculateBoard.tItem[i].i64WinMoney >= m_TCalculateBoard.tItem[iBigWinnerStation].i64WinMoney )
			{
				iBigWinnerStation = i;
			}
			//��Ӯ��
			if( m_TCalculateBoard.tItem[i].i64WinMoney <= m_TCalculateBoard.tItem[iBigLoserStation].i64WinMoney )
			{
				iBigLoserStation = i;
			}

		}
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			if ( NULL == m_pUserInfo[i] ) continue;
			if ( 0 == m_TCalculateBoard.tItem[i].i64WinMoney) continue;
			if ( m_TCalculateBoard.tItem[iBigWinnerStation].i64WinMoney == m_TCalculateBoard.tItem[i].i64WinMoney )
			{	
				m_TCalculateBoard.tItem[i].bWinner = true;
			}
			if ( m_TCalculateBoard.tItem[iBigLoserStation].i64WinMoney == m_TCalculateBoard.tItem[i].i64WinMoney )
			{
				m_TCalculateBoard.tItem[i].bLoser = true;
			}
		}

		for ( int i = 0; i < PLAY_COUNT; i++ )
		{
			if ( m_pUserInfo[i] == NULL )
			{
				continue;
			}
			SendGameDataEx( i, &m_TCalculateBoard, sizeof( m_TCalculateBoard ), MDM_GM_GAME_NOTIFY, S_C_UPDATE_CALCULATE_BOARD_SIG, 0 );
		}
		SendWatchDataEx( m_bMaxPeople, &m_TCalculateBoard, sizeof( m_TCalculateBoard ), MDM_GM_GAME_NOTIFY, S_C_UPDATE_CALCULATE_BOARD_SIG, 0 );
	}
	InitLastData();
}

//����й�����
void CServerGameDesk::HandleAuto(BYTE byUser, bool bAuto)
{
	if(!IsValidPlayer(byUser)) return;
	if(m_bAuto[byUser] == bAuto) return ;
	if(!IsPlayGameEx(byUser)) return;

	m_bAuto[byUser] = bAuto;
	TMSG_AUTO_RSP tAutoRsp;
	tAutoRsp.byUser = byUser;
	tAutoRsp.bAuto = bAuto;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(!IsValidPlayer(i)) continue;
		SendGameDataEx(i, &tAutoRsp, sizeof(tAutoRsp), MDM_GM_GAME_NOTIFY, MSG_AUTO_RSP, 0);
	}

	if(m_bAuto[byUser])
	{
		AutoAction(byUser);
	}
}

bool CServerGameDesk::UserNetCut( BYTE bDeskStation, CGameUserInfo *pLostUserInfo )
{
	g_outString.Format("dznn::���%d����", bDeskStation );
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	//���߿����й�
	if(!m_bIsBuy)
	{
		HandleAuto(bDeskStation, true);
	}

	m_bUserReady[bDeskStation] = false ; 
	return __super:: UserNetCut( bDeskStation, pLostUserInfo );
}

//�û���������
bool CServerGameDesk::UserReCome( BYTE bDeskStation, CGameUserInfo * pNewUserInfo )
{

	return __super:: UserReCome( bDeskStation, pNewUserInfo );
}

 bool CServerGameDesk::CanBeginGame()
 {
	 bool bRetValue = false; 
	 if (!m_bIsBuy)
	 {
		bRetValue = __super::CanBeginGame();//��ҳ�
	 }
	 else
	 {
		 int count =0;
		 for(int i =0 ; i<PLAY_COUNT ; i++)
		 {
			 if(m_pUserInfo[i]!=NULL && m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE) 
				 count++;  
		 }
		 if(m_iRunGameCount==0 && count >= 2)
			 bRetValue = true;
		 else if(m_iRunGameCount>0 && count == m_PlayerCount)
			 bRetValue = true;
	 }
//	 if (m_bIsBuy && m_iRunGameCount > 0 ) return true;
	 if(!m_bMasterStartGame && m_bIsBuy && m_bHaveBeginButton) return false;
	 return bRetValue;
 }



void CServerGameDesk::DissMissRoomBefore( )
{
	UpdateCalculateBoard( );
}

//��ȡĬ���淨
void CServerGameDesk::LoadPlayMode()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");

	m_iPlayMode = f.GetKeyVal( key, "PlayMode", 0);
	if(0 == m_iPlayMode)
	{
		//����Ĭ��ֵ
		m_iPlayMode = 0 | Mode_QZ_TBZZ | Mode_DF_CHOOSE1 | Mode_TZ_CHOOSE1 ;
	}
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame( BYTE bDeskStation )
{	
	if( !m_bIsBuy && m_byUserStation[bDeskStation] == STATE_NULL ) return false;
	if( GS_ROB_NT==m_bGameStation 
		|| GS_SEND_CARD==m_bGameStation 
		|| GS_OPEN_CARD==m_bGameStation
		|| GS_NOTE==m_bGameStation
		|| GS_GAME_BEGIN==m_bGameStation) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGameEx( BYTE bDeskStation )
{	
	if( !m_bIsBuy && m_byUserStation[bDeskStation] == STATE_NULL ) return false;
	if (m_bIsBuy && m_iRunGameCount > 0) return true;
	if( GS_ROB_NT==m_bGameStation 
		|| GS_SEND_CARD==m_bGameStation 
		|| GS_OPEN_CARD==m_bGameStation
		|| GS_NOTE==m_bGameStation
		|| GS_GAME_BEGIN==m_bGameStation) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk( BYTE bDeskStation, CGameUserInfo * pUserInfo )
{
	g_outString.Format("dznn::���%dվ��", bDeskStation );
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);

	if (m_bIsBuy)
	{
		InitLastData();
	}
	else
	{
		if( m_bGameStation <= GS_WAIT_ARGEE || m_bGameStation >= GS_WAIT_NEXT)
		{
			g_outString.Format("dbn::���%dվ��", bDeskStation );
			WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);
			InitLastData();
		}
		
	}
	
	m_bUserReady[bDeskStation] = false ; 
	return __super::UserLeftDesk( bDeskStation, pUserInfo );
}

//�Α���A����
int CServerGameDesk::GetRoomBasePoint( )
{
	//int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
	int iBasePoint = m_iBasePoint;//����
	return ( iBasePoint>0?iBasePoint:1 );
}
//���������ʾ����
void CServerGameDesk::HandleTiShi(BYTE bDeskStation, TMSG_TI_SHI_REQ* tTiShiReq)
{
	m_bySonStation[bDeskStation] = GS_OPEN_TI_SHI;

	TMSG_TI_SHI_RSP tTiShiRsp;
	GetBullCard( bDeskStation );

	tTiShiRsp.byStation = bDeskStation;

	tTiShiRsp.iShape = m_Logic.GetShape( m_iUserCard[bDeskStation], SH_USER_CARD, m_bNNValid, m_byBullCard[bDeskStation] );

	BYTE byUpCard[2];
	memset( byUpCard, 255, sizeof( byUpCard ) );
	{
		BYTE byTmpCard[SH_USER_CARD];
		memcpy( byTmpCard, m_iUserCard[bDeskStation], sizeof( byTmpCard ) );

		int iCountSameUnderCard = 0;// �������ţ���Ƿ����������
		for ( int i=0; i<SH_USER_CARD; i++ )
		{
			for( int j=0;j<3;j++ )
			{
				if ( m_byBullCard[bDeskStation][j] == byTmpCard[i] )
				{
					byTmpCard[i] = 255;
					iCountSameUnderCard ++;
					break;
				}
			}
		}

		int iIndex = 0;
		for ( int i=0; i<SH_USER_CARD; i++ )
		{
			if ( 255 != byTmpCard[i] )
			{
				byUpCard[iIndex] = byTmpCard[i];
				iIndex++;
				if ( iIndex >=2 )
				{
					break;
				}
			}
		}
	}

	if (tTiShiRsp.iShape > UG_BULL_BULL)
	{
		BYTE byCards[SH_USER_CARD];
		memcpy(byCards, m_iUserCard[bDeskStation], sizeof(byCards));
		m_Logic.SortByValue(byCards, SH_USER_CARD,tTiShiRsp.iShape);
		for (int i = 0; i < SH_USER_CARD; ++i)
		{
			if (i < 3)
			{
				m_byBullCard[bDeskStation][i] = byCards[i];
			}
			else
			{
				byUpCard[i - 3] = byCards[i];				
			}
		}
	}

	memcpy( tTiShiRsp.byUnderCard, m_byBullCard[bDeskStation], sizeof( tTiShiRsp.byUnderCard ) );
	memcpy( tTiShiRsp.byUpCard, byUpCard, sizeof( tTiShiRsp.byUpCard ) );

	SendGameDataEx( bDeskStation, &tTiShiRsp, sizeof( tTiShiRsp ), MDM_GM_GAME_NOTIFY, MSG_TI_SHI_RSP, 0 );
}

//����ׯ�ҿ�ʼ��Ϸ
void CServerGameDesk::HandleMasterStart(BYTE bDeskStation, TMSG_MASTER_START_GAME_REQ* pMasterStart)
{
	TMSG_MASTER__RSP tMasterRsp;
	if (NULL == m_pUserInfo[bDeskStation]) tMasterRsp.iSuccess = -1;
	if (m_pUserInfo[bDeskStation]->m_UserData.dwUserID != m_iDeskMaster) tMasterRsp.iSuccess = -2;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		//�����û׼��
		if (m_pUserInfo[i] != NULL && !m_bUserReady[i] )
		{
			tMasterRsp.iSuccess = -3;
		}
	}
	if (0 != m_iRunGameCount) tMasterRsp.iSuccess = -4;
	if (!pMasterStart->bCanBegin) tMasterRsp.iSuccess = -5;
	if (GetDeskPlayerNum() < 2) tMasterRsp.iSuccess = -6;

	if (tMasterRsp.iSuccess == 0)
	{
		m_bMasterStartGame = true;
		SendGameDataEx(bDeskStation, &tMasterRsp, sizeof(tMasterRsp), MDM_GM_GAME_NOTIFY, MSG_MASTER__RSP, 0);
		GameBegin( 0 );
		return;
	}
	SendGameDataEx(bDeskStation, &tMasterRsp, sizeof(tMasterRsp), MDM_GM_GAME_NOTIFY, MSG_MASTER__RSP, 0);
}

///�������ţţ���
BYTE CServerGameDesk::MaxNNUser( )
{
	vector<PlayerStation> vNNUser;
	for ( BYTE i = 0; i < PLAY_COUNT; ++i )
	{
		if ( !IsValidPlayer( i ) ) continue;
		if ( m_iOpenShape[i] == UG_BULL_BULL )
		{
			vNNUser.push_back( i );
		}
	}
	int iSize = ( int ) vNNUser.size( );
	if ( iSize == 0 )
	{
		return INVALID;
	} 
	else if ( iSize == 1 )
	{
		return vNNUser[0];
	}
	else
	{
		BYTE byTempPos = vNNUser[0];
		for ( BYTE i = 0; i < iSize - 1; ++i )
		{
			if( m_Logic.CompareCard( m_iUserCard[vNNUser[i]], SH_USER_CARD, m_iUserCard[vNNUser[i + 1]], 
				SH_USER_CARD, m_bNNValid ) != 1 )
			{
				byTempPos = vNNUser[i + 1];
			}
		}
		return byTempPos;
	}
}

///���ʤ�����
BYTE CServerGameDesk::GetWinPeo( )
{
	//������Ϸ�е���ҷ���һ��������
	int iArray[PLAY_COUNT] = {0};
	int iIndex = 0;

	for( int i = 0; i < PLAY_COUNT; ++i )
	{
		if( IsValidPlayer( i ) )
		{
			iArray[iIndex] = i;
			iIndex++;
		}
	}

	BYTE byTempPeo = iArray[0];
	for( int i = 1; i < iIndex; ++i )
	{
		if( m_Logic.CompareCard( m_iUserCard[byTempPeo], SH_USER_CARD, m_iUserCard[ iArray[i] ], 
			SH_USER_CARD, m_bNNValid ) != 1 )
		{
			byTempPeo = iArray[i];
		}
	}
	return byTempPeo;
}

//������ҷ���
int CServerGameDesk::ComputePoint( BYTE DeskStation )
{
	if (!IsValidPlayer(m_byUpGradePeople)) return 0;
	
	int Point = 0;
	if( 1 == m_Logic.CompareCard( m_iUserCard[DeskStation], m_iUserCardCount[DeskStation], 
		m_iUserCard[m_byUpGradePeople], m_iUserCardCount[m_byUpGradePeople], 
		m_bNNValid, 
		m_byBullCard[DeskStation], m_byBullCard[m_byUpGradePeople] ) )
	{
		Point = 1;
	}
	else
	{
		Point = -1;
	}

	int iShapePlayer = UG_NO_POINT;
	int iShapeNT = UG_NO_POINT;
	iShapePlayer = m_iOpenShape[DeskStation];
	iShapeNT = m_iOpenShape[m_byUpGradePeople];

	int iShapeBigger = iShapePlayer;
	if ( iShapeNT > iShapeBigger )
	{
		iShapeBigger = iShapeNT;
	}
	Point = Point * m_bCardShapeBase[iShapeBigger];
	return Point;
}

//����
BOOL CServerGameDesk::ChangeCard( BYTE bDestStation, BYTE bWinStation )
{
	for( int i = 0; i < SH_USER_CARD; i ++ )
	{
		BYTE bTemp = m_iTotalCard[bDestStation * SH_USER_CARD + i];
		m_iTotalCard[bDestStation * SH_USER_CARD + i ] = m_iTotalCard[bWinStation * SH_USER_CARD + i];
		m_iTotalCard[bWinStation * SH_USER_CARD + i] = bTemp;
	}
	return TRUE;
}

//����ָ��������ҵ�����
void	CServerGameDesk::ChangeTwoUserCard( BYTE byFirstDesk, BYTE bySecondDesk )
{
	BYTE byTmpCard[SH_USER_CARD];
	ZeroMemory( byTmpCard, sizeof( byTmpCard ) );

	memcpy( byTmpCard, m_iUserCard[byFirstDesk], sizeof( byTmpCard ) );
	memcpy( m_iUserCard[byFirstDesk], m_iUserCard[bySecondDesk], sizeof( m_iUserCard[byFirstDesk] ) );
	memcpy( m_iUserCard[bySecondDesk], byTmpCard, sizeof( m_iUserCard[bySecondDesk] ) );
}

//��ȡ��һ�����λ��
BYTE CServerGameDesk::GetNextDeskStation( BYTE bDeskStation )
{
	if( !m_bTurnRule )//˳ʱ��
	{
		int i=1;
		for ( ;i<PLAY_COUNT; i++ )
		{
			if ( m_pUserInfo[( bDeskStation + i ) % PLAY_COUNT] && m_byUserStation[( bDeskStation + i ) % PLAY_COUNT] != STATE_NULL )
				break;
		}
		return ( bDeskStation + i ) % PLAY_COUNT;
	}
	//��ʱ��
	int i=PLAY_COUNT-1;
	for ( ;i>=1; i-- )
	{
		if ( m_pUserInfo[( bDeskStation + i ) % PLAY_COUNT] &&  m_byUserStation[( bDeskStation + i ) % PLAY_COUNT] != STATE_NULL )
			break;
	}
	return 	( bDeskStation + i ) % PLAY_COUNT;
}

/*---------------------------------------------------------------------------------*/
//������ׯ
BOOL	CServerGameDesk::NotifyRobNT( )
{																
	m_bGameStation = GS_ROB_NT;   //��Ϸ״̬
	TMSG_ROB_NT_NTF TRobNT;						
	for( BYTE i = 0; i < PLAY_COUNT; i ++ )
	{
		if (!IsValidPlayer(i)) continue;
		TRobNT.byStation = i;
		SendGameDataEx( i, &TRobNT, sizeof( TRobNT ), MDM_GM_GAME_NOTIFY, ASS_CALL_ROBNT, 0 );
	}
	SendWatchDataEx( m_bMaxPeople, &TRobNT, sizeof( TRobNT ), MDM_GM_GAME_NOTIFY, ASS_CALL_ROBNT, 0 );

	SetTimer( TIME_CALL_ROBNT, m_iRobNTTime * 1000 );
	SetTimer( TIME_AUTO_OPERATOR, 1000);
	tRobNTTime = time( NULL );//��¼��ʼ��ׯʱ��
	return TRUE;
}

/*---------------------------------------------------------------------------------*/
//�Ƿ���ׯ����
BOOL CServerGameDesk::IsRobNTFinish( )
{
	int playnum = 0;
	int robNTFinishNum = 0;
	for( int i = 0; i < PLAY_COUNT; i ++ )
	{
		if( NULL == m_pUserInfo[i] )
		{
			continue;
		}
		if( m_byUserStation[i] == STATE_HAVE_ROBNT || m_byUserStation[i] == STATE_PLAY_GAME )
		{
			++playnum;
		}

		if( m_byUserStation[i] == STATE_HAVE_ROBNT )
		{
			++robNTFinishNum;
		}
	}
	return playnum == robNTFinishNum;
}

//��ȡ�����ׯ����
int CServerGameDesk::GetMaxRobNT( )
{
	int iMaxNote = 0;
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

//��ȡ�Ѿ�������Ϸ�����
int CServerGameDesk::GetPlayingPlayerCount( )
{
	return ( int )m_vPlayingPlayer.size( );
}

//��ȡ������עֵ
int CServerGameDesk::GetLimitNote( BYTE byUser, bool bMin )
{
	if ( bMin )
	{
		return m_iCanNote[byUser][0];
	}
	else
	{
		for ( int i = 2; i >=0; --i )
		{
			if ( m_iCanNote[byUser][i] != 0 )
			{
				return m_iCanNote[byUser][i];
			}
		}
	}
	return m_iCanNote[byUser][0];
}

//�����ܹ���ע�ķ������ڿ�����������ׯ��ǰ���ã���ֹ��һ�ѵ�ׯ����ע��
void CServerGameDesk::SetCanNote( vector<PlayerStation> &vRandLoser )
{
	if ( IsPlayMode( Mode_QZ_TBZZ ) )
	{
		for ( int i = 0; i < PLAY_COUNT; ++i )
		{
			if ( IsValidPlayer(i) )
			{
				m_iCanNote[i][0] = GetBasePoint( );
			}
		}
	}
	else
	{
		//��ע���ƣ� ������ׯ��������ׯ�У�������ׯ���������ׯ����ѡ�������עʱ��������С�֣�û�в��������ׯ����Ҳ���ʹ����ע��
		if( ( IsPlayMode( Mode_QZ_MPQZ ) || IsPlayMode( Mode_QZ_ZYQZ ) ) && IsPlayMode( Mode_GJ_NOTE_LIMIT ) )
		{
			for ( int i = 0; i < PLAY_COUNT; ++i )
			{
				if (!IsValidPlayer(i) || i == m_byUpGradePeople) continue;

				if ( ( int )vRandLoser.size( ) > 0 && find( vRandLoser.begin( ), vRandLoser.end( ), i ) != vRandLoser.end( ) )
				{
					//�������ѡ�м�
					m_iCanNote[i][0] = GetBasePoint( ) * 2;
					//Ӯ && ��һ��û��ע && �м� && ������һ��ׯ�� && ����ע����
					if ( m_iLastWinPoint[i] > 0 && m_iLastNote[i] <= GetBasePoint( ) * 2 && m_byLastGradePeople != i && !IsPlayMode(Mode_TZ_CHOOSE1))
					{
						m_iCanNote[i][1] = m_iCanNote[i][0] + m_iLastWinPoint[i];
					}
				}
				else
				{
					m_iCanNote[i][0] = GetBasePoint( );
					m_iCanNote[i][1] = m_iCanNote[i][0] * 2;
				}
			}
		}
		else
		{
			//Mode_TZ_CHOOSE1��ʾû����ע����
			if ( IsPlayMode(Mode_TZ_CHOOSE1))
			{
				for ( int i = 0; i < PLAY_COUNT; ++i )
				{
					if (!IsValidPlayer(i) || i == m_byUpGradePeople) continue;
					m_iCanNote[i][0] = GetBasePoint( );
					m_iCanNote[i][1] = m_iCanNote[i][0] * 2;
				}
			}
			else
			{
				for ( int i = 0; i < PLAY_COUNT; ++i )
				{
					if (!IsValidPlayer(i) || i == m_byUpGradePeople) continue;
					m_iCanNote[i][0] = GetBasePoint( );
					m_iCanNote[i][1] = m_iCanNote[i][0] * 2;
					//Ӯ && ��һ��û��ע && �м� && ������һ��ׯ��
					if ( m_iLastWinPoint[i] > 0 && m_iLastNote[i] <= GetBasePoint( ) * 2  && m_byLastGradePeople != i )
					{
						m_iCanNote[i][2] = m_iCanNote[i][1] + m_iLastWinPoint[i];
					}
				}
			}

		}
	}

	//�����ע����
	int iMaxValue = GetMaxTuiZhu( );
	if ( -1 != iMaxValue )		//-1��ʾû����ע����
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{	
				if (m_iCanNote[i][j] > iMaxValue)
				{
					m_iCanNote[i][j] = iMaxValue;
				}
			}
		}
	}
}

//��ȡ�����ע
int CServerGameDesk::GetMaxTuiZhu( )
{
	int iBasePoint = GetBasePoint();
	int iMaxValue = -1;
	if ( IsPlayMode(Mode_TZ_CHOOSE1) )
	{
		iMaxValue = -1;
	}
	else if ( IsPlayMode(Mode_TZ_CHOOSE2) )
	{
		iMaxValue = 5 * iBasePoint;
	}
	else if ( IsPlayMode(Mode_TZ_CHOOSE3) )
	{
		iMaxValue = 10 * iBasePoint;
	}
	else if ( IsPlayMode(Mode_TZ_CHOOSE4) )
	{
		iMaxValue = 15 * iBasePoint;
	}
	return iMaxValue;
}

//��ȡ�׷�
int CServerGameDesk::GetBasePoint( )
{
	int iBasePoint = 1;

	if ( IsPlayMode( Mode_DF_CHOOSE1 ) )
	{
		iBasePoint = 1;
	}
	else if ( IsPlayMode( Mode_DF_CHOOSE2 ) )
	{
		iBasePoint = 2;
	}
	else if ( IsPlayMode( Mode_DF_CHOOSE3 ) )
	{
		iBasePoint = 4;
	}
	else if ( IsPlayMode( Mode_DF_CHOOSE4 ) )
	{
		iBasePoint = 5;
	}
	return iBasePoint;
}


/*---------------------------------------------------------------------------------*/
//��ׯ
BOOL	CServerGameDesk::HandleUserRobNT( BYTE bDeskStation, int iVal )
{
	if( m_byUserStation[bDeskStation] != STATE_PLAY_GAME ) return TRUE;
	if(m_bGameStation != GS_ROB_NT) return TRUE;
	if(m_iRobNTBase[bDeskStation] != -1) return TRUE;

	if( iVal >= 0 && iVal <= GetMaxRobNT( ) ) 
	{
		m_iRobNTBase[bDeskStation] = iVal;
		WylDebugPrintf(m_RoomId, m_bDeskIndex,"�����˿��ƣ������ׯ%d��ׯ%d", bDeskStation,  iVal);

		if (iVal != 0)
		{
			m_TCalculateBoard.tItem[bDeskStation].iQiangZhuangCount++;
		}
	}
	else
	{
		return TRUE;
	}
	m_byUserStation[bDeskStation] = STATE_HAVE_ROBNT;

	TMSG_ROB_NT_RSP TRobNT;				
	TRobNT.byStation = bDeskStation;
	TRobNT.iValue	= m_iRobNTBase[bDeskStation];	

	for( int i = 0; i < PLAY_COUNT; i ++ )
	{
		if (!m_pUserInfo[i]) continue;
		SendGameDataEx( i, &TRobNT, sizeof( TRobNT ), MDM_GM_GAME_NOTIFY, ASS_CALL_ROBNT_RESULT, 0 );		
	}
	SendWatchDataEx( m_bMaxPeople, &TRobNT, sizeof( TRobNT ), MDM_GM_GAME_NOTIFY, ASS_CALL_ROBNT_RESULT, 0 );

	if( IsRobNTFinish( ) )
	{
		KillTimer( TIME_CALL_ROBNT );
		BYTE TopCallScore[PLAY_COUNT];
		memset( TopCallScore, 255, sizeof( TopCallScore ) );
		TopCallScore[0] = 0;
		int count = 1;
		bool bShowDing = false;
		for( int i = 1; i < PLAY_COUNT; ++i )
		{
			if( m_iRobNTBase[i] > m_iRobNTBase[TopCallScore[0]] )
			{
				memset( TopCallScore, 255, sizeof( TopCallScore ) );

				TopCallScore[0] = i;
				count = 1;
			}
			else if( m_iRobNTBase[i] == m_iRobNTBase[TopCallScore[0]] )
			{
				TopCallScore[count] = i;
				count++;
			}
		}
		m_byUpGradePeople = TopCallScore[rand( )%count];

		//������ׯ���������ѡ����
		vector<PlayerStation> vRandLoser;
		if ( m_iRobNTBase[TopCallScore[m_byUpGradePeople]] > 0 )
		{
			for ( int i = 0; i < count; ++i )
			{
				if ( TopCallScore[i] != m_byUpGradePeople )
				{
					vRandLoser.push_back( TopCallScore[i] );
				}
			}
		}

		if( count > 1 )										
		{	
			bShowDing = true;								//���������ׯʱ�ͻ�����Ҫ���Ŷ���
		}	
		if( m_iRobNTBase[TopCallScore[0]] == 0 )				//������ׯ
		{
			m_iRobNTBase[m_byUpGradePeople] = 1;			
			m_iNTBase = 1;	
			for( int i = 0; i < PLAY_COUNT; ++i )
			{
				if( TopCallScore[i] != 255 )
				{
					m_iRobNTBase[TopCallScore[i]] = 1;					//������ׯʱ��������Ҷ�����Ϊ1������ͻ����жϲ��Ŷ���
				}
			}
		}
		else
		{	
			m_iNTBase = m_iRobNTBase[m_byUpGradePeople];	
		}

		SetCanNote( vRandLoser );

		RobNTFinish( bShowDing, count );
	}	
	return true;
}
/*---------------------------------------------------------------------------------*/
//��ׯ����
//bShowDing: ��ʾ���ׯ�Ҷ������
//iPersonCount: ���ׯ�Ҷ����������
BOOL CServerGameDesk::RobNTFinish( bool bShowDing, int iPersonCount )
{
	RobNTResultStruct TRobNTresult;
	TRobNTresult.bNTDeskStation = m_byUpGradePeople;
	for( int i = 0; i < PLAY_COUNT; ++i )
	{
		TRobNTresult.bCatchNum[i] = m_iRobNTBase[i];
	}

	TRobNTresult.bShowDing = bShowDing;

	for( int i = 0; i < PLAY_COUNT; i ++ )
	{
		if ( m_pUserInfo[i] )
		{
			SendGameDataEx( i, &TRobNTresult, sizeof( TRobNTresult ), MDM_GM_GAME_NOTIFY, ASS_CALL_ROBNT_FINISH, 0 );		//����ׯ������������û�
		}
	}
	SendWatchDataEx( m_bMaxPeople, &TRobNTresult, sizeof( TRobNTresult ), MDM_GM_GAME_NOTIFY, ASS_CALL_ROBNT_FINISH, 0 );
	m_bGameStation = GS_NOTE;//��ֹ��ʾׯ�Ҷ���������������Э��ûׯ�ҡ�
	if( !bShowDing )
	{
		SetTimer( TIME_ROBNT_FINISH, 100 );
	}
	else
	{
		if ( iPersonCount < 2 )
		{
			iPersonCount = 2;
		} 
		else if( iPersonCount > PLAY_COUNT )
		{
			iPersonCount = PLAY_COUNT;
		}

		SetTimer( TIME_ROBNT_FINISH, m_iRandomNTTime*iPersonCount*m_iRandomNTRound+2000 );
	}
	return TRUE;
}

///�������м�ʱ��
void CServerGameDesk::KillAllTimer( )
{
	KillTimer( TIME_SEND_CARD_FINISH	 );
	KillTimer( TIME_AUTO_BULL		 );
	KillTimer( TIME_GAME_FINISH		 );
	KillTimer( TIME_CALL_ROBNT		 );
	KillTimer( TIME_NOTE			 );
	KillTimer( TIME_NOTE_FINISH	 );
	KillTimer( TIME_ROBNT_FINISH	 );
	KillTimer( TIME_GAME_BEGIN	   );
	KillTimer( TIME_AUTO_BEGIN		 );
	KillTimer( TIME_AUTO_NEXT );
	KillTimer( TIME_AUTO_OPERATOR );
}

int CServerGameDesk::GetMaxCardShape( BYTE iCard[], int iCardCount )
{
	if ( iCardCount < SH_USER_CARD*PLAY_COUNT )
	{
		return 0;
	}
	int shape = 0;
	for( int i = 0; i < PLAY_COUNT; i ++ )
	{
		if ( m_pUserInfo[i] && m_byUserStation[i]!=STATE_NULL )
		{
			int temp = m_Logic.GetShape( &m_iTotalCard[i*SH_USER_CARD], SH_USER_CARD, m_bNNValid );
			if ( shape < temp )
			{
				shape = temp;
			}
		}

	}
	return shape;
}

//�Ƿ���Ҫ�����˿���
bool CServerGameDesk::NeedAIControl()
{
	bool bHaveRobbot = false;
	bool bHavePeople = false;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(!IsValidPlayer(i)) continue;
		if(m_pUserInfo[i] == nullptr) continue;
		if(!m_pUserInfo[i]->m_UserData.isVirtual)
		{
			bHavePeople = true;
		}
		else
		{
			bHaveRobbot = true;
		}
	}

	return bHaveRobbot && bHavePeople && m_bAICtrl;
}

//------------------------------------------------------------------------------------
void CServerGameDesk::RecordAiHaveWinMoney( GameEndStruct *GameEnd )
{
	if ( !NeedAIControl() ) return;

	CString sTemp = CINIFile::GetAppPath ( );
	CINIFile f( sTemp +SKIN_FOLDER  + _T( "_s.ini" ) );
	TCHAR szSec[_MAX_PATH] = TEXT( "AIControl" );
	__int64 iReSetAIHaveWinMoney = f.GetKeyVal( szSec, "ReSetAIHaveWinMoney ", ( __int64 )0 );

	//�������ȣ�˵���ֶ��޸��������ļ��е�ֵ���û�������Ǯ�ˣ�����Ⱦ�ȥ�ۼӻ�����Ӯ��Ǯ;
	if ( GetAIJiangChi(true) != iReSetAIHaveWinMoney )
	{
		SetAIJiangChi( iReSetAIHaveWinMoney);
		SetAIJiangChi( iReSetAIHaveWinMoney, true);
	}
	else
	{
		for( int i=0;i<PLAY_COUNT;i++ )
		{
			if( NULL != m_pUserInfo[i] && !m_pUserInfo[i]->m_UserData.isVirtual )
			{
				SetAIJiangChi( GetAIJiangChi() + (GameEnd->iChangeMoney[i] * -1) );
			}
		}
	}

	sTemp.Format( "%I64d", GetAIJiangChi() );
	f.SetKeyValString( szSec, "AIHaveWinMoney ", sTemp );
}

//------------------------------------------------------------------------------
//�����û���֤
void	CServerGameDesk::SuperUserExamine( BYTE bDeskStation )
{
	SUPERSTATE TSuperUser;
	if ( NULL == m_pUserInfo[bDeskStation] )
	{
		return ;
	}
	if ( IsSuperUser( bDeskStation ) )
	{
		// ���û�Ϊ�����û�, ֪ͨ�ͻ���
		TSuperUser.byDeskStation = bDeskStation;
		// �����ͻ���
		TSuperUser.bEnable = true;
		SendGameDataEx( bDeskStation, &TSuperUser, sizeof( TSuperUser ), MDM_GM_GAME_NOTIFY, ASS_SUPER_USER, 0 );
	}
}
//---------------------------------------------------------------------------------------------------
//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
bool	CServerGameDesk::IsSuperUser( BYTE byDeskStation )
{
	if ( NULL == m_pUserInfo[byDeskStation] )
	{
		return false;
	}
#ifdef	SUPER_VERSION
	for( int i=0; i<(int)m_vlSuperID.size( ); i++ )
	{
		if ( NULL != m_pUserInfo[byDeskStation] )
		{
			//�Ҵ��˳������
			if ( m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vlSuperID.at( i ) )
			{
				return true;
			}
		}
	}
#endif
	return false;
}
//---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//�޸Ľ�������1( ƽ̨->��Ϸ )
/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPond( bool	bAIWinAndLostAutoCtrl )
{
	return false;
}

//�޸Ľ�������2( ƽ̨->��Ϸ )
/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1, 2, 3 4���öϵ�
/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1, 2, 3, 4ӮǮ�ĸ���
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx( __int64	iAIWantWinMoney[], int	iAIWinLuckyAt[] )
{
	return false;
}

//�޸Ľ�������3( ƽ̨->��Ϸ )
/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx( __int64	iReSetAIHaveWinMoney )
{
	return false;
}

//�ж�
BOOL CServerGameDesk::Judge( )
{
	if( JudgeWiner( ) )		//�ȴ���ʤ��
	{
		return TRUE;
	}

	if ( JudgeLoser( ) )		//��������
	{
		return TRUE;
	}		

	return FALSE;
}
//================================================
//�ж�ʤ��
//================================================
BOOL CServerGameDesk::JudgeLoser( )
{
	return FALSE;
}
//================================================
//�ж�ʤ��
//================================================
BOOL CServerGameDesk::JudgeWiner( )
{
	return false;
}
//GRM����
//���½�������( ��Ϸ->ƽ̨ )
/// @return ������ӮǮ��
void CServerGameDesk::UpDataRoomPond( __int64 iAIHaveWinMoney )
{

}

#if MODE_SET_CARD_DEBUG
//��ȡ��������
void CServerGameDesk::LoadSetCard( )
{
	CString s = CINIFile::GetAppPath ( );/////����·��
	CINIFile f( s +SKIN_FOLDER +"_test.ini" );
	CString key = TEXT( "test" );
	int iTemp = f.GetKeyVal( key, "TestSwitch", 0 );
	m_bTestSwitch = ( 0 == iTemp ? false : true );
	if( !m_bTestSwitch )
	{
		return;
	}

	//��ȡ��
	CString keyName;
	BYTE byTemp = 0;
	memset( m_byCardArr, 0x00, sizeof( m_byCardArr ) );
	for ( int i=0; i<PLAY_COUNT; i++ )
	{
		if(!IsValidPlayer(i)) continue;
		for ( int j=0; j<SH_USER_CARD; j++ )
		{
			keyName.Format( "CardArr[%d][%d]", i, j );
			byTemp = m_byCardArr[i][j] = f.GetKeyVal( key, keyName, 0x00 );
			if( 	  !( ( 0x01 <= byTemp &&  byTemp <= 0x0D )
				|| ( 0x11 <= byTemp &&  byTemp <= 0x1D )
				|| ( 0x21 <= byTemp &&  byTemp <= 0x2D )
				|| ( 0x31 <= byTemp &&  byTemp <= 0x3D ) ) )
			{
				m_bTestSwitch = false; // �趨����
				return;
			}
		}
	}
}

//���ƺ���
void CServerGameDesk::ChangeCard2Test( )
{
	if ( m_bTestSwitch )
	{
		// Change Card
		memcpy( m_iUserCard, m_byCardArr, sizeof( m_iUserCard ) );
	}
}
#endif // MODE_SET_CARD_DEBUG

//�Ƿ���ĳ���淨
bool CServerGameDesk::IsPlayMode( int iMode )
{
	if ( ( m_iPlayMode & iMode ) > 0 )
	{
		return true;
	}
	return false;
}
//�Ƿ�����Ч���
bool CServerGameDesk::IsValidPlayer( int iNum )
{
	if (iNum < 0 || iNum >= PLAY_COUNT)
	{
		return false;
	}
	if ( m_pUserInfo[iNum] != NULL && m_byUserStation[iNum] != STATE_NULL )
	{
		return true;
	}
	return false;
}

bool CServerGameDesk::SendGameDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	g_outString.Format("dznn::�����%d������Ϣ%d", bDeskStation, bAssID);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);
	return SendGameData(bDeskStation, bMainID, bAssID, bHandleCode);
}
bool CServerGameDesk::SendWatchDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	//g_outString.Format("dznn::�����%d������Ϣ%d", bDeskStation, bAssID);
	//WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);
	return SendWatchData(bDeskStation, bMainID, bAssID, bHandleCode);
}
bool CServerGameDesk::SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	g_outString.Format("dznn::�����%d������Ϣ%d", bDeskStation, bAssID);
	WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);
	return SendGameData(bDeskStation, pData, uSize, bMainID, bAssID, bHandleCode);
}
bool CServerGameDesk::SendWatchDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	//g_outString.Format("dznn::�����%d������Ϣ%d", bDeskStation, bAssID);
	//WylDebugPrintf(m_RoomId, m_bDeskIndex, g_outString);
	return SendWatchData(bDeskStation, pData, uSize, bMainID, bAssID, bHandleCode);
}

struct TReturnInfo
{
	CalculateBoardItem		tItem;									//���������
	bool					bMasterStartGame;						//�Ѿ���ʼ���
	int						iLastNote[PLAY_COUNT];					//��һ�������ע����
	__int64					iLastWinPoint[PLAY_COUNT];				//��һ����ҵ÷ַ���
	BYTE					byLastGradePeople;						//��һ��ׯ��λ��
	BYTE					byUpGradePeople;						//ׯ��λ��
	TReturnInfo()
	{
		memset(this, 0, sizeof(*this));
		byLastGradePeople = INVALID;
		byUpGradePeople = INVALID;
	}
};

void CServerGameDesk::ReleaseRoomBefore()
{
	TReturnInfo info[PLAY_COUNT];

	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (NULL == m_pUserInfo[i]) continue;
		memcpy( &info[i].tItem, &m_TCalculateBoard.tItem[i], sizeof(CalculateBoardItem) );
		info[i].bMasterStartGame = m_bMasterStartGame;
		memcpy( info[i].iLastNote, m_iLastNote, sizeof(m_iLastNote) );
		memcpy( info[i].iLastWinPoint, m_iLastWinPoint, sizeof(m_iLastWinPoint) );
		info[i].byLastGradePeople = m_byLastGradePeople;
		info[i].byUpGradePeople = m_byUpGradePeople;

		__super::ReleaseRoom( m_pUserInfo[i]->m_UserData.dwUserID, &info[i], sizeof(TReturnInfo) );
	}	
}

void CServerGameDesk::SetReturnGameInfo(int UserID,void* szUserGameInfo,int iSize,int iCount)
{
	if (nullptr == szUserGameInfo)
	{
		return;
	}
	for (int i = 0;i < m_bMaxPeople;i++)
	{
		if (UserID != m_pReturnInfo[i].iUserID) continue;
		
		TReturnInfo* info = (TReturnInfo*)szUserGameInfo;
		memcpy( &m_TCalculateBoard.tItem[i], &info->tItem, sizeof(CalculateBoardItem) );
		m_bMasterStartGame = info->bMasterStartGame;
		memcpy( m_iLastNote, info->iLastNote, sizeof(m_iLastNote) );
		memcpy( m_iLastWinPoint, info->iLastWinPoint, sizeof(m_iLastWinPoint) );
		m_byLastGradePeople = info->byLastGradePeople;
		m_byUpGradePeople =info->byUpGradePeople;
	}	
	
	m_bMasterStartGame = true;
}

void CServerGameDesk::MakeGoodCard()
{
	int temp = rand( )%100;
	//ϴ������
	if ( temp < m_iGoodCard )
	{
		for ( int i=0; i<20; i++ )
		{
			if ( GetMaxCardShape( m_iTotalCard, m_iAllCardCount ) > UG_BULL_SEVEN )
			{
				break;
			}
			m_Logic.RandCard( m_iTotalCard, m_iAllCardCount, m_bHaveKing ); 
		}
	}
}

void CServerGameDesk::SendGameBegin()
{
	for ( int i=0;i<PLAY_COUNT;i++ ) 
	{
		if ( NULL == m_pUserInfo[i] ) continue;
		SendGameDataEx( i, NULL, 0, MDM_GM_GAME_NOTIFY, MSG_GAME_BEGIN_NTF, 0 );
	}
	SendWatchDataEx( m_bMaxPeople, NULL, 0, MDM_GM_GAME_NOTIFY, MSG_GAME_BEGIN_NTF, 0 );
}

void CServerGameDesk::GameBeginInit()
{
	srand( ( UINT )time( NULL ) );

	SetValidNN( );
	SetNNPoint( );
	if( ( 0 == m_iRunGameCount && m_bIsBuy ) || (!m_bIsBuy && m_bNormalFirst) )
	{
		memset( &m_TCalculateBoard, 0, sizeof( m_TCalculateBoard ) );
		m_byLastGradePeople = 255;
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			m_iLastNote[i]			= ( numeric_limits<int>::max )( );			//�㹻�󣬱�֤��һ�ֲ��ܹ���ע
			m_iLastWinPoint[i]		= 0;										//û��Ӯ����֤��һ�ֲ��ܹ���ע}
		}
		m_tLastGameInfo.Init();
	}

	memset( m_bUserReady, 0, sizeof( m_bUserReady ) );
	memset( m_iUserCardCount, 0, sizeof( m_iUserCardCount ) );		//�û���������
	for ( int i = 0; i < PLAY_COUNT; i++ ) 
	{
		m_byUserStation[i] = STATE_NULL;
		if ( m_pUserInfo[i] != NULL)
		{
			m_byUserStation[i] = STATE_PLAY_GAME;
			m_vPlayingPlayer.push_back( i );
		}
	}
}

void CServerGameDesk::PrepareCard()
{
	//�ַ��˿���
	for( int j = 0 ; j<SH_USER_CARD ;j++ )
	{
		for( int i = 0 ;i<PLAY_COUNT;i++ )
		{
			if( NULL == m_pUserInfo[i] || m_byUserStation[i] == STATE_NULL )
			{
				m_iUserCardCount[i] = 0;
				memset( &m_iUserCard[i], 0, sizeof( m_iUserCard[i] ) );
			}
			else
			{
				m_iUserCardCount[i] = SH_USER_CARD;
				m_iUserCard[i][j] =  m_iTotalCard[i*SH_USER_CARD+j];	
			}			
		}
	}

#if MODE_SET_CARD_DEBUG
	LoadSetCard( );
	ChangeCard2Test( );
#endif // MODE_SET_CARD_DEBUG
}
