//////////////////////////////////////////////////////////////////////////
/// ������Ϣ��������Ϣ�����������
/// ���У�DebugPrintf���ڵ��Ը���ʱ�����Ϣ������ʱ�����޸ĺ����壬��д�ļ�ֱ�ӷ���
/// ErrorPrintf���ڳ������쳣״�������������ʱҲһ������������ٴ���ʱ�ṩ��Ϣ

#include "stdafx.h"
#include <tchar.h>
#include <stdio.h>
//#include "Debug_new.h"

#define DEBUGFILE_NAME 10003300

/// ��ȡ���г�������·�������ַ���ĩβ���ӷ�б��
TCHAR *G_GetAppPath()
{
	static TCHAR s_Path[MAX_PATH];
	static bool s_bIsReady = false;
	if (!s_bIsReady)
	{
		ZeroMemory(s_Path,sizeof(s_Path));
		DWORD dwLength=GetModuleFileName(GetModuleHandle(NULL), s_Path, sizeof(s_Path));
		TCHAR *p = _tcsrchr(s_Path, TEXT('\\'));
		*p = TEXT('\0');
		s_bIsReady = true;
	}
	return s_Path;
}
/// ��ӡ������Ϣ
void DebugPrintf(const TCHAR *p, ...)
{
	TCHAR szTimeStr[32];
	TCHAR szDateStr[32];
	TCHAR szFilename[256];
	wsprintf( szFilename, TEXT("%s\\Log\\%dLogic.txt"), G_GetAppPath(), DEBUGFILE_NAME );
	va_list arg;
	va_start( arg, p );
	FILE *fp = NULL;
	//fopen_s(&fp, szFilename, "a" );  _tfopen
	fp = _tfopen(szFilename, TEXT("a") );
	if (NULL == fp)
	{
		return;
	}
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	GetTimeFormat( LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &sysTime, TEXT("HH:mm:ss"), szTimeStr, 32);
	GetDateFormat( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, TEXT("yyyy-MM-dd"), szDateStr, 32);
	_ftprintf(fp, TEXT("[%s %s]--"), szDateStr, szTimeStr);
	_vftprintf(fp, p, arg );
	_ftprintf(fp, TEXT("\n"));
	fclose(fp);
}


/// ��ӡ������Ϣ
void ErrorPrintf(const TCHAR *p, ...)
{
	TCHAR szTimeStr[32];
	TCHAR szDateStr[32];
	TCHAR szFilename[256];
	wsprintf( szFilename, TEXT("%s\\Log\\%dLogicError.txt"), G_GetAppPath(), DEBUGFILE_NAME );
	va_list arg;
	va_start( arg, p );
	FILE *fp = NULL;
	//fopen_s(&fp, szFilename, "a" );  _tfopen
	fp = _tfopen(szFilename, TEXT("a") );
	if (NULL == fp)
	{
		return;
	}
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	GetTimeFormat( LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &sysTime, TEXT("HH:mm:ss"), szTimeStr, 32);
	GetDateFormat( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, TEXT("yyyy-MM-dd"), szDateStr, 32);
	_ftprintf(fp, TEXT("[%s %s]--"), szDateStr, szTimeStr);
	_vftprintf(fp, p, arg );
	_ftprintf(fp, TEXT("\n"));
	fclose(fp);
}