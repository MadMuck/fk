// Auto_AI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAuto_AIApp:
// �йش����ʵ�֣������ Auto_AI.cpp
//

class CAuto_AIApp : public CWinApp
{
public:
	CAuto_AIApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAuto_AIApp theApp;