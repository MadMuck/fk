// Auto_AIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Auto_AI.h"
#include "Auto_AIDlg.h"
//#include "afcfunction.h"
#include "iphlpapi.h"	//�������� zxj 2009-11-12
#include "Platconfig.h"
#include "Platglobaldata.h"
#include "PlatLog.h"
#include "Platlock.h"
#include "DiaogTaskList.h"
#include "DialogMsgInfo.h"

#pragma comment(lib,"Iphlpapi.lib")	//�������� zxj 2009-11-12
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define COL_USRNAME 0
#define COL_NICKNAME 1
#define COL_GSERVERLOGIN 2
#define COL_ROOMID 3
#define COL_DESKNO 4
#define COL_DESKSTATION 5
#define COL_USERSTATE 6
#define COL_TASKINFO 7

// CAuto_AIDlg �Ի���


CAuto_AIDlg::CAuto_AIDlg( CWnd * pParent)
: CBaseRoom(IDD_AUTO_AI_DIALOG)//Dialog(CAuto_AIDlg::IDD, pParent)
{
    m_ShowBeginNo = "";
    m_ShowEndNo = "";
    m_bExit = false;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_iLogonIndex = -2;

    PLATLOG->Init();
	//��ʼ�� SOCKET
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return;
	}
}

CAuto_AIDlg::~CAuto_AIDlg()
{

    PLATLOG->UnInit();
	::WSACleanup();
}

void CAuto_AIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_STATUS, m_ListUserStatus);
}

BEGIN_MESSAGE_MAP(CAuto_AIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	ON_WM_CREATE()
	ON_MESSAGE(IDM_CLOSE_GAME_ROOM,OnCloseGameRoom)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CAuto_AIDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CAuto_AIDlg::OnBnClickedButtonCancel)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CAuto_AIDlg::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_MIN, &CAuto_AIDlg::OnBnClickedButtonMin)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CAuto_AIDlg::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CAuto_AIDlg::OnBnClickedButtonReset)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_TASKLIST, &CAuto_AIDlg::OnBnClickedButtonTasklist)
    ON_BN_CLICKED(IDC_BUTTON_HALLINFO, &CAuto_AIDlg::OnBnClickedButtonHallinfo)
    ON_BN_CLICKED(IDC_BUTTON_ROOMINFO, &CAuto_AIDlg::OnBnClickedButtonRoominfo)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_STATUS, &CAuto_AIDlg::OnLvnColumnclickListStatus)
    ON_BN_CLICKED(IDC_BUTTON_ADDTASK, &CAuto_AIDlg::OnBnClickedButtonAddtask)
END_MESSAGE_MAP()


// CAuto_AIDlg ��Ϣ�������

