#include "StdAfx.h"
#include "ServerManage.h"
#include "writelog.h"
#include <math.h>
#include "StdAfx.h"
#include "ServerManage.h"
#include "writelog.h"
#include <math.h>
void DebugPrintf(const char *p, ...)
{
	return;
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\GameLog\\";
	SHCreateDirectoryEx(NULL, strPath, NULL);
	CTime time = CTime::GetCurrentTime();
	sprintf(szFilename, "%s%d__%d%d%d_log.txt",strPath ,NAME_ID,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	//����ʱ��
	char cTime[30];
	::memset(cTime,0,sizeof(cTime));
	sprintf(cTime,"[%d:%d:%d] ",time.GetHour(),time.GetMinute(),time.GetSecond());
	fprintf(fp,cTime);
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}

#include <math.h>
#include <windows.h> 
#include <DbgHelp.h>  
#include <stdlib.h>  
#pragma comment(lib, "dbghelp.lib")  
#pragma comment(lib, "dbghelp.lib")  

#ifndef _M_IX86  
#error "The following code only works for x86!"  
#endif  

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)  
{  
	if(pModuleName == 0)  
	{  
		return FALSE;  
	}  

	WCHAR szFileName[_MAX_FNAME] = L"";  
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);  

	if(wcsicmp(szFileName, L"ntdll") == 0)  
		return TRUE;  

	return FALSE;  
}  

inline BOOL CALLBACK MiniDumpCallback(PVOID                            pParam,  
	const PMINIDUMP_CALLBACK_INPUT   pInput,  
	PMINIDUMP_CALLBACK_OUTPUT        pOutput)  
{  
	if(pInput == 0 || pOutput == 0)  
		return FALSE;  

	switch(pInput->CallbackType)  
	{  
	case ModuleCallback:  
		if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)  
			if(!IsDataSectionNeeded(pInput->Module.FullPath))  
				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);  
	case IncludeModuleCallback:  
	case IncludeThreadCallback:  
	case ThreadCallback:  
	case ThreadExCallback:  
		return TRUE;  
	default:;  
	}  

	return FALSE;  
}  

inline void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName)  
{  
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,  
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  

	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))  
	{  
		MINIDUMP_EXCEPTION_INFORMATION mdei;  
		mdei.ThreadId           = GetCurrentThreadId();  
		mdei.ExceptionPointers  = pep;  
		mdei.ClientPointers     = NULL;  

		MINIDUMP_CALLBACK_INFORMATION mci;  
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;  
		mci.CallbackParam       = 0;  

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, (pep != 0) ? &mdei : 0, NULL, &mci);  

		CloseHandle(hFile);  
	}  
}  

LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)  
{  
	CString strPath;
	char szPath[MAX_PATH];
	memset(szPath, 0, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szPath);
	strPath.ReleaseBuffer();
	strPath.Format("%s\\D-Error\\", szPath);
	CreateDirectory(strPath, NULL);
	CString strFile;
	CTime tm = CTime::GetCurrentTime();
	strFile.Format(_T("%sError-game-%d-%d-%d&%d-%d-%d.dmp"), strPath,tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	CreateMiniDump(pExceptionInfo, strFile);  
	AfxMessageBox(strPath);
	return EXCEPTION_EXECUTE_HANDLER;  
}  

// �˺���һ���ɹ����ã�֮��� SetUnhandledExceptionFilter �ĵ��ý���Ч  
void DisableSetUnhandledExceptionFilter()  
{  
	void* addr = (void*)GetProcAddress(LoadLibrary("kernel32.dll"),  
		"SetUnhandledExceptionFilter");  

	if (addr)  
	{  
		unsigned char code[16];  
		int size = 0;  

		code[size++] = 0x33;  
		code[size++] = 0xC0;  
		code[size++] = 0xC2;  
		code[size++] = 0x04;  
		code[size++] = 0x00;  

		DWORD dwOldFlag, dwTempFlag;  
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);  
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);  
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);  
	}  
}  

void InitMinDump()  
{  
	//ע���쳣������  
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);  

	//ʹSetUnhandledExceptionFilter  
	DisableSetUnhandledExceptionFilter();  
} 


//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation	= GS_WAIT_SETGAME;	//��ׯ�ȴ�״̬
	m_byBetWaitTime=2;//����ע�׶η��Ƶȴ�ʱ��
	m_byOpenAwardWaitTime=2;//����ע�׶η��Ƶȴ�ʱ��
	m_iRemaindTime=0;//ʱ������

	m_vlSuperID.clear();//���泬����ҵ�����
	m_blNtWaitList.clear();	//��ׯ�б�
	m_iNtPlayCount=0;//ׯ����ׯ����
	m_byNtStation = 255;//ׯ��λ��
	m_bXiaZhuang = false;

	memset(m_i64LastBet,0,sizeof(m_i64LastBet));
	//��ҳɼ�
	memset(m_i64PlayerGrade,0,sizeof(m_i64PlayerGrade));
	//�����Ӯ���
	memset(m_i64PlayCoin,0,sizeof(m_i64PlayCoin));
	//�����Ľ����
	memset(m_i64ReturnMoney,0,sizeof(m_i64ReturnMoney));
	m_bSuperDeskStation=255;						//�����û���λ��
	m_bShowRobotBet=true;							//�����û����ƻ�������ע��ʾ
	memset(G_iChouMaValue,0,sizeof(G_iChouMaValue));
	ResetGameData();
	memset(m_i64WinMoney, 0, sizeof(m_i64WinMoney));
	InitMinDump();
}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}
//----------------------------------------------------------------------------------------------------
//��ʼ����������
bool	CServerGameDesk::InitDeskGameStation()
{
	LoadIni();
	if(m_pDataManage)
	{
		LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	}
	return true;
}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_iNtPlayLimite = f.GetKeyVal(key,"NtMaxPlay",10);		//ׯ����ׯ��������
	m_iApplyNtLimite=f.GetKeyVal(key,"shangzhuang",10000);		//��ׯ����
	m_iAreaBetLimite=f.GetKeyVal(key,"AreaBetLimite",1000);		//������ע����
	m_lSingleNoteLimit=f.GetKeyVal(key,"SingleNoteLimit",1000);   //��������ע����
	m_lRemainedNotes=m_lSingleNoteLimit; //��Ϸ�տ�ʼ��ʣ����ע�����ڵ�������ע����
	//������
	//�����׶ε�ʱ��

	m_byRobNTtime=f.GetKeyVal(key,"RobNTtime",10);		//��ׯ�׶�ʱ��
	if(m_byRobNTtime<1)
	{
		m_byRobNTtime=10;
	}

	m_byFreeTime=f.GetKeyVal(key,"freetime",5);			//���н׶�ʱ��
	if(m_byFreeTime<1)
	{
		m_byFreeTime=5;
	}
	m_byBetTime=f.GetKeyVal(key,"xiazhutime",15);			//��ע�׶�ʱ��
	if(m_byBetTime<1)
	{
		m_byBetTime=15;
	}
	m_byRolldiceTime=f.GetKeyVal(key,"rolldicetime",5);		//ҡ���׶�ʱ��
	if(m_byRolldiceTime<1)
	{
		m_byRolldiceTime=5;
	}
	m_byOpenCardTime=f.GetKeyVal(key,"opencardtime",5);//�����׶�ʱ��
	if(m_byOpenCardTime<1)
	{
		m_byOpenCardTime=5;
	}
	m_bShowChouMa=f.GetKeyVal(key,"showchouma",0);

	m_byNuoLimit = f.GetKeyVal(key,"NuoLimit",30);	//Ų����
	if(m_byNuoLimit > 100)
	{
		m_byNuoLimit = 100;
	}

	m_byDiceNum = f.GetKeyVal(key,"DiceNum",3);		//������
	if(m_byDiceNum != 2 && m_byDiceNum != 3)
	{
		m_byDiceNum = 3;
	}

	m_iPlayingMethod = (PM_MEM_CHUAN | PM_BAO_ZI | PM_NUO | PM_NUO_NEEDBET | PM_3DICE_GETTWO);

	m_byMaxNtPeople=f.GetKeyVal(key,"MaxNtPeople",20);;//�����ׯ����
	//����
	key = "SuperUser";
	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<iCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		lUserID = f.GetKeyVal(key, strText, 0);
		//�����г���ID ���浽����������
		m_vlSuperID.push_back(lUserID);
		CString cs;
		cs.Format("super ����ID=%d",lUserID);
		OutputDebugString(cs);
	}

	{
		int foundPos = 0;
		int lastPos = 0;
		CString cardListStr = f.GetKeyVal(key,"ChouMa","10,50,100,200,500");
		for (int j = 0; j < BETSTYLE; j ++)
		{
			lastPos = foundPos;
			foundPos = cardListStr.Find(',',foundPos);
			if (foundPos < 0)
			{
				foundPos = cardListStr.GetLength();
			}
			__int64 icard = _ttoi(cardListStr.Mid(lastPos, foundPos - lastPos));
			G_iChouMaValue[j] = icard;
			foundPos++;
		}
	}

	return TRUE;
}
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{

	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,iRoomID);

	m_iNtPlayLimite = f.GetKeyVal(szSec,"NtMaxPlay",m_iNtPlayLimite);		//ׯ����ׯ��������
	m_iApplyNtLimite=f.GetKeyVal(szSec,"shangzhuang",m_iApplyNtLimite);		//��ׯ����
	m_iAreaBetLimite=f.GetKeyVal(szSec,"AreaBetLimite",m_iAreaBetLimite);		//������ע����
	m_lSingleNoteLimit=f.GetKeyVal(szSec,"SingleNoteLimit",m_lSingleNoteLimit);           //��������ע����
	m_lRemainedNotes=m_lSingleNoteLimit; //��Ϸ�տ�ʼ��ʣ����ע�����ڵ�������ע����
	//������
	//�����׶ε�ʱ��

	m_byRobNTtime=f.GetKeyVal(szSec,"RobNTtime",m_byRobNTtime);		//��ׯ�׶�ʱ��
	if(m_byRobNTtime<1)
	{
		m_byRobNTtime=10;
	}

	m_byFreeTime=f.GetKeyVal(szSec,"freetime",m_byFreeTime);			//���н׶�ʱ��
	if(m_byFreeTime<1)
	{
		m_byFreeTime=5;
	}
	m_byBetTime=f.GetKeyVal(szSec,"xiazhutime",m_byBetTime);			//��ע�׶�ʱ��
	if(m_byBetTime<1)
	{
		m_byBetTime=15;
	}
	m_byRolldiceTime=f.GetKeyVal(szSec,"rolldicetime",m_byRolldiceTime);//ҡ���׶�ʱ��
	if(m_byRolldiceTime<1)
	{
		m_byRolldiceTime=5;
	}
	m_byOpenCardTime=f.GetKeyVal(szSec,"opencardtime",m_byOpenCardTime);//�����׶�ʱ��
	if(m_byOpenCardTime<1)
	{
		m_byOpenCardTime=5;
	}
	m_bShowChouMa=f.GetKeyVal(szSec,"showchouma",m_bShowChouMa);

	m_byMaxNtPeople=f.GetKeyVal(szSec,"MaxNtPeople",m_byMaxNtPeople);;//�����ׯ����

	m_byNuoLimit = f.GetKeyVal(szSec,"NuoLimit",m_byNuoLimit);	//Ų����
	if(m_byNuoLimit > 100)
	{
		m_byNuoLimit = 100;
	}

	m_byDiceNum = f.GetKeyVal(szSec,"DiceNum",m_byDiceNum);		//������
	if(m_byDiceNum != 2 && m_byDiceNum != 3)
	{
		m_byDiceNum = 3;
	}

	m_bIsRobotWinAndLostAutoCtrl = f.GetKeyVal(szSec,"IsRobotWinAndLostAutoCtrl",0);				//�Ƿ�����������Ӯ�Զ�����

	//�����˽�������
	m_iRobotWinMoneyA1	= f.GetKeyVal(szSec,"RobotWinMoneyA1",500000);						/*<������ӮǮ����1*/
	m_iRobotWinMoneyA2	= f.GetKeyVal(szSec,"RobotWinMoneyA2",5000000);						/*<������ӮǮ����2*/
	m_iRobotWinMoneyA3	= f.GetKeyVal(szSec,"RobotWinMoneyA3",50000000);						/*<������ӮǮ����3*/

	//�������ڸ�������ӮǮ���ʿ���
	m_iRobotWinLuckyAtA1	= f.GetKeyVal(szSec,"RobotWinLuckyAtA1",90);						/*<������������1ӮǮ�ĸ���*/
	m_iRobotWinLuckyAtA2	= f.GetKeyVal(szSec,"RobotWinLuckyAtA2",50);						/*<��������Ӯ���ƣ�������������2ӮǮ�ĸ���*/
	m_iRobotWinLuckyAtA3	= f.GetKeyVal(szSec,"RobotWinLuckyAtA3",30);						/*<��������Ӯ���ƣ�������������3ӮǮ�ĸ���*/
	m_iRobotWinLuckyAtA4	= f.GetKeyVal(szSec,"RobotWinLuckyAtA4",5);						/*<��������Ӯ���ƣ�������������4ӮǮ�ĸ���*/

	//��¼������ӮǮ��Ŀ
	m_iResetRobotHaveWinMoney	= f.GetKeyVal(szSec,"ReSetRobotHaveWinMoney",0);				/*<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ*/
	return true;
}

