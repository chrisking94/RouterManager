// ty2.02.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ty2.02.h"
#include "CodeString.h"

#pragma comment(lib,"Common.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// Cty202App

BEGIN_MESSAGE_MAP(Cty202App, CWinApp)
END_MESSAGE_MAP()


// Cty202App 构造

Cty202App::Cty202App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cty202App 对象

Cty202App theApp;


// Cty202App 初始化

BOOL Cty202App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


void Deal( CString &szAccount,CString &szPassword )
{
	CCodeString(szAccount).KDecrypt();
	CCodeString(szPassword).KDecrypt();

	ULONG tUl;
	CString szAcc=szAccount;
	CString szTimeHex=_T("");
	SYSTEMTIME st;
	GetLocalTime(&st);

	int y=st.wYear;
	if(y<200) y+=1900;
	int mo=st.wMonth;
	int d=st.wDay;
	int h=st.wHour;
	int m=st.wMinute;
	int s=st.wSecond;

	y-=1970;
	tUl=y*365*24*3600 + mo*30*24*3600 + d*24*3600 + h*3600 + m*60 + s;
	szTimeHex=CCodeString::ToHexString(tUl);
	szAccount=szTimeHex+_T("Tyroth")+szAccount+szPassword;
	szAccount=CCodeString(szAccount).Md5Encrypt();
	szAccount=szAccount.Mid(0,20);
	szAccount=_T("~ghca")+szTimeHex+_T("2002")+szAccount+szAcc;
}