BOOL CAuto_AIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|WS_MINIMIZEBOX);

    //������������
    GLOBALDATA->GameList.Create(TVS_HASBUTTONS | TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT | WS_VISIBLE | WS_TABSTOP,  CRect(0, 0,200,200), this, 200);
    GLOBALDATA->GameList.ShowWindow(SW_HIDE);
    GLOBALDATA->GameList.Init();

    {
        AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
        auto it = PLATCONFIG->m_DynamicConfigs.begin();
        while(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            CString errorMsg;
            if(!it->second.IsValid(errorMsg))
            {
                MSERVER_LOG_ERROR("����%s����%s", it->second.strTime, errorMsg);
                CString ErrorTip;
                ErrorTip.Format("����%s����:%s", it->second.strTime, errorMsg);
                MessageBox(ErrorTip);
                PostQuitMessage(0);
                return false;
            }
            it++;
        }
    }
    

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ

    SetWindowText(PLATCONFIG->strTitleText);

    // չʾ��̬����
    GetDlgItem(IDC_STATIC_TILEHEAD)->SetWindowText(PLATCONFIG->strTitleText);
    CString strStaticTip = _T("");
    CString strTmp = _T("");
    strStaticTip.Format(_T("%s��Ϸ���:%d\r\n\r\n"), strTmp, PLATCONFIG->KindID);
    strTmp = strStaticTip;
    strStaticTip.Format(_T("%s��ϷID  :%d\r\n\r\n"), strTmp, PLATCONFIG->NameID);
    strTmp = strStaticTip;
    strStaticTip.Format(_T("%s����ID  :%d\r\n\r\n"), strTmp, PLATCONFIG->RoomID);
    strTmp = strStaticTip;
    strStaticTip.Format(_T("%s���ĵ�ַ:%s:%d\r\n\r\n"), strTmp, PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort);
    strTmp = strStaticTip;
    strStaticTip.Format(_T("%s��½����:%s\r\n\r\n"), strTmp, PLATCONFIG->strAIPWD);
    strTmp = strStaticTip;
    if(PLATCONFIG->bFishGame)
        strStaticTip.Format(_T("%s��������Ϸ:��\r\n\r\n"), strTmp);
    else
        strStaticTip.Format(_T("%s��������Ϸ:��\r\n\r\n"), strTmp);
    strTmp = strStaticTip;
    SYSTEMTIME lTime;
    GetLocalTime(&lTime);
    strStaticTip.Format(_T("%s����ʱ��:%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n"), strTmp, lTime.wYear, lTime.wMonth, lTime.wDay, lTime.wHour, lTime.wMinute, lTime.wSecond);
    strTmp = strStaticTip;
    GetDlgItem(IDC_EDIT_STATIC)->SetWindowText(strStaticTip);

    ((CEdit*)GetDlgItem(IDC_EDIT_STATIC))->SetSel(-1,0);

    if(NextTask())
    {
        // չʾ��̬����
        ResetConfig();
    }

    MSERVER_LOG_INFO("����������%s",PLATCONFIG->strTitleText);
    ((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(IDC_BUTTON_RESET))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->EnableWindow(FALSE);

    SetTimer(TIMER_CHECK_MODIFY_PARA, 10*1000, NULL);

    DWORD dwStyle = m_ListUserStatus.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
    m_ListUserStatus.SetExtendedStyle(dwStyle);
    m_ListUserStatus.ModifyStyle(0, LVS_REPORT);
    m_ListUserStatus.InsertColumn(COL_USRNAME, _T("�û���"), LVCFMT_LEFT,120);
    m_ListUserStatus.InsertColumn(COL_NICKNAME, _T("�ǳ�"), LVCFMT_LEFT,120);
    m_ListUserStatus.InsertColumn(COL_GSERVERLOGIN, _T("��½"), LVCFMT_LEFT,60);
    m_ListUserStatus.InsertColumn(COL_ROOMID, _T("����ID"), LVCFMT_LEFT,60);
    m_ListUserStatus.InsertColumn(COL_DESKNO, _T("���Ӻ�"), LVCFMT_LEFT,60);
    m_ListUserStatus.InsertColumn(COL_DESKSTATION, _T("��λ��"), LVCFMT_LEFT,60);
    m_ListUserStatus.InsertColumn(COL_USERSTATE, _T("��Ϸ״̬"), LVCFMT_LEFT,60);
    m_ListUserStatus.InsertColumn(COL_TASKINFO, _T("������Ϣ"), LVCFMT_LEFT,300);
    ShowUserStatus();
    SetTimer(TIMER_UPDATE_USERSTATUS, 5000, NULL);

	int argc = __argc;
	if(argc >= 2)
	{//��������в��� 1 ����ű�����ʱ��½
		CString str = __argv[1];	
		if(str == "1")
		{
			OnBnClickedButtonLogin();
		}
	}
	GLOBALDATA->bLoginEnable = false;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAuto_AIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CAuto_AIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAuto_AIDlg::OnOK()
{

}
void CAuto_AIDlg::OnCancel()
{
    if(ModifyPara())
    {
        CString strTip;
        strTip.Format(_T("�Ƿ񱣴��޸�?"));
        if(IDOK == MessageBox(strTip, _T("�����޸�"), MB_OKCANCEL))
        {
            SaveConfig();
        }
    }
    CString strTip;
    strTip.Format(_T("ȷ���˳�%s?"), PLATCONFIG->strTitleText);
    if(IDOK == MessageBox(strTip, _T("�˳�"), MB_OKCANCEL))
    {
        MSERVER_LOG_INFO("�رջ�����%s",PLATCONFIG->strTitleText);
        // TODO: �ڴ���ӿؼ�֪ͨ����������\
        //add by 20090211zht�޸Ĺر�ʱ�Ƚ�������Ϸ�еĻ������˳�
        bool bExit = true;
        for (int i=0; i< GLOBALDATA->CurDynamicConfig.nLogonCount; i++)
        {
            if (GLOBALDATA->RoomInfo[i].pGameRoomWnd != NULL)
            {
                bExit = false;
                ::PostMessage(GLOBALDATA->RoomInfo[i].pGameRoomWnd->GetSafeHwnd(),IDM_CLOSE,0,0);
            }
        }
        KillTimer(TIMER_CHECK_MODIFY_PARA);
        m_bExit = true;
        if(bExit) PostQuitMessage(0);
        //add by 20090211zht�޸Ĺر�ʱ�Ƚ�������Ϸ�еĻ������˳�
    }
}

void	CAuto_AIDlg::OnStartRobot()
{
    InitUI(false);
    MSERVER_LOG_INFO("StartRobot");
    ((CButton*)GetDlgItem(IDC_BUTTON_LOGIN))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->EnableWindow(TRUE);
    GLOBALDATA->bConnectToWitchServer = 0; //

    m_ShowBeginNo.Format("%s%d", GLOBALDATA->CurDynamicConfig.strRobotPreName, GLOBALDATA->CurDynamicConfig.nBeginSequenceNo);
    m_ShowEndNo.Format("%s%d", GLOBALDATA->CurDynamicConfig.strRobotPreName, GLOBALDATA->CurDynamicConfig.nBeginSequenceNo + 10);

    ((CEdit*)GetDlgItem(IDC_EDIT_CHECKBEGINNO))->SetWindowText(m_ShowBeginNo);
    ((CEdit*)GetDlgItem(IDC_EDIT_CHECKENDNO))->SetWindowText(m_ShowEndNo);

    OnConnectToCenterServer();
    SetTimer(TIMER_CHECK_UNCONNECT_ACCOUNT, 5000, NULL);
}

void CAuto_AIDlg::OnStopRobot()
{
    CString strTip;
    strTip.Format(_T("ȷ��ȡ����½%s?"), PLATCONFIG->strTitleText);
    if(IDOK == MessageBox(strTip, _T("�˳�"), MB_OKCANCEL))
    {
        MSERVER_LOG_INFO("StopRobot");
        m_TCPSocket->CloseSocket(TRUE);
        ((CButton*)GetDlgItem(IDC_BUTTON_LOGIN))->EnableWindow(TRUE);
        ((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->EnableWindow(FALSE);
        KillTimer(TIMER_CHECK_UNCONNECT_ACCOUNT);
        m_iLogonIndex = -2;
        GLOBALDATA->ReInit();
        UIDATA->ReInit();
        // TODO: �ڴ���ӿؼ�֪ͨ����������\
        //add by 20090211zht�޸Ĺر�ʱ�Ƚ�������Ϸ�еĻ������˳�
        for (int i=0; i < MAX_GAME_ROOM; i++)
        {
            if (GLOBALDATA->RoomInfo[i].pGameRoomWnd != NULL)
                ::PostMessage(GLOBALDATA->RoomInfo[i].pGameRoomWnd->GetSafeHwnd(),IDM_CLOSE,0,0);
        }

        GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText("����״̬");
        //add by 20090211zht�޸Ĺر�ʱ�Ƚ�������Ϸ�еĻ������˳�
        if(NextTask())
        {
            // չʾ��̬����
            ResetConfig();
        }
        InitUI(true);
        UIQueueGame(true);
    }
}


//����AServer
void CAuto_AIDlg::OnConnectToCenterServer()
{
    KillTimer(TIME_CONNECT_ASERVER);
    SetTimer(TIME_CONNECT_ASERVER, 1000, NULL);
}

void CAuto_AIDlg::DoOnConnectToCenterServer()
{
    if (m_TCPSocket != NULL)
    {
        delete m_TCPSocket;
    }
    m_TCPSocket = new CTCPClientSocket(this);
    CString str;
    str.Format("��������AServer %s:%d", PLATCONFIG->CenterServerIPAddr,PLATCONFIG->CenterServerPort);
    GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
    if(m_TCPSocket->Connect(PLATCONFIG->CenterServerIPAddr,PLATCONFIG->CenterServerPort)==false)
    {// ����ʧ��,������������
        MSERVER_LOG_ERROR("����AServerʧ��%s:%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort);
        CString str;
        str.Format("����AServerʧ�� %s:%d", PLATCONFIG->CenterServerIPAddr,PLATCONFIG->CenterServerPort);
        GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
        OnConnectToCenterServer();
    }
}

//����MServer
void CAuto_AIDlg::OnConnectToLogonServer()
{

    KillTimer(TIME_CONNECT_MSERVER);
    SetTimer(TIME_CONNECT_MSERVER, 1000, NULL);
	
}

void CAuto_AIDlg::DoOnConnectToLogonServer()
{
    SetTimer(TIMER_PLC_CLOSE_SOCKET, 30*1000, NULL);
    if (m_TCPSocket != NULL)
    {
        delete m_TCPSocket;
    }
    m_TCPSocket = new CTCPClientSocket(this);
    CString str;
    str.Format("��������MServer %s:%d", GLOBALDATA->MainServerIPAddr,GLOBALDATA->MainServerPort);
    GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
    if(m_TCPSocket->Connect(GLOBALDATA->MainServerIPAddr,GLOBALDATA->MainServerPort)==false)
    {// ����ʧ��,������������
        MSERVER_LOG_ERROR("����MServerʧ��%s:%d", GLOBALDATA->MainServerIPAddr,GLOBALDATA->MainServerPort);
        CString str;
        str.Format("����MServerʧ�� %s:%d", GLOBALDATA->MainServerIPAddr,GLOBALDATA->MainServerPort);
        GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
    }
}


//�ӿں��� 
//����������Ϣ����
int CAuto_AIDlg::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(bMainID,bAssistantID,bHandleCode);//����������
}

//����������Ϣ����
int CAuto_AIDlg::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}

//�����ȡ��Ϣ
bool CAuto_AIDlg::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(!GLOBALDATA->bLoginEnable) return true;
	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:	//������Ϣ 
		{
			return OnConnectMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_REQURE_GAME_PARA:	//���ķ���ȫ�ֲ���//������Ϸȫ�ֲ���
		{
            GLOBALDATA->bConnectToWitchServer = 1;
			CenterServerMsg * msg=(CenterServerMsg *)pNetData;
			
            MSERVER_LOG_INFO("%s:%d AServerӦ��ȫ�ֲ�����Ϣ MServer:%s-%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort,msg->m_strMainserverIPAddr, msg->m_iMainserverPort);
            GLOBALDATA->MainServerIPAddr = msg->m_strMainserverIPAddr;
            GLOBALDATA->MainServerPort = msg->m_iMainserverPort;

			m_TCPSocket->CloseSocket (true);		

			return true;
		}
	case MDM_GP_LOGON:	//��½��Ϣ
		{
			return OnLogonMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_LIST:	//��Ϸ�б�
		{
			return OnListMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_CONTEST:
		{
			if (!EnterContestRoom(pNetHead,pNetData,uDataSize,pClientSocket))
			{
                m_TCPSocket->CloseSocket(true);
                return false;
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
                    break;
                }
            }
            {
                MSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_CurPlaceUserInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
                m_notHandleMsgs[strKey] = sysTime.wHour;
            }
        }
        
        break;
	}
	return true;
}

//����������Ϣ
bool CAuto_AIDlg::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	if(GLOBALDATA->bConnectToWitchServer == 0)//���ķ�����
	{
        if(uErrorCode == 0)
        {
            MSERVER_LOG_INFO("����AServer�¼�%s:%d-%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort,uErrorCode);
            CString str;
            str.Format("����AServer�ɹ� %s:%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort);
            GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
        }
        else
        {
            MSERVER_LOG_ERROR("����AServer�¼�%s:%d-%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort,uErrorCode);
            CString str;
            str.Format("����AServerʧ�� %s:%d-%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort,uErrorCode);
            GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
            OnConnectToCenterServer();
        }
	}
	else if (GLOBALDATA->bConnectToWitchServer ==1)
	{
        if(uErrorCode == 0)
        {
            MSERVER_LOG_INFO("����MServer�¼�%s:%d-%d", GLOBALDATA->MainServerIPAddr, GLOBALDATA->MainServerPort,uErrorCode);
            CString str;
            str.Format("����MServer�ɹ� %s:%d", GLOBALDATA->MainServerIPAddr, GLOBALDATA->MainServerPort);
            GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
        }
        else
        {
            MSERVER_LOG_ERROR("����MServer�¼�%s:%d-%d", GLOBALDATA->MainServerIPAddr, GLOBALDATA->MainServerPort,uErrorCode);
            CString str;
            str.Format("����MServerʧ�� %s:%d-%d", GLOBALDATA->MainServerIPAddr, GLOBALDATA->MainServerPort,uErrorCode);
            GetDlgItem(IDC_STATIC_LOGINSTATUS)->SetWindowText(str);
            OnConnectToLogonServer();
        }
	}
    else
    {
        MSERVER_LOG_ERROR("δ֪�����¼�");
    }
	return true;
}

//����ر���Ϣ
bool CAuto_AIDlg::OnSocketCloseEvent()
{    
    {
        if(GLOBALDATA->bConnectToWitchServer == 0)
        {
            OnConnectToCenterServer();
        }
        m_iLogonIndex = -1;
    }
	return true;
}

//������Ϣ����
bool CAuto_AIDlg::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//���������Ϣ
	{
		pClientSocket->SendData(MDM_CONNECT,ASS_NET_TEST,0);//������Ϣ����
		return true;
	}
	else if ((pNetHead->bAssistantID==2)||(pNetHead->bAssistantID == 3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		if(GLOBALDATA->bConnectToWitchServer == 0)//���ķ�����
		{
            MSERVER_LOG_INFO("��AServer����ȫ�ֲ�����Ϣ%s:%d", PLATCONFIG->CenterServerIPAddr, PLATCONFIG->CenterServerPort);
			m_TCPSocket->SendData(NULL,0,MDM_GP_REQURE_GAME_PARA,0,0);
			return true;
		}
		LoginMServer();
		return true;
	}
    else
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
        {
            MSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_CurPlaceUserInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
    }
	return false;
}

//��ȡ��������غ��� zxj 2009-11-12 ��������
CString innerGetMac()
{
	CString retMac="";
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{
		free(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{
		pAdapter=pAdapterInfo;
		CString temp;
		while(pAdapter)
		{
			if(pAdapter->Type==6)//pAdapter->Description�а���"PCI"Ϊ����������//pAdapter->Type��71Ϊ����������
			{
				for(UINT i=0;i<pAdapter->AddressLength;i++)
				{
					temp.Format("%02X",pAdapter->Address[i]);
					retMac+=temp;
					//	printf("%02X%c",pAdapter->Address[i],i==pAdapter->AddressLength-1?��n��:��-��);
				}
				break;
			}
			pAdapter=pAdapter->Next;
		}
	}
	if(pAdapterInfo)
		free(pAdapterInfo);
	return retMac;
}

//��ȡ��������غ��� zxj 2009-11-12 ��������
CString coreGetCode()
{
	CString strRet="";
	CString str=innerGetMac(),s;
	if(str=="")
		return "δ��⵽ϵͳ����Ethernet�豸��";

	int r=0;
	int l=str.GetLength();
	for(int i=0;i<l;i+=2)
	{
		int r1=0,r2=0;
		s=str.Mid(i,1);
		if(s>="0" && s<="9")
			r1=atoi(s);
		if(s=="A" || s=="a")	r1=10;
		if(s=="B" || s=="b")	r1=11;
		if(s=="C" || s=="c")	r1=12;
		if(s=="D" || s=="d")	r1=13;
		if(s=="E" || s=="e")	r1=14;
		if(s=="F" || s=="f")	r1=15;
		s=str.Mid(i+1,1);
		if(s>="0" && s<="9")
			r2=atoi(s);
		if(s=="A" || s=="a")	r2=10;
		if(s=="B" || s=="b")	r2=11;
		if(s=="C" || s=="c")	r2=12;
		if(s=="D" || s=="d")	r2=13;
		if(s=="E" || s=="e")	r2=14;
		if(s=="F" || s=="f")	r2=15;

		CString t;
		r+=r1*16+r2;
		srand(r);
		t.Format("%s%04X%s-",str.Mid(l-i-1,1),rand(),str.Mid(l-i-2,1));
		strRet+=t;
	}
	if(strRet.Right(1)=="-")strRet=strRet.Left(strRet.GetLength()-1);
	return strRet;
}

//��½��Ϣ����
bool CAuto_AIDlg::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID==ASS_GP_LOGON_SUCCESS)		//��½�ɹ�
	{
		//������Ϣ
		m_CurPlaceUserInfo = *((MSG_GP_R_LogonResult *)pNetData);

		wsprintf(m_CurPlaceUserInfo.szMD5Pass,"%s",PLATCONFIG->strAIPWD);

        MSERVER_LOG_INFO("%s ��½�ɹ�", m_CurPlaceUserInfo.szName);
        {

            AutoPlatLock lock(&UIDATA->uiSection);
            auto it = UIDATA->usrInfos.find(m_CurPlaceUserInfo.szName);
            if(it != UIDATA->usrInfos.end())
            {
                it->second.strUserName = m_CurPlaceUserInfo.szName;
                it->second.strNickName = m_CurPlaceUserInfo.nickName;
            }
            else
            {
                MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_CurPlaceUserInfo.szName);
            }
        }

        // �Ƚ���һ�����д�ȡ����
        AiControlBank();

		//��ȡ��Ϸ�б�
		return GetGameRoomList();
	}
	else if (pNetHead->bAssistantID==ASS_GP_LOGON_ERROR)	//��½ʧ��
	{
		//��ʾ������Ϣ
		CString strMessage;
		switch (pNetHead->bHandleCode)
		{
		case ERR_GP_USER_NO_FIND:
			{
				strMessage=TEXT("�ܱ�Ǹ�������ʺţ�");
				break;
			}
		case ERR_GP_USER_PASS_ERROR:
			{
				strMessage=TEXT("�ܱ�Ǹ���������");
				break;
			}
		case ERR_GP_USER_VALIDATA:
			{
				strMessage=TEXT("���ʺ��ѱ���ֹ��¼��");
				break;
			}
		case ERR_GP_USER_IP_LIMITED:
			{
				strMessage=TEXT("�����ڵ� IP ��ַ����ֹ��½��");
				break;
			}
		case ERR_GP_USER_EXIST:
			{
				strMessage=TEXT("�ܱ�Ǹ������û����Ѿ�����ע���ˣ�");
				break;
			}
		case ERR_GP_PASS_LIMITED:
			{
				strMessage=TEXT("������������");
				break;
			}
		case ERR_GP_IP_NO_ORDER:
			{
				strMessage=TEXT("����Ҫ��ָ���� IP ��ַ��ϵͳ��½��");
				break;
			}
		case ERR_GP_USER_LOGON:
			{
				strMessage=TEXT("���ʺ��Ѿ���¼��");
				break;
			}
		case ERR_GP_USERNICK_EXIST:
			{
				strMessage=TEXT("���ǳ��Ѿ����ڣ�");
				break;
			}		
		default:strMessage=TEXT("ϵͳ��¼���ִ����������Ա��ϵ��");
		}

        char  szLogonName[61] = {0};
        sprintf(szLogonName, "%s", m_CurPlaceUserInfo.szName);
        MSERVER_LOG_ERROR("%s ��½ʧ��%d %s", szLogonName, pNetHead->bHandleCode, strMessage);
	
		m_TCPSocket->CloseSocket(true);
		return true;
	}
    else
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
        MSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_CurPlaceUserInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
        m_notHandleMsgs[strKey] = sysTime.wHour;
    }
	return true;
}

//��ȡ��Ϸ�б�
bool CAuto_AIDlg::GetGameRoomList()
{
    MSERVER_LOG_INFO("%s ������Ϸ�б�", m_CurPlaceUserInfo.szName);
	m_TCPSocket->SendData(MDM_GP_LIST,ASS_GP_LIST_KIND,0);//������Ϸ�б�
	return true;
}

//�б���Ϣ
bool CAuto_AIDlg::OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GP_LIST_KIND:		//��ȡ������Ϸ����
		{
            MSERVER_LOG_INFO("%s �ɹ���ȡ��Ϸ����ASS_GP_LIST_KIND", m_CurPlaceUserInfo.szName);
			//��������
			GLOBALDATA->GameList.HandleKindData((ComKindInfo *)pNetData,uDataSize/sizeof(ComKindInfo));//�����ϼ�����Ϸ����
			return true;
		}
	case ASS_GP_LIST_NAME:		//��ȡ����Ϸ����
		{
            MSERVER_LOG_INFO("%s �ɹ���ȡ��Ϸ����ASS_GP_LIST_NAME", m_CurPlaceUserInfo.szName);
			//��������
			GLOBALDATA->GameList.HandleNameData((ComNameInfo *)pNetData,uDataSize/sizeof(ComNameInfo));

			//������ʾ
			if (pNetHead->bHandleCode==ERR_GP_LIST_FINISH) //������Ϸ�����б������־
			{
				MSG_GP_SR_GetRoomStruct GetList;
				GetList.uKindID = PLATCONFIG->KindID;
				GetList.uNameID = PLATCONFIG->NameID;
				m_TCPSocket->SendData(&GetList,sizeof(GetList),MDM_GP_LIST,ASS_GP_LIST_ROOM,0);
			}
			return true;
		}
	case ASS_GP_LIST_ROOM:		//��ȡ����Ϸ����
		{
			//Ч������ 
			if (uDataSize<sizeof(MSG_GP_SR_GetRoomStruct))
            {
                MSERVER_LOG_ERROR("%s MSG_GP_SR_GetRoomStruct �ṹ�岻һ��", m_CurPlaceUserInfo.szName);
				return false;
            }
            MSERVER_LOG_INFO("%s �ɹ���ȡ������ϢASS_GP_LIST_ROOM", m_CurPlaceUserInfo.szName);
			MSG_GP_SR_GetRoomStruct * pNetRoomBuf=(MSG_GP_SR_GetRoomStruct *)pNetData;
			//��������
			GLOBALDATA->GameList.HandleRoomData((ComRoomInfo *)((char *)pNetData+sizeof(MSG_GP_SR_GetRoomStruct)),
				(uDataSize-sizeof(MSG_GP_SR_GetRoomStruct))/sizeof(ComRoomInfo),pNetRoomBuf->uKindID,pNetRoomBuf->uNameID);

			if (pNetHead->bHandleCode == ERR_GP_LIST_FINISH) //������Ϸ�б������־
			{
				//���ӱ��������ж�
				CAFCRoomItem *pGameRoomItem = GLOBALDATA->GameList.FindRoomItem(PLATCONFIG->RoomID);
				if (NULL == pGameRoomItem)
				{
                    MSERVER_LOG_ERROR("%s δ��ȡ��ָ��������Ϣ", m_CurPlaceUserInfo.szName);
					return false;
				}

                {
                    AutoPlatLock lock(&UIDATA->uiSection);
                    UIDATA->roomInfo = pGameRoomItem->m_RoomInfo;
                    GetDlgItem(IDC_STATIC_ROOMNAME)->SetWindowText(pGameRoomItem->m_RoomInfo.szGameRoomName);
                }

                if (IsContestGame(pGameRoomItem->m_RoomInfo.dwRoomRule))
                {
                    UIQueueGame(false);
                    GetContestRoomID(pGameRoomItem->m_RoomInfo.uRoomID);
                }
                else
                {
                    if((pGameRoomItem->m_RoomInfo.dwRoomRule &GRR_QUEUE_GAME) != 0)
                    {
                        UIQueueGame(false);
                    }
                    else
                    {
                        UIQueueGame(true);
                    }
                    EnterGameRoom(pGameRoomItem->m_RoomInfo.uRoomID);
                }
			}
			return true;
		}
	case ASS_GP_LIST_COUNT:		//��������
		{
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
                    break;;
                }
            }
            MSERVER_LOG_INFO("%s δ������Ϣ %d:%d", m_CurPlaceUserInfo.szName, pNetHead->bMainID, pNetHead->bAssistantID);
            m_notHandleMsgs[strKey] = sysTime.wHour;
        }
        break;
	}
	return TRUE;
}