//�������ǿ����Ϣ
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}

	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			CString dwjlog;
			dwjlog.Format("yxxtest::��Ȼ�յ������ǿ�˵���ϢpNetHead->bAssistantID=%d",pNetHead->bAssistantID);
			OutputDebugString(dwjlog);
			return true ; 
		}
	}

	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//�����м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	
}

//������Ϸ��ʱ��
void	CServerGameDesk::SetGameTimer(UINT uTimerID, BYTE byTime)
{
	KillTimer(TIME_ONE_SECOND);
	m_iRemaindTime = byTime;
	SetTimer(TIME_ONE_SECOND ,1000) ;
	SetTimer(uTimerID ,byTime*1000) ;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return TRUE;
}

/*--------------------------------------------------------------*/
//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i])//��Ҳ����ھ�
		{
			continue;
		}
		m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
	}	
	if (__super::GameBegin(bBeginFlag)==false)
	{
		GameFinish(0,GF_SAFE);
		return false;
	}
	if (!m_bIsBuy && (255 == m_byNtStation || NULL == m_pUserInfo[m_byNtStation]))	//��ҳ�ׯ�Ҳ����ڲ���ʼ��Ϸ
	{
		GameFinish(0, GF_SAFE);
		return false;
	}

	LARGE_INTEGER nFrequency;

	if(QueryPerformanceFrequency(&nFrequency))
	{
		LARGE_INTEGER nStartCounter;
		QueryPerformanceCounter(&nStartCounter);
		srand((unsigned)nStartCounter.LowPart);		//�������������
	}	
	else
		srand((unsigned)GetTickCount());	//�������������

	//�����Ϸ��ʱ����
	ResetGameData();

	NotiGameCount();

	if(m_bIsBuy && 255 == m_byNtStation)
	{
		m_bGameStation	= GS_ROB_NT;
		m_blNtWaitList.clear();

		//֪ͨ��ׯ
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if (NULL==m_pUserInfo[i])
			{
				continue;
			}
			SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,S_C_ROB_NT_NTF,0);
		}

		SetGameTimer(TIME_ROB_NT, m_byRobNTtime);

		return true;
	}

	//����������ׯ�������ׯ���б�ֻ�п��ֵ�һ����Ҫ��ׯ
	if(m_bIsBuy)
	{
		m_blNtWaitList.clear();
	}

	//��ׯ����
	m_iNtPlayCount++;

	m_bXiaZhuang	= false;
	//������Ϸ״̬
	m_bGameStation	= GS_BET_STATUS;
	BeginBetMsg betmsg;
	betmsg.byNtStation	= m_byNtStation;
	betmsg.byNtCount	= m_iNtPlayCount;	//ׯ����ׯ����
	betmsg.byGameStation= m_bGameStation;	//��ǰ����Ϸ״̬
	betmsg.byNtStation	= m_byNtStation;
	//ׯ�ҳɼ�

	betmsg.i64NtScore=m_i64PlayerGrade[m_byNtStation];

	int i=0;
	for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{
		betmsg.byNtWaitList[i]=*ite;
		i++;
	}
	//������ע��Ϣ		
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&betmsg,sizeof(betmsg),MDM_GM_GAME_NOTIFY,ASS_BEGIN_BET,0);
	}
	
	//������ע��ʱ��
	SetGameTimer(TIME_XIAZHU_TIME,m_byBetTime);

	RandDice();

	return TRUE;
}

//��������
void	CServerGameDesk::ResetGameData()
{
	m_iRemaindTime=0;//ʱ������
	//�����ע���
	memset(m_i64UserBetCount,0,sizeof(m_i64UserBetCount));
	memset(m_i64AreaGain,0,sizeof(m_i64AreaGain));
	memset(m_i64AreaBetAllSum,0,sizeof(m_i64AreaBetAllSum));
	memset(m_i64UserBetSum,0,sizeof(m_i64UserBetSum));
	//����ע��
	m_i64SumAllBet = 0;
	//��ʵ���
	//����ʵ�����ÿ���������ע���
	memset(m_i64PeopleBetCount,0,sizeof(m_i64PeopleBetCount));
	//���������ʵ��ҵ�����ע
	memset(m_i64PeopleAreaBetAllSum,0,sizeof(m_i64PeopleAreaBetAllSum));
	//����ʵ��ҵ�����ע
	memset(m_i64PeopleBetSum,0,sizeof(m_i64PeopleBetSum));					
	//��ʵ�������ע��			
	m_i64PeopleSumAllBet=0;		

	//���Ų����
	memset(m_byNuoCount, 0, sizeof(m_byNuoCount));

	//�����¼
	memset(m_iChouMaCount, 0, sizeof(m_iChouMaCount));

	//���Ų���
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		for (int j = 0; j < NUO_COUNT; ++j)
		{
			m_sUserNuoData[i][j].Clear();
		}
	}

	memset(m_i64UserBetCountEx, 0, sizeof(m_i64UserBetCountEx));

	//����Y����Ϣ
	//�����Ӯ���
	memset(m_i64PlayCoin,0,sizeof(m_i64PlayCoin));
	//�����Ľ����
	memset(m_i64ReturnMoney,0,sizeof(m_i64ReturnMoney));
	memset(m_byWinPrize,255,sizeof(m_byWinPrize));

	m_bIsSuperControl=FALSE;							
	m_bSuperDeskStation=255;						//�����û���λ��
	m_bShowRobotBet=true;							//�����û����ƻ�������ע��ʾ
	
}

//��ȡ���ʣ�����Ǯ
long long	CServerGameDesk::GetUserRemaindMoney(BYTE byDeskStation)
{
	if (NULL != m_pUserInfo[byDeskStation])
	{
		return m_pUserInfo[byDeskStation]->m_UserData.i64Money - GetUserBetCount(byDeskStation);
	}
	return 0;
}

//��ȡ�����ע�ܶ�
long long		CServerGameDesk::GetUserBetCount(BYTE byDeskStation)
{
	long long iUserBetCount = 0;
	for (int i=0; i<BET_ARES;i++)
	{
		iUserBetCount += m_i64UserBetCount[byDeskStation][i];
	}
	return iUserBetCount;
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_ONE_SECOND:	//һ��ʣ���ʱ�� ��������׶ε�ʣ��ʱ��
		{
			m_iRemaindTime--;
			if (m_iRemaindTime <= 0)
			{
				KillTimer(TIME_ONE_SECOND);
				m_iRemaindTime = 0;
			}
			break;
		}
	case TIME_ROB_NT:
		{
			KillTimer(TIME_ROB_NT);
			OnGetZhuang();
			GameBegin(ALL_ARGEE);
			break;
		}
	case TIME_XIAZHU_TIME://��ע�׶�
		{
			KillTimer(TIME_XIAZHU_TIME);
			SendRolldice();
			break;
		}
	case TIME_ROLL_TIME://ҡ���׶�
		{
			KillTimer(TIME_ROLL_TIME);
			SendAllCard();
			break;
		}
	case TIME_SEND_TIME://�����׶�
		{
			//֪ͨ�ͷ�����ʾ����
			KillTimer(TIME_SEND_TIME);
			openAward();
			break;	
		}
	case TIME_GAME_FINISH:	//��Ϸ������ʱ��
		{
			KillTimer(TIME_GAME_FINISH);
			//��������
			GameFinish(0,GF_NORMAL);

			//�����˳��û��ɼ�
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL==m_pUserInfo[i])
				{
					m_i64PlayerGrade[i]=0;
				}
			}
			//���Ϳ�����Ϣ
			SendFree();
			break;
		}
	case TIME_FREE_TIME://���н׶�
		{
			StartGame();
			break;
		}
	default:
		{
			break;
		}
	}
	return __super::OnTimer(uTimerID);	
}

//����
void CServerGameDesk::openAward()
{
	OutputDebugString("yxxgame1::����");
	//ͳ����ҽ��
	accountPlayCoin();
	rememberLuzi();
	//��¼��������Ӯ���
	RecordRobotWinMoney();
	//�������ݿ�
	bool temp_cut[PLAY_COUNT];
	memset(temp_cut,0,sizeof(temp_cut));

	CString dwjlog;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		dwjlog.Format("yxxgame1::i64PlayCoin[%d]=%I64d",i,m_i64PlayCoin[i]);
		OutputDebugString(dwjlog);
	}
	ChangeUserPointint64(m_i64PlayCoin, temp_cut);
	memset(m_i64ChangePlayCoin,0,sizeof(m_i64ChangePlayCoin));
	__super::RecoderGameInfo(m_i64ChangePlayCoin);

	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		dwjlog.Format("yxxgame1::i64ChangePlayCoin[%d]=%I64d",i,m_i64ChangePlayCoin[i]);
		OutputDebugString(dwjlog);
	}

	BeginAward beginAward;
	memcpy(beginAward.i64PlayerGrade, m_i64PlayerGrade, sizeof(beginAward.i64PlayerGrade));
	memcpy(beginAward.i64PlayCoin, m_i64PlayCoin, sizeof(beginAward.i64PlayCoin));
	memcpy(beginAward.i64ReturnMoney, m_i64ReturnMoney, sizeof(beginAward.i64ReturnMoney));

	//���Ϳ�����Ϣ	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}

		memcpy(beginAward.i64AreaGain, m_i64AreaGain[i], sizeof(beginAward.i64AreaGain));

		if (m_bIsBuy)
		{
			m_i64WinMoney[i] += m_i64PlayCoin[i];		//��¼�����ɼ�
		}

		SendGameData(i, &beginAward, sizeof(beginAward), MDM_GM_GAME_NOTIFY, ASS_GAME_OPEN, 0);

		SendAwardRecord(i);
	}

	if (m_bIsBuy && m_iRunGameCount >= m_iVipGameCount)
	{
		SendScoreBoard();
	}

	//������Ϸ������ʱ��
	SetGameTimer(TIME_GAME_FINISH,3);
}

