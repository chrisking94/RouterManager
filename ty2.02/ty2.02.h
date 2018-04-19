// ty2.02.h : ty2.02 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// Cty202App
// 有关此类实现的信息，请参阅 ty2.02.cpp
//

class Cty202App : public CWinApp
{
public:
	Cty202App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern "C" __declspec(dllexport) void Deal(CString &szAccount,CString &szPassword);