//��ʱ��
void CAuto_AIDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIME_CONNECT_ASERVER:
		{
            KillTimer(nIDEvent);
			DoOnConnectToCenterServer();
			break;
		}
    case TIME_CONNECT_MSERVER:
        {
            KillTimer(nIDEvent);
            DoOnConnectToLogonServer();
            break;
        }
    case TIMER_CHECK_UNCONNECT_ACCOUNT:
        {
            if(m_iLogonIndex != -1 || GLOBALDATA->bConnectToWitchServer == 0 || m_iLogonIndex == -2) break;
            m_iLogonIndex = FindNextLogonIndex();
            if(m_iLogonIndex != -1)
            {// ���ʺ���Ҫ��½
                OnConnectToLogonServer();
            }
        }
        break;
    case TIMER_CHECK_MODIFY_PARA:
        if(ModifyPara())
        {
            ((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow(TRUE);
            ((CButton*)GetDlgItem(IDC_BUTTON_RESET))->EnableWindow(TRUE);
        }
        else
        {
            ((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow(FALSE);
            ((CButton*)GetDlgItem(IDC_BUTTON_RESET))->EnableWindow(FALSE);
        }
        break;
    case TIMER_UPDATE_USERSTATUS:
        if(((CButton*)GetDlgItem(IDC_CHECK_STOPSTATUS))->GetCheck()) return;
        ShowUserStatus();
        break;
    case TIMER_SORT_USERSTATUS:
        KillTimer(nIDEvent);
        m_ListUserStatus.SortItems(CompareProc, (DWORD_PTR)this);//����ڶ��������ǱȽϺ����ĵ���������
        break;
    case TIMER_PLC_CLOSE_SOCKET:
        {
            KillTimer(nIDEvent);
            if (NULL != m_TCPSocket)
            {
                //��ȡ�������ݳ�ʱ���Ͽ�����
                m_TCPSocket->CloseSocket(true);
                OnConnectToLogonServer();
            }
        }
        break;
    default:
        MSERVER_LOG_ERROR("δ����ʱ�� %d", nIDEvent);
        break;
	}
	return __super::OnTimer(nIDEvent);
}

//�رշ�������
LRESULT CAuto_AIDlg::OnCloseGameRoom(WPARAM wparam, LPARAM lparam)
{
	RoomInfoStruct * pRoomInfo=(RoomInfoStruct *)wparam;
	if ((pRoomInfo!=NULL)&&(pRoomInfo->bAccess)&&(pRoomInfo->pGameRoomWnd!=NULL)&&(IsWindow(pRoomInfo->pGameRoomWnd->GetSafeHwnd())))
	{
		
		for (int i = 0; i < MAX_GAME_ROOM; i++)
		{
			if (GLOBALDATA->Robots[i].iUserID == lparam)
			{
				GLOBALDATA->Robots[i].pRoomItem = NULL;
                break;
			}
		}
		delete pRoomInfo->pGameRoomWnd;
		pRoomInfo->pGameRoomWnd = NULL;
		memset(pRoomInfo,0,sizeof(RoomInfoStruct));
	}
    if(!m_bExit) return 0;
    for (int i = 0; i < MAX_GAME_ROOM; i++)
    {
        if(GLOBALDATA->Robots[i].pRoomItem != NULL)
        {
            return 0;
        }
        else
        {
            if(i + 1 ==  MAX_GAME_ROOM)
            {
                m_bExit = false;
                PostQuitMessage(0);
            }
        }
    }
	return 0;
}

bool CAuto_AIDlg::EnterContestRoom(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
    if(pNetHead->bAssistantID != ASS_GP_GET_CONTEST_ROOMID)
    {
        return true;
    }
	if (0 != pNetHead->bHandleCode)
	{
		MSERVER_LOG_INFO("%s ��ȡ����������Ϣ���� %d", m_CurPlaceUserInfo.szName, pNetHead->bHandleCode);
		return false;
	}

    if(uDataSize != sizeof(MSG_GP_GetContestRoomID_Result))
    {
        MSERVER_LOG_ERROR("MSG_GP_GetContestRoomID_Result �ṹ�岻һ��");
        return false;
    }

	MSG_GP_GetContestRoomID_Result *pContestRoom = (MSG_GP_GetContestRoomID_Result*)pNetData;

	EnterGameRoom(pContestRoom->tRoomInfo.uRoomID);
	return true;
}
//
BOOL CAuto_AIDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CBaseRoom::PreTranslateMessage(pMsg);
}

int CAuto_AIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseRoom::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CAuto_AIDlg::LoginMServer()
{
	CString LogonName;
	LogonName.Format("%s", m_CurPlaceUserInfo.szName);

	int rNum=0;
	CString TML_SN="";
	CString allword="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	for (int i = 0;i <= 120; i ++)
	{
		rNum =(rand() * 1000)% 62;
		TML_SN+=allword[rNum];
	}
	TML_SN += "";

	CString MatchineCode= coreGetCode();	//ȡ�ñ��������� zxj 2009-11-12 ��������
	

	MSG_GP_S_LogonByNameStruct LogonByName;///����һ���ṹ
	::memset(&LogonByName,0,sizeof(LogonByName));
	//LogonByName.uRoomVer=GAME_PLACE_MAX_VER;//�ṹ�İ汾
	lstrcpy(LogonByName.TML_SN,TML_SN);
	lstrcpy(LogonByName.szName, LogonName);//�ṹ���û���
	wsprintf(LogonByName.szMD5Pass,"%s",PLATCONFIG->strAIPWD);
/*	lstrcpy(LogonByName.szMathineCode, MatchineCode);*/	//�����������ѱ��������봫�ݵ���¼���ݰ��� zxj 2009-11-12
	//LogonByName.gsqPs = 5471;
	m_TCPSocket->SendData(&LogonByName,sizeof(LogonByName),MDM_GP_LOGON,ASS_GP_LOGON_BY_NAME,0);
    MSERVER_LOG_INFO("%s ��½Mserver", LogonName);

    
    {
        AutoPlatLock lock(&UIDATA->uiSection);
        auto it = UIDATA->usrInfos.find(LogonByName.szName);
        if(it != UIDATA->usrInfos.end())
        {
            it->second.cof = GLOBALDATA->CurDynamicConfig;
        }
        else
        {
            UserItemUIInfo itemInfo;
            itemInfo.cof = GLOBALDATA->CurDynamicConfig;
            UIDATA->usrInfos[LogonByName.szName] = itemInfo;
        }
    }
}

//��ȡ���ʵı�������ID
void CAuto_AIDlg::GetContestRoomID(int roomid)
{
	CAFCRoomItem *pGameRoomItem = GLOBALDATA->GameList.FindRoomItem(roomid);
	if (!IsContestGame(pGameRoomItem->m_RoomInfo.dwRoomRule))
	{
        MSERVER_LOG_ERROR("%s �Ǳ���������Ϣ%d", m_CurPlaceUserInfo.szName, roomid);
		return;
	}
    MSERVER_LOG_INFO("%s ��ȡ����������Ϣ %d", m_CurPlaceUserInfo.szName, pGameRoomItem->m_RoomInfo.iContestID);
	MSG_GP_GetContestRoomID getContestRoomID;
	getContestRoomID.iUserID = m_CurPlaceUserInfo.dwUserID;
	getContestRoomID.iContestID = pGameRoomItem->m_RoomInfo.iContestID;
	m_TCPSocket->SendData(&getContestRoomID, sizeof(MSG_GP_GetContestRoomID), MDM_GP_CONTEST, ASS_GP_GET_CONTEST_ROOMID, 0);

}

void CAuto_AIDlg::EnterGameRoom(int roomid)
{
	CAFCRoomItem *pGameRoomItem = GLOBALDATA->GameList.FindRoomItem(roomid);

	if (pGameRoomItem == NULL) 
	{
		MSERVER_LOG_ERROR("%s ��ȡ����������Ϣ%d", m_CurPlaceUserInfo.szName, roomid);
        m_TCPSocket->CloseSocket(true);
		return;
	}
    

    MSERVER_LOG_INFO("%s ����GameRoomEx", m_CurPlaceUserInfo.szName);

	//�ж��Ƿ����ⷿ��
	ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;
	if (((pComRoomInfo->uComType!=TY_NORMAL_GAME)&&(pComRoomInfo->uComType!=TY_MATCH_GAME)
		&&(pComRoomInfo->uComType!=TY_MONEY_GAME)))
	{
        MSERVER_LOG_ERROR("%s ��֧�ַ���%d", m_CurPlaceUserInfo.szName, roomid);
		m_TCPSocket->CloseSocket(true);
		AfxGetApp( )->m_pMainWnd->DestroyWindow ();
		return;		
	}	
	//�ж���Ϸ�Ƿ�װ��Ч��汾
	TCHAR szProessName[31],szGameName[61];
	GLOBALDATA->GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
	GLOBALDATA->GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));


	//������Ϸ����
    int index = m_iLogonIndex;
	RoomInfoStruct *pRoomInfoItem=&GLOBALDATA->RoomInfo[index];

	try
	{
        {
            AutoPlatLock lock(&UIDATA->uiSection);
            auto it = UIDATA->usrInfos.find(m_CurPlaceUserInfo.szName);
            if(it != UIDATA->usrInfos.end())
            {
                it->second.roomID = roomid;
            }
            else
            {
                MSERVER_LOG_ERROR("%s û���ҵ�UI��Ϣ", m_CurPlaceUserInfo.szName);
            }
        }
		pRoomInfoItem->bAccess=true;
		pRoomInfoItem->stComRoomInfo=*pComRoomInfo;
		lstrcpy(pRoomInfoItem->szProcessName,szProessName);

		if (pRoomInfoItem->pGameRoomWnd != NULL)
		{
			delete pRoomInfoItem->pGameRoomWnd;
			pRoomInfoItem->pGameRoomWnd = NULL;
		}
		pRoomInfoItem->pGameRoomWnd=new CGameRoomEx(szGameName,pRoomInfoItem,&m_CurPlaceUserInfo,GLOBALDATA->CurDynamicConfig);

		CGameRoomEx * pGameRoom = (CGameRoomEx *)pRoomInfoItem->pGameRoomWnd;

        if(index >= 0 && index < MAX_GAME_ROOM)
        {
		    GLOBALDATA->Robots[index].pRoomItem = pRoomInfoItem;
		    GLOBALDATA->Robots[index].iUserID = m_CurPlaceUserInfo.dwUserID;
        }
        else
        {
            MSERVER_LOG_ERROR("���Ϸ�����%d", index);
            throw TEXT("����A�����ʧ��");
        }

		if (pRoomInfoItem->pGameRoomWnd->Create(IDD_GAME_ROOM,this)==FALSE) 
			throw TEXT("����A�����ʧ��");
	}
	catch (...)
	{
        MSERVER_LOG_ERROR("%s ����GameRoomEx�쳣", m_CurPlaceUserInfo.szName);
		//������Ϣ
		delete pRoomInfoItem->pGameRoomWnd;
		memset(pRoomInfoItem,0,sizeof(RoomInfoStruct));
        m_TCPSocket->CloseSocket(true);
		return; 
	}

    KillTimer(TIMER_PLC_CLOSE_SOCKET);
    m_TCPSocket->CloseSocket(true);
	return;
}

