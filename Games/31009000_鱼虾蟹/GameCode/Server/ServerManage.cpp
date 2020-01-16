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
	//插入时间
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

// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效  
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
	//注册异常处理函数  
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);  

	//使SetUnhandledExceptionFilter  
	DisableSetUnhandledExceptionFilter();  
} 


//构造函数
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation	= GS_WAIT_SETGAME;	//无庄等待状态
	m_byBetWaitTime=2;//到下注阶段发牌等待时间
	m_byOpenAwardWaitTime=2;//到下注阶段发牌等待时间
	m_iRemaindTime=0;//时钟清零

	m_vlSuperID.clear();//保存超端玩家的容器
	m_blNtWaitList.clear();	//上庄列表
	m_iNtPlayCount=0;//庄家坐庄次数
	m_byNtStation = 255;//庄家位置
	m_bXiaZhuang = false;

	memset(m_i64LastBet,0,sizeof(m_i64LastBet));
	//玩家成绩
	memset(m_i64PlayerGrade,0,sizeof(m_i64PlayerGrade));
	//玩家输赢金币
	memset(m_i64PlayCoin,0,sizeof(m_i64PlayCoin));
	//返还的金币数
	memset(m_i64ReturnMoney,0,sizeof(m_i64ReturnMoney));
	m_bSuperDeskStation=255;						//超端用户座位号
	m_bShowRobotBet=true;							//超端用户控制机器人下注显示
	memset(G_iChouMaValue,0,sizeof(G_iChouMaValue));
	ResetGameData();
	memset(m_i64WinMoney, 0, sizeof(m_i64WinMoney));
	InitMinDump();
}

//析构函数
CServerGameDesk::~CServerGameDesk(void)
{
}
//----------------------------------------------------------------------------------------------------
//初始化桌子数据
bool	CServerGameDesk::InitDeskGameStation()
{
	LoadIni();
	if(m_pDataManage)
	{
		LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	}
	return true;
}
//加载ini配置文件
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_iNtPlayLimite = f.GetKeyVal(key,"NtMaxPlay",10);		//庄家坐庄次数限制
	m_iApplyNtLimite=f.GetKeyVal(key,"shangzhuang",10000);		//上庄条件
	m_iAreaBetLimite=f.GetKeyVal(key,"AreaBetLimite",1000);		//区域下注限制
	m_lSingleNoteLimit=f.GetKeyVal(key,"SingleNoteLimit",1000);   //单个人下注上限
	m_lRemainedNotes=m_lSingleNoteLimit; //游戏刚开始，剩余下注数等于当个人下注上限
	//碰碰车
	//各个阶段的时间

	m_byRobNTtime=f.GetKeyVal(key,"RobNTtime",10);		//抢庄阶段时间
	if(m_byRobNTtime<1)
	{
		m_byRobNTtime=10;
	}

	m_byFreeTime=f.GetKeyVal(key,"freetime",5);			//空闲阶段时间
	if(m_byFreeTime<1)
	{
		m_byFreeTime=5;
	}
	m_byBetTime=f.GetKeyVal(key,"xiazhutime",15);			//下注阶段时间
	if(m_byBetTime<1)
	{
		m_byBetTime=15;
	}
	m_byRolldiceTime=f.GetKeyVal(key,"rolldicetime",5);		//摇骰阶段时间
	if(m_byRolldiceTime<1)
	{
		m_byRolldiceTime=5;
	}
	m_byOpenCardTime=f.GetKeyVal(key,"opencardtime",5);//开奖阶段时间
	if(m_byOpenCardTime<1)
	{
		m_byOpenCardTime=5;
	}
	m_bShowChouMa=f.GetKeyVal(key,"showchouma",0);

	m_byNuoLimit = f.GetKeyVal(key,"NuoLimit",30);	//挪限制
	if(m_byNuoLimit > 100)
	{
		m_byNuoLimit = 100;
	}

	m_byDiceNum = f.GetKeyVal(key,"DiceNum",3);		//骰子数
	if(m_byDiceNum != 2 && m_byDiceNum != 3)
	{
		m_byDiceNum = 3;
	}

	m_iPlayingMethod = (PM_MEM_CHUAN | PM_BAO_ZI | PM_NUO | PM_NUO_NEEDBET | PM_3DICE_GETTWO);

	m_byMaxNtPeople=f.GetKeyVal(key,"MaxNtPeople",20);;//最大上庄人数
	//超端
	key = "SuperUser";
	// 判断超级用户在服务器中是否配置
	int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<iCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		lUserID = f.GetKeyVal(key, strText, 0);
		//将所有超端ID 保存到超端容器中
		m_vlSuperID.push_back(lUserID);
		CString cs;
		cs.Format("super 超端ID=%d",lUserID);
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

	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,iRoomID);

	m_iNtPlayLimite = f.GetKeyVal(szSec,"NtMaxPlay",m_iNtPlayLimite);		//庄家坐庄次数限制
	m_iApplyNtLimite=f.GetKeyVal(szSec,"shangzhuang",m_iApplyNtLimite);		//上庄条件
	m_iAreaBetLimite=f.GetKeyVal(szSec,"AreaBetLimite",m_iAreaBetLimite);		//区域下注限制
	m_lSingleNoteLimit=f.GetKeyVal(szSec,"SingleNoteLimit",m_lSingleNoteLimit);           //单个人下注上限
	m_lRemainedNotes=m_lSingleNoteLimit; //游戏刚开始，剩余下注数等于当个人下注上限
	//碰碰车
	//各个阶段的时间

	m_byRobNTtime=f.GetKeyVal(szSec,"RobNTtime",m_byRobNTtime);		//抢庄阶段时间
	if(m_byRobNTtime<1)
	{
		m_byRobNTtime=10;
	}

	m_byFreeTime=f.GetKeyVal(szSec,"freetime",m_byFreeTime);			//空闲阶段时间
	if(m_byFreeTime<1)
	{
		m_byFreeTime=5;
	}
	m_byBetTime=f.GetKeyVal(szSec,"xiazhutime",m_byBetTime);			//下注阶段时间
	if(m_byBetTime<1)
	{
		m_byBetTime=15;
	}
	m_byRolldiceTime=f.GetKeyVal(szSec,"rolldicetime",m_byRolldiceTime);//摇骰阶段时间
	if(m_byRolldiceTime<1)
	{
		m_byRolldiceTime=5;
	}
	m_byOpenCardTime=f.GetKeyVal(szSec,"opencardtime",m_byOpenCardTime);//开奖阶段时间
	if(m_byOpenCardTime<1)
	{
		m_byOpenCardTime=5;
	}
	m_bShowChouMa=f.GetKeyVal(szSec,"showchouma",m_bShowChouMa);

	m_byMaxNtPeople=f.GetKeyVal(szSec,"MaxNtPeople",m_byMaxNtPeople);;//最大上庄人数

	m_byNuoLimit = f.GetKeyVal(szSec,"NuoLimit",m_byNuoLimit);	//挪限制
	if(m_byNuoLimit > 100)
	{
		m_byNuoLimit = 100;
	}

	m_byDiceNum = f.GetKeyVal(szSec,"DiceNum",m_byDiceNum);		//骰子数
	if(m_byDiceNum != 2 && m_byDiceNum != 3)
	{
		m_byDiceNum = 3;
	}

	m_bIsRobotWinAndLostAutoCtrl = f.GetKeyVal(szSec,"IsRobotWinAndLostAutoCtrl",0);				//是否开启机器人输赢自动控制

	//机器人奖池配置
	m_iRobotWinMoneyA1	= f.GetKeyVal(szSec,"RobotWinMoneyA1",500000);						/*<机器人赢钱区域1*/
	m_iRobotWinMoneyA2	= f.GetKeyVal(szSec,"RobotWinMoneyA2",5000000);						/*<机器人赢钱区域2*/
	m_iRobotWinMoneyA3	= f.GetKeyVal(szSec,"RobotWinMoneyA3",50000000);						/*<机器人赢钱区域3*/

	//机器人在各个奖池赢钱概率控制
	m_iRobotWinLuckyAtA1	= f.GetKeyVal(szSec,"RobotWinLuckyAtA1",90);						/*<机器人在区域1赢钱的概率*/
	m_iRobotWinLuckyAtA2	= f.GetKeyVal(szSec,"RobotWinLuckyAtA2",50);						/*<机器人输赢控制：机器人在区域2赢钱的概率*/
	m_iRobotWinLuckyAtA3	= f.GetKeyVal(szSec,"RobotWinLuckyAtA3",30);						/*<机器人输赢控制：机器人在区域3赢钱的概率*/
	m_iRobotWinLuckyAtA4	= f.GetKeyVal(szSec,"RobotWinLuckyAtA4",5);						/*<机器人输赢控制：机器人在区域4赢钱的概率*/

	//记录机器人赢钱数目
	m_iResetRobotHaveWinMoney	= f.GetKeyVal(szSec,"ReSetRobotHaveWinMoney",0);				/*<机器人输赢控制：直接从配置文件中读取机器人已经赢钱的数目*/
	return true;
}

