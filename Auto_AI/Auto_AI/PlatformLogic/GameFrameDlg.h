#pragma once

#ifndef CGAMEFRAMEDLG_HEAD_FILE
#define CGAMEFRAMEDLG_HEAD_FILE


/*******************************************************************************************************/

//������Ϣ����
#define CM_USER_STATE					1						//״̬�ı�
#define IDM_GETPOINT						WM_USER + 430				//����
#define IDM_GET_ROOMNAME					WM_USER+435					//�õ���������

//��������
#define ACT_USER_UP						1						//�û�����
#define ACT_WATCH_UP					2						//�Թ�����
#define ACT_USER_SIT					3						//�û�����
#define ACT_WATCH_SIT					4						//�Թ�����
#define ACT_USER_CUT					5						//�û�����
#define ACT_USER_AGREE					6						//�û�ͬ��
#define ACT_GAME_BEGIN					7						//��Ϸ��ʼ
#define ACT_GAME_END					8						//��Ϸ����
#define ACT_USER_POINT					9						//�û�����ֵ

//�û�״̬�ı�
struct CM_UserState_Change
{
	BYTE								bActionCode;			//��������
	BYTE								bDeskStation;			//����λ��
	UserItemStruct						* pUserItem;			//�û�ָ��
};

/*******************************************************************************************************/

//��Ϸ��ܽӿ�
interface IFrameInterface
{
	//��������
public:
	//��ʼ�����״�����ʹ�ã�
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom)=0;
	virtual ~IFrameInterface() {}
	//��������
	virtual bool AFCCreateFrame()=0;
	//�رմ���
	virtual bool AFCCloseFrame()=0;
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)=0;
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)=0;
	//��Ϸ��ʱ��
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount) = 0;
};

/*******************************************************************************************************/



/*******************************************************************************************************/

//�����û��ӿ�
interface IFindUserInterface
{
	//��������
public:
	//�����û�
	virtual UserItemStruct * FindOnLineUser(long int dwUserID)=0;
	////�����û�
	//virtual UserItemStruct * FindOnLineUser(int dwAccID)=0;
	//�����û�
	virtual INT_PTR FindOnLineUser(const TCHAR * szUserName, CPtrArray & ResultPtrArray)=0;
	//�����û�
	virtual INT_PTR FindOnLineUser(BYTE bDeskIndex, CPtrArray & PlayPtrArray, CPtrArray & WatchPtrArray)=0;
};

/*******************************************************************************************************/

//��Ϸ������� 
class EXT_CLASS CLoveSendClass/*C-GameFrameDlg*/ : public CDialog, public IFrameInterface
{
	//��������
protected:
	CWnd								* m_pGameRoom;					//���䴰��

	GameInfoStruct						* m_pGameInfo;					//��Ϸ��Ϣָ��
	UserItemStruct						* m_pUserInfo[MAX_PEOPLE];		//��Ϸ�����Ϣ
	UINT								m_uTimeID;						//��ʱ�� ID
	UINT								m_uTimeCount;					//��ʱ������
	BYTE								m_bTimeStation;					//������λ��
	TCHAR								m_ErrorMsg[MAX_PATH];			//������־
	//��������
protected:
	//���캯��
	CLoveSendClass(UINT uTemplate);
	//��������
	virtual ~CLoveSendClass();
	enum			{ IDD = IDD_GAME_FRAME };		     	//ID ����
	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ż��󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
protected:
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize)=0;
	//�������ý���
	virtual void ResetGameFrame();
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation=1){};																									  

	//��Ϸ���ƺ���
protected:
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin()=0;

	//���ܺ���
public:
	void SetInifile(LPCTSTR path);
	//��ȡ�Լ���Ϣ
	UserInfoStruct * GetMeUserInfo() { return &m_pGameInfo->pMeUserInfo->GameUserInfo; };
	//����״̬����
	void SetStationParameter(BYTE bGameStation) { m_pGameInfo->bGameStation=bGameStation; };
	//��ȡ״̬����
	BYTE GetStationParameter() { return m_pGameInfo->bGameStation; };
	//����������Ϣ����
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	//��ܺ���
	virtual bool OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	//�ӿں���������Ҫ���أ�
private:
	//��ʼ��
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom);
	//��������
	virtual bool AFCCreateFrame();
public:
	//�رմ���
	virtual bool AFCCloseFrame();
private:
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
public:
	//��ʱ��
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID);
	//ɱ��ʱ��
	virtual bool KillGameTimer(UINT uTimeID);
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
/*******************************************************************************************************/

#endif
