#include "StdAfx.h"
#include "GameRoomEx.h"
#include "auto_aidlg.h"
#include "gameroommessage.h"
#include "Platglobaldata.h"
#include "Platconfig.h"
#include "PlatLog.h"
#include "Platlock.h"

BEGIN_MESSAGE_MAP(CGameRoomEx, CBaseRoom)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_SET_GAME_TIME,SetGameTime)
	ON_MESSAGE(IDM_KILL_GAME_TIME,KillGameTime)
	ON_MESSAGE(IDM_CLOSE,OnCloseGame)//20090211
    ON_MESSAGE(IDM_STAND_UP,OnStandingDesk)
END_MESSAGE_MAP()

//���캯��
CGameRoomEx::CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo, DynamicConfig cof) 
: CBaseRoom(IDD_GAME_ROOM)
{
    m_CurDynamicConfig = cof;
	m_IGameFrame = NULL; 	//��Ϸ�ӿ�
    m_bInitContest = false;
	memset(&m_MServerLogonInfo,0,sizeof(m_MServerLogonInfo));

	m_pRoomInfo = pRoomInfo;
	////��ʼ����������
	memset(&m_GameInfo,0,sizeof(m_GameInfo));
	lstrcpy(m_GameInfo.szGameName,szGameName);
	m_GameInfo.pISocketSend=this;
	m_GameInfo.pIFindUser=&m_UserManage;
	m_GameInfo.bShowUserInfo=true;
	m_GameInfo.bEnableSound=true;
	m_GameInfo.bEnableWatch=false;
	m_GameInfo.uNameID=pRoomInfo->stComRoomInfo.uNameID;
	m_GameInfo.uRoomID=pRoomInfo->stComRoomInfo.uRoomID;
	//m_GameInfo.dwGameLSVer=pRoomInfo->dwGameLSVer;
	m_GameInfo.uComType=pRoomInfo->stComRoomInfo.uComType;
	m_GameInfo.uDeskPeople=pRoomInfo->stComRoomInfo.uDeskPeople;
	m_GameInfo.dwRoomRule = pRoomInfo->stComRoomInfo.dwRoomRule ; 

	CopyMemory(&m_MServerLogonInfo,(MSG_GP_R_LogonResult*)pLogonInfo,sizeof(m_MServerLogonInfo));

	

	return;
}

//��������
CGameRoomEx::~CGameRoomEx()
{
	if (m_IGameFrame != NULL)
	{
		delete m_IGameFrame;
		m_IGameFrame = NULL;
	}
}

//���ݰ󶨺���
void CGameRoomEx::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

}

//��ʼ������
BOOL CGameRoomEx::OnInitDialog()
{
	__super::OnInitDialog();
	ConnectToGServer();
	return TRUE;
}
//��½��Ϸ����
void  CGameRoomEx::ConnectToGServer()
{
    KillTimer(TIMER_CONNECT_TO_GSERVER);
    SetTimer(TIMER_CONNECT_TO_GSERVER, 1000, NULL);
	return ;
}
//����������Ϣ
bool CGameRoomEx::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	if (uErrorCode!=0)	//���ӳ��ִ���
	{
        GSERVER_LOG_ERROR("����GServerʧ�� %s:%d - %d", m_pRoomInfo->stComRoomInfo.szServiceIP, m_pRoomInfo->stComRoomInfo.uServicePort, uErrorCode);
		::PostMessage(GetParent()->m_hWnd,IDM_CLOSE_GAME_ROOM,(WPARAM)m_pRoomInfo, m_MServerLogonInfo.dwUserID);
		return true;
	}
	return true;
}

