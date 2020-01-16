#pragma once


// CDialogMsgInfo �Ի���

#define TIMER_UPDATE_MSGINFO 1

class CDialogMsgInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMsgInfo)

    enum Msg_Type
    {
        Hall_Msg,
        Room_Msg
    };

public:
	CDialogMsgInfo(Msg_Type, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogMsgInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_MSGINFO };

    virtual BOOL OnInitDialog() override;

    Msg_Type m_type;

public:
    //��ʱ��
    void OnTimer(UINT_PTR nIDEvent);

    // ���´�����־
    void UpdateMServerInfo();
    // ���·�����־
    void UpdateGServerInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCheckErrorinfo();
};
