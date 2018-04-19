// Common.h : Common DLL 的主头文件
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CCommonApp
// 有关此类实现的信息，请参阅 Common.cpp
//
#include "CodeString.h"
#include "Config.h"
#include "HtmlDocument.h"
#include "HttpWebConnection.h"
#include "IniFile.h"
#include "MD5.h"
#include "Reg.h"
#include "Regex.h"
#include "Thread.h"
#include "Xml.h"
#include "ArrayEx.h"
#include "Update.h"
#include "Ping.h"

class __declspec(dllexport) Common
{
public:
	/************************静态函数声明************************/
	static void GetTempFolderPath(CString& pathReturned);
	static CString GetWebFolder();
	static BOOL GetMyDocumentPath(CString& strPathRet);
	static BOOL GetMyPicturesPath(CString& strPathRet);
	static BOOL GetSpecialFolder(CString& strPathRet , int csidl);
	static void GetStartUpPath(CString &strPathRet);
	static void GetEXEPath(CString &strNameRet);//取得执行文件名称
	static CString GetStartUpPath();
	static CString GetEXEPath();
	static void SetFont(CWnd* pWnd,LPCTSTR lpszFontName,int nSize,BOOL bItalic=FALSE);
	static CString GetMyComputerName();
	static BOOL IsSystemWow64();
	static BOOL PathFileExiest(CString szPath);
	static BOOL CreateDirectory(CString szPathName,LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);//存在或创建成功返回TRUE
	static CString GetWindownTextById(INT nID,CWnd* wndParent);
	static void SetWindownTextById(INT nID,CString szSet,CWnd* wndParent );
	/************************静态函数.END************************/

public:
	/************************静态变量声明************************/
	
	/************************静态变量.END************************/
};

class __declspec(dllexport) CCommonApp : public CWinApp
{
public:
	CCommonApp();
	~CCommonApp();
// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CCommonApp)//OLE
public:
};
