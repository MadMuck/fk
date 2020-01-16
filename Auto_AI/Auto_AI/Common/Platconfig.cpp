#include "stdafx.h"
#include "Platconfig.h"

#define CONFIG_FILENAME ".\\RobotSet.ini"
#define STATIC_SECTION "Static"
#define DYNAMIC_SECTION "Dynamic"

bool DynamicConfig::IsValid(CString &errorMsg)
{
    if((nCheckInMaxMoney != 0 && nCheckOutMinMoney != 0 && nCheckInMaxMoney < nCheckOutMinMoney + 1000)
    || nCheckInMaxMoney < 0 || nCheckOutMinMoney < 0)
    {
        errorMsg.Format("��ȡǮ%lld-%lld���ô���(�������1000���)",nCheckOutMinMoney,nCheckInMaxMoney);
        return false;
    }
    if(strRobotPreName.GetLength() > 30)
    {
        errorMsg.Format("����������ǰ׺����%s",strRobotPreName);
        return false;
    }
    if(nLogonCount < 0)
    {
        errorMsg.Format("��½�������ô���%d",nLogonCount);
        return false;
    }
    if(nBeginSequenceNo < 0)
    {
        errorMsg.Format("��ʼ�������ô���%d",nBeginSequenceNo);
        return false;
    }
    if(bMachineDeskCount <= 0)
    {
        errorMsg.Format("ÿ�������˸������ô���%d",bMachineDeskCount);
        return false;
    }
    if(nFishGameTimeLeave <= 0)
    {
        errorMsg.Format("������Ϸռ��ʱ�����Ǵ���%d",nFishGameTimeLeave);
        return false;
    }
    if(nKeepInDeskSeconds <= 15)
    {
        errorMsg.Format("�ȴ���Ϸ��ʼʱ�����ô���(����15��)%d",nKeepInDeskSeconds);
        return false;
    }
    if(nGameEndLeaveDesk < 0 || nGameEndLeaveDesk > 100)
    {
        errorMsg.Format("��Ϸ����վ��������ô���(0-100)%d",nGameEndLeaveDesk);
        return false;
    }
    return true;
}

CPlatConfig*CPlatConfig::getInstance()
{
    static CPlatConfig s_config;
    return &s_config;
}
CPlatConfig::~CPlatConfig()
{
    Save();
    DeleteCriticalSection(&m_DynamicSection);
}