//�����ȡ��Ϣ
bool CGameRoomEx::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:			//������Ϣ,׼�����뷿�� 
		{
			return OnConnectMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_LOGON:			//�����½
		{
			return OnLogonMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_USER_LIST:		//�û��б�
		{
			return OnUserListMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_USER_ACTION:	//�û�����
		{
			return OnUserActionMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_ROOM:			//������Ϣ
		{
			return OnRoomMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
    case MDM_GM_GAME_NOTIFY:
        break;
    case MDM_GM_GAME_FRAME:
        break;
    default:
        {
            CString strKey;
            strKey.Format("%d_%d", pNetHead->bMainID, pNetHead->bAssistantID);
            SYSTEMTIME sysTime;
            GetLocalTime(&sysTime);
            if(m_notHandleMsgs.find(strKey) != m_notHandleMsgs.end())
            {
                if(m_notHandleMsgs[strKey] == sysTime.wHour)
                {
                    break;;
                }
            }
            GSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_MServerLogonInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
        break;
	}

	////��Ϸ��Ϣ
	if (m_IGameFrame!=NULL)
		if(!m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket))
        {
            {

                CString strKey;
                strKey.Format("%d_%d", pNetHead->bMainID, pNetHead->bAssistantID);
                SYSTEMTIME sysTime;
                GetLocalTime(&sysTime);
                if(m_notHandleMsgs.find(strKey) != m_notHandleMsgs.end())
                {
                    if(m_notHandleMsgs[strKey] == sysTime.wHour)
                    {
                        return false;
                    }
                }
                GSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_MServerLogonInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
                m_notHandleMsgs[strKey] = sysTime.wHour;
            }
            return false;
        }

	return true;
}

//����ر���Ϣ
bool CGameRoomEx::OnSocketCloseEvent()
{
	try
	{
		if(m_IGameFrame)
		{
			m_IGameFrame->AFCCloseFrame();
			//����
		}
	}catch(...)
	{
		if (m_IGameFrame != NULL)
		{
			delete m_IGameFrame;
		}
		m_IGameFrame = NULL;
	}
    AutoPlatLock lock(&UIDATA->uiSection);
    auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
    if(it != UIDATA->usrInfos.end())
    {
        if(it->second.bUserState == USER_PLAY_GAME && !PLATCONFIG->bFishGame)
        {
            it->second.bUserState = USER_CUT_GAME;
        }
        else
        {
            if(it->second.bUserState != USER_CUT_GAME)
            {
                it->second.GLoginStatus = 0;
                it->second.bUserState = USER_NO_STATE;
                it->second.bDeskNo = 255;
                it->second.bDeskStation = 255;
            }
        }
    }
    else
    {
        MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
    }
	::PostMessage(GetParent()->m_hWnd,IDM_CLOSE_GAME_ROOM,(WPARAM)m_pRoomInfo, m_MServerLogonInfo.dwUserID);
	return true;
}
//�رշ���
void CGameRoomEx:: CloseGameRoom()
{
	m_TCPSocket->CloseSocket(false);
	try
	{
		if(m_IGameFrame)
		{
			m_IGameFrame->AFCCloseFrame();
			//����
		}
	}catch(...)
	{
		if (m_IGameFrame != NULL)
		{
			delete m_IGameFrame;
		}
		m_IGameFrame = NULL;
	}
    AutoPlatLock lock(&UIDATA->uiSection);
    auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
    if(it != UIDATA->usrInfos.end())
    {
        if(it->second.bUserState == USER_PLAY_GAME && !PLATCONFIG->bFishGame)
        {
            it->second.bUserState = USER_CUT_GAME;
        }
        else
        {
            if(it->second.bUserState != USER_CUT_GAME)
            {
                it->second.GLoginStatus = 0;
                it->second.bUserState = USER_NO_STATE;
                it->second.bDeskNo = 255;
                it->second.bDeskStation = 255;
            }
        }
    }
    else
    {
        MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
    }
	::PostMessage(GetParent()->m_hWnd,IDM_CLOSE_GAME_ROOM,(WPARAM)m_pRoomInfo, m_MServerLogonInfo.dwUserID);
	return ;
}
//������Ϣ����
bool CGameRoomEx::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID==ASS_NET_TEST)	//���������Ϣ
	{
		pClientSocket->SendData(MDM_CONNECT,ASS_NET_TEST,0);
		return true;
	}
	else if ((pNetHead->bAssistantID==ASS_CONNECT_SUCCESS)||(pNetHead->bAssistantID==3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		//���ɹ�
		LoginGServer();
		return true;
	}
	return true;
}

//001,�����½����,һ���뷿��  
bool CGameRoomEx::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_LOGON_SUCCESS:		//��½�ɹ�
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_LogonResult)) 
            {
                GSERVER_LOG_ERROR("MSG_GR_R_LogonResult �ṹ�岻һ��");
                return false;
            }

			MSG_GR_R_LogonResult * pLogonInfo=(MSG_GR_R_LogonResult *)pNetData;

			/*m_GameInfo.uLessPoint=pLogonInfo->uLessPoint;*/
			m_GameInfo.pMeUserInfo = m_UserManage.AddGameUser(&pLogonInfo->pUserInfoStruct,1,0);

            GSERVER_LOG_INFO("%s ��½GServer�ɹ�", m_MServerLogonInfo.szName);
						
			if(m_GameInfo.pMeUserInfo == NULL)
			{
				GSERVER_LOG_ERROR("AddGameUser ʧ�� %s", m_MServerLogonInfo.szName);
				
				CloseGameRoom();
				return false;
			}

            AutoPlatLock lock(&UIDATA->uiSection);
            auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
            if(it != UIDATA->usrInfos.end())
            {
                it->second.GLoginStatus = 2;
                it->second.bUserState = pLogonInfo->pUserInfoStruct.bUserState;
            }
            else
            {
                MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
            }
			return true;
		}
	case ASS_GR_LOGON_ERROR:		//��½ʧ��
		{
            GSERVER_LOG_ERROR("%s ��½ʧ��", m_MServerLogonInfo.szName);
            // ��½ʧ�ܹرշ���
            CloseGameRoom();

            AutoPlatLock lock(&UIDATA->uiSection);
            auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
            if(it != UIDATA->usrInfos.end())
            {
                it->second.GLoginStatus = 3;
            }
            else
            {
                MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
            }
			break;
		}
	case ASS_GR_SEND_FINISH:		//��½���
		{	
            GSERVER_LOG_INFO("%s ��½GServer���", m_MServerLogonInfo.szName);
            if(m_GameInfo.pMeUserInfo == NULL)
            {
                GSERVER_LOG_ERROR("%s �û���ϢΪ��", m_MServerLogonInfo.szName);
            }
            else if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == 255)
            {// �Ƕ�������
                // ����
                SitDesk();
            }
			break;
		}
    default:
        {

            CString strKey;
            strKey.Format("%d_%d", pNetHead->bMainID, pNetHead->bAssistantID);
            SYSTEMTIME sysTime;
            GetLocalTime(&sysTime);
            if(m_notHandleMsgs.find(strKey) != m_notHandleMsgs.end())
            {
                if(m_notHandleMsgs[strKey] == sysTime.wHour)
                {
                    return true;
                }
            }
            GSERVER_LOG_INFO("%s δ������Ϣ %d-%d", m_MServerLogonInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
        break;
	}
	return true;
}

//��������վ����
LRESULT CGameRoomEx::OnStandingDesk(WPARAM,LPARAM)
{
    StandingDesk();
    return TRUE;
}

//����
bool CGameRoomEx::StandingDesk()
{
    //���淢��վ�����Ϣ
    if (m_IGameFrame!=NULL)
    {
        CClientGameDlg *dlg = NULL;
        dlg = (CClientGameDlg*)m_IGameFrame;
        dlg->KillAllTimer();
    }
	m_TCPSocket->SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
	return true;
}