//�����˸��ݽ�Ҳ�������
void CAuto_AIDlg::AiControlBank()
{
    if (GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney != 0 && m_CurPlaceUserInfo.i64Money < GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney)
    {// ȡǮ
        __int64 i64RandMoney = ((GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney - GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) / 2 +  GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) - m_CurPlaceUserInfo.i64Money;
        if(rand()%2 == 0)
        {
            i64RandMoney -= (rand() % ((GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney - GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) / 4));
        }
        else
        {
            i64RandMoney += (rand() % ((GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney - GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) / 4));
        }
        if(i64RandMoney > m_CurPlaceUserInfo.i64Bank)
        {
            i64RandMoney = m_CurPlaceUserInfo.i64Bank;
        }
        if(i64RandMoney > 0)			
            CheckMoney(i64RandMoney, 1);			
    }
    else if (GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney != 0 && m_CurPlaceUserInfo.i64Money >  GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney)
    {// ��Ǯ
        __int64 i64RandMoney = m_CurPlaceUserInfo.i64Money - ((GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney -GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) / 2 +  GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney);
        if(rand()%2 == 0)
        {
            i64RandMoney -= (rand() % ((GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney - GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) / 4));
        }
        else
        {
            i64RandMoney += (rand() % ((GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney - GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney) / 4));
        }
        if(i64RandMoney > 0)
            CheckMoney(i64RandMoney, 2);	
    }
}

