// ty2.02.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ty2.02.h"
#include "CodeString.h"

#pragma comment(lib,"Common.lib")

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

// Cty202App

BEGIN_MESSAGE_MAP(Cty202App, CWinApp)
END_MESSAGE_MAP()


// Cty202App ����

Cty202App::Cty202App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cty202App ����

Cty202App theApp;


// Cty202App ��ʼ��

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
