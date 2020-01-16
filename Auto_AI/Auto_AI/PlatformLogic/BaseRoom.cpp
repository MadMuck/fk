///��������
#include "StdAfx.h"
#include "BaseRoom.h"

BEGIN_MESSAGE_MAP(CBaseRoom, CDialog)
//	ON_WM_SIZE()
END_MESSAGE_MAP()


//��������ʱ�� ID
#define ID_PRO_TIME					100			
/*******************************************************************************************************/


/*******************************************************************************************************/

//���캯��
CBaseRoom::CBaseRoom(UINT uTemplate) :CDialog(uTemplate)
{
	m_TCPSocket = new CTCPClientSocket(this);
}

//��������
CBaseRoom::~CBaseRoom()
{
	if (m_TCPSocket != NULL)
		delete m_TCPSocket;
}

//��ʼ������
BOOL CBaseRoom::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}


//����������Ϣ����
int CBaseRoom::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(bMainID,bAssistantID,bHandleCode);//����������
}

//����������Ϣ����
int CBaseRoom::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}

/*******************************************************************************************************/