//����
bool CGameRoomEx::SitDesk()
{
	if((m_GameInfo.dwRoomRule &GRR_QUEUE_GAME) != 0)
	{// �Ŷӻ�
		m_TCPSocket->SendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE,0);
		return  true ; 
	}
    if(IsContestGame(m_GameInfo.dwRoomRule))
    {// ��������
        m_TCPSocket->SendData(MDM_GR_USER_ACTION, ASS_GR_CONTEST_APPLY, 0);
        return true;
    }

	BYTE bDeskIndex = 255;
	BYTE bDeskStation = 255;

	OnFindBestDesk(bDeskIndex,bDeskStation);

	if(bDeskIndex == 255||bDeskStation == 255)
	{
        GSERVER_LOG_ERROR("û���ҵ����ʵ�����%d-%d", bDeskIndex, bDeskStation);
        CloseGameRoom();
		return true;
	}

    if(PLATCONFIG->bFishGame)
    {
        KillTimer(TIMER_FISHGAME_AIBANK);
        SetTimer(TIMER_FISHGAME_AIBANK, 5000, NULL);
        KillTimer(TIMER_FISHGAME_LEAVE);
        int iRand = (m_CurDynamicConfig.nFishGameTimeLeave / 2) - m_CurDynamicConfig.nFishGameTimeLeave / 4;
        if(iRand == 0 || iRand == 1) iRand = 2;
        SetTimer(TIMER_FISHGAME_LEAVE, 
            (m_CurDynamicConfig.nFishGameTimeLeave + (rand() % iRand))* 60 * 1000, NULL);
    }
    else
    {
        SetTimer(TIMER_CHECK_USERSTATE, (m_CurDynamicConfig.nKeepInDeskSeconds + (rand() % (m_CurDynamicConfig.nKeepInDeskSeconds / 2) - m_CurDynamicConfig.nKeepInDeskSeconds / 4)) * 1000, NULL);
    }
	MSG_GR_S_UserSit UserSit;
	UserSit.bDeskIndex = bDeskIndex;
	UserSit.bDeskStation = bDeskStation;
	//���淢�����µ���Ϣ
	m_TCPSocket->SendData(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_USER_SIT,0);

    GSERVER_LOG_INFO("%s �������� %d-%d", m_MServerLogonInfo.szName, bDeskIndex, bDeskStation);
	return true;
}

// ���ǲ���
bool CGameRoomEx::OnFindBestDesk(BYTE &bDeskIndex, BYTE &bDeskStation)
{
    int nTruePlayerNumber = -1;
    int nAIPlayerNumber = -1;
    //��ÿһ�����ӿ�ʼ����
    for(UINT desk = 0; desk < m_pRoomInfo->stComRoomInfo.uDeskCount; desk ++)
    {
        if(CountMachine(desk) >= m_CurDynamicConfig.bMachineDeskCount)
        {// �����ϻ������Ѵ�����
            continue;
        }
        int nRealNumber = CountTruePlayer(desk);
        int nAINumber = CountMachine(desk);
        if(!m_CurDynamicConfig.bMachineAndPlayer && nRealNumber > 0)
        {// �����˲����������ͬ��
            continue;
        }
        //��λ
        for(UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
        {// �ж���λ���Ƿ�����,�����Ƿ��������
            int i = 0;
            int nUsrNumber = m_UserManage.GetOnLineUserCount();
            for(; i < nUsrNumber; i++)
            {
                UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
                if(pFindUserItem == NULL)
                    continue;
                if(pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.dwUserID < 0)
                {// ���ӱ���
                    break;
                }
                if(pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.bDeskStation == station)
                {
                    break;
                }
            }
            if(i < nUsrNumber)
            {// ��λ������
                continue;
            }
            else
            {
                if(m_CurDynamicConfig.bMachineAndPlayer)
                {// ����������ͬ��
                    if(nRealNumber > nTruePlayerNumber)
                    {// ����������������
                        nTruePlayerNumber = nRealNumber;
                        nAIPlayerNumber = nAINumber;
                        bDeskIndex = desk;
                        bDeskStation = station;
                    }
                    else if (nRealNumber == nTruePlayerNumber)
                    {// ��������ͬ
                        if(nAINumber > nAIPlayerNumber)
                        {
                            nTruePlayerNumber = nRealNumber;
                            nAIPlayerNumber = nAINumber;
                            bDeskIndex = desk;
                            bDeskStation = station;
                        }
                    }
                }
                else
                {
                    if(nAINumber > nAIPlayerNumber)
                    {// ѡ���������������
                        nTruePlayerNumber = nRealNumber;
                        nAIPlayerNumber = nAINumber;
                        bDeskIndex = desk;
                        bDeskStation = station;
                    }
                }
            }
        }
    }
    return true;
}

//ͳ��ĳ������������
BYTE CGameRoomEx::CountMachine(BYTE bDeskIndex)
{
	int iCount = 0;
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
			continue;
		if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && pFindUserItem->GameUserInfo.isVirtual)
		{
			iCount ++ ;
		}
	}
	return iCount;
}

//ͳ��ĳ����������
BYTE CGameRoomEx::CountTruePlayer(BYTE bDeskIndex)
{
	int iCount = 0;
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
			continue;
		if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && !pFindUserItem->GameUserInfo.isVirtual)
		{
			iCount++ ;
		}
	}
	return iCount;
}

//���͵�½��Ϣ
bool CGameRoomEx::LoginGServer()
{
    GSERVER_LOG_INFO("%s ��½GServer", m_MServerLogonInfo.szName);
	//�����û���¼��Ϣ
	MSG_GR_S_RoomLogon RoomLogon;
	//RoomLogon.uGameVer = 0;//m_GameInfo.dwGameLSVer;
	//RoomLogon.uRoomVer = GAME_PLACE_MAX_VER;
	RoomLogon.uNameID = m_pRoomInfo->stComRoomInfo.uNameID;
	RoomLogon.dwUserID = m_MServerLogonInfo.dwUserID;
	lstrcpy(RoomLogon.szMD5Pass,m_MServerLogonInfo.szMD5Pass);
    RoomLogon.bLogonbyphone = false;
	//���͵�½
	m_TCPSocket->SendData(&RoomLogon,sizeof(RoomLogon),MDM_GR_LOGON,ASS_GR_LOGON_BY_ID,0);
    AutoPlatLock lock(&UIDATA->uiSection);
    auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
    if(it != UIDATA->usrInfos.end())
    {
        it->second.GLoginStatus = 1;
    }
    else
    {
        MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
    }


	return true;
}