//拦截玩家强退消息
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
			dwjlog.Format("yxxtest::居然收到了玩家强退的消息pNetHead->bAssistantID=%d",pNetHead->bAssistantID);
			OutputDebugString(dwjlog);
			return true ; 
		}
	}

	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//清所有计时器
void CServerGameDesk::KillAllTimer()
{
	
}

//启动游戏计时器
void	CServerGameDesk::SetGameTimer(UINT uTimerID, BYTE byTime)
{
	KillTimer(TIME_ONE_SECOND);
	m_iRemaindTime = byTime;
	SetTimer(TIME_ONE_SECOND ,1000) ;
	SetTimer(uTimerID ,byTime*1000) ;
}

//重置游戏状态
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return TRUE;
}

/*--------------------------------------------------------------*/
//游戏开始
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i])//玩家不存在就
		{
			continue;
		}
		m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//所有玩家置为同意状态
	}	
	if (__super::GameBegin(bBeginFlag)==false)
	{
		GameFinish(0,GF_SAFE);
		return false;
	}
	if (!m_bIsBuy && (255 == m_byNtStation || NULL == m_pUserInfo[m_byNtStation]))	//金币场庄家不存在不开始游戏
	{
		GameFinish(0, GF_SAFE);
		return false;
	}

	LARGE_INTEGER nFrequency;

	if(QueryPerformanceFrequency(&nFrequency))
	{
		LARGE_INTEGER nStartCounter;
		QueryPerformanceCounter(&nStartCounter);
		srand((unsigned)nStartCounter.LowPart);		//重置随机数种子
	}	
	else
		srand((unsigned)GetTickCount());	//重置随机数种子

	//清空游戏临时数据
	ResetGameData();

	NotiGameCount();

	if(m_bIsBuy && 255 == m_byNtStation)
	{
		m_bGameStation	= GS_ROB_NT;
		m_blNtWaitList.clear();

		//通知抢庄
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

	//创建房间抢庄过后情况庄家列表，只有开局第一次需要抢庄
	if(m_bIsBuy)
	{
		m_blNtWaitList.clear();
	}

	//坐庄次数
	m_iNtPlayCount++;

	m_bXiaZhuang	= false;
	//设置游戏状态
	m_bGameStation	= GS_BET_STATUS;
	BeginBetMsg betmsg;
	betmsg.byNtStation	= m_byNtStation;
	betmsg.byNtCount	= m_iNtPlayCount;	//庄家坐庄次数
	betmsg.byGameStation= m_bGameStation;	//当前的游戏状态
	betmsg.byNtStation	= m_byNtStation;
	//庄家成绩

	betmsg.i64NtScore=m_i64PlayerGrade[m_byNtStation];

	int i=0;
	for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{
		betmsg.byNtWaitList[i]=*ite;
		i++;
	}
	//发送下注消息		
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&betmsg,sizeof(betmsg),MDM_GM_GAME_NOTIFY,ASS_BEGIN_BET,0);
	}
	
	//启动下注定时器
	SetGameTimer(TIME_XIAZHU_TIME,m_byBetTime);

	RandDice();

	return TRUE;
}

//重置数据
void	CServerGameDesk::ResetGameData()
{
	m_iRemaindTime=0;//时钟清零
	//清除下注情况
	memset(m_i64UserBetCount,0,sizeof(m_i64UserBetCount));
	memset(m_i64AreaGain,0,sizeof(m_i64AreaGain));
	memset(m_i64AreaBetAllSum,0,sizeof(m_i64AreaBetAllSum));
	memset(m_i64UserBetSum,0,sizeof(m_i64UserBetSum));
	//总下注数
	m_i64SumAllBet = 0;
	//真实玩家
	//各真实玩家在每个区域的下注情况
	memset(m_i64PeopleBetCount,0,sizeof(m_i64PeopleBetCount));
	//各区域的真实玩家的总下注
	memset(m_i64PeopleAreaBetAllSum,0,sizeof(m_i64PeopleAreaBetAllSum));
	//各真实玩家的总下注
	memset(m_i64PeopleBetSum,0,sizeof(m_i64PeopleBetSum));					
	//真实玩家总下注数			
	m_i64PeopleSumAllBet=0;		

	//玩家挪次数
	memset(m_byNuoCount, 0, sizeof(m_byNuoCount));

	//筹码记录
	memset(m_iChouMaCount, 0, sizeof(m_iChouMaCount));

	//玩家挪情况
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		for (int j = 0; j < NUO_COUNT; ++j)
		{
			m_sUserNuoData[i][j].Clear();
		}
	}

	memset(m_i64UserBetCountEx, 0, sizeof(m_i64UserBetCountEx));

	//清除結算信息
	//玩家输赢金币
	memset(m_i64PlayCoin,0,sizeof(m_i64PlayCoin));
	//返还的金币数
	memset(m_i64ReturnMoney,0,sizeof(m_i64ReturnMoney));
	memset(m_byWinPrize,255,sizeof(m_byWinPrize));

	m_bIsSuperControl=FALSE;							
	m_bSuperDeskStation=255;						//超端用户座位号
	m_bShowRobotBet=true;							//超端用户控制机器人下注显示
	
}

//获取玩家剩余多少钱
long long	CServerGameDesk::GetUserRemaindMoney(BYTE byDeskStation)
{
	if (NULL != m_pUserInfo[byDeskStation])
	{
		return m_pUserInfo[byDeskStation]->m_UserData.i64Money - GetUserBetCount(byDeskStation);
	}
	return 0;
}

