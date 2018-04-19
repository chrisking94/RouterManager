// Common.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Common.h"
#include "shlobj.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib, "Iphlpapi.lib")//���error LNK2019: �޷��������ⲿ���� _GetAdaptersInfo@8���÷����ں��� ...�������൱��������.cpp��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CCommonApp

BEGIN_MESSAGE_MAP(CCommonApp, CWinApp)
END_MESSAGE_MAP()
IMPLEMENT_OLECREATE(CCommonApp, "CCommonApp", 0x208BCC3B,0xC842,0x447F,0xA0,0x90,0x29,0x95,0x84,0x51,0x04,0x33)//OLE

// CCommonApp ����

CCommonApp::CCommonApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	 AfxOleLockApp();
}


// Ψһ��һ�� CCommonApp ����

CCommonApp theApp;

const GUID CDECL _tlid = { 0x208BCC3B, 0xC842, 0x447F, { 0xA0, 0x90, 0x29, 0x95, 0x84, 0x51, 0x4, 0x33 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CCommonApp ��ʼ��

BOOL CCommonApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ������ OLE ������(����)ע��Ϊ���С��⽫ʹ
	//  OLE ����Դ�����Ӧ�ó��򴴽�����
	COleObjectFactory::RegisterAll();

	return TRUE;
}

CCommonApp::~CCommonApp()
{
	AfxOleUnlockApp();
}

// DllGetClassObject - �����๤��

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - ���� COM ж�� DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}

void Common::GetTempFolderPath( CString& pathReturned )
{
	GetTempPathW(MAX_PATH,pathReturned.GetBuffer(MAX_PATH));
	pathReturned.ReleaseBuffer();
	pathReturned+=_T("\\");
}

CString Common::GetWebFolder()
{
	return _T("http://crystsoft.xyz/products/RouterManager/");
}

BOOL Common::GetMyDocumentPath( CString& strPathRet )
{
	return GetSpecialFolder(strPathRet,CSIDL_PERSONAL);
}

BOOL Common::GetSpecialFolder( CString& strPathRet , int csidl)
{
	BOOL bRet=FALSE;
	strPathRet=_T("");

	LPITEMIDLIST pidl=NULL;
	SHGetSpecialFolderLocation(NULL, csidl, &pidl);
	if(pidl)
	{
		bRet=SHGetPathFromIDListW(pidl, strPathRet.GetBuffer(MAX_PATH));
	}
	strPathRet.ReleaseBuffer();
	strPathRet+=_T("\\");

	return bRet;
}

BOOL Common::GetMyPicturesPath( CString& strPathRet )
{
	BOOL bRet=GetMyDocumentPath(strPathRet);
	strPathRet+=_T("My Pictures\\");

	return bRet;
}

void Common::GetStartUpPath( CString &strPathRet )
{
	GetCurrentDirectory(MAX_PATH,strPathRet.GetBuffer(MAX_PATH));
	strPathRet.ReleaseBuffer();
	strPathRet+=_T("\\");
}

CString Common::GetStartUpPath()
{
	CString sRet;
	GetStartUpPath(sRet);
	return sRet;
}

void Common::SetFont( CWnd* pWnd,LPCTSTR lpszFontName,int nSize,BOOL bItalic/*=FALSE*/ )
{
	CFont font;
	VERIFY(font.CreateFont(
		nSize,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		bItalic,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		lpszFontName
		));                 // lpszFacename
	pWnd->SetFont(&font);
	font.DeleteObject();
}

CString Common::GetMyComputerName()
{
	DWORD MaxComputerlength=64; 
	TCHAR chBuffer[64];
	GetComputerName(chBuffer,&MaxComputerlength);
	
	return CString(chBuffer);
}

void Common::GetEXEPath( CString &strNameRet )
{
		HMODULE module = GetModuleHandle(0); 
		GetModuleFileName(module, strNameRet.GetBuffer(MAX_PATH), MAX_PATH); 
		strNameRet.ReleaseBuffer();
}

CString Common::GetEXEPath()
{
	CString szRet;
	GetEXEPath(szRet);

	return szRet;
}

BOOL Common::IsSystemWow64()
{
	typedef   BOOL   (WINAPI   *LPFN_ISWOW64PROCESS)   (HANDLE,   PBOOL); 
	LPFN_ISWOW64PROCESS   
		fnIsWow64Process   =   (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process"); 


	static BOOL  bIsWow64   =   2;
	if(bIsWow64<2)
	{
		return bIsWow64;
	}

	CString str64;
	if   (NULL   !=   fnIsWow64Process) 
	{ 
		if   (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) 
		{
			//MessageBox(0,_T("Handle Error!"),0,0);
			bIsWow64 = 2;
		} 
	} 

	return bIsWow64;
}

BOOL Common::PathFileExiest( CString szPath )
{
	return ::PathFileExists(szPath);
}

BOOL Common::CreateDirectory( CString szPathName,LPSECURITY_ATTRIBUTES lpSecurityAttributes/*=NULL*/ )
{
	if(!Common::PathFileExiest(szPathName))
	{
		return ::CreateDirectory(szPathName,lpSecurityAttributes);
	}
	else
	{
		return TRUE;
	}
}

CString Common::GetWindownTextById( INT nID,CWnd* wndParent )
{
	CString szTmp;

	wndParent->GetDlgItem(nID)->GetWindowText(szTmp);

	return szTmp;
}

void Common::SetWindownTextById( INT nID,CString szSet,CWnd* wndParent )
{
	wndParent->GetDlgItem(nID)->SetWindowText(szSet);
}


