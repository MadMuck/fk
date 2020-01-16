
#include "StdAfx.h"
#include "resource.h"
#include "GameFrameDlg.h"
#include <SYS/Stat.h>

BEGIN_MESSAGE_MAP(CLoveSendClass, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


//013���캯��
CLoveSendClass::CLoveSendClass(UINT uTemplate):CDialog(uTemplate)
{
	m_pGameRoom = NULL;

	memset(m_pUserInfo,0,sizeof(m_pUserInfo));
	return;
}

//��������
CLoveSendClass::~CLoveSendClass()
{

}


//008�ż��󶨺���
void CLoveSendClass::DoDataExchange(CDataExchange * pDX)
{
	CDialog::DoDataExchange(pDX);

}

//007��ʼ������
BOOL CLoveSendClass::OnInitDialog()
{
	__super::OnInitDialog();

	
	AfxMessageBox("11111111");

	return false;
}
//��������
bool CLoveSendClass::AFCCreateFrame()
{
	//��������

	Create(IDD_GAME_FRAME,m_pGameRoom);
	//��ȡ��ǰ������Ϣ
	ResetGameStation();
	return true;
}

//�رմ���
bool CLoveSendClass::AFCCloseFrame()
{
	//���ò���
	SetStationParameter(0);

	if (GetSafeHwnd()!=NULL) 
	{
		ResetGameFrame();
		this->DestroyWindow();
	}
	return true;
}
//��ʼ��
bool CLoveSendClass::InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom)
{
	m_pGameInfo=pGameInfo;
	m_pGameRoom = pGameRoom;
	return true;
}


//������Ϣ 
UINT CLoveSendClass::OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	//��������
	switch (uControlCode)
	{
	case CM_USER_STATE:		//�û�״̬�ı�
		{
			//Ч������
			if (uDataSize!=sizeof(CM_UserState_Change)) return 1;
			CM_UserState_Change * pStateChange=(CM_UserState_Change *)pControlData;

			//��������
			switch (pStateChange->bActionCode)
			{
			case ACT_USER_UP:		//�û�����
				//case ACT_WATCH_UP:		//�Թ�����
				{
					return 0;
				}
			case ACT_USER_SIT:		//�û�����
				//	case ACT_WATCH_SIT:		//�Թ�����
				if (pStateChange->pUserItem!=m_pGameInfo->pMeUserInfo)   //���˽���
				{
					//GameUserCome(pStateChange->bDeskStation,pStateChange->pUserItem,pStateChange->bActionCode==ACT_WATCH_SIT);
					m_pUserInfo[pStateChange->bDeskStation] = pStateChange->pUserItem;

				}
				else   //�Լ�����
				{
					BYTE bMeDeskStation=m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO;
					if (bMeDeskStation==255)
						return 0;

					//��ȡ�û�����
					CPtrArray PlayUser,WatchUser;
					m_pGameInfo->pIFindUser->FindOnLineUser(bMeDeskStation,PlayUser,WatchUser);

					for (INT_PTR i=0;i<PlayUser.GetCount();i++)
					{
						UserItemStruct * pUserItem=(UserItemStruct *)PlayUser.GetAt(i);
						if (pUserItem!=NULL) 
							m_pUserInfo[pUserItem->GameUserInfo.bDeskStation] = pUserItem;
						//GameUserCome(pUserItem->GameUserInfo.bDeskStation,pUserItem,false);*/
					}

					//���ͻ�ȡ��Ϣ
					MSG_GM_S_ClientInfo ClientInfo;
					::memset(&ClientInfo,0,sizeof(MSG_GM_S_ClientInfo));
					ClientInfo.bEnableWatch = 0;
					SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
					return 0;
				}
			case ACT_USER_CUT:		//�û�����
				{
					return 0;
				}
			case ACT_USER_AGREE:	//�û�ͬ��
				{
					return 0;
				}
			case ACT_GAME_BEGIN:	//��Ϸ��ʼ
			case ACT_GAME_END:		//��Ϸ����
				{
					return 0;
				}
			case ACT_USER_POINT:	//�û�����ֵ
				{
					return 0;
				}
			}
		}
	}

	return 0;
}


//�����ȡ��Ϣ
bool CLoveSendClass::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//���˴���
	if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255) 
		return true;
	//������Ϣ
	switch (pNetHead->bMainID)
	{
	case MDM_GM_GAME_FRAME:		//�����Ϣ
		{
			return OnFrameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GM_MESSAGE:		//��Ϣ��Ϣ
		{
			break;//return OnSystemMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GM_GAME_NOTIFY:	//��Ϸ��Ϣ
		{
			return HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
		
	//case MDM_GR_POINT:
	//	{
	//		break;;//return HandlePointMessage(pNetHead,pNetData,uDataSize,pClientSocket);
	//	}
	}
	return true;
}


//�����Ϣ������������Ҫ���أ�
bool CLoveSendClass::OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_GAME_INFO:		//��Ϸ��Ϣ
		{
			//Ч������
			MSG_GM_S_GameInfo * pGameInfo=(MSG_GM_S_GameInfo *)pNetData;

			SetStationParameter(pGameInfo->bGameStation);
			return true;
		}
	case ASS_GM_GAME_STATION:	//��Ϸ״̬
		{	
			return SetGameStation(pNetData,uDataSize);//����DLL�еĺ��������齫��uDataSizeΪ51
		}
	case ASS_GM_NORMAL_TALK:	//��ͨ����
		{
			return true;
		}
	case ASS_GM_WATCH_SET:		//�Թ�����
		{
			return true;
		}
	case ASS_GM_SET_VIDEOADDR:
		{
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CLoveSendClass::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID == MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{	
		case 2:				//����
			{
				return true;
			}
		}
		
	}
	return true;
}


//�������ý���
void CLoveSendClass::ResetGameFrame()
{
	m_pGameInfo->bGameStation=0;
	memset(m_pUserInfo,0,sizeof(m_pUserInfo));
	return;
}

//���Ż��㵽��ͼλ��
BYTE CLoveSendClass::ViewStation(BYTE bDeskStation)
{
	return bDeskStation;
}

//014���ͺ���
int CLoveSendClass::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255) 
		return 0;
	return m_pGameInfo->pISocketSend->SendGameData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}

//���ͺ���
int CLoveSendClass::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255)
		return 0;
	return m_pGameInfo->pISocketSend->SendGameData(bMainID,bAssistantID,bHandleCode);
}

//��ʱ��
bool CLoveSendClass::SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID)
{
	if(m_pGameRoom != NULL)
		m_pGameRoom->SendMessage(IDM_SET_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeCount*1000);
//	OnGameTimer(bDeskStation,uTimeID,uTimeCount);
	return true;
}

//ɱ��ʱ��
bool  CLoveSendClass::KillGameTimer(UINT uTimeID)
{
	if(m_pGameRoom != NULL)
		m_pGameRoom->SendMessage(IDM_KILL_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeID);
	return true;
}

void CLoveSendClass::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	__super::OnTimer(nIDEvent);
}