//�û��б���
bool CGameRoomEx::OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_ONLINE_USER:
	case ASS_GR_NETCUT_USER:	//�û��б�
		{
			UINT uUserCount = uDataSize/sizeof(UserInfoStruct);

            if(pNetHead->bAssistantID == ASS_GR_ONLINE_USER)
            {
                GSERVER_LOG_INFO("%s �յ������û��б�%d��", m_MServerLogonInfo.szName, uUserCount);
            }
            else
            {
                GSERVER_LOG_INFO("%s �յ������û��б�%d��", m_MServerLogonInfo.szName, uUserCount);
            }
            
			if (uUserCount>0L)
			{
				//�������
				UserItemStruct * pUserItem = NULL;
				UserInfoStruct * pUserInfo = (UserInfoStruct *)pNetData;

				for (UINT i = 0; i < uUserCount; i ++)
				{
					pUserItem = m_UserManage.AddGameUser(pUserInfo+i,1,0);
				}
			}

			return true;
		}
	case ASS_GR_DESK_STATION:	//����״̬ 
		{
			if (uDataSize!=sizeof(MSG_GR_DeskStation)) 
            {
                GSERVER_LOG_ERROR("%s MSG_GR_DeskStation�ṹ�岻һ��", m_MServerLogonInfo.szName);
                return false;
            }
			MSG_GR_DeskStation * pDeskStation=(MSG_GR_DeskStation *)pNetData;
			for (UINT i=0;i<pNetHead->bHandleCode;i++)
			{
				if ((pDeskStation->bVirtualDesk[i/8]&(1<<(i%8)))!=0)    //change by yjj 090325
				{
					UserInfoStruct pUserInfo;      //�����Ϣ
					int c=0;

					for (int k=0; k<m_GameInfo.uDeskPeople;k++)
					{
						
						pUserInfo.bUserState=USER_PLAY_GAME;
						pUserInfo.bDeskNO=i;
						pUserInfo.bDeskStation=k;
						pUserInfo.dwUserID = -1;//�����û�

						m_UserManage.AddGameUser(&pUserInfo,1,0);
					}
				}
			}
			return true;
		}
    default:
        {
            CString strKey;
            strKey.Format("%d_%d", pNetHead->bMainID, pNetHead->bAssistantID);
            SYSTEMTIME sysTime;
            GetLocalTime(&sysTime);
            if(m_notHandleMsgs.find(strKey) != m_notHandleMsgs.end())
            {
                if(m_notHandleMsgs[strKey] == sysTime.wHour)
                {
                    return true;
                }
            }
            GSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_MServerLogonInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
        break;
	}
	return true;
}