void CAuto_AIDlg::CheckMoney(__int64 iMoney, int type)
{
    TMSG_GP_BankCheck checkMoney;
    checkMoney._game_id = 0;
    checkMoney._money = iMoney;
    checkMoney._operate_type = type;
    checkMoney._user_id = m_CurPlaceUserInfo.dwUserID;
    memcpy(checkMoney._szMD5Pass,m_CurPlaceUserInfo.szMD5Pass,sizeof(m_CurPlaceUserInfo.szMD5Pass));
    m_TCPSocket->SendData(&checkMoney, sizeof(TMSG_GP_BankCheck), MDM_GP_BANK, ASS_GP_BANK_CHECK, 0);
}

bool CAuto_AIDlg::NextTask()
{
    SYSTEMTIME lTime;
    GetLocalTime(&lTime);
    CString key;
    key.Format("%02d%02d", lTime.wHour, lTime.wMinute);
    DynamicConfig cof;
    cof.nLogonCount = -1;
    
    {
        AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
        auto it = PLATCONFIG->m_DynamicConfigs.begin();
        while(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            if(it->first <= key.GetBuffer())
            {
                cof = it->second;
            }
            else
            {// �ҵ�
                if(GLOBALDATA->CurDynamicConfig.strTime != cof.strTime)
                {
                    MSERVER_LOG_INFO("Start Task %s", cof.strTime);
                    GLOBALDATA->CurDynamicConfig = cof;
                    CString strInfo;
                    strInfo.Format("��ǰ����:%s", GLOBALDATA->CurDynamicConfig.strTime);
                    GetDlgItem(IDC_STATIC_TASKINFO)->SetWindowText(strInfo);
                    ResetConfig();
                    return true;
                }
                else
                {
                    break;
                }
            }
            it++;
        }
    }
    if(cof.nLogonCount != -1 && GLOBALDATA->CurDynamicConfig.strTime != cof.strTime)
    {// �ҵ�
        MSERVER_LOG_INFO("Start Task %s", cof.strTime);
        GLOBALDATA->CurDynamicConfig = cof;
        CString strInfo;
        strInfo.Format("��ǰ����:%s", GLOBALDATA->CurDynamicConfig.strTime);
        GetDlgItem(IDC_STATIC_TASKINFO)->SetWindowText(strInfo);
        ResetConfig();
        return true;
    }
    
    return false;
}

