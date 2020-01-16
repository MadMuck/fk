// DiaogTaskList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Auto_AI.h"
#include "DiaogTaskList.h"
#include "afxdialogex.h"
#include "Platconfig.h"
#include "DialogTaskEdit.h"
#include "Platglobaldata.h"
#include "DialogTaskEdit.h"
#include "Platlock.h"

#define COL_TASKTIME 0 // ����ʱ��
#define COL_LOGONCOUNT 1 // ��½����
#define COL_HEADNAME 2 // ����ǰ׺
#define COL_BEGINNO 3 // ��ʼ����
#define COL_CHECKOUT 4 // ȡǮ��ֵ
#define COL_CHECKIN 5 // ��Ǯ��ֵ
#define COL_FISHGAMELEAVE 6 // ����������
#define COL_WAITGAMEBEGIN 7 // �ȴ���ʼʱ��
#define COL_AINUMBER 8 // ÿ�������˸���
#define COL_ALLOWAIWITHPLAER 9 // �Ƿ���������ͬ��
#define COL_GAMEENDLEAVE 10 // ��Ϸ����վ�����


// CDiaogTaskList �Ի���

IMPLEMENT_DYNAMIC(CDiaogTaskList, CDialogEx)

CDiaogTaskList::CDiaogTaskList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaogTaskList::IDD, pParent)
{

}

CDiaogTaskList::~CDiaogTaskList()
{
}

void CDiaogTaskList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_TASKLIST, m_TaskList);
}


BEGIN_MESSAGE_MAP(CDiaogTaskList, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_ADDTASK, &CDiaogTaskList::OnBnClickedButtonAddtask)
    ON_BN_CLICKED(IDC_BUTTON_DELTASK, &CDiaogTaskList::OnBnClickedButtonDeltask)
    ON_BN_CLICKED(IDC_BUTTON_MODTASK, &CDiaogTaskList::OnBnClickedButtonModtask)
END_MESSAGE_MAP()


// CDiaogTaskList ��Ϣ�������


BOOL CDiaogTaskList::OnInitDialog()
{
    __super::OnInitDialog();
    DWORD dwStyle = m_TaskList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
    m_TaskList.SetExtendedStyle(dwStyle);
    m_TaskList.ModifyStyle(0, LVS_REPORT);
    m_TaskList.InsertColumn(COL_TASKTIME, _T("����ʱ��"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_LOGONCOUNT, _T("��½����"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_HEADNAME, _T("����ǰ׺"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_BEGINNO, _T("��ʼ����"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_CHECKOUT, _T("ȡǮ��ֵ"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_CHECKIN, _T("��Ǯ��ֵ"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_FISHGAMELEAVE, _T("����������"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_WAITGAMEBEGIN, _T("�ȴ���ʼʱ��"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_AINUMBER, _T("ÿ�������˸���"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_ALLOWAIWITHPLAER, _T("�Ƿ���������ͬ��"), LVCFMT_LEFT,60);
    m_TaskList.InsertColumn(COL_GAMEENDLEAVE, _T("��Ϸ����վ�����"), LVCFMT_LEFT,60);
    ShowTaskInfo();
    return TRUE;
}

void CDiaogTaskList::ShowTaskInfo()
{
    m_TaskList.SetRedraw(FALSE);
    m_TaskList.DeleteAllItems();
    AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
    auto it = PLATCONFIG->m_DynamicConfigs.begin();
    int nRow = 0;
    while(it != PLATCONFIG->m_DynamicConfigs.end())
    {
        CString strTmp;
        m_TaskList.InsertItem(nRow, "TASKINFO");
        m_TaskList.SetItemData(nRow,nRow);
        m_TaskList.SetItemText(nRow, COL_TASKTIME, it->second.strTime);
        strTmp.Format("%d", it->second.nLogonCount);
        m_TaskList.SetItemText(nRow, COL_LOGONCOUNT, strTmp);
        m_TaskList.SetItemText(nRow, COL_HEADNAME, it->second.strRobotPreName);
        strTmp.Format("%d", it->second.nBeginSequenceNo);
        m_TaskList.SetItemText(nRow, COL_BEGINNO, strTmp);
        strTmp.Format("%lld", it->second.nCheckOutMinMoney);
        m_TaskList.SetItemText(nRow, COL_CHECKOUT, strTmp);
        strTmp.Format("%lld", it->second.nCheckInMaxMoney);
        m_TaskList.SetItemText(nRow, COL_CHECKIN, strTmp);
        strTmp.Format("%d", it->second.nFishGameTimeLeave);
        m_TaskList.SetItemText(nRow, COL_FISHGAMELEAVE, strTmp);
        strTmp.Format("%d", it->second.nKeepInDeskSeconds);
        m_TaskList.SetItemText(nRow, COL_WAITGAMEBEGIN, strTmp);
        strTmp.Format("%d", it->second.bMachineDeskCount);
        m_TaskList.SetItemText(nRow, COL_AINUMBER, strTmp);
        strTmp.Format("%d", it->second.bMachineAndPlayer);
        m_TaskList.SetItemText(nRow, COL_ALLOWAIWITHPLAER, strTmp);
        strTmp.Format("%d", it->second.nGameEndLeaveDesk);
        m_TaskList.SetItemText(nRow, COL_GAMEENDLEAVE, strTmp);
        nRow++;
        it++;
    }
    m_TaskList.SetRedraw(TRUE);
}



void CDiaogTaskList::OnBnClickedButtonAddtask()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos = m_TaskList.GetFirstSelectedItemPosition();
    if(pos == NULL)
    {
        CDialogTaskEdit dlgEdit(NULL, CDialogTaskEdit::Task_Add, this);
        dlgEdit.DoModal();
        ShowTaskInfo();
    }
    else
    {
        int index = m_TaskList.GetNextSelectedItem(pos);
        CString strTime = m_TaskList.GetItemText(index, COL_TASKTIME);
        DynamicConfig conf;
        {
            AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
            auto it = PLATCONFIG->m_DynamicConfigs.find(strTime.GetBuffer());
            if(it != PLATCONFIG->m_DynamicConfigs.end())
            {
                conf = it->second;
            }
            else
            {
                MessageBox(_T("δ֪�쳣"));
                return;
            }
        }
        CDialogTaskEdit dlgEdit(&conf, CDialogTaskEdit::Task_Add, this);
        dlgEdit.DoModal();
        ShowTaskInfo();
    }
}


void CDiaogTaskList::OnBnClickedButtonDeltask()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos = m_TaskList.GetFirstSelectedItemPosition();
    if(pos == NULL)
    {
        MessageBox(_T("��ѡ��Ҫɾ��������?"));
        return;
    }

    int index = m_TaskList.GetNextSelectedItem(pos);
    CString strTime = m_TaskList.GetItemText(index, COL_TASKTIME);
    if(strTime == GLOBALDATA->CurDynamicConfig.strTime && GLOBALDATA->bConnectToWitchServer != 255)
    {
        // ����ִ�е���������ɾ��
        MessageBox(_T("����ִ�е���������ɾ��?"));
        return;
    }
    {

        AutoPlatLock lock(&PLATCONFIG->m_DynamicSection);
        auto it = PLATCONFIG->m_DynamicConfigs.find(strTime.GetBuffer());
        if(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            CString tip;
            tip.Format("ȷ��ɾ������%s?",strTime);
            if(IDOK == MessageBox(tip, "����", MB_OKCANCEL))
            {
                PLATCONFIG->m_DynamicConfigs.erase(it);
            }
        }
    }
    ShowTaskInfo();
}


void CDiaogTaskList::OnBnClickedButtonModtask()
{

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos = m_TaskList.GetFirstSelectedItemPosition();
    if(pos == NULL)
    {
        MessageBox(_T("��ѡ��Ҫ�޸ĵ�����?"));
        return;
    }

    int index = m_TaskList.GetNextSelectedItem(pos);
    CString strTime = m_TaskList.GetItemText(index, COL_TASKTIME);
    if(strTime == GLOBALDATA->CurDynamicConfig.strTime && GLOBALDATA->bConnectToWitchServer != 255)
    {
        // ����ִ�е����������޸�
        MessageBox(_T("����ִ�е����������޸�?"));
        return;
    }
    DynamicConfig conf;
    {
        auto it = PLATCONFIG->m_DynamicConfigs.find(strTime.GetBuffer());
        if(it != PLATCONFIG->m_DynamicConfigs.end())
        {
            conf = it->second;
        }
        else
        {
            MessageBox(_T("δ֪�쳣"));
            return;
        }
    }
    CDialogTaskEdit dlgEdit(&conf, CDialogTaskEdit::Task_Mod, this);
    dlgEdit.DoModal();
    ShowTaskInfo();
}