//������Ϣ����
bool CGameRoomEx::OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GR_ROOM);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_INIT_CONTEST:	
        {
            m_bInitContest = true;
            return true;
        }
	case ASS_GR_CAHNGE_ROOM:
		{// ���������͵��˳���������
            if(!m_bInitContest)
			CloseGameRoom();//�رշ���				
			return true;
		}
	case ASS_GR_CONTEST_GAMEOVER:
	case ASS_GR_CONTEST_KICK:
		{// ����̭
            m_bInitContest = false;
			CloseGameRoom();//�رշ���
			return true;
		}
    case ASS_GR_USER_AGREE:
        {   
            if(sizeof(MSG_GR_R_UserAgree) != uDataSize)
            {
                GSERVER_LOG_ERROR("MSG_GR_R_UserAgree �ṹ�岻һ��");
                return false;
            }
            MSG_GR_R_UserAgree *pUsrAgree = (MSG_GR_R_UserAgree*)pNetData;
            AutoPlatLock lock(&UIDATA->uiSection);
            auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
            if(it != UIDATA->usrInfos.end())
            {
                if(pUsrAgree->bDeskNO == it->second.bDeskNo && pUsrAgree->bDeskStation == it->second.bDeskStation) it->second.bUserState = USER_ARGEE;
            }
            else
            {
                MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
            }
        }
        return true;
	case ASS_GR_USER_CONTEST:
		{
			//��������
			if (uDataSize != sizeof(MSG_GR_ContestChange)) 
            {
                GSERVER_LOG_ERROR("MSG_GR_ContestChange �ṹ�岻һ��");
                return false;
            }
			MSG_GR_ContestChange* _p = (MSG_GR_ContestChange*)pNetData;

			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(_p->dwUserID);
			if (pUserItem != NULL)
			{
				//����̭��Ҵ�ʱ������������������ֵҪ�󣬱����û�����̭��ʱ����Ϊ7������������Ϊ6���˴��޸�һ����ʾ
				_p->iRemainPeople = _p->iRemainPeople>_p->iRankNum ? _p->iRemainPeople : _p->iRankNum;
				pUserItem->GameUserInfo.iContestCount = _p->iContestCount;
				pUserItem->GameUserInfo.i64ContestScore = _p->i64ContestScore;
				pUserItem->GameUserInfo.iRankNum = _p->iRankNum;
			}
			return true;
		}
	case ASS_GR_GAME_BEGIN:		//��Ϸ��ʼ
		{
			KillTimer(TIMER_STAND_UP);
			//Ч������
			if (uDataSize!=0) 
            {
                GSERVER_LOG_ERROR("ASS_GR_GAME_BEGIN �ṹ�岻һ��");
                return false;
            }

			//��������
			BYTE bDeskIndex = pNetHead->bHandleCode;
			for(UINT i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
					continue;
				if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
				{
					pFindUserItem->GameUserInfo.bUserState = USER_PLAY_GAME;					
				}
			}

            {
                AutoPlatLock lock(&UIDATA->uiSection);
                auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                if(it != UIDATA->usrInfos.end())
                {
                    if(pNetHead->bHandleCode == it->second.bDeskNo) it->second.bUserState = USER_PLAY_GAME;
                }
                else
                {
                    MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                }
            }
			return true;
		}
	case ASS_GR_GAME_END:		//��Ϸ����
		{
            if (m_GameInfo.pMeUserInfo == NULL) return true;
			//Ч������
			if (uDataSize!=0) 
			{
                GSERVER_LOG_ERROR("ASS_GR_GAME_END �ṹ�岻һ��");
				return false;
			}
			if (IsContestGame(m_GameInfo.dwRoomRule))
			{
				//������ʱ������һ�̺󷿼䲻���ȡ�κδ�ʩ��ֱ�ӵȴ����������
                {
                    AutoPlatLock lock(&UIDATA->uiSection);
                    auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                    if(it != UIDATA->usrInfos.end())
                    {
                        if(pNetHead->bHandleCode == it->second.bDeskNo)
                        {
                            it->second.bDeskNo = 255;
                            it->second.bDeskStation = 255;
                            it->second.bUserState = USER_NO_STATE;
                        }
                    }
                    else
                    {
                        MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                    }
                }
				return true;
			}
            {
                AutoPlatLock lock(&UIDATA->uiSection);
                auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                if(it != UIDATA->usrInfos.end())
                {
                    if(pNetHead->bHandleCode == it->second.bDeskNo) it->second.bUserState = USER_SITTING;
                }
                else
                {
                    MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                }
            }

            if(CheckNeedMoneyOp() || m_CurDynamicConfig != GLOBALDATA->CurDynamicConfig)
            {// ������ﵽ��ȡҪ��,�������ñ仯
                CloseGameRoom();
                return true;
            }

		    bool bHaveplayer=false; //�Ƿ�����ʵ���
			//��������
			BYTE bDeskIndex = pNetHead->bHandleCode;
			for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
				{
					continue;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
				{
					pFindUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;
					if (pFindUserItem->GameUserInfo.isVirtual == false)
					{
						bHaveplayer = true;
					}
				}
			}
            if (bHaveplayer && m_CurDynamicConfig.bMachineAndPlayer == false)//��������ͬ��
            {
                // վ��
              //  StandingDesk();
				SetTimer(TIMER_STAND_UP,(rand()%5 + 3)*1000,NULL);
            }	
            else
            {
			    //������Ϸ�����л����뿪
			    if(m_GameInfo.pMeUserInfo!=NULL&&bDeskIndex == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
			    {
			        if(rand() % 100 < m_CurDynamicConfig.nGameEndLeaveDesk)
                    {
                        // վ��
                    //    StandingDesk();
						SetTimer(TIMER_STAND_UP,(rand()%5 + 3)*1000,NULL);
						
                    }
                    else
                    {
                        SetTimer(TIMER_CHECK_USERSTATE, (m_CurDynamicConfig.nKeepInDeskSeconds + (rand() % (m_CurDynamicConfig.nKeepInDeskSeconds / 2) - m_CurDynamicConfig.nKeepInDeskSeconds / 4)) * 1000, NULL);
                    }
			    }
            }			
			return true;
		}
	case ASS_GR_USER_POINT:		//�û���һ���
		{
			if(pNetHead->bHandleCode==10)//ͬ�����
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_RefalshMoney)) 
				{
                    GSERVER_LOG_ERROR("MSG_GR_S_RefalshMoney �ṹ�岻һ��");
					return false;
				}
				MSG_GR_S_RefalshMoney * pReflashMoney=(MSG_GR_S_RefalshMoney*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserItem==NULL) 
				{
                    GSERVER_LOG_ERROR("%s δ�ҵ������û� %d", m_MServerLogonInfo.szName, pReflashMoney->dwUserID);
					return false;
				}

				pUserItem->GameUserInfo.i64Money=pReflashMoney->i64Money;

				if(m_GameInfo.pMeUserInfo!=NULL && pReflashMoney->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);

				}

				return true;
			}else if(pNetHead->bHandleCode==11)//ͬ������ֵ
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_RefalshMoney)) 
				{
                    GSERVER_LOG_ERROR("MSG_GR_S_RefalshMoney �ṹ�岻һ��");
					return false;
				}
				MSG_GR_S_RefalshMoney * pReflashMoney=(MSG_GR_S_RefalshMoney*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserItem==NULL) 
				{
                    GSERVER_LOG_ERROR("%s δ�ҵ������û� %d", m_MServerLogonInfo.szName, pReflashMoney->dwUserID);
					return false;
				}
				//pUserItem->GameUserInfo.dwPoint=pReflashMoney->i64Money;

				if(m_GameInfo.pMeUserInfo!=NULL && pReflashMoney->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);

				}
				return true;
			}

			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserPoint)) return false;
			MSG_GR_R_UserPoint * pUserPoint=(MSG_GR_R_UserPoint *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserPoint->dwUserID);
			if (pUserItem!=NULL)
			{
				//��������

				//pUserItem->GameUserInfo.dwPoint+=pUserPoint->dwPoint;
				pUserItem->GameUserInfo.i64Money+=pUserPoint->dwMoney;
				pUserItem->GameUserInfo.uWinCount+=pUserPoint->bWinCount;
				pUserItem->GameUserInfo.uLostCount+=pUserPoint->bLostCount;
				pUserItem->GameUserInfo.uMidCount+=pUserPoint->bMidCount;
				pUserItem->GameUserInfo.uCutCount+=pUserPoint->bCutCount;

				if(m_GameInfo.pMeUserInfo!=NULL && pUserPoint->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);
				}
			}
            else
            {
                GSERVER_LOG_ERROR("%s δ�ҵ������û� %d", m_MServerLogonInfo.szName, pUserPoint->dwUserID);
            }
			return true;
		}
	default:
        {
            CString strKey;
            strKey.Format("%d_%d", pNetHead->bMainID, pNetHead->bAssistantID);
            SYSTEMTIME sysTime;
            GetLocalTime(&sysTime);
            if(m_notHandleMsgs.find(strKey) != m_notHandleMsgs.end())
            {
                if(m_notHandleMsgs[strKey] == sysTime.wHour)
                {
                    return true;
                }
            }
            GSERVER_LOG_INFO("%s δ������Ϣ%d-%d", m_MServerLogonInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
		return true;
	}
	return true;
}