int	CAuto_AIDlg::FindNextLogonIndex()
{
    static int s_index = -1;
    if(NextTask())
    {
        s_index = -1;
    }
    int i = s_index + 1;
    if(i >= GLOBALDATA->CurDynamicConfig.nLogonCount) i = 0;

	for (; i < GLOBALDATA->CurDynamicConfig.nLogonCount; i++)
	{
		if (GLOBALDATA->Robots[i].pRoomItem == NULL)
		{
            s_index = i;
            ZeroMemory(m_CurPlaceUserInfo.szName, sizeof(m_CurPlaceUserInfo.szName));
            sprintf_s(m_CurPlaceUserInfo.szName, "%s%d", GLOBALDATA->CurDynamicConfig.strRobotPreName, i + GLOBALDATA->CurDynamicConfig.nBeginSequenceNo);
			return i;
		}
	}
    if( i == GLOBALDATA->CurDynamicConfig.nLogonCount)
    {
        for (i = 0; i <= s_index; i++)
        {
            if (GLOBALDATA->Robots[i].pRoomItem == NULL)
            {
                s_index = i;
                ZeroMemory(m_CurPlaceUserInfo.szName, sizeof(m_CurPlaceUserInfo.szName));
                sprintf_s(m_CurPlaceUserInfo.szName, "%s%d", GLOBALDATA->CurDynamicConfig.strRobotPreName, i + GLOBALDATA->CurDynamicConfig.nBeginSequenceNo);
                return i;
            }
        }
    }

	return -1;
}

