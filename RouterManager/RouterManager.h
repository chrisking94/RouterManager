
// RouterManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRouterManagerApp:
// �йش����ʵ�֣������ RouterManager.cpp
//

class CRouterManagerApp : public CWinApp
{
public:
	CRouterManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRouterManagerApp theApp;