//�û���������
bool CGameRoomEx::OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(NULL == m_GameInfo.pMeUserInfo )
	{
	    return true;
	}
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_CONTEST_APPLY:
		{
			if (uDataSize != sizeof(MSG_GR_O_ContestApply))	
            {
                GSERVER_LOG_ERROR("%s MSG_GR_ContestApply �ṹ�岻һ��", m_MServerLogonInfo.szName);
                return false;
            }

			MSG_GR_O_ContestApply *pMsgContestApply = (MSG_GR_O_ContestApply*)pNetData;


			//���ݷ���˷����ı���״��������
			switch (pMsgContestApply->bResult)
			{
			case 1://�����ɹ�
				{
                    GSERVER_LOG_INFO("%s ���������ɹ� %d", m_MServerLogonInfo.szName, pMsgContestApply->bResult);
					break;
				}
			default:
				{
                    GSERVER_LOG_ERROR("%s ������������ %d", m_MServerLogonInfo.szName, pMsgContestApply->bResult);
					CloseGameRoom();
					break;
				}
			}
			return true;
		}	
	case ASS_GR_USER_HIT_BEGIN:
		{
			//Ч������
			if(uDataSize!=sizeof(MSG_GM_WatchSet))
            {
                GSERVER_LOG_ERROR("%s MSG_GM_WatchSet �ṹ�岻һ��", m_MServerLogonInfo.szName);
                return false;
            }
			MSG_GM_WatchSet* pUserID = (MSG_GM_WatchSet*) pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserID->dwUserID);
            if(pUserItem != NULL)
			    pUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;
            else
                GSERVER_LOG_ERROR("%s δ�ҵ������û� %d", m_MServerLogonInfo.szName, pUserID->dwUserID);
			return true;
		}
	case ASS_GR_USER_UP:		//�û�����
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserSit))
            {
                GSERVER_LOG_ERROR("%s MSG_GR_R_UserSit �ṹ�岻һ��", m_MServerLogonInfo.szName);
                return false;
            }
			MSG_GR_R_UserSit * pUserUp=(MSG_GR_R_UserSit *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserUp->dwUserID);

			if (pUserItem!=NULL)
			{
				//����
				BYTE bTemp = pUserItem->GameUserInfo.bDeskNO;
				//��������
				pUserItem->GameUserInfo.bDeskNO = 255;
				pUserItem->GameUserInfo.bDeskStation = 255;
				pUserItem->GameUserInfo.bUserState = pUserUp->bUserState;

				if(pUserUp->dwUserID == m_MServerLogonInfo.dwUserID)//�Լ�����
				{
                    {
                        AutoPlatLock lock(&UIDATA->uiSection);
                        auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                        if(it != UIDATA->usrInfos.end())
                        {
                            it->second.bDeskNo = 255;
                            it->second.bDeskStation = 255;
                            it->second.bUserState = pUserUp->bUserState;
                        }
                        else
                        {
                            MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                        }
                    }
                    GSERVER_LOG_INFO("%s վ��", m_MServerLogonInfo.szName);
                    if(!IsContestGame(m_GameInfo.dwRoomRule)) CloseGameRoom();
					return true;
				}
			}
			return true;
		}
	case ASS_GR_USER_SIT:		//�û�����
		{
	//Ч������

			if (uDataSize!=sizeof(MSG_GR_R_UserSit)) 
			{
                GSERVER_LOG_ERROR("%s MSG_GR_R_UserSit �ṹ�岻һ��", m_MServerLogonInfo.szName);
				return false;
			}
			MSG_GR_R_UserSit * pUserSit=(MSG_GR_R_UserSit *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserSit->dwUserID);
			if (pUserItem!=NULL)
			{
				/////////////////////////////////////////
				//��������
				pUserItem->GameUserInfo.bDeskNO=pUserSit->bDeskIndex;

				pUserItem->GameUserInfo.bDeskStation=pUserSit->bDeskStation;

				pUserItem->GameUserInfo.bUserState=pUserSit->bUserState;
				if(pUserItem->GameUserInfo.dwUserID == m_MServerLogonInfo.dwUserID )//��������
				{
                    {
                        AutoPlatLock lock(&UIDATA->uiSection);
                        auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                        if(it != UIDATA->usrInfos.end())
                        {
                            it->second.bDeskNo = pUserSit->bDeskIndex;
                            it->second.bDeskStation = pUserSit->bDeskStation;
                            it->second.bUserState = pUserSit->bUserState;
                            if(PLATCONFIG->bFishGame)
                            {// ��������Ϸ���¼���Ϊ������Ϸ��
                                it->second.bUserState = USER_PLAY_GAME;
                            }
                        }
                        else
                        {
                            MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                        }
                    }
                    GSERVER_LOG_INFO("%s ���³ɹ� %d:%d", m_MServerLogonInfo.szName, pUserSit->bDeskIndex, pUserSit->bDeskStation);
					StartGameClient();
					//���ƿͻ���
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_SIT;
					ControlMessage.bDeskStation = pUserItem->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem = m_GameInfo.pMeUserInfo;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

				}
				else
				{
					//���ƿͻ���
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_SIT;
					ControlMessage.bDeskStation = pUserItem->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem = pUserItem;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

					if (m_CurDynamicConfig.bMachineAndPlayer == false)//���������ͬ��
					{
						if(pUserItem->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
						{//���������,����������
							if(!pUserItem->GameUserInfo.isVirtual)
                            {
                                // վ��
								StandingDesk();
                            }
						}
					}
                    else
                    {// ������ͬ��
                        if(PLATCONFIG->bFishGame && !pUserItem->GameUserInfo.isVirtual)
                        {// ��������Ϸ,��������
                            if(pUserItem->GameUserInfo.bDeskNO != m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
                            {// �����������˲���ͬһ��
                                if(CountTruePlayer(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO) == 0)
                                {// ������û������
                                    // վ��
                                    //SetTimer(TIMER_FISHGAME_WITHTRUEPAYER, (m_CurDynamicConfig.nKeepInDeskSeconds + \
									//	(rand() % (m_CurDynamicConfig.nKeepInDeskSeconds / 2) - m_CurDynamicConfig.nKeepInDeskSeconds / 4)) * 1000, NULL);
                                }
                            }
                        }
                    }
			
				}
			}
			return true;
		}
	case ASS_GR_QUEUE_USER_SIT:
		{
			/*if(pNetHead->uMessageSize - sizeof(NetMessageHead) != sizeof(MSG_GR_S_UserQueueInfo)*m_GameInfo.uDeskPeople)
            {
                GSERVER_LOG_ERROR("ASS_GR_QUEUE_USER_SIT�ṹ�岻һ��");
                return false;
            }*/
			MSG_GR_S_UserQueueInfo *pQueueInfo = (MSG_GR_S_UserQueueInfo*)pNetData;

			bool bIsSelf = false;
			for (int i = 0; i < m_GameInfo.uDeskPeople; i++)
			{
				UserItemStruct * pUserItem = m_UserManage.FindOnLineUser((long)pQueueInfo->iUserID);
				if (pUserItem != NULL)
				{
					// PengJiLin, 2010-5-19���Ŷӻ�������������ʾ�����Ϣ
					if(pUserItem->GameUserInfo.dwUserID !=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
						strcpy(pUserItem->GameUserInfo.nickName, "���");

					pUserItem->GameUserInfo.bDeskNO= pNetHead->bHandleCode;
					pUserItem->GameUserInfo.bDeskStation = pQueueInfo->bDeskStation;
					pUserItem->GameUserInfo.bUserState = USER_SITTING;
					if ((pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID))
					{
                        {
                            AutoPlatLock lock(&UIDATA->uiSection);
                            auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                            if(it != UIDATA->usrInfos.end())
                            {
                                it->second.bDeskNo = pNetHead->bHandleCode;
                                it->second.bDeskStation = pQueueInfo->bDeskStation;
                                it->second.bUserState = USER_SITTING;
                                if(PLATCONFIG->bFishGame)
                                {// ��������Ϸ���¼���Ϊ������Ϸ��
                                    it->second.bUserState = USER_PLAY_GAME;
                                }
                            }
                            else
                            {
                                MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                            }
                        }
						bIsSelf = true;
						m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO = pNetHead->bHandleCode;
						m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation = pQueueInfo->bDeskStation;
					}
				}
                else
                {
                    GSERVER_LOG_ERROR("δ�ҵ������û�%d", pQueueInfo->iUserID);
                }
                pQueueInfo++;

			}

			if (bIsSelf)
			{
                GSERVER_LOG_INFO("%s �յ��Ŷӻ�����", m_MServerLogonInfo.szName);

				StartGameClient();

				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode= ACT_USER_SIT;
				ControlMessage.bDeskStation= m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
			}
			return true ;
		}
	case ASS_GR_USER_COME:		//�û�����
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserCome)) return false;
			MSG_GR_R_UserCome * pUserCome=(MSG_GR_R_UserCome *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.AddGameUser(&pUserCome->pUserInfoStruct,1,0);

            if(pUserItem == NULL)
            {
                GSERVER_LOG_ERROR("%s AddGameUserʧ�� %d", m_MServerLogonInfo.szName, pUserCome->pUserInfoStruct.dwUserID);
            }
            else
            {
                if(pUserCome->pUserInfoStruct.dwUserID == m_MServerLogonInfo.dwUserID)
                {
                    AutoPlatLock lock(&UIDATA->uiSection);
                    auto it = UIDATA->usrInfos.find(m_MServerLogonInfo.szName);
                    if(it != UIDATA->usrInfos.end())
                    {
                        it->second.bDeskNo = pUserItem->GameUserInfo.bDeskNO;
                        it->second.bDeskStation = pUserItem->GameUserInfo.bDeskStation;
                        it->second.bUserState = pUserItem->GameUserInfo.bUserState;
                    }
                    else
                    {
                        MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_MServerLogonInfo.szName);
                    }
                }
            }

			return true;
		}
	case ASS_GR_USER_LEFT:		//�û��뿪
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserLeft)) return false;
			MSG_GR_R_UserLeft * pUserLeft=(MSG_GR_R_UserLeft *)pNetData;


			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserLeft->dwUserID);
			if (pUserItem!=NULL)
			{
                if(pUserLeft->dwUserID == m_MServerLogonInfo.dwUserID)
                {
                    CloseGameRoom();
                }
				m_UserManage.DeleteGameUser(pUserItem);
			}
            else
            {
                GSERVER_LOG_ERROR("%s δ�ҵ������û� %d", m_MServerLogonInfo.szName, pUserLeft->dwUserID);
            }

			return true;
		}
	case ASS_GR_USER_CUT:		//�û�����
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserCut)) return false;
			MSG_GR_R_UserCut * pUserCut=(MSG_GR_R_UserCut *)pNetData;
			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserCut->dwUserID);
			if (pUserItem!=NULL)
			{
				//��������
				pUserItem->GameUserInfo.bUserState=USER_CUT_GAME;
			}
            else
            {
                GSERVER_LOG_ERROR("%s δ�ҵ������û� %d", m_MServerLogonInfo.szName, pUserCut->dwUserID);
            }

			return true;
		}
	case ASS_GR_SET_TABLE_BASEPOINT:	//�����û����ӱ���
		{	
            GSERVER_LOG_INFO("ASS_GR_SET_TABLE_BASEPOINT");
			return true;
		}
	case ASS_GR_SIT_ERROR:		//���´���
		{
            GSERVER_LOG_ERROR("%s ���´��� %d", m_MServerLogonInfo.szName, pNetHead->bHandleCode);
			CloseGameRoom();
			return true;
		}
    default:
        {
            CString strKey;
            strKey.Format("%d_%d", pNetHead->bMainID, pNetHead->bAssistantID);
            SYSTEMTIME sysTime;
            GetLocalTime(&sysTime);
            if(m_notHandleMsgs.find(strKey) != m_notHandleMsgs.end())
            {
                if(m_notHandleMsgs[strKey] == sysTime.wHour)
                {
                    return true;
                }
            }
            GSERVER_LOG_INFO("%s δ������Ϣ %d-%d", m_MServerLogonInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
        break;
	}
	return true;
}

