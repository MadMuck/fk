#pragma once
#include "afxcmn.h"


// CDiaogTaskList �Ի���

class CDiaogTaskList : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaogTaskList)

public:
	CDiaogTaskList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaogTaskList();

// �Ի�������
	enum { IDD = IDD_DIALOG_TASKLIST };

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_TaskList;

    void ShowTaskInfo();
    afx_msg void OnBnClickedButtonAddtask();
    afx_msg void OnBnClickedButtonDeltask();
    afx_msg void OnBnClickedButtonModtask();
};
