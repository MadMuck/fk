#ifndef __PLATCONFIG_H__
#define __PLATCONFIG_H__


class DynamicConfig
{
public:
    CString strTime; // ����ʱ��
    //  ��̬����
    int nLogonCount; // ��½�˺Ÿ���
    CString strRobotPreName; // ����������ǰ׺
    int nBeginSequenceNo; // ��ʼ���к�,�����������ǰ׺���
    __int64 nCheckOutMinMoney; // ȡǮ����
    __int64 nCheckInMaxMoney; // ��Ǯ����
    BYTE bMachineDeskCount;//ÿ�������˸���
    bool bMachineAndPlayer; //������������Ƿ�ͬ��
    int  nFishGameTimeLeave; // ������Ϸռ��ʱ��
    int  nKeepInDeskSeconds; // ռ��λʱ��,��λ��
    int  nGameEndLeaveDesk; // ��Ϸ����վ�����

    bool operator == (const DynamicConfig & other)
    {
        return strTime == other.strTime && nLogonCount == other.nLogonCount && strRobotPreName == other.strRobotPreName
            && nBeginSequenceNo == other.nBeginSequenceNo && nCheckInMaxMoney == other.nCheckInMaxMoney && nCheckOutMinMoney == other.nCheckOutMinMoney
            && bMachineAndPlayer == other.bMachineAndPlayer && bMachineDeskCount == other.bMachineDeskCount && nFishGameTimeLeave == other.nFishGameTimeLeave
            && nKeepInDeskSeconds == other.nKeepInDeskSeconds && nGameEndLeaveDesk == other.nGameEndLeaveDesk;
    }
    bool operator != (const DynamicConfig & other)
    {
        return strTime != other.strTime || nLogonCount != other.nLogonCount || strRobotPreName != other.strRobotPreName
            || nBeginSequenceNo != other.nBeginSequenceNo || nCheckInMaxMoney != other.nCheckInMaxMoney || nCheckOutMinMoney != other.nCheckOutMinMoney
            || bMachineAndPlayer != other.bMachineAndPlayer || bMachineDeskCount != other.bMachineDeskCount || nFishGameTimeLeave != other.nFishGameTimeLeave
            || nKeepInDeskSeconds != other.nKeepInDeskSeconds || nGameEndLeaveDesk != other.nGameEndLeaveDesk;
    }
    bool IsValid(CString &errorMsg);
};

// ������ͨ�������ļ���д��
class CPlatConfig 
{
public:
    static CPlatConfig*getInstance();
    virtual ~CPlatConfig();

    //  ��̬����
    CString strLogFile; // ��־�ļ���
    CString strTitleText;// ������
    int KindID; // ��Ϸ����
    int NameID; // ��ϷID
    int RoomID; // ����ID
    CString	CenterServerIPAddr; // AServer IP��ַ
    int		CenterServerPort; // AServer �˿�
    CString strAIPWD; // AI��½����
    bool bFishGame; // �Ƿ�Ϊ��������Ϸ

    CRITICAL_SECTION m_DynamicSection;
    std::map<string, DynamicConfig> m_DynamicConfigs; // ��̬����

    void Save();
private:
    CPlatConfig();
    CPlatConfig(const CPlatConfig&);
    CPlatConfig &operator=(const CPlatConfig&);
};

#define PLATCONFIG CPlatConfig::getInstance()

#endif