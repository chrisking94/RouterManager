// ty2.02.h : ty2.02 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cty202App
// �йش���ʵ�ֵ���Ϣ������� ty2.02.cpp
//

class Cty202App : public CWinApp
{
public:
	Cty202App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern "C" __declspec(dllexport) void Deal(CString &szAccount,CString &szPassword);

