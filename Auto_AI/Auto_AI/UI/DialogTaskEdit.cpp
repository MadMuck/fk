// DialogTaskEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Auto_AI.h"
#include "DialogTaskEdit.h"
#include "afxdialogex.h"
#include "Platlock.h"

// CDialogTaskEdit �Ի���

IMPLEMENT_DYNAMIC(CDialogTaskEdit, CDialogEx)

CDialogTaskEdit::CDialogTaskEdit(const DynamicConfig*pconf, TaskEditType type, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTaskEdit::IDD, pParent)
{
    m_type = type;
    if(pconf != NULL)
    {
        m_conf = *pconf;
    }
    else
    {
        m_conf.bMachineAndPlayer = true;
        m_conf.bMachineDeskCount = 4;
        m_conf.nBeginSequenceNo = 1101;
        m_conf.nCheckInMaxMoney = 100000;
        m_conf.nCheckOutMinMoney = 10000;
        m_conf.nFishGameTimeLeave = 10;
        m_conf.nGameEndLeaveDesk = 30;
        m_conf.nKeepInDeskSeconds = 30;
        m_conf.nLogonCount = 10;
        m_conf.strRobotPreName = "C011";
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        m_conf.strTime.Format("%02d%02d", sysTime.wHour, sysTime.wMinute);
    }
}

CDialogTaskEdit::~CDialogTaskEdit()
{
}

void CDialogTaskEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTaskEdit, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDialogTaskEdit::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDialogTaskEdit::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogTaskEdit ��Ϣ�������

BOOL CDialogTaskEdit::OnInitDialog()
{
    __super::OnInitDialog();
    CComboBox *boxHour = (CComboBox *)GetDlgItem(IDC_COMBO_HOUR);
    CString strTmp;
    for(int i = 0; i < 24; ++i)
    {
        strTmp.Format(("%d"), i);
        boxHour->AddString(strTmp);
        boxHour->SetItemData(i, i);
    }
    boxHour->SetCurSel(0);
    CComboBox *boxMinute = (CComboBox *)GetDlgItem(IDC_COMBO_MINUTE);
    for(int i = 0; i < 60; ++i)
    {
        strTmp.Format(("%d"), i);
        boxMinute->AddString(strTmp);
        boxMinute->SetItemData(i, i);
    }
    boxMinute->SetCurSel(0);
    InitUI();
    return TRUE;
}

void CDialogTaskEdit::InitUI()
{
    if(m_type == Task_Mod)
    {
        SetWindowText(_T("�޸�����"));
    }
    else
    {
        SetWindowText(_T("�������"));
    }
    if(!PLATCONFIG->bFishGame)
    {
        GetDlgItem(IDC_EDIT_FISHGAMELEAVE)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_WAITTIME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->EnableWindow(FALSE);
    }

    // ��������ʱ��
    CComboBox *boxHour = (CComboBox *)GetDlgItem(IDC_COMBO_HOUR);
    boxHour->SetCurSel(atoi(m_conf.strTime) / 100);
    CComboBox *boxMinute = (CComboBox *)GetDlgItem(IDC_COMBO_MINUTE);
    boxMinute->SetCurSel(atoi(m_conf.strTime) % 100);

    CString strTmp;
    // ���õ�½����
    strTmp.Format("%d", m_conf.nLogonCount);
    GetDlgItem(IDC_EDIT_LOGINCOUNT)->SetWindowText(strTmp);
    // ����ǰ׺
    strTmp.Format("%s", m_conf.strRobotPreName);
    GetDlgItem(IDC_EDIT_HEADNAME)->SetWindowText(strTmp);
    // ��ʼ���
    strTmp.Format("%d", m_conf.nBeginSequenceNo);
    GetDlgItem(IDC_EDIT_BEGINNO)->SetWindowText(strTmp);
    // ȡǮ��ֵ
    strTmp.Format("%lld", m_conf.nCheckOutMinMoney);
    GetDlgItem(IDC_EDIT_CHECKOUT2)->SetWindowText(strTmp);
    // ��Ǯ��ֵ
    strTmp.Format("%lld", m_conf.nCheckInMaxMoney);
    GetDlgItem(IDC_EDIT_CHECKIN2)->SetWindowText(strTmp);
    // ����������
    strTmp.Format("%d", m_conf.nFishGameTimeLeave);
    GetDlgItem(IDC_EDIT_FISHGAMELEAVE)->SetWindowText(strTmp);
    // �ȴ���Ϸ��ʼʱ��
    strTmp.Format("%d", m_conf.nKeepInDeskSeconds);
    GetDlgItem(IDC_EDIT_WAITTIME)->SetWindowText(strTmp);
    // ÿ�������˸���
    strTmp.Format("%d", m_conf.bMachineDeskCount);
    GetDlgItem(IDC_EDIT_AINUMBER)->SetWindowText(strTmp);
    // ��Ϸ����վ�����
    strTmp.Format("%d", m_conf.nGameEndLeaveDesk);
    GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->SetWindowText(strTmp);
    // �Ƿ�������ͬ��
    if(m_conf.bMachineAndPlayer)
    {
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOWWITHPLAYER))->SetCheck(1);
    }
    else
    {
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOWWITHPLAYER))->SetCheck(0);
    }
}

