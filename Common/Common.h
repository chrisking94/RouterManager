// Common.h : Common DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CCommonApp
// �йش���ʵ�ֵ���Ϣ������� Common.cpp
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
	/************************��̬��������************************/
	static void GetTempFolderPath(CString& pathReturned);
	static CString GetWebFolder();
	static BOOL GetMyDocumentPath(CString& strPathRet);
	static BOOL GetMyPicturesPath(CString& strPathRet);
	static BOOL GetSpecialFolder(CString& strPathRet , int csidl);
	static void GetStartUpPath(CString &strPathRet);
	static void GetEXEPath(CString &strNameRet);//ȡ��ִ���ļ�����
	static CString GetStartUpPath();
	static CString GetEXEPath();
	static void SetFont(CWnd* pWnd,LPCTSTR lpszFontName,int nSize,BOOL bItalic=FALSE);
	static CString GetMyComputerName();
	static BOOL IsSystemWow64();
	static BOOL PathFileExiest(CString szPath);
	static BOOL CreateDirectory(CString szPathName,LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);//���ڻ򴴽��ɹ�����TRUE
	static CString GetWindownTextById(INT nID,CWnd* wndParent);
	static void SetWindownTextById(INT nID,CString szSet,CWnd* wndParent );
	/************************��̬����.END************************/

public:
	/************************��̬��������************************/
	
	/************************��̬����.END************************/
};

class __declspec(dllexport) CCommonApp : public CWinApp
{
public:
	CCommonApp();
	~CCommonApp();
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CCommonApp)//OLE
public:
};