void CServerGameDesk::SendAwardRecord(BYTE byDeskStation)
{
	S_C_AwardRecord msg;
	for(int i = 0; i < m_dstAward.size(); i++)
	{
		memcpy_s(msg.stAward[i].byWinArea, sizeof(msg.stAward[i].byWinArea), m_dstAward[i].byWinArea, sizeof(m_dstAward[i].byWinArea));
	}
	SendGameData(byDeskStation, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, S_C_AWARD_RECORD_NTF,0);
}

void  CServerGameDesk::SendAllCard()
{
	//������Ϸ״̬
	m_bGameStation	= GS_SEND_STATUS;
	BeginSendCardMsg SendCard;
	SendCard.byGameStation=m_bGameStation;

	if(m_bIsRobotWinAndLostAutoCtrl && FALSE==m_bIsSuperControl)//����ǻ������Զ����ƣ����ǳ��˿���
	{
		RobotAutoControl();
	}
	//����λ��
	memcpy(SendCard.byWinPrize,m_byWinPrize,sizeof(SendCard.byWinPrize));
	//memcpy(SendCard.iRunTime,m_iRunTime,sizeof(SendCard.iRunTime));
	//���ͷ�����Ϣ��
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		bool bSuccese = SendGameData(i,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SHOW_DICE,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_SHOW_DICE---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}

	//����������ʱ��
	SetGameTimer(TIME_SEND_TIME,m_byOpenCardTime);
	return ;
}

//�������Զ�����
void CServerGameDesk::RobotAutoControl()
{
	//����ȥ�ж� ��һ���Ƿ���л������������� 
	bool bHaveRobot = false;
	bool bHaveUser	= false;

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}

		if(m_i64UserBetSum[i] > 0 || i == m_byNtStation)
		{
			if(m_pUserInfo[i]->m_UserData.isVirtual)
				bHaveRobot = true;
			else
				bHaveUser = true;
		}
	}

	//���л�������������  ��ȥ���п���
	if (bHaveUser && bHaveRobot)
	{
		BOOL RobotIsWin=FALSE;
		int iResult = rand()%100;
		CString s = CINIFile::GetAppPath ();/////����·��
		CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
		TCHAR szSec[_MAX_PATH] = TEXT("");
		_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
		//��ȡ�����ļ��л������Ѿ�Ӯ�Ľ��
		m_iResetRobotHaveWinMoney	= f.GetKeyVal(szSec,"ReSetRobotHaveWinMoney ",0);	

		if(m_iResetRobotHaveWinMoney<=0)//������ӮǮ��Ϊ�������㣬�����˱�Ӯ
		{
			RobotIsWin=TRUE;
		}
		else if(m_iResetRobotHaveWinMoney>0 && m_iResetRobotHaveWinMoney<=m_iRobotWinMoneyA1)
		{
			if(iResult<m_iRobotWinLuckyAtA1)
			{
				RobotIsWin=TRUE;
			}
		}
		else if(m_iResetRobotHaveWinMoney>m_iRobotWinMoneyA1 && m_iResetRobotHaveWinMoney<=m_iRobotWinMoneyA2)
		{
			if(iResult<m_iRobotWinLuckyAtA2)
			{
				RobotIsWin=TRUE;
			}
		}
		else if(m_iResetRobotHaveWinMoney>m_iRobotWinMoneyA2 && m_iResetRobotHaveWinMoney <= m_iRobotWinMoneyA3)
		{
			if(iResult<m_iRobotWinLuckyAtA3)
			{
				RobotIsWin=TRUE;
			}
		}
		else if(m_iResetRobotHaveWinMoney > m_iRobotWinMoneyA3)
		{
			if(iResult<m_iRobotWinLuckyAtA4)
			{
				RobotIsWin=TRUE;
			}
		}

		if(RobotIsWin)//������ӮǮ
		{
			int tryCount = 0;
			__int64 AIWinMoney = CountAIWinMoney();
			while(AIWinMoney < 0 && tryCount < 100)
			{
				++tryCount;
				RandDice();
				AIWinMoney = CountAIWinMoney();
			}
		}
		else//��������Ǯ
		{
			int tryCount = 0;
			__int64 AIWinMoney = CountAIWinMoney();
			while(AIWinMoney > 0 && tryCount < 100)
			{
				++tryCount;
				RandDice();
				AIWinMoney = CountAIWinMoney();
			}
		}
	}
}

//�ж���ѡ��
int CServerGameDesk::shuangxx(BYTE winArea[],BYTE a,BYTE b)
{
	int cnta=0,cntb=0;
	for(int i=0;i<3;i++)
		if(winArea[i]==a) cnta++;
	for(int i=0;i<3;i++)
		if(winArea[i]==b) cntb++;

	if((cnta==2&&cntb==1)||(cnta==1&&cntb==2))		//������,����(����һ)
	{
		if(IsPlayModeVaild(PM_3DICE_GETTWO))
		{
			return 80;
		}
		else
		{
			return 100;
		}
	}
	else if(cnta==1&&cntb==1)
	{
		if(2 == GetDiceNum())						//˫���ӣ����ж�(����)
		{
			if(IsPlayModeVaild(PM_2DICE_CHUAN))
			{
				return 80;
			}
			else
			{
				return 100;
			}
		}
		else if(3 == GetDiceNum())					//�����ӣ����ж�
		{
			return 50;
		}
	}
	return -10;
}

//�ж���ѡ��
int CServerGameDesk::sanxx(BYTE winArea[],BYTE a,BYTE b,BYTE c)
{
	int cnta=0,cntb=0,cntc=0;
	for(int i=0;i<3;i++)
		if(winArea[i]==a)
			cnta++;
	for(int i=0;i<3;i++)
		if(winArea[i]==b)
			cntb++;
	for(int i=0;i<3;i++)
		if(winArea[i]==c)
			cntc++;
	if(cnta==1&&cntb==1&&cntc==1)		//������
	{
		return 230;
	}
	else if((cnta==2||cntb==2||cntc==2)&&cnta+cntb+cntc==3)		//���ж�(����һ)
	{
		return 50;
	}
	else if(cnta==1&&cntb==1&&cntc==0||cnta==1&&cntb==0&&cntc==1||cnta==0&&cntb==1&&cntc==1)	//���ж�(��ͨ)
	{
		if(IsPlayModeVaild(PM_3DICE_GETTWO))
		{
			return 20;
		}
		else
		{
			return 25;
		}
	}
	return -10;
}

//�жϱ���
int CServerGameDesk::isBaozi(BYTE winArea[],BYTE a)
{
	bool flag = true;
	for(int i = 0; i < 3; ++i)
	{
		if(255 != winArea[i] && a != winArea[i])
		{
			flag = false;
			break;
		}
	}
	if(flag)
	{
		if(2 == GetDiceNum())		//˫���ӱ���
		{
			if(IsPlayModeVaild(PM_2DICE_BAOZI))
			{
				return 150;
			}
			else
			{
				return 100;
			}
		}
		else						//�����ӱ���
		{
			if(IsPlayModeVaild(PM_3DICE_BAOZI))
			{
				return 250;
			}
			else
			{
				return 300;
			}
		}
	}
	return -10;
}

void CServerGameDesk::accountPlayCoin()
{
	memset(m_i64PlayCoin,0,sizeof(m_i64PlayCoin));
	
	int res[BET_ARES];
	memset(res, 0, sizeof(res));

	for(int i = 0;i < BET_ARES; ++i)		//��ȡÿ������Ľ��
	{
		res[i] = GetAresResult(i);
	}

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(i == m_byNtStation || NULL == m_pUserInfo[i])
		{
			continue;
		}

		for(int j = 0; j < BET_ARES; ++j)	//ͳ��ÿ��������Ӯ����¼
		{
			if(m_i64UserBetCount[i][j] > 0)			//��ע�������
			{
				int ret = res[j];
				m_i64PlayCoin[i] += m_i64UserBetCount[i][j] * ret / 10;
				m_i64AreaGain[i][j] += m_i64UserBetCount[i][j] * ret / 10;
				m_i64AreaGain[m_byNtStation][j] -= m_i64AreaGain[i][j];
				if(ret > 0)
				{
					m_i64ReturnMoney[i] += m_i64UserBetCount[i][j];
				}
			}

			if (m_i64UserBetCountEx[i][j] > 0)		//Ų�������
			{
				int ret = res[j];
				m_i64PlayCoin[i] += m_i64UserBetCountEx[i][j] * ret / 10;
				m_i64AreaGain[i][j] += m_i64UserBetCountEx[i][j] * ret / 10;
				m_i64AreaGain[m_byNtStation][j] -= m_i64AreaGain[i][j];
				if (ret > 0)
				{
					m_i64ReturnMoney[i] += m_i64UserBetCountEx[i][j];
				}
			}
		}

		m_i64PlayCoin[m_byNtStation] -= m_i64PlayCoin[i];
		m_i64PlayerGrade[m_byNtStation] -= m_i64PlayCoin[i];
		m_i64PlayerGrade[i] += m_i64PlayCoin[i];	
	}
}