bool CDialogTaskEdit::SavePara()
{
    DynamicConfig conf;
    // ��������ʱ��
    CComboBox *boxHour = (CComboBox *)GetDlgItem(IDC_COMBO_HOUR);
    int hour = boxHour->GetCurSel();
    if(hour == -1) hour = 0;
    CComboBox *boxMinute = (CComboBox *)GetDlgItem(IDC_COMBO_MINUTE);
    int minute = boxMinute->GetCurSel();
    if(minute == -1) minute = 0;
    conf.strTime.Format("%02d%02d", hour, minute);

    CString strTmp;
    // ���õ�½����
    GetDlgItem(IDC_EDIT_LOGINCOUNT)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��½�������ô���");
        return false;
    }
    conf.nLogonCount = atoi(strTmp);
    // ����ǰ׺
    GetDlgItem(IDC_EDIT_HEADNAME)->GetWindowText(strTmp);
    conf.strRobotPreName = strTmp;
    // ��ʼ���
    GetDlgItem(IDC_EDIT_BEGINNO)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��ʼ������ô���");
        return false;
    }
    conf.nBeginSequenceNo = atoi(strTmp);
    // ȡǮ��ֵ
    GetDlgItem(IDC_EDIT_CHECKOUT2)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("ȡǮ��ֵ���ô���");
        return false;
    }
    conf.nCheckOutMinMoney = _atoi64(strTmp);
    // ��Ǯ��ֵ
    GetDlgItem(IDC_EDIT_CHECKIN2)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��Ǯ��ֵ���ô���");
        return false;
    }
    conf.nCheckInMaxMoney = _atoi64(strTmp);
    // ����������
    GetDlgItem(IDC_EDIT_FISHGAMELEAVE)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("�������������ô���");
        return false;
    }
    conf.nFishGameTimeLeave = atoi(strTmp);
    // �ȴ���Ϸ��ʼʱ��
    GetDlgItem(IDC_EDIT_WAITTIME)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("�ȴ���Ϸ��ʼʱ�����ô���");
        return false;
    }
    conf.nKeepInDeskSeconds = atoi(strTmp);
    // ÿ�������˸���
    GetDlgItem(IDC_EDIT_AINUMBER)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("ÿ�������˸������ô���");
        return false;
    }
    conf.bMachineDeskCount = atoi(strTmp);
    // ��Ϸ����վ�����
    GetDlgItem(IDC_EDIT_GAMEENDLEAVE)->GetWindowText(strTmp);
    if(!IsValidNumber(strTmp))
    {
        MessageBox("��Ϸ����վ��������ô���");
        return false;
    }
    conf.nGameEndLeaveDesk = atoi(strTmp);
    // �Ƿ�������ͬ��
    if(((CButton*)GetDlgItem(IDC_CHECK_ALLOWWITHPLAYER))->GetCheck())
    {
        conf.bMachineAndPlayer = true;
    }
    else
    {
        conf.bMachineAndPlayer = false;
    }

    m_conf = conf;
    return true;
}

void CDialogTaskEdit::OnBnClickedOk()
{
    DynamicConfig tmpConf = m_conf;
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!SavePara()) return;
    CString errorMsg;
    if(!m_conf.IsValid(errorMsg))
    {
        MessageBox(errorMsg);
        return;
    }
    if(m_type == Task_Mod)
    {// �޸�
        AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
        auto it = PLATCONFIG->m_DynamicConfigs.find(m_conf.strTime.GetBuffer());
        if(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            if(it->second.strTime != tmpConf.strTime)
            {
                MessageBox(_T("��ʱ����Ѵ�������"));
                return;
            }
            else if(IDOK == MessageBox(_T("�����޸�?"), _T("��ʾ"), MB_OKCANCEL))
            {
                it->second = m_conf;
                PLATCONFIG->Save();
            }
        }
        else
        {// ��ɾ�������
            auto it = PLATCONFIG->m_DynamicConfigs.find(tmpConf.strTime.GetBuffer());
            if(it != PLATCONFIG->m_DynamicConfigs.end())
            {
                PLATCONFIG->m_DynamicConfigs.erase(it);
            }
            PLATCONFIG->m_DynamicConfigs[m_conf.strTime.GetBuffer()] = m_conf;
        }
    }
    else
    {// ���
        AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
        auto it = PLATCONFIG->m_DynamicConfigs.find(m_conf.strTime.GetBuffer());
        if(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            MessageBox(_T("��ʱ����Ѵ�������"));
            return;
        }
        else
        {
            if(IDOK == MessageBox(_T("ȷ�����?"), _T("��ʾ"), MB_OKCANCEL))
            {
                PLATCONFIG->m_DynamicConfigs[m_conf.strTime.GetBuffer()] = m_conf;
                PLATCONFIG->Save();
            }
        }
    }

    CDialogEx::OnOK();
}


void CDialogTaskEdit::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}
