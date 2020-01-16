
#ifndef __PLATGLOBAL_DATA_H__
#define __PLATGLOBAL_DATA_H__

#include <queue>
#include "Platconfig.h"

#define				 MAX_GAME_ROOM				500			//��󷿼���
//��Ϸ������Ϣ
struct RoomInfoStruct
{
    //�ṹ����
    bool							bAccess;							//���ʱ�־
    UINT							uTabIndex;							//��������
    DWORD							dwGameMSVer;						//���汾��
    DWORD							dwGameLSVer;						//���汾��
    CBaseRoom						* pGameRoomWnd;						//����ָ��
    TCHAR							szProcessName[31];					//��������
    ComRoomInfo						stComRoomInfo;						//��������
    void operator=(RoomInfoStruct & paiData)
    {
        bAccess = paiData.bAccess;
        uTabIndex = paiData.uTabIndex;
        dwGameMSVer = paiData.dwGameMSVer;
        dwGameLSVer = paiData.dwGameLSVer;
        pGameRoomWnd = paiData.pGameRoomWnd;
        memcpy(&stComRoomInfo,&paiData.stComRoomInfo,sizeof(stComRoomInfo));
        memcpy(&szProcessName,&paiData.szProcessName,sizeof(szProcessName));
    }
};

struct TRobotItem
{
    int iUserID;
    RoomInfoStruct* pRoomItem;
public:
    TRobotItem()
    {
        ZeroMemory(this, sizeof(TRobotItem));
    }
};




// ȫ��������Ϣ
class CGlobalData
{
public:
    static CGlobalData*getInstance();
    virtual ~CGlobalData();

    BYTE									bConnectToWitchServer; // 0-δ����ȫ��������,1-������ȫ�ֲ�������
    CString									MainServerIPAddr; // MServer IP��ַ
    int										MainServerPort; // MServer �˿�
    CGameListCtrl							GameList; //��Ϸ�б�
    RoomInfoStruct							RoomInfo[MAX_GAME_ROOM];    //������Ϣ,ÿ�������˶�Ӧһ��������Ϣ
    TRobotItem								Robots[MAX_GAME_ROOM];
    DynamicConfig                           CurDynamicConfig; // ����ִ�е���������

	bool									bLoginEnable;

    void ReInit();

private:
    CGlobalData();
    CGlobalData(const CGlobalData&);
    CGlobalData &operator=(const CGlobalData&);
};

#define GLOBALDATA CGlobalData::getInstance()

// UIչʾ��Ϣ
struct UserItemUIInfo
{
    DynamicConfig cof; // ������Ϣ
    string strUserName;
    string strNickName;
    int GLoginStatus; // GServer��½״̬ 0-δ��½,1-���ڵ�½,2-�ɹ���½,3-��½ʧ��
    bool bRobot; // �Ƿ�Ϊ������
    int roomID;
    BYTE bDeskNo; // ���Ӻ�
    BYTE bDeskStation; // ��λ��
    BYTE bUserState; // ��Ϸ״̬
    UserItemUIInfo()
    {
        roomID = 0;
        strNickName = "";
        GLoginStatus = 0;
        bRobot = true;
        bDeskNo = 255;
        bDeskStation = 255;
        bUserState = USER_NO_STATE;
    }
};

struct UILogInfos
{
    int type;
    string Msg;
};
class UIInfo
{
public:
    ~UIInfo();
    static UIInfo*getInstance();
    CRITICAL_SECTION uiSection;
    ComRoomInfo roomInfo;
    std::map<string, UserItemUIInfo> usrInfos; // �û���Ϊkey

    CRITICAL_SECTION LogMServerSection;
    std::queue<UILogInfos> LogMServerInfos;

    CRITICAL_SECTION LogGServerSection;
    std::queue<UILogInfos> LogGServerInfos;

    void ReInit();
private:
    UIInfo();
    UIInfo(const UIInfo&);
    UIInfo &operator=(const UIInfo&);
};

#define UIDATA UIInfo::getInstance()

#endif // __PLATGLOBAL_DATA_H__