//��ס·��
void CServerGameDesk::rememberLuzi()
{
	//·�Ӽ�¼���
	//ֻ��¼15��·��
	STruOpenAward res;
	for(int i = 0; i < GetDiceNum(); ++i)
	{
		res.byWinArea[i] = m_byWinPrize[i];
	}

	if (m_dstAward.size() >= RECORD_COUNT)//����15��
	{
		m_dstAward.pop_front();
		m_dstAward.push_back(res);
	}
	else
	{
		m_dstAward.push_back(res);
	}
}
//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bWatchUser)
	{
		return true;
	}
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_AGREE_GAME:
		{
			if (!m_bIsBuy)						//��ҳ�����Ҫ׼��
			{
				return true;
			}
			else if (m_iRunGameCount > 0)		//������ֻ�е�һ�ֿ�ʼ��ʱ��Ҫ׼��
			{
				return true;
			}
				
			return __super::HandleNotifyMessage(bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser);
		}
	case ASS_USER_BET:	//�����ע
		{
			if(uSize != sizeof(UserBetMsg))
			{
				return true ; 
			}
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealUserBet(pData);
			return true;
		}
	case C_S_BET_INFO_REQ:		//�����ע��Ϣ����
		{
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealUserBetInfo(bDeskStation, pData);
			return true;
		}
	case C_S_CHANGE_BET_REQ:	//���Ųע
		{
			if(uSize != sizeof(TMSG_CHANGE_BET_REQ))
			{
				return true ; 
			}
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealUserNuo(bDeskStation, pData);
			return true;
		}
	case ASS_APPLY_NT:	//���������ׯ
		{
			if(uSize != sizeof(ApplyNtMsg))
			{
				return true ; 
			}
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			//�������������ׯ��Ϣ
			DealUserApplyNt(pData);
			return true;
		}
	case ASS_APPLY_LEAVE:	//������ׯ
		{
			if(uSize != sizeof(ApplyLiveNtMsg))
			{
				return true ; 
			}
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealUserApplyLeaveNt(pData);
			return true;
		}
	case ASS_CONTINUE_BET://�����ѹ
		{
			return true;		//û����ѹ
			if(uSize!=sizeof(CS_ContinueBetMsg))
				return true;
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealContinueBet(pData);//flog
			return true;
		}
	case ASS_CLEAR_BET://��������ע
		{
			return true;		//���ɳ���������Ų
			if(uSize!=sizeof(CS_CleartBetMsg))
				return true;
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealClearBet(pData);
			return true;
		}
	case ASS_SUPER_RESULT://�����û���Ϣ
		{
			if(uSize != sizeof(SuperUserControlReslut))
			{
				return true ; 
			}
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealSuperUserMessage(pData);
			return true;
		}
	default:
		{
			break;
		}
	}
	bool bTest = __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
	
	dwjlog.Format("yxxtest::bTest=%d",bTest);
	OutputDebugString(dwjlog);
	return bTest;
}

//�������û����ƽ��
void	CServerGameDesk::DealSuperUserMessage(void *pBuffer)
{
	SuperUserControlReslut *SuperUserMessage = (SuperUserControlReslut *)pBuffer;
	if(NULL == SuperUserMessage)
	{
		return;
	}
	if(GS_BET_STATUS!=m_bGameStation)
	{
		return;
	}
	if(3 == GetDiceNum() && SuperUserMessage->bySuperReslut[0]>=1 && SuperUserMessage->bySuperReslut[0]<=6&&SuperUserMessage->bySuperReslut[1]>=1 && SuperUserMessage->bySuperReslut[1]<=6&&SuperUserMessage->bySuperReslut[2]>=1 && SuperUserMessage->bySuperReslut[2]<=6)
	{			
		m_bIsSuperControl=TRUE;
		//m_bySuperReslut=ProduceResultSuper(SuperUserMessage->bySuperReslut);//�����û����ƵĽ��
		memcpy(m_byWinPrize,SuperUserMessage->bySuperReslut,sizeof(m_byWinPrize));
		//m_byWinPrize[0]=m_bySuperReslut;
		////�������
		//m_byWinPrizeStart=ProduceStart(m_byWinPrize[0]);
	}
	else if(2 == GetDiceNum() && SuperUserMessage->bySuperReslut[0]>=1 && SuperUserMessage->bySuperReslut[0]<=6&&SuperUserMessage->bySuperReslut[1]>=1 && SuperUserMessage->bySuperReslut[1]<=6)
	{
		m_bIsSuperControl=TRUE;
		//m_bySuperReslut=ProduceResultSuper(SuperUserMessage->bySuperReslut);//�����û����ƵĽ��
		memcpy(m_byWinPrize,SuperUserMessage->bySuperReslut,sizeof(m_byWinPrize));
		m_byWinPrize[2] = 255;
	}
	m_bSuperDeskStation=SuperUserMessage->bSuperDeskStation;					//�����û���λ��
	m_bShowRobotBet=SuperUserMessage->bShowRobotBet;							//�����û����ƻ�������ע��ʾ
	SendBetDataToClient(m_bSuperDeskStation);
	SuperUserControlStart SuperStart;
	memcpy(SuperStart.bySuperStart,m_byWinPrize,sizeof(SuperStart.bySuperStart));
	//SuperStart.bySuperStart=m_byWinPrizeStart;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		bool bSuccese =SendGameData(i,&SuperStart,sizeof(SuperStart),MDM_GM_GAME_NOTIFY,ASS_SUPER_SEND,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_SUPER_SEND---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}
}

//��������ע
void CServerGameDesk::DealClearBet(void *pBuffer)
{
	CS_CleartBetMsg  *pClearBet=(CS_CleartBetMsg *)pBuffer;
	if(pClearBet==NULL||m_i64UserBetSum[pClearBet->byDeskStation]==0) return;
	SC_ClearBetResultMsg pClearBetResultMsg;
	pClearBetResultMsg.byDeskStation=pClearBet->byDeskStation;
	pClearBetResultMsg.i64BeforeClearBet=m_i64UserBetSum[pClearBet->byDeskStation];
	for(int i=0;i<BET_ARES;i++)
	{
		if(m_i64UserBetCount[pClearBet->byDeskStation][i]>0)
		{
			m_i64AreaBetAllSum[i]=m_i64AreaBetAllSum[i]-m_i64UserBetCount[pClearBet->byDeskStation][i]; 
			m_i64PeopleAreaBetAllSum[i]-=m_i64UserBetCount[pClearBet->byDeskStation][i];//��ʵ����ܸ�����������ע
			m_i64SumAllBet-=m_i64UserBetCount[pClearBet->byDeskStation][i];
			m_i64PeopleSumAllBet-=m_i64UserBetCount[pClearBet->byDeskStation][i];//��ʵ�������ע
			m_i64UserBetCount[pClearBet->byDeskStation][i]=0; 
			m_i64PeopleBetCount[pClearBet->byDeskStation][i]=0;
		}
	}
	m_i64UserBetSum[pClearBet->byDeskStation]=0;
	m_i64PeopleBetSum[pClearBet->byDeskStation]=0;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		if(m_bSuperDeskStation==i && !m_bShowRobotBet)//���˿���ֻ��ʾ��ʵ�����ע
		{
			pClearBetResultMsg.i64SumAllBet=m_i64PeopleSumAllBet;
			pClearBetResultMsg.i64UserAllBet=GetUserBetCount(i);//flog
			for(int j=0;j<BET_ARES;j++)
			{
				pClearBetResultMsg.i64AreaBetAllSum[j]=m_i64PeopleAreaBetAllSum[j];
				pClearBetResultMsg.i64UserBetCount[j]=m_i64PeopleBetCount[i][j];
			}
		}
		else 
		{
			pClearBetResultMsg.i64SumAllBet=m_i64SumAllBet;
			pClearBetResultMsg.i64UserAllBet=GetUserBetCount(i);//flog
			for(int j=0;j<BET_ARES;j++)
			{
				pClearBetResultMsg.i64AreaBetAllSum[j]=m_i64AreaBetAllSum[j];
				pClearBetResultMsg.i64UserBetCount[j]=m_i64UserBetCount[i][j];
			}
		}
		
		bool bSuccese = SendGameData(i,&pClearBetResultMsg,sizeof(pClearBetResultMsg),MDM_GM_GAME_NOTIFY,ASS_CLEAR_BET_RESULT ,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_CLEAR_BET_RESULT---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}
	
}

//�������������ׯ��Ϣ
void	CServerGameDesk::DealUserApplyNt(void *pBuffer)
{
	ApplyNtMsg *pApplyNt = (ApplyNtMsg *)pBuffer;
	if(NULL == pApplyNt)
	{
		return;
	}
	if (m_pUserInfo[pApplyNt->byDeskStation] == NULL)
	{
		return;
	}

	ApplyNtResultMsg TApplyNtResult;
	
	TApplyNtResult.byDeskStation=pApplyNt->byDeskStation;
	
	if(m_bIsBuy && GS_ROB_NT != m_bGameStation)//�����������ׯ�׶β�����ׯ
	{
		return;
	}

	if(m_blNtWaitList.size()>=m_byMaxNtPeople)//�����ׯ�����ж�
	{
		return;
	}

	//�Ƿ�ׯ��||�Ѿ�����ׯ�б���
	if(pApplyNt->byDeskStation == m_byNtStation || IsInWaitNtList(pApplyNt->byDeskStation))
	{
		return;
	}

	//�Ƿ����㹻��Ǯ��ׯ
	if (!CanMinusPoint() && GetUserRemaindMoney(pApplyNt->byDeskStation) < m_iApplyNtLimite )
	{
		TApplyNtResult.bApplay=true;
		SendGameData(TApplyNtResult.byDeskStation,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_APPLY_RESULT,0);
		return;
	}
	//�Ǵ������������ǰû��ׯ�� && �ȴ��б�����û���� && ��ô������ ��ֱ�ӿ�ʼ��Ϸ
	if (!m_bIsBuy && 255 == m_byNtStation)
	{
		m_byNtStation	=pApplyNt->byDeskStation;//��סׯ��λ��
		m_iNtPlayCount	=0;
		TApplyNtResult.bNtFlag=TRUE;
		StartGame();
	}
	else//����ׯ���б�
	{
		m_blNtWaitList.push_back(pApplyNt->byDeskStation);
		int i=0;
		for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
		{
			TApplyNtResult.byWaitNtList[i]=*ite;
			i++;
		}
		TApplyNtResult.byNtStation = m_byNtStation;
		TApplyNtResult.byNtcount=m_iNtPlayCount;//ׯ����ׯ����
		//ׯ�ҳɼ�
		if(255==m_byNtStation)
		{
			TApplyNtResult.i64NtScore=0;
		}
		else
		{
			TApplyNtResult.i64NtScore=m_i64PlayerGrade[m_byNtStation];
		}
		TApplyNtResult.bGameStation=m_bGameStation;
		for (int i = 0; i < PLAY_COUNT; i ++)
		{
			if (NULL==m_pUserInfo[i])
			{
				continue;
			}
			SendGameData(i,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_APPLY_RESULT,0);
		}
	}
}

//�������������ׯ��Ϣ
void	CServerGameDesk::DealUserApplyLeaveNt(void *pBuffer)
{
	ApplyLiveNtMsg *pApplyLeaveNt = (ApplyLiveNtMsg *)pBuffer;
	if (pApplyLeaveNt == NULL)
	{
		return;
	}

	if(m_bIsBuy)		//�������䲻����������ׯ
		return;

	ApplyLiveNtResultMsg TApplyNtResult;
	TApplyNtResult.byDeskStation=pApplyLeaveNt->byDeskStation;
	if((pApplyLeaveNt->byDeskStation == m_byNtStation))//����״̬ׯ������ׯ
	{
		m_bXiaZhuang = true;
	}
	else if(IsInWaitNtList(pApplyLeaveNt->byDeskStation))
	{
		vector<BYTE>::iterator ite=m_blNtWaitList.begin();
		for(;ite!=m_blNtWaitList.end();ite++)
		{
			if(pApplyLeaveNt->byDeskStation==*ite)
			{
				m_blNtWaitList.erase(ite);
				break;
			}
		}
	}
	else
	{
		return;
	}
	//����ׯ��
	TApplyNtResult.byNtStation=m_byNtStation;
	TApplyNtResult.byNtcount=m_iNtPlayCount;//ׯ����ׯ����
	//ׯ�ҵĳɼ�
	TApplyNtResult.i64NtScore=m_i64PlayerGrade[m_byNtStation];
	int i=0;
	////��ׯ�б�
	for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{
		TApplyNtResult.byWaitNtList[i]=*ite;
		i++;
	}
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		bool bSuccese = SendGameData(i,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_LEAVE_RESULT,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_LEAVE_RESULT---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}

}