//��������
void  CAuto_AIDlg::SaveConfig()
{
    CString strTmp;
    bool bAddUser = false;
    DynamicConfig conf;
    GetDlgItem(IDC_EDIT_LOGINCOUNT)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��½�������ô���");
        return;
    }
    if(conf.nLogonCount < atoi(strTmp) ) bAddUser = true;
    conf.nLogonCount = atoi(strTmp);
    GetDlgItem(IDC_EDIT_HEADNAME)->GetWindowText(strTmp);
    conf.strRobotPreName = strTmp;
    GetDlgItem(IDC_EDIT_BEGINNO)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��ʼ�������ô���");
        return;
    }
    conf.nBeginSequenceNo = atoi(strTmp);
    GetDlgItem(IDC_EDIT_CHECKOUT)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("ȡǮ��ֵ���ô���");
        return;
    }
    conf.nCheckOutMinMoney = _atoi64(strTmp);
    GetDlgItem(IDC_EDIT_CHECKIN)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��Ǯ��ֵ���ô���");
        return;
    }
    conf.nCheckInMaxMoney = _atoi64(strTmp);

    GetDlgItem(IDC_EDIT_ONEDESKROBOTNO)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("ÿ�������˸������ô���");
        return;
    }
    conf.bMachineDeskCount = atoi(strTmp);
    GetDlgItem(IDC_EDIT_TIMEFISHGAME)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("������Ϸ����������ô���");
        return;
    }
    conf.nFishGameTimeLeave = atoi(strTmp);
    GetDlgItem(IDC_EDIT_KEEPINDESK)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("�ȴ���Ϸ��ʼʱ�����ô���");
        return;
    }
    conf.nKeepInDeskSeconds = atoi(strTmp);
    GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��Ϸ�����뿪�������ô���");
        return;
    }
    conf.nGameEndLeaveDesk = atoi(strTmp);
    if(((CButton*)GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER))->GetCheck() == 1)
    {
        conf.bMachineAndPlayer = true;
    }
    else
    {
        conf.bMachineAndPlayer = false;
    }
    CString errorMsg;
    if(!conf.IsValid(errorMsg))
    {
        MessageBox(errorMsg);
        return;
    }
    conf.strTime = GLOBALDATA->CurDynamicConfig.strTime;
    GLOBALDATA->CurDynamicConfig = conf;
    {
        AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
        auto it = PLATCONFIG->m_DynamicConfigs.find(GLOBALDATA->CurDynamicConfig.strTime.GetBuffer());
        if(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            it->second = GLOBALDATA->CurDynamicConfig;
        }
    }

    PLATCONFIG->Save();

    if(bAddUser && GLOBALDATA->bConnectToWitchServer == 1)
    {// �����û���������½
        OnConnectToLogonServer();
    }
}
//�ָ��޸�
void  CAuto_AIDlg::ResetConfig()
{
    if(GLOBALDATA->CurDynamicConfig.nLogonCount == -1)
    {
        CString strTmp = "";
        GetDlgItem(IDC_STATIC_TASKINFO)->SetWindowText("��ǰ����");
        GetDlgItem(IDC_EDIT_LOGINCOUNT)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_HEADNAME)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_BEGINNO)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_CHECKOUT)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_CHECKIN)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_ONEDESKROBOTNO)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_TIMEFISHGAME)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_KEEPINDESK)->SetWindowText(strTmp);
        GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->SetWindowText(strTmp);
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER))->SetCheck(0);
        return;
    }

    CString strTmp;
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nLogonCount);
    GetDlgItem(IDC_EDIT_LOGINCOUNT)->SetWindowText(strTmp);
    strTmp.Format(_T("%s"),GLOBALDATA->CurDynamicConfig.strRobotPreName);
    GetDlgItem(IDC_EDIT_HEADNAME)->SetWindowText(strTmp);
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nBeginSequenceNo);
    GetDlgItem(IDC_EDIT_BEGINNO)->SetWindowText(strTmp);
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney);
    GetDlgItem(IDC_EDIT_CHECKOUT)->SetWindowText(strTmp);
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney);
    GetDlgItem(IDC_EDIT_CHECKIN)->SetWindowText(strTmp);
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.bMachineDeskCount);
    GetDlgItem(IDC_EDIT_ONEDESKROBOTNO)->SetWindowText(strTmp);
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nFishGameTimeLeave);
    GetDlgItem(IDC_EDIT_TIMEFISHGAME)->SetWindowText(strTmp);
    if(!PLATCONFIG->bFishGame)
    {
        GetDlgItem(IDC_EDIT_TIMEFISHGAME)->EnableWindow(FALSE);
    }
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nKeepInDeskSeconds);
    GetDlgItem(IDC_EDIT_KEEPINDESK)->SetWindowText(strTmp);
    if(PLATCONFIG->bFishGame)
    {
        GetDlgItem(IDC_EDIT_KEEPINDESK)->EnableWindow(FALSE);
    }
    strTmp.Format(_T("%d"),GLOBALDATA->CurDynamicConfig.nGameEndLeaveDesk);
    GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->SetWindowText(strTmp);
    if(PLATCONFIG->bFishGame)
    {
        GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->EnableWindow(FALSE);
    }
    if(GLOBALDATA->CurDynamicConfig.bMachineAndPlayer)
    {
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER))->SetCheck(1);
    }
    else
    {
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER))->SetCheck(0);
    }
}

bool CAuto_AIDlg::ModifyPara()
{
    if(GLOBALDATA->CurDynamicConfig.nLogonCount == -1) return false;
    bool bModify = false;
    /////////////
    CString strTmp;
    GetDlgItem(IDC_EDIT_LOGINCOUNT)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nLogonCount != atoi(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_LOGINCOUNT)->Invalidate();

    /////////////
    GetDlgItem(IDC_EDIT_HEADNAME)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.strRobotPreName != strTmp)
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_HEADNAME)->Invalidate();

    ////////////////
    GetDlgItem(IDC_EDIT_BEGINNO)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nBeginSequenceNo != atoi(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_BEGINNO)->Invalidate();

    //////////////
    GetDlgItem(IDC_EDIT_CHECKOUT)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney != _atoi64(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_CHECKOUT)->Invalidate();

    //////////////
    GetDlgItem(IDC_EDIT_CHECKIN)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney != _atoi64(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_CHECKIN)->Invalidate();

    //////////////
    GetDlgItem(IDC_EDIT_ONEDESKROBOTNO)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.bMachineDeskCount != atoi(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_ONDESKROBOTNO)->Invalidate();

    /////////////////
    GetDlgItem(IDC_EDIT_KEEPINDESK)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nKeepInDeskSeconds != atoi(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_KEEPINDESK)->Invalidate();

    /////////////////
    GetDlgItem(IDC_EDIT_TIMEFISHGAME)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nFishGameTimeLeave != atoi(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_TIMEFISHGAME)->Invalidate();

    /////////////////
    GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->GetWindowText(strTmp);
    if(GLOBALDATA->CurDynamicConfig.nGameEndLeaveDesk != atoi(strTmp))
    {
        bModify = true;
    }
    GetDlgItem(IDC_STATIC_GAMEENDLEAVE)->Invalidate();

    //////////////////
    bool b;
    if(((CButton*)GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER))->GetCheck() == 1)
    {
        b = true;
    }
    else
    {
        b = false;
    }
    if(b != GLOBALDATA->CurDynamicConfig.bMachineAndPlayer)
    {
        bModify = true;
    }
    GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER)->Invalidate();

    return bModify;
}

void CAuto_AIDlg::InitUI(bool bEnable)
{
    GetDlgItem(IDC_EDIT_HEADNAME)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_BEGINNO)->EnableWindow(bEnable);
    if(PLATCONFIG->bFishGame) GetDlgItem(IDC_EDIT_TIMEFISHGAME)->EnableWindow(bEnable);
}

void CAuto_AIDlg::UIQueueGame(bool bEnable)
{
    GetDlgItem(IDC_EDIT_ONEDESKROBOTNO)->EnableWindow(bEnable);
    if(PLATCONFIG->bFishGame)   GetDlgItem(IDC_EDIT_TIMEFISHGAME)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_KEEPINDESK)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER)->EnableWindow(bEnable);
}

void CAuto_AIDlg::OnBnClickedButtonLogin()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
	GLOBALDATA->bLoginEnable = true;
    OnStartRobot();
}


void CAuto_AIDlg::OnBnClickedButtonCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
	GLOBALDATA->bLoginEnable = false;
    OnStopRobot();
}


void CAuto_AIDlg::OnBnClickedButtonClose()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    OnCancel();
}


void CAuto_AIDlg::OnBnClickedButtonMin()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ::SendMessage(GetSafeHwnd(),WM_SYSCOMMAND,SC_MINIMIZE,0);
}


void CAuto_AIDlg::OnBnClickedButtonSave()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SaveConfig();
}


void CAuto_AIDlg::OnBnClickedButtonReset()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ResetConfig();
}

void CAuto_AIDlg::OnLButtonDown(UINT nFlags,CPoint point)
{
    PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}