//获取玩家下注总额
long long		CServerGameDesk::GetUserBetCount(BYTE byDeskStation)
{
	long long iUserBetCount = 0;
	for (int i=0; i<BET_ARES;i++)
	{
		iUserBetCount += m_i64UserBetCount[byDeskStation][i];
	}
	return iUserBetCount;
}

//定时器消息
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_ONE_SECOND:	//一秒剩余计时器 计算个各阶段的剩余时间
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
	case TIME_XIAZHU_TIME://下注阶段
		{
			KillTimer(TIME_XIAZHU_TIME);
			SendRolldice();
			break;
		}
	case TIME_ROLL_TIME://摇骰阶段
		{
			KillTimer(TIME_ROLL_TIME);
			SendAllCard();
			break;
		}
	case TIME_SEND_TIME://开奖阶段
		{
			//通知客服端显示结算
			KillTimer(TIME_SEND_TIME);
			openAward();
			break;	
		}
	case TIME_GAME_FINISH:	//游戏结束定时器
		{
			KillTimer(TIME_GAME_FINISH);
			//正常结束
			GameFinish(0,GF_NORMAL);

			//清理退出用户成绩
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL==m_pUserInfo[i])
				{
					m_i64PlayerGrade[i]=0;
				}
			}
			//发送空闲消息
			SendFree();
			break;
		}
	case TIME_FREE_TIME://空闲阶段
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

//开奖
void CServerGameDesk::openAward()
{
	OutputDebugString("yxxgame1::开奖");
	//统计玩家金币
	accountPlayCoin();
	rememberLuzi();
	//记录机器人所赢金币
	RecordRobotWinMoney();
	//存入数据库
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

	//发送开奖消息	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}

		memcpy(beginAward.i64AreaGain, m_i64AreaGain[i], sizeof(beginAward.i64AreaGain));

		if (m_bIsBuy)
		{
			m_i64WinMoney[i] += m_i64PlayCoin[i];		//记录大结算成绩
		}

		SendGameData(i, &beginAward, sizeof(beginAward), MDM_GM_GAME_NOTIFY, ASS_GAME_OPEN, 0);

		SendAwardRecord(i);
	}

	if (m_bIsBuy && m_iRunGameCount >= m_iVipGameCount)
	{
		SendScoreBoard();
	}

	//启动游戏结束计时器
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
	//设置游戏状态
	m_bGameStation	= GS_SEND_STATUS;
	BeginSendCardMsg SendCard;
	SendCard.byGameStation=m_bGameStation;

	if(m_bIsRobotWinAndLostAutoCtrl && FALSE==m_bIsSuperControl)//如果是机器人自动控制，不是超端控制
	{
		RobotAutoControl();
	}
	//开奖位置
	memcpy(SendCard.byWinPrize,m_byWinPrize,sizeof(SendCard.byWinPrize));
	//memcpy(SendCard.iRunTime,m_iRunTime,sizeof(SendCard.iRunTime));
	//发送发牌消息包
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		bool bSuccese = SendGameData(i,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SHOW_DICE,0);

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_SHOW_DICE---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}

	//启动开奖定时器
	SetGameTimer(TIME_SEND_TIME,m_byOpenCardTime);
	return ;
}

//机器人自动控制
void CServerGameDesk::RobotAutoControl()
{
	//首先去判断 这一桌是否既有机器人又有真人 
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

	//既有机器人又有真人  才去进行控制
	if (bHaveUser && bHaveRobot)
	{
		BOOL RobotIsWin=FALSE;
		int iResult = rand()%100;
		CString s = CINIFile::GetAppPath ();/////本地路径
		CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
		TCHAR szSec[_MAX_PATH] = TEXT("");
		_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
		//读取配置文件中机器人已经赢的金币
		m_iResetRobotHaveWinMoney	= f.GetKeyVal(szSec,"ReSetRobotHaveWinMoney ",0);	

		if(m_iResetRobotHaveWinMoney<=0)//机器人赢钱数为负数或零，机器人必赢
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

		if(RobotIsWin)//机器人赢钱
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
		else//机器人输钱
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

//判断任选二
int CServerGameDesk::shuangxx(BYTE winArea[],BYTE a,BYTE b)
{
	int cnta=0,cntb=0;
	for(int i=0;i<3;i++)
		if(winArea[i]==a) cnta++;
	for(int i=0;i<3;i++)
		if(winArea[i]==b) cntb++;

	if((cnta==2&&cntb==1)||(cnta==1&&cntb==2))		//三骰子,串爆(二夹一)
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
		if(2 == GetDiceNum())						//双骰子，二中二(连串)
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
		else if(3 == GetDiceNum())					//三骰子，二中二
		{
			return 50;
		}
	}
	return -10;
}

//判断任选三
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
	if(cnta==1&&cntb==1&&cntc==1)		//三中三
	{
		return 230;
	}
	else if((cnta==2||cntb==2||cntc==2)&&cnta+cntb+cntc==3)		//三中二(二夹一)
	{
		return 50;
	}
	else if(cnta==1&&cntb==1&&cntc==0||cnta==1&&cntb==0&&cntc==1||cnta==0&&cntb==1&&cntc==1)	//三中二(普通)
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

//判断豹子
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
		if(2 == GetDiceNum())		//双骰子豹子
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
		else						//三骰子豹子
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

	for(int i = 0;i < BET_ARES; ++i)		//获取每个区域的结果
	{
		res[i] = GetAresResult(i);
	}

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(i == m_byNtStation || NULL == m_pUserInfo[i])
		{
			continue;
		}

		for(int j = 0; j < BET_ARES; ++j)	//统计每个区域输赢并记录
		{
			if(m_i64UserBetCount[i][j] > 0)			//下注结果计算
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

			if (m_i64UserBetCountEx[i][j] > 0)		//挪结果计算
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

//记住路子
void CServerGameDesk::rememberLuzi()
{
	//路子记录结果
	//只记录15个路子
	STruOpenAward res;
	for(int i = 0; i < GetDiceNum(); ++i)
	{
		res.byWinArea[i] = m_byWinPrize[i];
	}

	if (m_dstAward.size() >= RECORD_COUNT)//发送15把
	{
		m_dstAward.pop_front();
		m_dstAward.push_back(res);
	}
	else
	{
		m_dstAward.push_back(res);
	}
}
//游戏数据包处理函数
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
			if (!m_bIsBuy)						//金币场不需要准备
			{
				return true;
			}
			else if (m_iRunGameCount > 0)		//房卡场只有第一局开始的时候要准备
			{
				return true;
			}
				
			return __super::HandleNotifyMessage(bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser);
		}
	case ASS_USER_BET:	//玩家下注
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
	case C_S_BET_INFO_REQ:		//玩家下注信息请求
		{
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealUserBetInfo(bDeskStation, pData);
			return true;
		}
	case C_S_CHANGE_BET_REQ:	//玩家挪注
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
	case ASS_APPLY_NT:	//玩家申请上庄
		{
			if(uSize != sizeof(ApplyNtMsg))
			{
				return true ; 
			}
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			//处理玩家申请上庄消息
			DealUserApplyNt(pData);
			return true;
		}
	case ASS_APPLY_LEAVE:	//申请下庄
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
	case ASS_CONTINUE_BET://玩家续压
		{
			return true;		//没有续压
			if(uSize!=sizeof(CS_ContinueBetMsg))
				return true;
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealContinueBet(pData);//flog
			return true;
		}
	case ASS_CLEAR_BET://玩家清空下注
		{
			return true;		//不可撤销，可以挪
			if(uSize!=sizeof(CS_CleartBetMsg))
				return true;
			if (NULL == m_pUserInfo[bDeskStation])
			{
				return false;
			}
			DealClearBet(pData);
			return true;
		}
	case ASS_SUPER_RESULT://超级用户消息
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

//处理超端用户控制结果
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
		//m_bySuperReslut=ProduceResultSuper(SuperUserMessage->bySuperReslut);//超端用户控制的结果
		memcpy(m_byWinPrize,SuperUserMessage->bySuperReslut,sizeof(m_byWinPrize));
		//m_byWinPrize[0]=m_bySuperReslut;
		////产生起点
		//m_byWinPrizeStart=ProduceStart(m_byWinPrize[0]);
	}
	else if(2 == GetDiceNum() && SuperUserMessage->bySuperReslut[0]>=1 && SuperUserMessage->bySuperReslut[0]<=6&&SuperUserMessage->bySuperReslut[1]>=1 && SuperUserMessage->bySuperReslut[1]<=6)
	{
		m_bIsSuperControl=TRUE;
		//m_bySuperReslut=ProduceResultSuper(SuperUserMessage->bySuperReslut);//超端用户控制的结果
		memcpy(m_byWinPrize,SuperUserMessage->bySuperReslut,sizeof(m_byWinPrize));
		m_byWinPrize[2] = 255;
	}
	m_bSuperDeskStation=SuperUserMessage->bSuperDeskStation;					//超端用户座位号
	m_bShowRobotBet=SuperUserMessage->bShowRobotBet;							//超端用户控制机器人下注显示
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

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_SUPER_SEND---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}
}