//���������ע
void	CServerGameDesk::DealUserBet(void *pBuffer)
{
	if(GS_BET_STATUS!=m_bGameStation)
	{
		return;
	}
	UserBetMsg *pUserBet = (UserBetMsg *)pBuffer;
	if(NULL == pUserBet)
	{
		return;
	}
	bool bPeopleUpdate=false;//��ʵ�����ע�����Ƿ��и���

	UserBetResultMsg TUserBetResult;

	//ׯ�Ҳ�����
	if (255 == m_byNtStation || NULL == m_pUserInfo[m_byNtStation])
	{
		TUserBetResult.bErrorCode = 6;
		SendGameData(pUserBet->byDeskStation, &TUserBetResult, sizeof(TUserBetResult), MDM_GM_GAME_NOTIFY, ASS_BET_RESULT, 0);
		return;
	}

	//�ж���ע�����Ƿ���Ч
	if(!GetAreaBetVaild(pUserBet->byBetArea))		
	{
		TUserBetResult.bErrorCode = 5;
	}

	//�жϳ��������Ƿ���Ч
	if(pUserBet->byChouMaType >= BETSTYLE)
	{
		TUserBetResult.bErrorCode = 5;
	}

	//������ע����
	long long i64RemainBetNum = RemainingBettingNumber(pUserBet->byBetArea);
	if(!CanMinusPoint() && (m_i64AreaBetAllSum[pUserBet->byBetArea]+G_iChouMaValue[pUserBet->byChouMaType])>i64RemainBetNum)
	{
		TUserBetResult.bErrorCode = 1;
	}
	//�Լ���Ǯ��������ע
	if(!CanMinusPoint() && G_iChouMaValue[pUserBet->byChouMaType]>GetUserRemaindMoney(pUserBet->byDeskStation))
	{
		TUserBetResult.bErrorCode = 2;
	}

	//������ע����
	if((m_i64AreaBetAllSum[pUserBet->byBetArea]+G_iChouMaValue[pUserBet->byChouMaType])>GetAreaBetLimite(pUserBet->byBetArea))
	{
		TUserBetResult.bErrorCode = 1;
	}
	//��ע��ׯ�ҷ���
	if (pUserBet->byDeskStation == m_byNtStation)
	{
		TUserBetResult.bErrorCode = 3;
	}
	//��ע������������ע����
	if(m_i64UserBetSum[pUserBet->byDeskStation] + G_iChouMaValue[pUserBet->byChouMaType]>m_lSingleNoteLimit)
	{
		TUserBetResult.bErrorCode = 4;
	}

	if (TUserBetResult.bErrorCode != 0)
	{
		SendGameData(pUserBet->byDeskStation, &TUserBetResult, sizeof(TUserBetResult), MDM_GM_GAME_NOTIFY, ASS_BET_RESULT, 0);
		return;
	}

	//��¼��ע���
	//�������ÿ���������ע���
	m_i64UserBetCount[pUserBet->byDeskStation][pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];

	//�����������ע
	m_i64AreaBetAllSum[pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
	//�����ע����
	m_i64UserBetSum[pUserBet->byDeskStation] += G_iChouMaValue[pUserBet->byChouMaType];
	//����ע��
	m_i64SumAllBet+= G_iChouMaValue[pUserBet->byChouMaType];

	//��¼������
	++m_iChouMaCount[pUserBet->byBetArea][pUserBet->byChouMaType];

	//���������ʵ��ҵ�����ע
	if(!m_pUserInfo[pUserBet->byDeskStation]->m_UserData.isVirtual)
	{
		//����ʵ�����ÿ���������ע���
		m_i64PeopleBetCount[pUserBet->byDeskStation][pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
		//����ʵ��ҵ�����ע
		m_i64PeopleBetSum[pUserBet->byDeskStation] += G_iChouMaValue[pUserBet->byChouMaType];					
		//���������ʵ��ҵ�����ע								
		m_i64PeopleAreaBetAllSum[pUserBet->byBetArea]+=G_iChouMaValue[pUserBet->byChouMaType];
		//��ʵ�������ע��		;
		m_i64PeopleSumAllBet+= G_iChouMaValue[pUserBet->byChouMaType];
		bPeopleUpdate=true;
	}
	//OutputDebugString("super::��ע�ɹ�");
	
	TUserBetResult.bDynamicBet=true;
	//��ͷ��˷�����ע����
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		
		if(m_bSuperDeskStation==i && !m_bShowRobotBet)//���˿���ֻ��ʾ��ʵ�����ע
		{
			if(!bPeopleUpdate)
			{
				continue;
			}
			//����ע
			TUserBetResult.i64SumAllBet=m_i64PeopleSumAllBet;
			//��ǰ�������ע
			TUserBetResult.i64UserAllBet=GetUserBetCount(i);//flog
			//��ע��λ��
			TUserBetResult.byDeskStation=pUserBet->byDeskStation;
			//��ע����
			TUserBetResult.byBetArea = pUserBet->byBetArea;
			//��ע����ֵ
			TUserBetResult.byChouMaType = pUserBet->byChouMaType;
			for(int j=0;j<BET_ARES;j++)
			{
				//�Լ�����ע���
				TUserBetResult.i64UserBetCount[j]=m_i64PeopleBetCount[i][j];
				//��������ע���
				TUserBetResult.i64AreaBetAllSum[j]=m_i64PeopleAreaBetAllSum[j];
			}
		}
		else//����ʾ
		{
			//����ע
			TUserBetResult.i64SumAllBet=m_i64SumAllBet;
			//�������ע
			TUserBetResult.i64UserAllBet=GetUserBetCount(i);//flog
			//��ע��λ��
			TUserBetResult.byDeskStation=pUserBet->byDeskStation;
			//��ע����
			TUserBetResult.byBetArea = pUserBet->byBetArea;
			//��ע����ֵ
			TUserBetResult.byChouMaType = pUserBet->byChouMaType;
			for(int j=0;j<BET_ARES;j++)
			{
				TUserBetResult.i64UserBetCount[j]=m_i64UserBetCount[i][j];
				TUserBetResult.i64AreaBetAllSum[j]=m_i64AreaBetAllSum[j];
			}
		}
		m_lRemainedNotes=m_lSingleNoteLimit-m_i64UserBetSum[i];
		TUserBetResult.i64RemainedNotes=m_lRemainedNotes;
		bool bSuccese = SendGameData(i,&TUserBetResult,sizeof(TUserBetResult),MDM_GM_GAME_NOTIFY,ASS_BET_RESULT,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_BET_RESULT---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	
	}
}

//���������ѹ
void CServerGameDesk::DealContinueBet(void *pBuffer)
{
	CS_ContinueBetMsg *pUserContinueBet = (CS_ContinueBetMsg *)pBuffer;
	if(NULL == pUserContinueBet)
	{
		return;
	}
	long long total_bet=0;
	for(int i=0;i<BET_ARES;i++)
	{
		total_bet+=m_i64LastBet[pUserContinueBet->byDeskStation][i];
	}
	if(!CanMinusPoint() && total_bet>GetUserRemaindMoney(pUserContinueBet->byDeskStation))
	{
	return;
	}
	
	CS_ContinueBetResultMsg pContinueBetResult;
	pContinueBetResult.byDeskStation=pUserContinueBet->byDeskStation;
	
	if(total_bet==0) 
	{
		pContinueBetResult.bFlag=false;
	}
	else 
	{
		pContinueBetResult.bFlag=true;
	}
	UserBetMsg pUserBet;
	pUserBet.byDeskStation=pUserContinueBet->byDeskStation;
	for(int i=0;i<BET_ARES;i++)
	{
		if(m_i64LastBet[pUserContinueBet->byDeskStation][i]>0)
		{
			pUserBet.byBetArea=i;
			__int64 i64Tmp=m_i64LastBet[pUserContinueBet->byDeskStation][i];
			if(i64Tmp%G_iChouMaValue[4]==0)
			{
				pUserBet.byChouMaType=4;
				for(int i=0;i<i64Tmp/G_iChouMaValue[4];i++)
				{
					DealUserBet(&pUserBet);
				}
				i64Tmp%=G_iChouMaValue[4];
			}
			if(i64Tmp%G_iChouMaValue[3]==0)
			{
				pUserBet.byChouMaType=3;
				for(int i=0;i<i64Tmp/G_iChouMaValue[3];i++)
				{
					DealUserBet(&pUserBet);
				}
				i64Tmp%=G_iChouMaValue[3];
			}
			 if(i64Tmp%G_iChouMaValue[2]==0)
			{
				pUserBet.byChouMaType=2;
				for(int i=0;i<i64Tmp/G_iChouMaValue[2];i++)
				{
					DealUserBet(&pUserBet);
				}
				i64Tmp%=G_iChouMaValue[2];
			}
			 if(i64Tmp%G_iChouMaValue[1]==0)
			{
				pUserBet.byChouMaType=1;
				for(int i=0;i<i64Tmp/G_iChouMaValue[1];i++)
				{
					DealUserBet(&pUserBet);
				}
				i64Tmp%=G_iChouMaValue[1];
			}
			 if(i64Tmp%G_iChouMaValue[0]==0)
			{
				pUserBet.byChouMaType=0;
				for(int i=0;i<i64Tmp/G_iChouMaValue[0];i++)
				{
					DealUserBet(&pUserBet);
				}
				i64Tmp%=G_iChouMaValue[0];
			}
		}
	}
	SendGameData(pContinueBetResult.byDeskStation,&pContinueBetResult,sizeof(pContinueBetResult),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_BET_RESULT,0);
}

//��ͷ��˷�����ע����(���˿���)
void CServerGameDesk::SendBetDataToClient(BYTE bDeskStation)
{
	//��ͷ��˷�����ע����
	if (NULL==m_pUserInfo[bDeskStation])
	{
		return;
	}
	UserBetResultMsg TUserBetResult;
	TUserBetResult.bDynamicBet=false;
	TUserBetResult.i64RemainedNotes=m_lRemainedNotes;
	if(m_pUserInfo[m_bSuperDeskStation] && !m_bShowRobotBet)//���˿���ֻ��ʾ��ʵ�����ע
	{
		//����ע
		TUserBetResult.i64SumAllBet=m_i64PeopleSumAllBet;
		TUserBetResult.i64UserAllBet=GetUserBetCount(bDeskStation);
		for(int j=0;j<BET_ARES;j++)
		{
			//�Լ�����ע���
			TUserBetResult.i64UserBetCount[j]=m_i64PeopleBetCount[bDeskStation][j];
			//��������ע���
			TUserBetResult.i64AreaBetAllSum[j]=m_i64PeopleAreaBetAllSum[j];
		}
		SendGameData(bDeskStation,&TUserBetResult,sizeof(TUserBetResult),MDM_GM_GAME_NOTIFY,ASS_BET_RESULT,0);
	}
	else
	{
		//����ע
		TUserBetResult.i64SumAllBet=m_i64SumAllBet;
		for(int j=0;j<BET_ARES;j++)
		{
			//�Լ�����ע���
			TUserBetResult.i64UserBetCount[j]=m_i64UserBetCount[bDeskStation][j];
			//��������ע���
			TUserBetResult.i64AreaBetAllSum[j]=m_i64AreaBetAllSum[j];
		}
		SendGameData(bDeskStation,&TUserBetResult,sizeof(TUserBetResult),MDM_GM_GAME_NOTIFY,ASS_BET_RESULT,0);
	}
}

//�õ�����ʣ����ע��
long long CServerGameDesk::RemainingBettingNumber(BYTE byArea)
{
	long long betNum=0;
	for(int i=0;i<BET_ARES;i++)
	{
		if(byArea==i)
		{
			continue;
		}
		betNum+=m_i64AreaBetAllSum[i];
	}
	long long MaxBetNum=0;

	MaxBetNum = (m_pUserInfo[m_byNtStation]->m_UserData.i64Money + betNum) / G_iArePeiLv[byArea];
	
	return MaxBetNum;
}

//�Ƿ�����ׯ�б���
bool	CServerGameDesk::IsInWaitNtList(BYTE byDeskStation)
{
	vector<BYTE>::iterator ite=m_blNtWaitList.begin();
	for(;ite!=m_blNtWaitList.end();ite++)
	{
		if(byDeskStation==*ite)
		{
			return true;
		}
	}
	return false;
}

//�����û���֤
void	CServerGameDesk::SuperUserExamine(BYTE bDeskStation)
{
	SuperUserState TSuperUser;
	if (NULL == m_pUserInfo[bDeskStation])
	{
		return ;
	}
	if (IsSuperUser(bDeskStation))
	{
		// ���û�Ϊ�����û�, ֪ͨ�ͻ���
		TSuperUser.byDeskStation = bDeskStation;
		// �����ͻ���
		TSuperUser.bEnable = true;
		SendGameData(bDeskStation, &TSuperUser, sizeof(TSuperUser), MDM_GM_GAME_NOTIFY, ASS_SUPER_USER, 0);

	}
}

//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
bool	CServerGameDesk::IsSuperUser(BYTE byDeskStation)
{
	if (NULL == m_pUserInfo[byDeskStation])
	{
		return false;
	}
	for(int i=0; i<m_vlSuperID.size(); i++)
	{
		if (NULL != m_pUserInfo[byDeskStation])
		{
			//�Ҵ��˳������
			if (m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vlSuperID.at(i))
			{
				return true;
			}
		}
	}
	return false;
}

//��ȡ��Ϸ״̬��Ϣ����һ�ε��ã�
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	//��������Ϸ��ʼ���������״̬û�����õ����������Ϸ�д�Ǯ�����ڴ���������
	if (IsPlayGame(bDeskStation))
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;//���������Ϊͬ��״̬
	}
	// ��֤�����û�Ȩ��
	SuperUserExamine(bDeskStation);

	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.byRobNTtime	= m_byRobNTtime;		//��ׯʱ��
			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.byRolldiceTime =m_byRolldiceTime;      //ҡ��ʱ��
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��
			TGameStation.bShowChouMa    =m_bShowChouMa;
			TGameStation.byBetWaitTime	= m_byBetWaitTime;	//����ע�׶η��Ƶȴ�ʱ��				
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//�������׶η��Ƶȴ�ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����
			TGameStation.lSingleNoteLimit	= m_lSingleNoteLimit;     //�����ע����
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_FREE_STATUS:	//���н׶�
	case GS_ROB_NT:			//��ׯ�׶�
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.byRobNTtime	= m_byRobNTtime;		//��ׯʱ��
			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//����ע�׶η��Ƶȴ�ʱ��
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//�������׶η��Ƶȴ�ʱ��

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //�����ע����
			//ׯ����Ϣ�������ͳɼ���
			if(255==m_byNtStation)
			{
				TGameStation.i64NtGrade		= 0;	
			}
			else
			{
				TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	
			}
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//��ҳɿ�
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			//������ׯ�б�
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{
				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_BET_STATUS:	//��Ϸ��ע�׶�
		{

			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.byRobNTtime	= m_byRobNTtime;		//��ׯʱ��
			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//����ע�׶η��Ƶȴ�ʱ��
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//�������׶η��Ƶȴ�ʱ��

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //�����ע����
			//ׯ����Ϣ�������ͳɼ���

			TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	

			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//��ҳɿ�
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			//������ׯ�б�
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{
				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			TUpdateHistoryBet updateBet;//������ʷ��ע
			updateBet.i64SumAllBet=m_i64SumAllBet;
			updateBet.i64UserAllBet=GetUserBetCount(bDeskStation);//flog
			memcpy_s(updateBet.iChouMaCount, sizeof(updateBet.iChouMaCount), m_iChouMaCount, sizeof(updateBet.iChouMaCount));
			memcpy(updateBet.i64AreaBetAllSum,m_i64AreaBetAllSum,sizeof(updateBet.i64AreaBetAllSum));
			memcpy(updateBet.i64UserBetSum,m_i64UserBetCount[bDeskStation],sizeof(updateBet.i64UserBetSum));
			SendGameData(bDeskStation,&updateBet,sizeof(updateBet),MDM_GM_GAME_NOTIFY,ASS_UPDATE_BET,0);
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_ROLL_STATUS://ҡ���׶�
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.byRobNTtime	= m_byRobNTtime;		//��ׯʱ��
			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//����ע�׶η��Ƶȴ�ʱ��
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//�������׶η��Ƶȴ�ʱ��

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//��ҳɿ�
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //�����ע����
			//ׯ����Ϣ�������ͳɼ���

			TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			//������ׯ�б�
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{

				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			TUpdateHistoryBet updateBet;//������ʷ��ע
			updateBet.i64SumAllBet=m_i64SumAllBet;	
			updateBet.i64UserAllBet=GetUserBetCount(bDeskStation);//flog
			memcpy_s(updateBet.iChouMaCount, sizeof(updateBet.iChouMaCount), m_iChouMaCount, sizeof(updateBet.iChouMaCount));
			memcpy(updateBet.i64AreaBetAllSum,m_i64AreaBetAllSum,sizeof(updateBet.i64AreaBetAllSum));
			memcpy(updateBet.i64UserBetSum,m_i64UserBetCount[bDeskStation],sizeof(updateBet.i64UserBetSum));
			SendGameData(bDeskStation,&updateBet,sizeof(updateBet),MDM_GM_GAME_NOTIFY,ASS_UPDATE_BET,0);
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_SEND_STATUS://���ƽ׶�
		{

			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.byRobNTtime	= m_byRobNTtime;		//��ׯʱ��
			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//����ע�׶η��Ƶȴ�ʱ��
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//�������׶η��Ƶȴ�ʱ��

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//��ҳɿ�
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //�����ע����
			//ׯ����Ϣ�������ͳɼ���
			TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	

			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{

				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			TUpdateHistoryBet updateBet;//������ʷ��ע
			updateBet.i64SumAllBet=m_i64SumAllBet;	
			updateBet.i64UserAllBet=GetUserBetCount(bDeskStation);//flog
			memcpy_s(updateBet.iChouMaCount, sizeof(updateBet.iChouMaCount), m_iChouMaCount, sizeof(updateBet.iChouMaCount));
			memcpy(updateBet.i64AreaBetAllSum,m_i64AreaBetAllSum,sizeof(updateBet.i64AreaBetAllSum));
			memcpy(updateBet.i64UserBetSum,m_i64UserBetCount[bDeskStation],sizeof(updateBet.i64UserBetSum));
			SendGameData(bDeskStation,&updateBet,sizeof(updateBet),MDM_GM_GAME_NOTIFY,ASS_UPDATE_BET,0);
			SendAwardRecord(bDeskStation);
			break;
		}
	default:
		{
			break;
		}
	}

	NotiGameCount(bDeskStation);

	return true;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch(bCloseFlag)
	{
	case GF_NORMAL:	//��������
		{
			m_bGameStation	= GS_FREE_STATUS;
			memcpy(m_i64LastBet,m_i64UserBetCount,sizeof(m_i64LastBet));//flog
			//������Ϸ����
			ResetGameData();
			break;
		}
	case GF_SAFE:			//��Ϸ��ȫ����
		{
			//��ʼ����������
			ResetGameData();
			//���ׯ��
			m_i64PlayerGrade[m_byNtStation]=0;
			m_byNtStation	=255;//��סׯ��λ��
			m_iNtPlayCount	=0;
			m_bXiaZhuang = false;
			m_blNtWaitList.clear();	//��ׯ�б�
			m_bGameStation = GS_WAIT_SETGAME;
			break;
		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			if(m_bGameStation>GS_FREE_STATUS && m_bGameStation<GS_WAIT_NEXT)// ���ǿ��
			{
				memset(m_i64LastBet,0,sizeof(m_i64LastBet));//flog
				//���ǿ�� ���û��ע �ͼ�ʱ�����,ׯ�ҳ���
				if (m_i64UserBetSum[bDeskStation] <=0 && m_byNtStation!=bDeskStation)
				{
					//��Ӧ��ҳɼ�����
					MakeUserOffLine(bDeskStation);
				}
			}
			break;
		}
	case GFF_DISSMISS_FINISH:
		{
			SendScoreBoard();
			break;
		}
	}

	return __super::GameFinish(bDeskStation,bCloseFlag);;
}

//����ҡ����Ϣ
void CServerGameDesk::SendRolldice()
{
	//������Ϸ״̬
	m_bGameStation	= GS_ROLL_STATUS;
	RollDiceMsg rollMsg;
	rollMsg.byGameStation=m_bGameStation;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		//������Ϸ��Ϣ
		bool bSuccese = SendGameData(i,&rollMsg,sizeof(rollMsg),MDM_GM_GAME_NOTIFY,ASS_ROLL_DICE,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_ROLL_DICE---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}

	SetGameTimer(TIME_ROLL_TIME,m_byRolldiceTime);
}

//���Ϳ�����Ϣ
void CServerGameDesk::SendFree()
{
	//������Ϸ״̬
	m_bGameStation	= GS_FREE_STATUS;
	BeginFreeMsg freeMsg;
	freeMsg.bFlag=true;
	freeMsg.byFreeTime=m_byFreeTime;
	freeMsg.byNtStation=m_byNtStation;
	freeMsg.byNtcount=m_iNtPlayCount;
	freeMsg.byGameStation= m_bGameStation;         ///��ǰ����Ϸ״̬
	if(255==m_byNtStation)
	{
		freeMsg.i64NtScore=0;
	}
	else
	{
		freeMsg.i64NtScore=m_i64PlayerGrade[m_byNtStation];
	}
	int i=0;
	for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{
		freeMsg.byWaitNtList[i]=*ite;
		i++;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		//������Ϸ��Ϣ
		bool bSuccese = SendGameData(i,&freeMsg,sizeof(freeMsg),MDM_GM_GAME_NOTIFY,ASS_GAME_FREE,0);

		dwjlog.Format("yxxgame::�����[%d]����ASS_GAME_FREE---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	
	}
	//�������ж�ʱ��5��
	SetGameTimer(TIME_FREE_TIME,m_byFreeTime);
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation>GS_FREE_STATUS && m_bGameStation<GS_WAIT_NEXT)
	{
		if (m_byNtStation == bDeskStation)
		{
			return true;
		}
		else if (m_i64UserBetSum[bDeskStation] > 0)
		{
			return true;
		}
	}
	return false;
}

//�û��뿪��Ϸ��(����û�û��gamebegin�û��˳�����ô˺���)
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_i64PlayerGrade[bDeskStation]=0;

	//�������������ׯ��Ϣ
	ApplyLiveNtMsg TLeaveNt;
	TLeaveNt.byDeskStation = bDeskStation;
	DealUserApplyLeaveNt(&TLeaveNt);

	//�ص��Ժ��˳���ҵ�m_pUserInfo�ṹ��ͻ�Ϊ��
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	//�������������ׯ��Ϣ
	ApplyLiveNtMsg TLeaveNt;
	TLeaveNt.byDeskStation = bDeskStation;
	DealUserApplyLeaveNt(&TLeaveNt);

	__super::UserNetCut(bDeskStation,  pLostUserInfo);
	return true;
}

//�������
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	auto res = __super::UserSitDesk(pUserSit, pUserInfo);

	if(m_bIsBuy && 1 == GetDeskPlayerNum())			//���洴�������淨
	{
		SetPlayMode();
		memset(m_i64WinMoney, 0, sizeof(m_i64WinMoney));
		m_dstAward.clear();
	}

	return res;
}

//������Զ���ʼ���ɷ���˿���
BOOL CServerGameDesk::StartGame()
{
	KillTimer(TIME_FREE_TIME);
	ResetGameData();

	BYTE byLastNTStation = m_byNtStation;

	//ׯ�Ҳ����� ��ׯ��
	if (m_byNtStation != 255 && NULL == m_pUserInfo[m_byNtStation])
	{
		m_byNtStation	= 255;
		m_iNtPlayCount	= 0;
	}
	//ׯ����ׯ�������˸û�ׯ��
	if(m_byNtStation != 255 && m_iNtPlayLimite <= m_iNtPlayCount && m_iNtPlayLimite > 0)
	{
		m_byNtStation	= 255;
		m_iNtPlayCount	= 0;
	}
	//��Ҳ��㻻ׯ��
	if(!CanMinusPoint() && 255!=m_byNtStation && m_pUserInfo[m_byNtStation])
	{
		if((m_pUserInfo[m_byNtStation]->m_UserData.i64Money)<m_iApplyNtLimite)
		{
			m_byNtStation	= 255;
			m_iNtPlayCount	= 0;
		}
	}

	if (m_bXiaZhuang && m_byNtStation != 255)
	{
		m_byNtStation	= 255;
		m_iNtPlayCount	= 0;
	}
	//�ֻ�ׯ��
	if (255 == m_byNtStation)
	{
		ChangeNt(byLastNTStation);
	}

	//ׯ�Ҵ��� �ſ�ʼ��Ϸ
	if(255	!= m_byNtStation && m_pUserInfo[m_byNtStation])
	{
		GameBegin(ALL_ARGEE);//��Ϸ��ʼ
	}
	else
	{
		NoNtWait();
	}

	return true;
}

void CServerGameDesk::NoNtWait()
{
	m_bGameStation	= GS_WAIT_SETGAME;	//�ȴ�ͬ��״̬
	m_blNtWaitList.clear();
	//������Ϣ���ͻ��� ���߿ͻ��� ��ׯ�ȴ�
	memset(m_i64PlayerGrade,0,sizeof(m_i64PlayerGrade));
	m_byNtStation	= 255;//��סׯ��λ��
	m_iNtPlayCount	= 0;
	m_bXiaZhuang	= false;

	WaiteNtMsg	TWaiteNt;
	TWaiteNt.bWait = true;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&TWaiteNt,sizeof(TWaiteNt),MDM_GM_GAME_NOTIFY,ASS_WAITE_NT,0);
	}
}

