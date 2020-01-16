#pragma once

#include "Platconfig.h"

// CDialogTaskEdit �Ի���

class CDialogTaskEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTaskEdit)

public:
    enum TaskEditType
    {
        Task_Add, // ���
        Task_Mod // �޸�
    };

public:
	CDialogTaskEdit(const DynamicConfig*pconf, CDialogTaskEdit::TaskEditType type, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogTaskEdit();

// �Ի�������
	enum { IDD = IDD_DIALOG_TASK_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog() override;

    void InitUI();

    bool SavePara();

private:
    DynamicConfig m_conf;
    TaskEditType m_type;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
