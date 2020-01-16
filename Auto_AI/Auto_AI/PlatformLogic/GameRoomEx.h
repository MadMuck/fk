#pragma once
#ifndef AFC_GAMEROOMEX_HEAD_FILE
#define AFC_GAMEROOMEX_HEAD_FILE
#include "resource.h"
#include "Stdafx.h"

#include "BaseRoom.h"
#include "clientcomstruct.h"
#include "gameusermanage.h"
#include "gameroommessage.h"
#include "gameframedlg.h"
#include "map"
#include "string"
#include "Platconfig.h"

//�ṹ˵��
struct RoomInfoStruct;

// ��Ϣ�¼�
#define IDM_SET_GAME_TIME				WM_USER+100
#define IDM_KILL_GAME_TIME				WM_USER+101
#define IDM_CLOSE                       WM_USER+102  //20090211
#define IDM_STAND_UP					WM_USER+103 // �ṩ����Ϸ����֧��վ��

// ��ʱ����Ϣ
#define TIMER_CONNECT_TO_GSERVER        1 // ����GServer��ʱ��
#define TIMER_FISHGAME_AIBANK 2         // ��������Ϸ��ұ仯���
#define TIMER_FISHGAME_LEAVE 3          // ����������ʱ����������ֹһֱռ������
#define TIMER_CHECK_USERSTATE 4         // �û���״̬���,��ֹ��ʱ��ռ��
#define TIMER_FISHGAME_WITHTRUEPAYER  5 // �������º�,������������������������
#define TIMER_STAND_UP 6

//��Ϸ������
class CGameRoomEx : public CBaseRoom
{
	//��Ϣ����
protected:
	HINSTANCE						m_hGameInstance;					//��Ϸ����
protected:
	
    bool                            m_bInitContest;                     //�����Ƿ��ʼ��
	CGameUserManage					m_UserManage;						//�û�����
	
	GameInfoStruct					m_GameInfo;							//��Ϸ��Ϣ
	IFrameInterface					* m_IGameFrame;						//��Ϸ���
	RoomInfoStruct                  * m_pRoomInfo;                      //������Ӧ�ķ�����Ϣ

	MSG_GP_R_LogonResult			m_MServerLogonInfo;				    //������½��Ϣ

    DynamicConfig                   m_CurDynamicConfig;

    std::map<CString,int>           m_notHandleMsgs;

//��½����
	//��������
public:
	//���캯��
	CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo, DynamicConfig cof); 
	//��������
	virtual ~CGameRoomEx();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ӿں��� 
public:
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();

    // ����������Ϣ
protected:
    // ���ӷ���
    void ConnectToGServer();
    // ��½����
    bool LoginGServer();
    //����
    bool SitDesk();
    //����
    bool StandingDesk();
    //���Ϳ�����Ϣ 
    inline UINT ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize);

	//����Ӧ����Ϣ
protected:
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�����½����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û��б���
	bool OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û���������
	bool OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ����
	bool OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

public:
	//�رշ���
	void CloseGameRoom();
    //��Ѳ�����λ
    bool OnFindBestDesk(BYTE &bDeskIndex, BYTE &bDeskStation);
	//������Ϸ�߼�
	bool StartGameClient();

    //��Ϣ�¼�����
public:
    LRESULT OnStandingDesk(WPARAM,LPARAM);
	//��ʱ����Ϣ
	LRESULT SetGameTime(WPARAM,LPARAM);
	//��ʱ����Ϣ
	LRESULT KillGameTime(WPARAM,LPARAM);
    //�ر���Ϸ
    LRESULT OnCloseGame(WPARAM wparam,LPARAM lparam);//20090211
	//��ʱ��
	void OnTimer(UINT_PTR uIDEvent);

private:

    bool CheckNeedMoneyOp();

    // ����GServer
    void DoConnectToGServer();

	//ͳ��ĳ������������
	BYTE CountMachine(BYTE bDeskIndex);
	//ͳ��ĳ����������
	BYTE CountTruePlayer(BYTE bDeskIndex);
};
#endif