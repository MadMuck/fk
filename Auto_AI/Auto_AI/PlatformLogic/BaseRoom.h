#pragma once

#include "TCPClientSocket.h"

/*******************************************************************************************************/

/*******************************************************************************************************/

//���������ӿ�
interface IBaseGameRoom
{
	//��������
public:
	//ѯ�ʹر�
	virtual bool CanCloseNow()=0;
	//�رմ���
	virtual bool CloseGameRoom()=0;
	//ȡ��������Ϣ
	virtual bool OnCancelConnectEvent()=0;
	//λ�ñ仯
	virtual bool OnSizeEvent(UINT nSizeType, int cx, int cy)=0;
	//�������Ϣ
	virtual bool OnSplitEvent(UINT uSplitID, UINT uXPos, UINT uYPos)=0;
	//����λ��
	virtual void FixControlStation(int iWidth, int iHeight)=0;
};
//������Ϣ���ͽӿ�
interface ISocketSendInterface
{
	//��������
public:
	//����������Ϣ����
	virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
	//����������Ϣ����
	virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
};
/*******************************************************************************************************/

//��������� 
class CBaseRoom  : public CDialog , public IClientSocketService, public ISocketSendInterface//, public IBaseGameRoom
{
	//��������
public:
	CTCPClientSocket*							m_TCPSocket;					//���� SOCKET
	TCHAR										m_ErrorMsg[MAX_PATH];

	//��������
public:
	//���캯��
	CBaseRoom(UINT uTemplate); 
	//��������
	virtual ~CBaseRoom();

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel() { return; }

	//��Ϣ����
public:
	//�ӿں���
protected:
	//����������Ϣ����
	inline virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	inline virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	DECLARE_MESSAGE_MAP()
};

/*******************************************************************************************************/