void CPlatConfig::Save()
{
    TCHAR szTmp[256]= {0};

    auto it = m_DynamicConfigs.begin();
    int i = 0;
    while(it != m_DynamicConfigs.end())
    {
        CString key;
        key.Format("Time_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%s"), it->second.strTime);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp)); // ��������ʱ��
        key.Format("LogonCount_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nLogonCount);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ��½����
        key.Format("RobotPreName_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%s"), it->second.strRobotPreName);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp)); // ����������ǰ׺
        key.Format("BeginSequenceNo_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nBeginSequenceNo);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ��½��ʼ���к�
        key.Format("CheckOutMinMoney_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nCheckOutMinMoney);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ȡǮ����
        key.Format("CheckInMaxMoney_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nCheckInMaxMoney);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ��Ǯ����
        key.Format("MachineDeskCount_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.bMachineDeskCount);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ÿ�������˸���
        key.Format("MachineAndPlayer_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.bMachineAndPlayer);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ������������Ƿ�ͬ��
        key.Format("FishGameTimeLeave_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nFishGameTimeLeave);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ������Ϸռ��ʱ��,��λ����
        key.Format("KeepInDeskSeconds_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nKeepInDeskSeconds);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ռ��λʱ��,��λ��
        key.Format("GameEndLeaveDesk_%d",i);
        sprintf_s(szTmp, sizeof(szTmp), _T("%d"), it->second.nGameEndLeaveDesk);
        WritePrivateProfileString(DYNAMIC_SECTION, key, szTmp, CONFIG_FILENAME);
        ZeroMemory(szTmp, sizeof(szTmp));// ��Ϸ����վ�����
        i++;
        it++;
    }
    sprintf_s(szTmp, sizeof(szTmp), _T("%d"), i);
    WritePrivateProfileString(DYNAMIC_SECTION, "TaskNumber", szTmp, CONFIG_FILENAME);
}

CPlatConfig::CPlatConfig()
{
    InitializeCriticalSection(&m_DynamicSection);
    TCHAR szTmp[256]= {0};
    // ��־�ļ���
    GetPrivateProfileString(STATIC_SECTION, "LogFile", _T("RobotLog"), szTmp, sizeof(szTmp), CONFIG_FILENAME);
    strLogFile.Format(_T("%s"), szTmp);
    ZeroMemory(szTmp, sizeof(szTmp));
    // ������
    GetPrivateProfileString(STATIC_SECTION, "TitleText", _T("������"), szTmp, sizeof(szTmp), CONFIG_FILENAME);
    strTitleText.Format(_T("%s"), szTmp);
    ZeroMemory(szTmp, sizeof(szTmp));
    // ��Ϸ����
    KindID = GetPrivateProfileInt(STATIC_SECTION, "KindID", 1, CONFIG_FILENAME);
    // ��ϷID
    NameID = GetPrivateProfileInt(STATIC_SECTION, "NameID", 10100103, CONFIG_FILENAME);
    // ��ϷID
    RoomID = GetPrivateProfileInt(STATIC_SECTION, "RoomID", 1, CONFIG_FILENAME);
    // AServerIP��ַ
    GetPrivateProfileString(STATIC_SECTION, "CenterServerIPAddr", _T("127.0.0.1"), szTmp, sizeof(szTmp), CONFIG_FILENAME);
    CenterServerIPAddr.Format(_T("%s"), szTmp);
    ZeroMemory(szTmp, sizeof(szTmp));
    // AServer�˿�
    CenterServerPort = GetPrivateProfileInt(STATIC_SECTION, "CenterServerPort", 13025, CONFIG_FILENAME);
    // AI����
    GetPrivateProfileString(STATIC_SECTION, "AIPWD", _T("6fc175a72fe0af5093a6e1dfb2c8d1d5"), szTmp, sizeof(szTmp), CONFIG_FILENAME);
    strAIPWD.Format(_T("%s"), szTmp);
    ZeroMemory(szTmp, sizeof(szTmp));
    // �Ƿ�Ϊ��������Ϸ
    bFishGame = GetPrivateProfileInt(STATIC_SECTION, "FishGame", 1, CONFIG_FILENAME) != 0?true:false;

    // �������
    int nNumber = GetPrivateProfileInt(DYNAMIC_SECTION, "TaskNumber", 0, CONFIG_FILENAME);

    for(int i = 0; i < nNumber; ++i)
    {
        DynamicConfig cof;

        // ��������ʱ��
        CString key;
        CString strMapKey;
        key.Format("Time_%d", i);
        GetPrivateProfileString(DYNAMIC_SECTION, key, _T("0000"), szTmp, sizeof(szTmp), CONFIG_FILENAME);
        strMapKey.Format(_T("%s"), szTmp);
        cof.strTime = strMapKey;
        ZeroMemory(szTmp, sizeof(szTmp));
        // ��½��Ϸ����
        key.Format("LogonCount_%d", i);
        cof.nLogonCount = GetPrivateProfileInt(DYNAMIC_SECTION, key, 10, CONFIG_FILENAME);
        // ����������ǰ׺
        key.Format("RobotPreName_%d", i);
        GetPrivateProfileString(DYNAMIC_SECTION, key, _T("C011"), szTmp, sizeof(szTmp), CONFIG_FILENAME);
        cof.strRobotPreName.Format(_T("%s"), szTmp);
        ZeroMemory(szTmp, sizeof(szTmp));
        // ��½��ʼ���к�
        key.Format("BeginSequenceNo_%d", i);
        cof.nBeginSequenceNo = GetPrivateProfileInt(DYNAMIC_SECTION, key, 1001, CONFIG_FILENAME);
        // ȡǮ����
        key.Format("CheckOutMinMoney_%d", i);
        cof.nCheckOutMinMoney = GetPrivateProfileInt(DYNAMIC_SECTION, key, 10000, CONFIG_FILENAME);
        // ��Ǯ����
        key.Format("CheckInMaxMoney_%d", i);
        cof.nCheckInMaxMoney = GetPrivateProfileInt(DYNAMIC_SECTION, key, 100000, CONFIG_FILENAME);
        // ÿ�������˸���
        key.Format("MachineDeskCount_%d", i);
        cof.bMachineDeskCount = GetPrivateProfileInt(DYNAMIC_SECTION, key, PLAY_COUNT, CONFIG_FILENAME);
        // ������������Ƿ�ͬ��
        key.Format("MachineAndPlayer_%d", i);
        cof.bMachineAndPlayer = GetPrivateProfileInt(DYNAMIC_SECTION, key, 1, CONFIG_FILENAME) != 0?true:false;
        // ������Ϸռ��ʱ��,��λ����
        key.Format("FishGameTimeLeave_%d", i);
        cof.nFishGameTimeLeave = GetPrivateProfileInt(DYNAMIC_SECTION, key, 30, CONFIG_FILENAME);
        // ռ��λʱ��,��λ��
        key.Format("KeepInDeskSeconds_%d", i);
        cof.nKeepInDeskSeconds = GetPrivateProfileInt(DYNAMIC_SECTION, key, 30, CONFIG_FILENAME);
        // ��Ϸ����վ�����
        key.Format("GameEndLeaveDes_%d", i);
        cof.nGameEndLeaveDesk = GetPrivateProfileInt(DYNAMIC_SECTION, key, 30, CONFIG_FILENAME);
        m_DynamicConfigs[strMapKey.GetBuffer()] = cof;
    }    
}