HBRUSH CAuto_AIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CBaseRoom::OnCtlColor(pDC, pWnd, nCtlColor);
    if(GLOBALDATA->CurDynamicConfig.nLogonCount < 0) return hbr;
    CString strTmp;
    bool b;
    switch(pWnd->GetDlgCtrlID())
    {
    case IDC_STATIC_LOGINCOUNT:
        GetDlgItem(IDC_EDIT_LOGINCOUNT)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nLogonCount != atoi(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_HEADNAME:
        GetDlgItem(IDC_EDIT_HEADNAME)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.strRobotPreName != strTmp)
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_BEGINNO:
        GetDlgItem(IDC_EDIT_BEGINNO)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nBeginSequenceNo != atoi(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_CHECKOUT:
        GetDlgItem(IDC_EDIT_CHECKOUT)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nCheckOutMinMoney != _atoi64(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_CHECKIN:
        GetDlgItem(IDC_EDIT_CHECKIN)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nCheckInMaxMoney != _atoi64(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_ONDESKROBOTNO:
        GetDlgItem(IDC_EDIT_ONEDESKROBOTNO)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.bMachineDeskCount != atoi(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_TIMEFISHGAME:
        GetDlgItem(IDC_EDIT_TIMEFISHGAME)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nFishGameTimeLeave != atoi(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_KEEPINDESK:
        GetDlgItem(IDC_EDIT_KEEPINDESK)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nKeepInDeskSeconds != atoi(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_STATIC_GAMEENDLEAVE:
        GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->GetWindowText(strTmp);
        if(GLOBALDATA->CurDynamicConfig.nGameEndLeaveDesk != atoi(strTmp))
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    case IDC_CHECK_ALLOWROBOTWITHPLAYER:
        if(((CButton*)GetDlgItem(IDC_CHECK_ALLOWROBOTWITHPLAYER))->GetCheck() == 1)
        {
            b = true;
        }
        else
        {
            b = false;
        }
        if(b != GLOBALDATA->CurDynamicConfig.bMachineAndPlayer)
        {
            pDC->SetTextColor(RGB(255,0,0));
        }
        break;
    default:
        break;
    }

    // TODO:  �ڴ˸��� DC ���κ�����

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}



void CAuto_AIDlg::OnBnClickedButtonTasklist()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDiaogTaskList taskDlg;
    taskDlg.DoModal();
    if(GLOBALDATA->bConnectToWitchServer != 1)
    {
        if(GLOBALDATA->CurDynamicConfig.nLogonCount != -1)
        {
            {
                AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
                auto it = PLATCONFIG->m_DynamicConfigs.find(GLOBALDATA->CurDynamicConfig.strTime.GetBuffer());
                if(it != PLATCONFIG->m_DynamicConfigs.end())
                {
                    GLOBALDATA->CurDynamicConfig = it->second;
                }
                else
                {
                    GLOBALDATA->CurDynamicConfig.nLogonCount = -1;
                }
            }
            NextTask();
            ResetConfig();
        }
        else
        {
            if(NextTask())
            {
                ResetConfig();
            }
        }
        
    }
}


void CAuto_AIDlg::OnBnClickedButtonHallinfo()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogMsgInfo dlg(CDialogMsgInfo::Hall_Msg, this);
    dlg.DoModal();
}


void CAuto_AIDlg::OnBnClickedButtonRoominfo()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogMsgInfo dlg(CDialogMsgInfo::Room_Msg, this);
    dlg.DoModal();
}

void CAuto_AIDlg::ShowUserStatus()
{
    std::map<string, UserItemUIInfo> usrInfos;
    {
        AutoPlatLock lock(&UIDATA->uiSection);
        usrInfos = UIDATA->usrInfos;
    }
    m_ListUserStatus.SetRedraw(FALSE); // ��ֹ��˸ 
    m_ListUserStatus.DeleteAllItems();
    auto it = usrInfos.begin();
    int nRow = 0;
    while(it != usrInfos.end())
    {
        if(m_ShowBeginNo > CString(it->second.strUserName.c_str()))
        {
            it++;
            continue;
        }
        if(m_ShowEndNo < CString(it->second.strUserName.c_str()))
        {
            it++;
            continue;
        }
        CString strTmp;
        m_ListUserStatus.InsertItem(nRow, "USERINFO");
        m_ListUserStatus.SetItemData(nRow,nRow);
        m_ListUserStatus.SetItemText(nRow, COL_USRNAME, it->first.c_str());
        m_ListUserStatus.SetItemText(nRow, COL_NICKNAME, it->second.strNickName.c_str());
        switch(it->second.GLoginStatus)
        {
        case 0:
            strTmp.Format(_T("δ��½(%d)"),it->second.GLoginStatus);
            break;
        case 1:
            strTmp.Format(_T("���ڵ�½(%d)"),it->second.GLoginStatus);
            break;
        case 2:
            strTmp.Format(_T("�ѵ�½(%d)"),it->second.GLoginStatus);
            break;
        case 3:
            strTmp.Format(_T("��½ʧ��(%d)"),it->second.GLoginStatus);
            break;
        default:
            strTmp.Format(_T("δ֪(%d)"),it->second.GLoginStatus);
            break;
        }
        m_ListUserStatus.SetItemText(nRow, COL_GSERVERLOGIN, strTmp);
        strTmp.Format("%d", it->second.roomID);
        m_ListUserStatus.SetItemText(nRow, COL_ROOMID, strTmp);
        strTmp.Format("%d", it->second.bDeskNo);
        m_ListUserStatus.SetItemText(nRow, COL_DESKNO, strTmp);
        strTmp.Format("%d", it->second.bDeskStation);
        m_ListUserStatus.SetItemText(nRow, COL_DESKSTATION, strTmp);
        switch(it->second.bUserState)
        {
        case USER_NO_STATE:
            strTmp.Format(_T("δ֪(%d)"), it->second.bUserState);
            break;
        case USER_SITTING:
            strTmp.Format(_T("����(%d)"), it->second.bUserState);
            break;
        case USER_ARGEE:
            strTmp.Format(_T("׼��(%d)"), it->second.bUserState);
            break;
        case USER_CUT_GAME:
            strTmp.Format(_T("����(%d)"), it->second.bUserState);
            break;
        case USER_PLAY_GAME:
            strTmp.Format(_T("��Ϸ��(%d)"), it->second.bUserState);
            break;
        default:
            strTmp.Format(_T("δ֪(%d)"), it->second.bUserState);
            break;
        }
        m_ListUserStatus.SetItemText(nRow, COL_USERSTATE, strTmp);
        strTmp.Format("����ʱ��:%s,��ȡ��ֵ:%lld-%lld,��������:%d,�ȴ���ʼ:%d,��������ͬ��:%d,ÿ�������˸���:%d,����վ��:%d",it->second.cof.strTime
            ,it->second.cof.nCheckOutMinMoney,it->second.cof.nCheckInMaxMoney
            ,it->second.cof.nFishGameTimeLeave,it->second.cof.nKeepInDeskSeconds
            ,it->second.cof.bMachineAndPlayer,it->second.cof.bMachineDeskCount
            ,it->second.cof.nGameEndLeaveDesk);
        m_ListUserStatus.SetItemText(nRow, COL_TASKINFO, strTmp);
        nRow++;
        it++;
    }
    m_ListUserStatus.SetRedraw(TRUE); 
    SetTimer(TIMER_SORT_USERSTATUS, 10, NULL);
}
int CAuto_AIDlg::CompareProc(LPARAM p1, LPARAM p2, LPARAM p3)
{
    CAuto_AIDlg *pDialog = (CAuto_AIDlg*)p3;
    if(pDialog != NULL)
        return pDialog->CompareFunc(p1, p2);
    return 0;
}

int CAuto_AIDlg::CompareFunc(LPARAM p1, LPARAM p2)
{
    if(m_sort_column == -1) return 0;
    int row1 = (int)p1;
    int row2 =(int)p2;
    CString lp1 = m_ListUserStatus.GetItemText(row1,m_sort_column);
    CString lp2 = m_ListUserStatus.GetItemText(row2,m_sort_column);

    if(lp1 == lp2) return 0;
    if(lp1 < lp2)
    {
        if(!m_bDescSort) return -1;
        return 1;
    }
    if(!m_bDescSort) return 1;
    return -1;
}

void CAuto_AIDlg::OnLvnColumnclickListStatus(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    *pResult = 0;

    m_bDescSort = !m_bDescSort;

    m_sort_column = pNMLV->iSubItem;  

    SetTimer(TIMER_SORT_USERSTATUS, 3000, NULL);
    *pResult =0;
}


void CAuto_AIDlg::OnBnClickedButtonAddtask()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ((CEdit*)GetDlgItem(IDC_EDIT_CHECKBEGINNO))->GetWindowText(m_ShowBeginNo);
    ((CEdit*)GetDlgItem(IDC_EDIT_CHECKENDNO))->GetWindowText(m_ShowEndNo);
}