//������Ϸ�ͻ���
bool CGameRoomEx::StartGameClient()
{
	try
	{
		//��ȡ���� 
		if (m_IGameFrame != NULL)
		{
			delete m_IGameFrame;
			m_IGameFrame = NULL;
		}
		//if (m_IGameFrame==NULL)
		{
			m_IGameFrame = new CClientGameDlg();

			m_IGameFrame->InitFrame(&m_GameInfo,this);
		}
		return true;
	}
	catch (...) {}

	m_IGameFrame->AFCCreateFrame();

	return false;
}

//���Ϳ�����Ϣ 
UINT CGameRoomEx::ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	try
	{
		if (m_IGameFrame!=NULL)
			m_IGameFrame->OnControlMessage(uControlCode,pControlData,uDataSize);
	}
	catch (...) {}
	return 0;
}

//��ʱ����Ϣ
LRESULT CGameRoomEx::SetGameTime(WPARAM wparam,LPARAM lparam)
{
	SetTimer(wparam,lparam,NULL);
	return TRUE;
}

//�ر���Ϸ,����Ϸ�еĻ����˷�ǿ����Ϣ��
//add 20090211
//zht
LRESULT CGameRoomEx::OnCloseGame(WPARAM wparam,LPARAM lparam)
{
	//if (NULL != m_IGameFrame)
	//{
	//	CClientGameDlg* FrameGame = (CClientGameDlg*)m_IGameFrame;
	//	//sdp20140624 д���ж����������ú꣬��ֹÿ�α�����ܲ�ͨ��
	//	if (FrameGame->GetStationParameter() >= 20 &&FrameGame->GetStationParameter() <28)
	//	{
	//		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	//	}

	//	m_IGameFrame->AFCCloseFrame();
	//}
    CloseGameRoom();
	return TRUE;
}