//玩家清空下注
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
			m_i64PeopleAreaBetAllSum[i]-=m_i64UserBetCount[pClearBet->byDeskStation][i];//真实玩家总各个区域总下注
			m_i64SumAllBet-=m_i64UserBetCount[pClearBet->byDeskStation][i];
			m_i64PeopleSumAllBet-=m_i64UserBetCount[pClearBet->byDeskStation][i];//真实玩家总下注
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
		if(m_bSuperDeskStation==i && !m_bShowRobotBet)//超端控制只显示真实玩家下注
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

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_CLEAR_BET_RESULT---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}
	
}

//处理玩家申请上庄消息
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
	
	if(m_bIsBuy && GS_ROB_NT != m_bGameStation)//创建房间非抢庄阶段不可上庄
	{
		return;
	}

	if(m_blNtWaitList.size()>=m_byMaxNtPeople)//最大上庄人数判断
	{
		return;
	}

	//是否庄家||已经在上庄列表中
	if(pApplyNt->byDeskStation == m_byNtStation || IsInWaitNtList(pApplyNt->byDeskStation))
	{
		return;
	}

	//是否有足够的钱上庄
	if (!CanMinusPoint() && GetUserRemaindMoney(pApplyNt->byDeskStation) < m_iApplyNtLimite )
	{
		TApplyNtResult.bApplay=true;
		SendGameData(TApplyNtResult.byDeskStation,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_APPLY_RESULT,0);
		return;
	}
	//非创建房间如果当前没有庄家 && 等待列表里面没有人 && 那么申请了 就直接开始游戏
	if (!m_bIsBuy && 255 == m_byNtStation)
	{
		m_byNtStation	=pApplyNt->byDeskStation;//记住庄家位置
		m_iNtPlayCount	=0;
		TApplyNtResult.bNtFlag=TRUE;
		StartGame();
	}
	else//加入庄家列表
	{
		m_blNtWaitList.push_back(pApplyNt->byDeskStation);
		int i=0;
		for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
		{
			TApplyNtResult.byWaitNtList[i]=*ite;
			i++;
		}
		TApplyNtResult.byNtStation = m_byNtStation;
		TApplyNtResult.byNtcount=m_iNtPlayCount;//庄家坐庄次数
		//庄家成绩
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

//处理玩家申请下庄消息
void	CServerGameDesk::DealUserApplyLeaveNt(void *pBuffer)
{
	ApplyLiveNtMsg *pApplyLeaveNt = (ApplyLiveNtMsg *)pBuffer;
	if (pApplyLeaveNt == NULL)
	{
		return;
	}

	if(m_bIsBuy)		//创建房间不允许主动下庄
		return;

	ApplyLiveNtResultMsg TApplyNtResult;
	TApplyNtResult.byDeskStation=pApplyLeaveNt->byDeskStation;
	if((pApplyLeaveNt->byDeskStation == m_byNtStation))//空闲状态庄家能下庄
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
	//复制庄家
	TApplyNtResult.byNtStation=m_byNtStation;
	TApplyNtResult.byNtcount=m_iNtPlayCount;//庄家坐庄次数
	//庄家的成绩
	TApplyNtResult.i64NtScore=m_i64PlayerGrade[m_byNtStation];
	int i=0;
	////上庄列表
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

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_LEAVE_RESULT---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}

}