//��ׯ��
bool CServerGameDesk::ChangeNt(BYTE byLastNTStation)
{
	//��ׯ��
	if(!m_bIsBuy)
	{
		vector<BYTE>::iterator ite=m_blNtWaitList.begin();
		for(;ite!=m_blNtWaitList.end();)
		{
			if (m_pUserInfo[*ite])
			{
				m_byNtStation=*ite;
				m_blNtWaitList.erase(ite);
				m_iNtPlayCount=0;
				m_i64PlayerGrade[m_byNtStation]=0;
				return true;
			}
			ite = m_blNtWaitList.erase(ite);
		}
	}
	else
	{
		vector<BYTE> tmp_User;
		for(int i = 0; i < PLAY_COUNT; ++i)
		{
			if(m_pUserInfo[i] != NULL)
			{
				if(CanMinusPoint())
					tmp_User.push_back(i);
				else if (m_pUserInfo[i]->m_UserData.i64Money >= m_iApplyNtLimite)
					tmp_User.push_back(i);
			}
		}

		if(1 == tmp_User.size())
		{
			m_byNtStation = *tmp_User.begin();
			return true;
		}
		else if(tmp_User.size() > 1)
		{
			int tryCount = 0;
			do 
			{
				++tryCount;
				m_byNtStation = tmp_User[rand() % tmp_User.size()];
			} while (byLastNTStation != 255 && byLastNTStation == m_byNtStation && tryCount < 50);
			return true;
		}
	}
	
	return false;
}