//��ʱ����Ϣ
LRESULT CGameRoomEx::KillGameTime(WPARAM wparam,LPARAM lparam)
{
	if((UINT)wparam == 0)
	{
		for(int i = 100;i < 110;i++)
		{
			KillTimer(i);
		}
		return 0;

	}
	KillTimer((UINT)wparam);
	return TRUE;
}

//��ʱ��
void CGameRoomEx::OnTimer(UINT_PTR uIDEvent)
{
	if(uIDEvent >= 100 && uIDEvent <= 200 )//100-110��ʱ��Ϊ��Ϸ�߼�ʹ��
	{
		KillTimer(uIDEvent);
		if (m_IGameFrame!=NULL)
		{
			m_IGameFrame->OnGameTimer(0,uIDEvent,0);
		}
		return;
	}
    switch(uIDEvent)
    {
    case TIMER_CONNECT_TO_GSERVER:
        KillTimer(uIDEvent);
        DoConnectToGServer();
        break;
    case TIMER_CHECK_USERSTATE:
        KillTimer(uIDEvent);
        if(m_GameInfo.pMeUserInfo == NULL) break;
        if(m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_PLAY_GAME && !PLATCONFIG->bFishGame)
        {
            CloseGameRoom();
        }
        break;
    case TIMER_FISHGAME_AIBANK:
        if(m_GameInfo.pMeUserInfo == NULL) break;
        if(CheckNeedMoneyOp() || m_CurDynamicConfig != GLOBALDATA->CurDynamicConfig)
        {
            CloseGameRoom();
        }
        break;
    case TIMER_FISHGAME_LEAVE:
        StandingDesk();
		 KillTimer(uIDEvent);
        break;
    case TIMER_FISHGAME_WITHTRUEPAYER:
        StandingDesk();
		 KillTimer(uIDEvent);
        break;
	case TIMER_STAND_UP:
		KillTimer(uIDEvent);
		StandingDesk();
        break;
    default:
        GSERVER_LOG_ERROR("δ����ʱ�� %d", uIDEvent);
        break;
    }
	return __super::OnTimer(uIDEvent);
}

bool CGameRoomEx::CheckNeedMoneyOp()
{
    if (m_GameInfo.pMeUserInfo == NULL) return false;
    if(IsContestGame(m_GameInfo.dwRoomRule))
    {// ��ʱ���ͱ���������������
        return false;
    }
    if (m_CurDynamicConfig.nCheckOutMinMoney != 0 && m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_CurDynamicConfig.nCheckOutMinMoney)
    {// ȡǮ
        __int64 i64RandMoney = ((m_CurDynamicConfig.nCheckInMaxMoney - m_CurDynamicConfig.nCheckOutMinMoney) / 2 +  m_CurDynamicConfig.nCheckOutMinMoney) - m_GameInfo.pMeUserInfo->GameUserInfo.i64Money;
        if(i64RandMoney > m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank)
        {
            i64RandMoney = m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank;
        }
        if(i64RandMoney > 0)			
            return true;			
    }
    else if (m_CurDynamicConfig.nCheckInMaxMoney != 0 && m_GameInfo.pMeUserInfo->GameUserInfo.i64Money >  m_CurDynamicConfig.nCheckInMaxMoney)
    {// ��Ǯ
        __int64 i64RandMoney = m_GameInfo.pMeUserInfo->GameUserInfo.i64Money - ((m_CurDynamicConfig.nCheckInMaxMoney -m_CurDynamicConfig.nCheckOutMinMoney) / 2 +  m_CurDynamicConfig.nCheckOutMinMoney);
        if(i64RandMoney > m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank)
        {
            i64RandMoney = m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank;
        }
        if(i64RandMoney > 0)
            return true;	
    }
    return false;
}

void CGameRoomEx::DoConnectToGServer()
{
    if(m_TCPSocket != NULL)
    {
        delete m_TCPSocket;
    }
    m_TCPSocket = new CTCPClientSocket(this);
    if(m_TCPSocket->Connect(m_pRoomInfo->stComRoomInfo.szServiceIP,m_pRoomInfo->stComRoomInfo.uServicePort)==false)
    {
        GSERVER_LOG_ERROR("����GServerʧ��%s:%d", m_pRoomInfo->stComRoomInfo.szServiceIP,m_pRoomInfo->stComRoomInfo.uServicePort);
        ConnectToGServer();
    }
}