//处理玩家下注
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
	bool bPeopleUpdate=false;//真实玩家下注数据是否有更新

	UserBetResultMsg TUserBetResult;

	//庄家不存在
	if (255 == m_byNtStation || NULL == m_pUserInfo[m_byNtStation])
	{
		TUserBetResult.bErrorCode = 6;
		SendGameData(pUserBet->byDeskStation, &TUserBetResult, sizeof(TUserBetResult), MDM_GM_GAME_NOTIFY, ASS_BET_RESULT, 0);
		return;
	}

	//判断下注区域是否有效
	if(!GetAreaBetVaild(pUserBet->byBetArea))		
	{
		TUserBetResult.bErrorCode = 5;
	}

	//判断筹码类型是否有效
	if(pUserBet->byChouMaType >= BETSTYLE)
	{
		TUserBetResult.bErrorCode = 5;
	}

	//区域下注限制
	long long i64RemainBetNum = RemainingBettingNumber(pUserBet->byBetArea);
	if(!CanMinusPoint() && (m_i64AreaBetAllSum[pUserBet->byBetArea]+G_iChouMaValue[pUserBet->byChouMaType])>i64RemainBetNum)
	{
		TUserBetResult.bErrorCode = 1;
	}
	//自己的钱够不够下注
	if(!CanMinusPoint() && G_iChouMaValue[pUserBet->byChouMaType]>GetUserRemaindMoney(pUserBet->byDeskStation))
	{
		TUserBetResult.bErrorCode = 2;
	}

	//区域下注限制
	if((m_i64AreaBetAllSum[pUserBet->byBetArea]+G_iChouMaValue[pUserBet->byChouMaType])>GetAreaBetLimite(pUserBet->byBetArea))
	{
		TUserBetResult.bErrorCode = 1;
	}
	//下注是庄家返回
	if (pUserBet->byDeskStation == m_byNtStation)
	{
		TUserBetResult.bErrorCode = 3;
	}
	//下注数超过个人下注限制
	if(m_i64UserBetSum[pUserBet->byDeskStation] + G_iChouMaValue[pUserBet->byChouMaType]>m_lSingleNoteLimit)
	{
		TUserBetResult.bErrorCode = 4;
	}

	if (TUserBetResult.bErrorCode != 0)
	{
		SendGameData(pUserBet->byDeskStation, &TUserBetResult, sizeof(TUserBetResult), MDM_GM_GAME_NOTIFY, ASS_BET_RESULT, 0);
		return;
	}

	//记录下注结果
	//各玩家在每个区域的下注情况
	m_i64UserBetCount[pUserBet->byDeskStation][pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];

	//各区域的总下注
	m_i64AreaBetAllSum[pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
	//玩家下注总数
	m_i64UserBetSum[pUserBet->byDeskStation] += G_iChouMaValue[pUserBet->byChouMaType];
	//总下注数
	m_i64SumAllBet+= G_iChouMaValue[pUserBet->byChouMaType];

	//记录筹码数
	++m_iChouMaCount[pUserBet->byBetArea][pUserBet->byChouMaType];

	//各区域的真实玩家的总下注
	if(!m_pUserInfo[pUserBet->byDeskStation]->m_UserData.isVirtual)
	{
		//各真实玩家在每个区域的下注情况
		m_i64PeopleBetCount[pUserBet->byDeskStation][pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
		//各真实玩家的总下注
		m_i64PeopleBetSum[pUserBet->byDeskStation] += G_iChouMaValue[pUserBet->byChouMaType];					
		//各区域的真实玩家的总下注								
		m_i64PeopleAreaBetAllSum[pUserBet->byBetArea]+=G_iChouMaValue[pUserBet->byChouMaType];
		//真实玩家总下注数		;
		m_i64PeopleSumAllBet+= G_iChouMaValue[pUserBet->byChouMaType];
		bPeopleUpdate=true;
	}
	//OutputDebugString("super::下注成功");
	
	TUserBetResult.bDynamicBet=true;
	//向客服端发送下注数据
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		
		if(m_bSuperDeskStation==i && !m_bShowRobotBet)//超端控制只显示真实玩家下注
		{
			if(!bPeopleUpdate)
			{
				continue;
			}
			//总下注
			TUserBetResult.i64SumAllBet=m_i64PeopleSumAllBet;
			//当前玩家总下注
			TUserBetResult.i64UserAllBet=GetUserBetCount(i);//flog
			//下注座位号
			TUserBetResult.byDeskStation=pUserBet->byDeskStation;
			//下注区域
			TUserBetResult.byBetArea = pUserBet->byBetArea;
			//下注筹码值
			TUserBetResult.byChouMaType = pUserBet->byChouMaType;
			for(int j=0;j<BET_ARES;j++)
			{
				//自己的下注情况
				TUserBetResult.i64UserBetCount[j]=m_i64PeopleBetCount[i][j];
				//各区域下注情况
				TUserBetResult.i64AreaBetAllSum[j]=m_i64PeopleAreaBetAllSum[j];
			}
		}
		else//都显示
		{
			//总下注
			TUserBetResult.i64SumAllBet=m_i64SumAllBet;
			//玩家总下注
			TUserBetResult.i64UserAllBet=GetUserBetCount(i);//flog
			//下注座位号
			TUserBetResult.byDeskStation=pUserBet->byDeskStation;
			//下注区域
			TUserBetResult.byBetArea = pUserBet->byBetArea;
			//下注筹码值
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

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_BET_RESULT---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	
	}
}

//处理玩家续压
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

//向客服端发送下注数据(超端控制)
void CServerGameDesk::SendBetDataToClient(BYTE bDeskStation)
{
	//向客服端发送下注数据
	if (NULL==m_pUserInfo[bDeskStation])
	{
		return;
	}
	UserBetResultMsg TUserBetResult;
	TUserBetResult.bDynamicBet=false;
	TUserBetResult.i64RemainedNotes=m_lRemainedNotes;
	if(m_pUserInfo[m_bSuperDeskStation] && !m_bShowRobotBet)//超端控制只显示真实玩家下注
	{
		//总下注
		TUserBetResult.i64SumAllBet=m_i64PeopleSumAllBet;
		TUserBetResult.i64UserAllBet=GetUserBetCount(bDeskStation);
		for(int j=0;j<BET_ARES;j++)
		{
			//自己的下注情况
			TUserBetResult.i64UserBetCount[j]=m_i64PeopleBetCount[bDeskStation][j];
			//各区域下注情况
			TUserBetResult.i64AreaBetAllSum[j]=m_i64PeopleAreaBetAllSum[j];
		}
		SendGameData(bDeskStation,&TUserBetResult,sizeof(TUserBetResult),MDM_GM_GAME_NOTIFY,ASS_BET_RESULT,0);
	}
	else
	{
		//总下注
		TUserBetResult.i64SumAllBet=m_i64SumAllBet;
		for(int j=0;j<BET_ARES;j++)
		{
			//自己的下注情况
			TUserBetResult.i64UserBetCount[j]=m_i64UserBetCount[bDeskStation][j];
			//各区域下注情况
			TUserBetResult.i64AreaBetAllSum[j]=m_i64AreaBetAllSum[j];
		}
		SendGameData(bDeskStation,&TUserBetResult,sizeof(TUserBetResult),MDM_GM_GAME_NOTIFY,ASS_BET_RESULT,0);
	}
}

//得到区域剩余下注数
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

//是否在上庄列表中
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

//超级用户验证
void	CServerGameDesk::SuperUserExamine(BYTE bDeskStation)
{
	SuperUserState TSuperUser;
	if (NULL == m_pUserInfo[bDeskStation])
	{
		return ;
	}
	if (IsSuperUser(bDeskStation))
	{
		// 该用户为超级用户, 通知客户端
		TSuperUser.byDeskStation = bDeskStation;
		// 超级客户端
		TSuperUser.bEnable = true;
		SendGameData(bDeskStation, &TSuperUser, sizeof(TSuperUser), MDM_GM_GAME_NOTIFY, ASS_SUPER_USER, 0);

	}
}

//是否超端用户 超端用户要发送结果给他看到
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
			//找打了超端玩家
			if (m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vlSuperID.at(i))
			{
				return true;
			}
		}
	}
	return false;
}