//������ׯ�б�
void CServerGameDesk::ClearNtList()
{
	for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();)
	{
		if((NULL==m_pUserInfo[*ite])||(!CanMinusPoint() && m_pUserInfo[*ite]->m_UserData.i64Money < m_iApplyNtLimite ))
		{
			m_blNtWaitList.erase(ite);
		}
		else
		{
			ite++;
		}
	}
}

//��¼��������Ӯ���
void CServerGameDesk::RecordRobotWinMoney()
{
	if (!m_bIsRobotWinAndLostAutoCtrl)
		return;

	long long i64Temp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual)
		{
			i64Temp+=m_i64PlayCoin[i];
		}
	}
	CString sTemp="";
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
	//����ǰ������Ӯ��Ǯд�������ļ�����
	m_iResetRobotHaveWinMoney+=i64Temp;
	sTemp.Format("%I64d",m_iResetRobotHaveWinMoney);
	f.SetKeyValString(szSec,"ReSetRobotHaveWinMoney ",sTemp);
}

//�ܷ񵽸��֣���ͨ��ҳ������ֲ���ҳ������ֲ����ֳ������������ҳ��ȶ����ܵ����֣�ֻ�д���������ֳ����ԣ�
bool CServerGameDesk::CanMinusPoint()
{
	//��ƽ̨�ṩ�˱�ʶ
	//m_CurrencyType�����Ľ��㷽ʽ(������ר���ֶ�).0.��ҽ���   1---���ֲ����ֽ���    2----���еĻ��ֽ���

	return m_CurrencyType == 2;
}

//���������
int CServerGameDesk::GetDiceNum()
{
	return m_byDiceNum;
}

//��ׯ���б��л�ȡ��ׯ��(��ׯ)
bool CServerGameDesk::OnGetZhuang()
{
	if(!m_blNtWaitList.empty())			//����ׯ�б����ѡһ�˵�ׯ
	{
		m_byNtStation = m_blNtWaitList[rand() % m_blNtWaitList.size()];
		return true;
	}
	else
	{
		vector<BYTE> tmp_User;
		for(int i = 0; i < PLAY_COUNT; ++i)
		{
			if(m_pUserInfo[i] != NULL)
			{
				tmp_User.push_back(i);
			}
		}

		if(!tmp_User.empty())
		{
			m_byNtStation = tmp_User[rand() % tmp_User.size()];
			return true;
		}
	}

	return false;
}

//�������������ע��Ϣ
void CServerGameDesk::DealUserBetInfo(BYTE byDeskStation, void *pData)
{
	TMSG_BET_INFO_RSP msg;			
	for(int i = 0, idx = 0; i < PLAY_COUNT && idx < BET_INFO_MAX; ++i)
	{
		if(NULL == m_pUserInfo[i] || i == m_byNtStation)
			continue;

		msg.i64UserBetInfos[idx].byBetStation = i;
		for (int j = 0; j < BET_ARES; ++j)
		{
			msg.i64UserBetInfos[idx].i64UserBetCount[j] = m_i64PeopleBetCount[i][j];
		}

		++idx;
	}
	SendGameData(byDeskStation, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, S_C_BET_INFO_RSP, 0);

	//���䷢��Ų��Ϣ
	TMSG_NUO_INFO_RSP nuo_msg;
	for (int i = 0, idx = 0; i < PLAY_COUNT && idx < BET_INFO_MAX; ++i)
	{
		if (NULL == m_pUserInfo[i] || i == m_byNtStation)
			continue;

		nuo_msg.sUserNuoInfos[idx].byBetStation = i;

		memcpy_s(nuo_msg.sUserNuoInfos[idx].sUserNuoData, sizeof(nuo_msg.sUserNuoInfos[idx].sUserNuoData),
			m_sUserNuoData[i], sizeof(m_sUserNuoData[i]));

		++idx;
	}
	SendGameData(byDeskStation, &nuo_msg, sizeof(nuo_msg), MDM_GM_GAME_NOTIFY, S_C_NUO_INFO_RSP, 0);
}

