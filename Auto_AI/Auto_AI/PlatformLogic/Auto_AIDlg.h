// Auto_AIDlg.h : ͷ�ļ�
//
#include "stdafx.h"
#include "resource.h"

#include "Platconfig.h"


#include <vector>
#include <functional>
#include <algorithm>
#include "afxwin.h"
#include "afxcmn.h"
using namespace std;

#pragma once

// ��ʱ��ID
#define				 TIME_CONNECT_ASERVER		1			    //����AServer�¼�
#define				 TIME_CONNECT_MSERVER		2			    //����MServer�¼�
#define              TIMER_CHECK_UNCONNECT_ACCOUNT 5            //��ʱ���δ���ӵ��˺�
#define              TIMER_CHECK_MODIFY_PARA    6               //����Ƿ����޸Ķ�̬����
#define              TIMER_UPDATE_USERSTATUS 7 // �����û�״̬��Ϣ
#define              TIMER_SORT_USERSTATUS 8 // ����
#define              TIMER_PLC_CLOSE_SOCKET 10

// �Զ�����Ϣ
#define				 IDM_CLOSE_GAME_ROOM		WM_USER+200		//�رշ���


/*-----------------------------------------------------------------------------------------------*/
// CAuto_AIDlg �Ի���
class CAuto_AIDlg : public CBaseRoom
{
	// ����
public:
	//���캯��
	CAuto_AIDlg(CWnd * pParent=NULL);
	~CAuto_AIDlg();

	// �Ի�������
	enum { IDD = IDD_AUTO_AI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
public:
	HICON									m_hIcon;
    bool                                    m_bExit;
    MSG_GP_R_LogonResult					m_CurPlaceUserInfo;	//�û�����
    volatile int                            m_iLogonIndex; // ��ǰ��½����,GLOBALDATA ��Robots����
    std::map<CString,int>                   m_notHandleMsgs;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();

    DECLARE_MESSAGE_MAP()
    // ����OnOK��OnCancel
    virtual void OnOK() override;
    virtual void OnCancel() override;
	//ȷ������,������½
	virtual void OnStartRobot();
	//ȡ������,�˳�������
	virtual void OnStopRobot();
    //��������
    void SaveConfig();
    //�ָ��޸�
    void ResetConfig();
public:
    // ����
    //����AServer
    void OnConnectToCenterServer();
    //��½MServer
    void OnConnectToLogonServer();
    // ��½MServer
    void LoginMServer();
    // ���뷿��
    void EnterGameRoom(int roomid);
    //��ȡ��Ϸ�б�
    bool GetGameRoomList();
    //���������ȡ��������ID	
    void GetContestRoomID(int roomid);
public:
    // Ӧ��
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��½��Ϣ����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�б���Ϣ
	bool OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������˷��͵ı�������ID
	bool EnterContestRoom(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�رշ�������
	LRESULT OnCloseGameRoom(WPARAM wparam, LPARAM lparam);

public:
	//��ʱ��
	void OnTimer(UINT_PTR nIDEvent);

	//�ӿں���
public:
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();
protected:
	//����������Ϣ����
	inline virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	inline virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

private:
    // ִ������AServer
	void DoOnConnectToCenterServer();
    // ִ������MServer
    void DoOnConnectToLogonServer();
    // ������һ����½����m_iCurLogonIndex
	int	FindNextLogonIndex();
    // ������һ������
    bool NextTask();
    // ����Ƿ��޸�������
    bool ModifyPara();
    ///��ȡǮ
    void CheckMoney(__int64 iMoney, int type);
    //�����˸��ݽ�Ҳ�������
    void AiControlBank();
    //����״̬����
    void InitUI(bool bEnable);
    void UIQueueGame(bool bEnable);

    //״̬��Ϣ
    static int CALLBACK CompareProc(LPARAM p1, LPARAM p2, LPARAM p3);
    int m_sort_column; // ��¼�������
    bool m_bDescSort; // ��¼�ȽϷ���
    int CompareFunc(LPARAM p1, LPARAM p2);
    CListCtrl m_ListUserStatus;
    CString m_ShowBeginNo;
    CString m_ShowEndNo;
    void ShowUserStatus();
    afx_msg void OnLvnColumnclickListStatus(NMHDR *pNMHDR, LRESULT *pResult);

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonMin();
    afx_msg void OnBnClickedButtonSave();
    afx_msg void OnBnClickedButtonReset();
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // ���ڴ����϶�
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonTasklist();
    afx_msg void OnBnClickedButtonHallinfo();
    afx_msg void OnBnClickedButtonRoominfo();
    afx_msg void OnBnClickedButtonAddtask();
};
/*-----------------------------------------------------------------------------------------------*/