//获取游戏状态信息（第一次调用）
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	//由于在游戏开始后进来，该状态没有设置导致玩家在游戏中存钱，故在此重新设置
	if (IsPlayGame(bDeskStation))
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;//所有玩家置为同意状态
	}
	// 验证超级用户权限
	SuperUserExamine(bDeskStation);

	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//游戏没有开始状态
	case GS_WAIT_ARGEE:			//等待玩家开始状态
	case GS_WAIT_NEXT:
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//游戏状态
			TGameStation.byRobNTtime	= m_byRobNTtime;		//抢庄时间
			TGameStation.byFreeTime		= m_byFreeTime;			//空闲时间
			TGameStation.byBetTime		= m_byBetTime;			//下注时间
			TGameStation.byRolldiceTime =m_byRolldiceTime;      //摇骰时间
			TGameStation.bySendTime		= m_byOpenCardTime;	//开牌时间
			TGameStation.bShowChouMa    =m_bShowChouMa;
			TGameStation.byBetWaitTime	= m_byBetWaitTime;	//到下注阶段发牌等待时间				
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//到开奖阶段发牌等待时间

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//上庄条件
			TGameStation.byNtStation	= m_byNtStation;		//庄家位置
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//最大上庄人数
			TGameStation.lSingleNoteLimit	= m_lSingleNoteLimit;     //最大下注限制
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_FREE_STATUS:	//空闲阶段
	case GS_ROB_NT:			//抢庄阶段
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//游戏状态
			TGameStation.byRobNTtime	= m_byRobNTtime;		//抢庄时间
			TGameStation.byFreeTime		= m_byFreeTime;			//空闲时间
			TGameStation.byBetTime		= m_byBetTime;			//下注时间
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//开牌时间
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//到下注阶段发牌等待时间
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//到开奖阶段发牌等待时间

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //剩余时间

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//上庄条件
			TGameStation.byNtStation	= m_byNtStation;		//庄家位置
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//最大上庄人数
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //最大下注限制
			//庄家信息（把数和成绩）
			if(255==m_byNtStation)
			{
				TGameStation.i64NtGrade		= 0;	
			}
			else
			{
				TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	
			}
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//庄家坐庄次数
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//玩家成績
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			//复制上庄列表
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
	case GS_BET_STATUS:	//游戏下注阶段
		{

			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			TGameStation.bGameStation	= m_bGameStation;		//游戏状态
			TGameStation.byRobNTtime	= m_byRobNTtime;		//抢庄时间
			TGameStation.byFreeTime		= m_byFreeTime;			//空闲时间
			TGameStation.byBetTime		= m_byBetTime;			//下注时间
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//开牌时间
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//到下注阶段发牌等待时间
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//到开奖阶段发牌等待时间

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //剩余时间

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//上庄条件
			TGameStation.byNtStation	= m_byNtStation;		//庄家位置
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//最大上庄人数
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //最大下注限制
			//庄家信息（把数和成绩）

			TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	

			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//庄家坐庄次数
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//玩家成績
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			//复制上庄列表
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{
				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			TUpdateHistoryBet updateBet;//更新历史下注
			updateBet.i64SumAllBet=m_i64SumAllBet;
			updateBet.i64UserAllBet=GetUserBetCount(bDeskStation);//flog
			memcpy_s(updateBet.iChouMaCount, sizeof(updateBet.iChouMaCount), m_iChouMaCount, sizeof(updateBet.iChouMaCount));
			memcpy(updateBet.i64AreaBetAllSum,m_i64AreaBetAllSum,sizeof(updateBet.i64AreaBetAllSum));
			memcpy(updateBet.i64UserBetSum,m_i64UserBetCount[bDeskStation],sizeof(updateBet.i64UserBetSum));
			SendGameData(bDeskStation,&updateBet,sizeof(updateBet),MDM_GM_GAME_NOTIFY,ASS_UPDATE_BET,0);
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_ROLL_STATUS://摇骰阶段
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//游戏状态
			TGameStation.byRobNTtime	= m_byRobNTtime;		//抢庄时间
			TGameStation.byFreeTime		= m_byFreeTime;			//空闲时间
			TGameStation.byBetTime		= m_byBetTime;			//下注时间
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//开牌时间
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//到下注阶段发牌等待时间
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//到开奖阶段发牌等待时间

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //剩余时间

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//上庄条件
			TGameStation.byNtStation	= m_byNtStation;		//庄家位置
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//最大上庄人数
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//玩家成績
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //最大下注限制
			//庄家信息（把数和成绩）

			TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//庄家坐庄次数
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			//复制上庄列表
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{

				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			TUpdateHistoryBet updateBet;//更新历史下注
			updateBet.i64SumAllBet=m_i64SumAllBet;	
			updateBet.i64UserAllBet=GetUserBetCount(bDeskStation);//flog
			memcpy_s(updateBet.iChouMaCount, sizeof(updateBet.iChouMaCount), m_iChouMaCount, sizeof(updateBet.iChouMaCount));
			memcpy(updateBet.i64AreaBetAllSum,m_i64AreaBetAllSum,sizeof(updateBet.i64AreaBetAllSum));
			memcpy(updateBet.i64UserBetSum,m_i64UserBetCount[bDeskStation],sizeof(updateBet.i64UserBetSum));
			SendGameData(bDeskStation,&updateBet,sizeof(updateBet),MDM_GM_GAME_NOTIFY,ASS_UPDATE_BET,0);
			SendAwardRecord(bDeskStation);
			break;
		}
	case GS_SEND_STATUS://开牌阶段
		{

			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));

			TGameStation.bGameStation	= m_bGameStation;		//游戏状态
			TGameStation.byRobNTtime	= m_byRobNTtime;		//抢庄时间
			TGameStation.byFreeTime		= m_byFreeTime;			//空闲时间
			TGameStation.byBetTime		= m_byBetTime;			//下注时间
			TGameStation.byRolldiceTime =m_byRolldiceTime;
			TGameStation.bySendTime		= m_byOpenCardTime;	//开牌时间
			TGameStation.bShowChouMa    =m_bShowChouMa;

			TGameStation.byBetWaitTime	=	m_byBetWaitTime;						//到下注阶段发牌等待时间
			TGameStation.byOpenAwardWaitTime	=	m_byOpenAwardWaitTime;			//到开奖阶段发牌等待时间

			TGameStation.byRemaindTime	= m_iRemaindTime;                     //剩余时间

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//上庄条件
			TGameStation.byNtStation	= m_byNtStation;		//庄家位置
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//最大上庄人数
			TGameStation.i64PlayerGrade =m_i64PlayerGrade[bDeskStation];//玩家成績
			TGameStation.lSingleNoteLimit=m_lSingleNoteLimit;     //最大下注限制
			//庄家信息（把数和成绩）
			TGameStation.i64NtGrade		= m_i64PlayerGrade[m_byNtStation];	

			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//庄家坐庄次数
			memcpy_s(TGameStation.iChouMaValue, sizeof(TGameStation.iChouMaValue), G_iChouMaValue, sizeof(G_iChouMaValue));
			TGameStation.playMode = *(tagPM*)m_szDeskConfig;
			int i=0;
			for(vector<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
			{

				TGameStation.byWaitNtList[i]=*ite;
				i++;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			TUpdateHistoryBet updateBet;//更新历史下注
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

//游戏结束
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch(bCloseFlag)
	{
	case GF_NORMAL:	//正常结束
		{
			m_bGameStation	= GS_FREE_STATUS;
			memcpy(m_i64LastBet,m_i64UserBetCount,sizeof(m_i64LastBet));//flog
			//清理游戏数据
			ResetGameData();
			break;
		}
	case GF_SAFE:			//游戏安全结束
		{
			//初始化所有数据
			ResetGameData();
			//清空庄家
			m_i64PlayerGrade[m_byNtStation]=0;
			m_byNtStation	=255;//记住庄家位置
			m_iNtPlayCount	=0;
			m_bXiaZhuang = false;
			m_blNtWaitList.clear();	//上庄列表
			m_bGameStation = GS_WAIT_SETGAME;
			break;
		}
	case GFF_FORCE_FINISH:		//用户断线离开
		{
			if(m_bGameStation>GS_FREE_STATUS && m_bGameStation<GS_WAIT_NEXT)// 玩家强退
			{
				memset(m_i64LastBet,0,sizeof(m_i64LastBet));//flog
				//玩家强退 如果没下注 就及时清理掉,庄家除外
				if (m_i64UserBetSum[bDeskStation] <=0 && m_byNtStation!=bDeskStation)
				{
					//对应玩家成绩清零
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

//发送摇骰消息
void CServerGameDesk::SendRolldice()
{
	//设置游戏状态
	m_bGameStation	= GS_ROLL_STATUS;
	RollDiceMsg rollMsg;
	rollMsg.byGameStation=m_bGameStation;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		//发送游戏消息
		bool bSuccese = SendGameData(i,&rollMsg,sizeof(rollMsg),MDM_GM_GAME_NOTIFY,ASS_ROLL_DICE,0);

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_ROLL_DICE---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	}

	SetGameTimer(TIME_ROLL_TIME,m_byRolldiceTime);
}

//发送空闲消息
void CServerGameDesk::SendFree()
{
	//设置游戏状态
	m_bGameStation	= GS_FREE_STATUS;
	BeginFreeMsg freeMsg;
	freeMsg.bFlag=true;
	freeMsg.byFreeTime=m_byFreeTime;
	freeMsg.byNtStation=m_byNtStation;
	freeMsg.byNtcount=m_iNtPlayCount;
	freeMsg.byGameStation= m_bGameStation;         ///当前的游戏状态
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
		//发送游戏消息
		bool bSuccese = SendGameData(i,&freeMsg,sizeof(freeMsg),MDM_GM_GAME_NOTIFY,ASS_GAME_FREE,0);

		dwjlog.Format("yxxgame::给玩家[%d]发送ASS_GAME_FREE---[%d]",m_pUserInfo[i]->m_UserData.dwUserID,bSuccese);
		OutputDebugString(dwjlog);
	
	}
	//启动空闲定时器5秒
	SetGameTimer(TIME_FREE_TIME,m_byFreeTime);
}

//判断是否正在游戏
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

//用户离开游戏桌(如果用户没有gamebegin用户退出会调用此函数)
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_i64PlayerGrade[bDeskStation]=0;

	//处理玩家申请下庄消息
	ApplyLiveNtMsg TLeaveNt;
	TLeaveNt.byDeskStation = bDeskStation;
	DealUserApplyLeaveNt(&TLeaveNt);

	//回调以后退出玩家的m_pUserInfo结构体就会为空
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//用户断线离开
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	//处理玩家申请下庄消息
	ApplyLiveNtMsg TLeaveNt;
	TLeaveNt.byDeskStation = bDeskStation;
	DealUserApplyLeaveNt(&TLeaveNt);

	__super::UserNetCut(bDeskStation,  pLostUserInfo);
	return true;
}

//玩家坐下
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	auto res = __super::UserSitDesk(pUserSit, pUserInfo);

	if(m_bIsBuy && 1 == GetDeskPlayerNum())			//保存创建房间玩法
	{
		SetPlayMode();
		memset(m_i64WinMoney, 0, sizeof(m_i64WinMoney));
		m_dstAward.clear();
	}

	return res;
}

//服务端自动开始不由服务端控制
BOOL CServerGameDesk::StartGame()
{
	KillTimer(TIME_FREE_TIME);
	ResetGameData();

	BYTE byLastNTStation = m_byNtStation;

	//庄家不存在 换庄家
	if (m_byNtStation != 255 && NULL == m_pUserInfo[m_byNtStation])
	{
		m_byNtStation	= 255;
		m_iNtPlayCount	= 0;
	}
	//庄家坐庄次数到了该换庄了
	if(m_byNtStation != 255 && m_iNtPlayLimite <= m_iNtPlayCount && m_iNtPlayLimite > 0)
	{
		m_byNtStation	= 255;
		m_iNtPlayCount	= 0;
	}
	//金币不足换庄家
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
	//轮换庄家
	if (255 == m_byNtStation)
	{
		ChangeNt(byLastNTStation);
	}

	//庄家存在 才开始游戏
	if(255	!= m_byNtStation && m_pUserInfo[m_byNtStation])
	{
		GameBegin(ALL_ARGEE);//游戏开始
	}
	else
	{
		NoNtWait();
	}

	return true;
}

void CServerGameDesk::NoNtWait()
{
	m_bGameStation	= GS_WAIT_SETGAME;	//等待同意状态
	m_blNtWaitList.clear();
	//发送消息给客户端 告诉客户端 无庄等待
	memset(m_i64PlayerGrade,0,sizeof(m_i64PlayerGrade));
	m_byNtStation	= 255;//记住庄家位置
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

//换庄家
bool CServerGameDesk::ChangeNt(BYTE byLastNTStation)
{
	//换庄家
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

//清理上庄列表
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

//记录机器人所赢金币
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
	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
	//将当前机器人赢的钱写入配置文件当中
	m_iResetRobotHaveWinMoney+=i64Temp;
	sTemp.Format("%I64d",m_iResetRobotHaveWinMoney);
	f.SetKeyValString(szSec,"ReSetRobotHaveWinMoney ",sTemp);
}

//能否到负分（普通金币场，俱乐部金币场，俱乐部积分场，创建房间金币场等都不能到负分，只有创建房间积分场可以）
bool CServerGameDesk::CanMinusPoint()
{
	//待平台提供此标识
	//m_CurrencyType此桌的结算方式(房卡场专用字段).0.金币结算   1---俱乐部积分结算    2----现有的积分结算

	return m_CurrencyType == 2;
}

//获得骰子数
int CServerGameDesk::GetDiceNum()
{
	return m_byDiceNum;
}

//从庄家列表中获取出庄家(抢庄)
bool CServerGameDesk::OnGetZhuang()
{
	if(!m_blNtWaitList.empty())			//从上庄列表随机选一人当庄
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

//处理玩家请求下注信息
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

	//补充发送挪信息
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

//处理玩家挪操作
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
		change_Msg.byErrorCode = 4;		//选择区域错误
	}
	else if(m_i64AreaBetAllSum[byOriBetArea] < iChangeMoney)
	{
		change_Msg.byErrorCode = 1;		//超过原下注区域下注值
	}
	else if((m_i64AreaBetAllSum[byNewBetArea] + iChangeMoney) > GetAreaBetLimite(byNewBetArea))	
	{
		change_Msg.byErrorCode = 2;		//超过区域限制
	}
	else if(!CanMinusPoint() && (m_i64AreaBetAllSum[byNewBetArea]+iChangeMoney) > i64RemainBetNum)
	{
		change_Msg.byErrorCode = 2;		//超过区域限制
	}
	else if(m_byNuoCount[byDeskStation] >= NUO_COUNT)
	{
		change_Msg.byErrorCode = 3;		//超过挪限制次数
	}

	SendGameData(byDeskStation, &change_Msg, sizeof(change_Msg), MDM_GM_GAME_NOTIFY, S_C_CHANGE_BET_RSP, 0);

	if(0 == change_Msg.byErrorCode)	//成功
	{
		//记录本次挪数据
		int curNuoCount = m_byNuoCount[byDeskStation];
		if (curNuoCount < NUO_COUNT)
		{
			m_sUserNuoData[byDeskStation][curNuoCount].byBefore = byOriBetArea;
			m_sUserNuoData[byDeskStation][curNuoCount].byAfter = byNewBetArea;
			m_sUserNuoData[byDeskStation][curNuoCount].iNuoMoney = iChangeMoney;

			m_i64UserBetCountEx[byDeskStation][byNewBetArea] += iChangeMoney;
		}
		
		++m_byNuoCount[byDeskStation];		//增加挪次数

		//各区域的总下注
		m_i64AreaBetAllSum[byOriBetArea] -= iChangeMoney;
		m_i64AreaBetAllSum[byNewBetArea] += iChangeMoney;

		TMSG_UPDATE_BET_NTF update_msg;

		//向客服端发送更新下注数据
		for (int i = 0; i < PLAY_COUNT; i ++)
		{
			if (NULL==m_pUserInfo[i])
			{
				continue;
			}

			if(m_bSuperDeskStation == i && !m_bShowRobotBet)//超端控制只显示真实玩家下注
			{
				for(int j=0;j<BET_ARES;j++)
				{
					//自己的下注情况
					update_msg.i64UserBetCount[j] = m_i64PeopleBetCount[i][j];
					//各区域下注情况
					update_msg.i64AreaBetAllSum[j] = m_i64PeopleAreaBetAllSum[j];
				}
			}
			else//都显示
			{
				for(int j=0;j<BET_ARES;j++)
				{
					//自己的下注情况
					update_msg.i64UserBetCount[j] = m_i64PeopleBetCount[i][j];
					//各区域下注情况
					update_msg.i64AreaBetAllSum[j] = m_i64AreaBetAllSum[j];
				}
			}
			
			SendGameData(i, &update_msg, sizeof(update_msg), MDM_GM_GAME_NOTIFY, S_C_UPDATE_BET_NTF, 0);
		}
	}
}

//获得区域下注限制
long long CServerGameDesk::GetAreaBetLimite(int iArea)
{
	//如果玩家选择每个宫格的最高上限是1000（只算单押）；那么每类单独的串，下注上限为200；豹子为：100下注；
	long long tmp_AreaBetLimite = m_iAreaBetLimite;
	if(iArea >= 6 && iArea <= 11)		//豹子(10%)
	{
		tmp_AreaBetLimite = m_iAreaBetLimite * 10 / 100;
	}
	else if(iArea >= 12 && iArea <= 46)	//每类单独的串(20%)
	{
		tmp_AreaBetLimite = m_iAreaBetLimite * 20 / 100;
	}

	return tmp_AreaBetLimite;
}

//下注区域是否有效
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

//通知游戏局数
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

//随机生成骰子数据
void CServerGameDesk::RandDice()
{
	m_byWinPrize[0] = 1 + rand() % 6;
	m_byWinPrize[1] = 1 + rand() % 6;

	if(3 == GetDiceNum())					//用几个骰子
	{
		m_byWinPrize[2] = 1 + rand() % 6;
	}
	else
	{
		m_byWinPrize[2] = 255;
	}
}

//此处返回结果是正常的10倍，计算分数时需乘此结果后除10
int CServerGameDesk::GetAresResult(int iArea)
{
	//判断下注区域是否有效
	if(!GetAreaBetVaild(iArea))		
	{
		return 0;	
	}

	if(iArea >= DanYa_Start && iArea <= DanYa_End)			//单压中一个1倍、中两个2倍、中三个5倍
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
	else if(iArea >= BaoZi_Start && iArea <= BaoZi_End)		//豹子
	{
		int idx = iArea - BaoZi_Start;
		int ret = isBaozi(m_byWinPrize, G_iBaoZiPoint[idx]);
		return ret;
	}
	else if(iArea >= RenEr_Start && iArea <= RenEr_End)		//任选二
	{
		int idx = iArea - RenEr_Start;
		int ret = shuangxx(m_byWinPrize,G_iChooseTwo[idx][0],G_iChooseTwo[idx][1]);
		return ret;
	}
	else if(iArea >= RenSan_Start && iArea < RenSan_End)	//任选三
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

	for(int i = 0; i < BET_ARES; ++i)		//获取每个区域的结果
	{
		res[i] = GetAresResult(i);
	}

	if (m_byNtStation >= 0 && m_byNtStation < PLAY_COUNT && NULL != m_pUserInfo[m_byNtStation])
	{
		if(m_pUserInfo[m_byNtStation]->m_UserData.isVirtual)	//如果机器人是庄家,计算真实玩家的钱
		{
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL == m_pUserInfo[i] || m_pUserInfo[i]->m_UserData.isVirtual || m_i64UserBetSum[i] <= 0 || i == m_byNtStation)
					continue;

				//计算真实玩家各区域的赢钱数目
				for(int j = 0; j < BET_ARES; ++j)
				{
					if(m_i64UserBetCount[i][j] > 0)
					{
						//统计真实玩家赢的钱,真实玩家赢钱就是AI输钱
						AIWinMoney -= m_i64UserBetCount[i][j] * res[j] / 10;
					}	

					if (m_i64UserBetCountEx[i][j] > 0)
					{
						//统计真实玩家赢的钱,真实玩家赢钱就是AI输钱
						AIWinMoney -= m_i64UserBetCountEx[i][j] * res[j] / 10;
					}
				}
			}
		}
		else
		{	
			//如果真实玩家是庄家,计算机器人赢的钱
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL == m_pUserInfo[i] || !m_pUserInfo[i]->m_UserData.isVirtual || m_i64UserBetSum[i] <= 0 || i == m_byNtStation)
					continue;

				//计算AI各区域的赢钱数目
				for(int j = 0; j < BET_ARES; ++j)
				{
					if(m_i64UserBetCount[i][j] > 0)
					{
						//统计AI赢的钱
						AIWinMoney += m_i64UserBetCount[i][j] * res[j] / 10;
					}	

					if (m_i64UserBetCountEx[i][j] > 0)
					{
						//统计真实玩家赢的钱,真实玩家赢钱就是AI输钱
						AIWinMoney += m_i64UserBetCountEx[i][j] * res[j] / 10;
					}
				}
			}
		}
	}

	return AIWinMoney;
}