//�������Ų����
void CServerGameDesk::DealUserNuo(BYTE byDeskStation, void *pData)
{
	TMSG_CHANGE_BET_REQ *pUserNuo = (TMSG_CHANGE_BET_REQ *)pData;

	TMSG_CHANGE_BET_RSP change_Msg;
	change_Msg.byErrorCode = 0;

	int iChangeMoney = 0;

	for (int i = 0; i < BETSTYLE; ++i)
	{
		if (pUserNuo->byChangeCount[i] > 0)
		{
			iChangeMoney += (G_iChouMaValue[i] * pUserNuo->byChangeCount[i]);
		}
		
	}

	BYTE byOriBetArea = pUserNuo->byOriBetArea;
	BYTE byNewBetArea = pUserNuo->byNewBetArea;
	long long i64RemainBetNum = RemainingBettingNumber(byNewBetArea);
	
	if(!GetAreaBetVaild(byOriBetArea) || !GetAreaBetVaild(byNewBetArea))
	{
		change_Msg.byErrorCode = 4;		//ѡ���������
	}
	else if(m_i64AreaBetAllSum[byOriBetArea] < iChangeMoney)
	{
		change_Msg.byErrorCode = 1;		//����ԭ��ע������עֵ
	}
	else if((m_i64AreaBetAllSum[byNewBetArea] + iChangeMoney) > GetAreaBetLimite(byNewBetArea))	
	{
		change_Msg.byErrorCode = 2;		//������������
	}
	else if(!CanMinusPoint() && (m_i64AreaBetAllSum[byNewBetArea]+iChangeMoney) > i64RemainBetNum)
	{
		change_Msg.byErrorCode = 2;		//������������
	}
	else if(m_byNuoCount[byDeskStation] >= NUO_COUNT)
	{
		change_Msg.byErrorCode = 3;		//����Ų���ƴ���
	}

	SendGameData(byDeskStation, &change_Msg, sizeof(change_Msg), MDM_GM_GAME_NOTIFY, S_C_CHANGE_BET_RSP, 0);

	if(0 == change_Msg.byErrorCode)	//�ɹ�
	{
		//��¼����Ų����
		int curNuoCount = m_byNuoCount[byDeskStation];
		if (curNuoCount < NUO_COUNT)
		{
			m_sUserNuoData[byDeskStation][curNuoCount].byBefore = byOriBetArea;
			m_sUserNuoData[byDeskStation][curNuoCount].byAfter = byNewBetArea;
			m_sUserNuoData[byDeskStation][curNuoCount].iNuoMoney = iChangeMoney;

			m_i64UserBetCountEx[byDeskStation][byNewBetArea] += iChangeMoney;
		}
		
		++m_byNuoCount[byDeskStation];		//����Ų����

		//�����������ע
		m_i64AreaBetAllSum[byOriBetArea] -= iChangeMoney;
		m_i64AreaBetAllSum[byNewBetArea] += iChangeMoney;

		TMSG_UPDATE_BET_NTF update_msg;

		//��ͷ��˷��͸�����ע����
		for (int i = 0; i < PLAY_COUNT; i ++)
		{
			if (NULL==m_pUserInfo[i])
			{
				continue;
			}

			if(m_bSuperDeskStation == i && !m_bShowRobotBet)//���˿���ֻ��ʾ��ʵ�����ע
			{
				for(int j=0;j<BET_ARES;j++)
				{
					//�Լ�����ע���
					update_msg.i64UserBetCount[j] = m_i64PeopleBetCount[i][j];
					//��������ע���
					update_msg.i64AreaBetAllSum[j] = m_i64PeopleAreaBetAllSum[j];
				}
			}
			else//����ʾ
			{
				for(int j=0;j<BET_ARES;j++)
				{
					//�Լ�����ע���
					update_msg.i64UserBetCount[j] = m_i64PeopleBetCount[i][j];
					//��������ע���
					update_msg.i64AreaBetAllSum[j] = m_i64AreaBetAllSum[j];
				}
			}
			
			SendGameData(i, &update_msg, sizeof(update_msg), MDM_GM_GAME_NOTIFY, S_C_UPDATE_BET_NTF, 0);
		}
	}
}

//���������ע����
long long CServerGameDesk::GetAreaBetLimite(int iArea)
{
	//������ѡ��ÿ����������������1000��ֻ�㵥Ѻ������ôÿ�൥���Ĵ�����ע����Ϊ200������Ϊ��100��ע��
	long long tmp_AreaBetLimite = m_iAreaBetLimite;
	if(iArea >= 6 && iArea <= 11)		//����(10%)
	{
		tmp_AreaBetLimite = m_iAreaBetLimite * 10 / 100;
	}
	else if(iArea >= 12 && iArea <= 46)	//ÿ�൥���Ĵ�(20%)
	{
		tmp_AreaBetLimite = m_iAreaBetLimite * 20 / 100;
	}

	return tmp_AreaBetLimite;
}

//��ע�����Ƿ���Ч
bool CServerGameDesk::GetAreaBetVaild(int iArea)
{
	if(3 == GetDiceNum() && (iArea < DanYa_Start || iArea > RenSan_End))
	{
		return false;
	}
	else if(2 == GetDiceNum() && (iArea < DanYa_Start || iArea > RenEr_End))
	{
		return false;
	}

	return true;
}

//֪ͨ��Ϸ����
void CServerGameDesk::NotiGameCount(BYTE byDeskStation)
{
	if(m_bIsBuy)
	{
		TMSG_GAME_COUNT_NTF msg;
		msg.iRemainCount = m_iVipGameCount - m_iRunGameCount;
		msg.iTotalCount = m_iVipGameCount;

		if(255 == byDeskStation)
		{
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL != m_pUserInfo[i])
				{
					SendGameData(i, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, S_C_GAME_COUNT_NTF, 0);
				}
			}
		}
		else if(byDeskStation < PLAY_COUNT && NULL != m_pUserInfo[byDeskStation])
		{
			SendGameData(byDeskStation, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, S_C_GAME_COUNT_NTF, 0);
		}
	}
}

//���������������
void CServerGameDesk::RandDice()
{
	m_byWinPrize[0] = 1 + rand() % 6;
	m_byWinPrize[1] = 1 + rand() % 6;

	if(3 == GetDiceNum())					//�ü�������
	{
		m_byWinPrize[2] = 1 + rand() % 6;
	}
	else
	{
		m_byWinPrize[2] = 255;
	}
}

//�˴����ؽ����������10�����������ʱ��˴˽�����10
int CServerGameDesk::GetAresResult(int iArea)
{
	//�ж���ע�����Ƿ���Ч
	if(!GetAreaBetVaild(iArea))		
	{
		return 0;	
	}

	if(iArea >= DanYa_Start && iArea <= DanYa_End)			//��ѹ��һ��1����������2����������5��
	{
		int count = 0;
		int ret = -1;
		for(int i = 0; i < 3; ++i)
		{
			if(m_byWinPrize[i] == G_iNoToPoint[iArea])
			{
				++count;
			}
		}
		
		if(1 == count)
		{
			ret = 10;
		}
		else if(2 == count)
		{
			ret = 20;
		}
		else if(3 == count)
		{
			ret = 50;
		}

		return ret;
	}
	else if(iArea >= BaoZi_Start && iArea <= BaoZi_End)		//����
	{
		int idx = iArea - BaoZi_Start;
		int ret = isBaozi(m_byWinPrize, G_iBaoZiPoint[idx]);
		return ret;
	}
	else if(iArea >= RenEr_Start && iArea <= RenEr_End)		//��ѡ��
	{
		int idx = iArea - RenEr_Start;
		int ret = shuangxx(m_byWinPrize,G_iChooseTwo[idx][0],G_iChooseTwo[idx][1]);
		return ret;
	}
	else if(iArea >= RenSan_Start && iArea < RenSan_End)	//��ѡ��
	{
		int idx = iArea - RenSan_Start;
		int ret = sanxx(m_byWinPrize,G_iChooseThree[idx][0],G_iChooseThree[idx][1],G_iChooseThree[idx][2]);
		return ret;
	}

	return 0;
}

__int64	CServerGameDesk::CountAIWinMoney() 
{
	__int64	 AIWinMoney = 0;

	int res[BET_ARES];
	memset(res, 0, sizeof(res));

	for(int i = 0; i < BET_ARES; ++i)		//��ȡÿ������Ľ��
	{
		res[i] = GetAresResult(i);
	}

	if (m_byNtStation >= 0 && m_byNtStation < PLAY_COUNT && NULL != m_pUserInfo[m_byNtStation])
	{
		if(m_pUserInfo[m_byNtStation]->m_UserData.isVirtual)	//�����������ׯ��,������ʵ��ҵ�Ǯ
		{
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL == m_pUserInfo[i] || m_pUserInfo[i]->m_UserData.isVirtual || m_i64UserBetSum[i] <= 0 || i == m_byNtStation)
					continue;

				//������ʵ��Ҹ������ӮǮ��Ŀ
				for(int j = 0; j < BET_ARES; ++j)
				{
					if(m_i64UserBetCount[i][j] > 0)
					{
						//ͳ����ʵ���Ӯ��Ǯ,��ʵ���ӮǮ����AI��Ǯ
						AIWinMoney -= m_i64UserBetCount[i][j] * res[j] / 10;
					}	

					if (m_i64UserBetCountEx[i][j] > 0)
					{
						//ͳ����ʵ���Ӯ��Ǯ,��ʵ���ӮǮ����AI��Ǯ
						AIWinMoney -= m_i64UserBetCountEx[i][j] * res[j] / 10;
					}
				}
			}
		}
		else
		{	
			//�����ʵ�����ׯ��,���������Ӯ��Ǯ
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL == m_pUserInfo[i] || !m_pUserInfo[i]->m_UserData.isVirtual || m_i64UserBetSum[i] <= 0 || i == m_byNtStation)
					continue;

				//����AI�������ӮǮ��Ŀ
				for(int j = 0; j < BET_ARES; ++j)
				{
					if(m_i64UserBetCount[i][j] > 0)
					{
						//ͳ��AIӮ��Ǯ
						AIWinMoney += m_i64UserBetCount[i][j] * res[j] / 10;
					}	

					if (m_i64UserBetCountEx[i][j] > 0)
					{
						//ͳ����ʵ���Ӯ��Ǯ,��ʵ���ӮǮ����AI��Ǯ
						AIWinMoney += m_i64UserBetCountEx[i][j] * res[j] / 10;
					}
				}
			}
		}
	}

	return AIWinMoney;
}

//���ô��������淨
void CServerGameDesk::SetPlayMode()
{
	tagPM *m_tagPM = (tagPM *)m_szDeskConfig;

	if(m_tagPM->iBetLimit > 0)
	{
		m_iAreaBetLimite = m_tagPM->iBetLimit;	//����������ע����
	}

	if(m_tagPM->byReadyTime > 0)
	{
		m_byFreeTime = m_tagPM->byReadyTime;	//׼��ʱ��(����ʱ��)
	}

	if(m_tagPM->byRobNtTime > 0)
	{
		m_byRobNTtime = m_tagPM->byRobNtTime;	//��ׯʱ��
	}

	if(m_tagPM->byBetTime > 0)
	{
		m_byBetTime = m_tagPM->byBetTime;		//��עʱ��
	}

	if(m_tagPM->byNuoLimit >= 0 && m_tagPM->byNuoLimit <= 100)
	{
		m_byNuoLimit = m_tagPM->byNuoLimit;		//Ų����
	}

	if(2 == m_tagPM->byDiceNum || 3 == m_tagPM->byDiceNum)
	{
		m_byDiceNum = m_tagPM->byDiceNum;
	}

	m_iPlayingMethod = m_tagPM->byDiceNum;
}

//�Ƿ���type�淨
bool CServerGameDesk::IsPlayModeVaild(int type)
{
	return (m_iPlayingMethod & type) > 0;
}

void CServerGameDesk::SendScoreBoard()
{
	if (m_iRunGameCount > 0 || (m_bGameStation > GS_FREE_STATUS && m_bGameStation < GS_WAIT_NEXT))
	{
		TMSG_CALCULATE_BOARD msg;
		memcpy(msg.i64WinMoney, m_i64WinMoney, sizeof(msg.i64WinMoney));

		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			if (NULL != m_pUserInfo[i])
			{
				SendGameData(i, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, S_C_CALCULATE_BOARD, 0);
			}
		}

		memset(m_i64WinMoney, 0, sizeof(m_i64WinMoney));
	}
}

//�޸Ľ�������1(ƽ̨->��Ϸ)
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return true;
}

//�޸Ľ�������2(ƽ̨->��Ϸ)
bool CServerGameDesk::SetRoomPondEx(long long	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return true;
}

//�޸Ľ�������3(ƽ̨->��Ϸ)
bool CServerGameDesk::SetRoomPondEx(long long	iReSetAIHaveWinMoney)
{
	return true;
}
//����ʤ��
BOOL CServerGameDesk::Judge()
{
	return true;
}
//ʤ
BOOL CServerGameDesk::JudgeWiner()
{
	return true;
}
//��
BOOL CServerGameDesk::JudgeLoser()
{
	return false;
}
