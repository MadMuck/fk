#include "Stdafx.h"
#include <afxdllx.h>
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE GameClientDLL = { NULL, NULL };
DWORD      g_bucFileHandle;  //buc�ļ����


//��ʼ����Ϸ��Դ
BOOL InitSource()
{

	return true;
}

//�ͷ���Ϸ��Դ
void ReleaseSource()
{
	//�ر�buc�ļ�
	//::wuiClose(g_bucFileHandle);
}



//DLL ��ں���
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameClientDLL, hInstance)) return 0;
		//InitSource();
		new CDynLinkLibrary(GameClientDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		/*	ReleaseSource();*/
		AfxTermExtensionModule(GameClientDLL);
	}
	return 1;
}

/********************************************************************************************/

//�ȼ��жϺ����������Ϸ��
extern "C" __declspec(dllexport) TCHAR * GetGameOrderName(__int64 dwPoint)
{
	static __int64 uBasePoint[18]=
	{
		0,100,200,400,
		800,1600,3200,6400,		
		12800,25600,51200,102400,
		204800,409600,819200,1638400,
		3276800,6553600
	};
	static TCHAR * szOrderName[18]=
	{
		TEXT("�̹�"),TEXT("����"),TEXT("ƶũ"),TEXT("��ũ"),
		TEXT("��ũ"),TEXT("�ƹ�"),TEXT("����"),TEXT("����"),
		TEXT("С����"),TEXT("�����"),TEXT("С����"),TEXT("�����"),
		TEXT("֪��"),TEXT("֪��"),TEXT("�ܶ�"),TEXT("Ѳ��"),
		TEXT("ة��"),TEXT("����")
	};
	/*TEXT("�̹�"),TEXT("����"),TEXT("ƶũ"),TEXT("��ũ"),
		TEXT("��ũ"),TEXT("�ƹ�"),TEXT("С����"),TEXT("�׳�"),
		TEXT("����"),TEXT("����"),TEXT("Ա��"),TEXT("����"),
		TEXT("ʿ��"),TEXT("����"),TEXT("������"),TEXT("��Ե���"),
		TEXT("����"),TEXT("�����"),TEXT("����"),TEXT("�󸻺�"),
		TEXT("����")*/
	for (int i=0;i<18;i++)	if (dwPoint<uBasePoint[i]) return szOrderName[i];
	return TEXT("�����");
}

/********************************************************************************************/

//������Ϸ�ͻ���
//�ͻ�����������
GETCLIENTINFO;
GETGAMEICON(NAME_ID);
//GETFRAMEINTERFACE(CClientGameDlg)
GETORDERFUNCTION(GetGameOrderName);

/********************************************************************************************/