//设置创建房间玩法
void CServerGameDesk::SetPlayMode()
{
	tagPM *m_tagPM = (tagPM *)m_szDeskConfig;

	if(m_tagPM->iBetLimit > 0)
	{
		m_iAreaBetLimite = m_tagPM->iBetLimit;	//单个区域下注限制
	}

	if(m_tagPM->byReadyTime > 0)
	{
		m_byFreeTime = m_tagPM->byReadyTime;	//准备时间(空闲时间)
	}

	if(m_tagPM->byRobNtTime > 0)
	{
		m_byRobNTtime = m_tagPM->byRobNtTime;	//抢庄时间
	}

	if(m_tagPM->byBetTime > 0)
	{
		m_byBetTime = m_tagPM->byBetTime;		//下注时间
	}

	if(m_tagPM->byNuoLimit >= 0 && m_tagPM->byNuoLimit <= 100)
	{
		m_byNuoLimit = m_tagPM->byNuoLimit;		//挪限制
	}

	if(2 == m_tagPM->byDiceNum || 3 == m_tagPM->byDiceNum)
	{
		m_byDiceNum = m_tagPM->byDiceNum;
	}

	m_iPlayingMethod = m_tagPM->byDiceNum;
}

//是否有type玩法
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

//修改奖池数据1(平台->游戏)
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return true;
}

//修改奖池数据2(平台->游戏)
bool CServerGameDesk::SetRoomPondEx(long long	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return true;
}

//修改奖池数据3(平台->游戏)
bool CServerGameDesk::SetRoomPondEx(long long	iReSetAIHaveWinMoney)
{
	return true;
}
//控制胜负
BOOL CServerGameDesk::Judge()
{
	return true;
}
//胜
BOOL CServerGameDesk::JudgeWiner()
{
	return true;
}
//输
BOOL CServerGameDesk::JudgeLoser()
{
	return false;
}
