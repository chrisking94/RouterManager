
// DebugerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Debuger.h"
#include "DebugerDlg.h"
#include "afxdialogex.h"
#include "NetworkAdapter.h"
#include "Udp.h"
#include "Common.h"
#include "Winsock2.h"
#include "afxtempl.h"
#include "Winnetwk.h"
#include "RecordHeapFile.h"
#include "HtmlDocument.h"
//#include "JavaScript.h"

#pragma comment(lib,"Common.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mpr.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDebugerDlg �Ի���




CDebugerDlg::CDebugerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDebugerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDebugerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDebugerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDebugerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDebugerDlg ��Ϣ�������

BOOL CDebugerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	USES_CONVERSION;
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//return TRUE; 
// 	CJavaScript js = CJavaScript(_T("h:\\test.html"));
// 	js.Invoke(_T(""));

	CFile file(_T("G:\\TL-WR745N.html"),CFile::modeRead);
	CString sz=_T("");
	int n=0;
	char buffer[1024];
	while(n = file.Read(buffer,1024))
	{
		sz+=CString(buffer,n);
	}
	CHtmlDocument hdoc(sz);
	return TRUE;
	/*
	TestStru stru;
	Nest nst;
	nst.o = 'o';
	nst.f = 1.225;
	nst.p = 'p';
	nst.d = 1.445;
	nst.q = 'q';

	stru.sz = "abc";
	stru.a = 'a';
	stru.s = 16;
	stru.b = 'b';
	stru.i = 32;
	stru.c = 'c';
	stru.l = 64;
	stru.d = 'd';
	stru.pNest = &nst;
	stru.e = 'e';

	CRecordHeapFile f(_T("G:\\HeapRecordfile.txt"));

	f.Open(CFile::OpenFlags( (CFile::modeCreate) | (CFile::modeWrite)));
	f.WriteStruct("sctcicLc<cfcdc>c",(byte*)&stru);
	f.Close();
	f.Open(CFile::modeRead);
	TestStru sread;
	sread.pNest = new Nest;
	f.ReadStruct("sctcicLc<cfcdc>c",(byte*)&sread);
	*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDebugerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDebugerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//typedef void (WINAPI *CreateFunc)();

//CUdp udp(9896);
void CDebugerDlg::OnBnClickedOk()
{
	//CHuffmanEncoding(_T("H:\\22138-�Ĵ�ʽֽ����ֻ�.ppt")).WriteToFile(_T("H:\\out.txt"));
	

	return;
	//CInternetSession m_session(_T("session"),0,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD);
	static BOOL bLogin = FALSE;
	if(!bLogin)
	{
		CHttpWebConnection hwc(_T("http://192.168.0.1/LoginCheck"),CHttpWebConnection::POST);
		//hwc.sendRequest(m_session);

		hwc.writeForm(_T("Username=admin&Password=admin"));

		//hwc.addRequestHeaderString(_T("Accept: image/jpeg, application/x-ms-application, image/gif, application/xaml+xml, image/pjpeg, application/x-ms-xbap, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*"));
		//hwc.addRequestHeaderString(_T("Referer: http://192.168.0.1/index.asp"));
		//hwc.addRequestHeaderString(_T("Accept-Language: en-US"));
		//hwc.addRequestHeaderString(_T("User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; Trident/6.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET4.0C; .NET4.0E; InfoPath.3; Tablet PC 2.0)"));
		//hwc.addRequestHeaderString(_T("Accept-Encoding: gzip, deflate"));
		//hwc.addRequestHeaderString(_T("Pragma: no-cache"));
		//hwc.addRequestHeaderString(_T("Connection: Keep-Alive"));
		//hwc.addRequestHeaderString(_T(""));
		//hwc.setCookie(_T(""),_T("admin:language=cn"));
		hwc.sendRequest();

		CString szt;

		CHttpWebConnection::HeadersDef headers;
		hwc.getResponseHeaders(headers);

		bLogin = TRUE;
		hwc.close();
	}
	
	
	CHttpWebConnection hwcindex(_T("http://192.168.0.1/lan.asp"),CHttpWebConnection::GET);
	hwcindex.sendRequest();
	AfxMessageBox(hwcindex.readResponseString());

	
	hwcindex.close();
	//m_session.Close();
	return;
	/*
	HINSTANCE hDll = AfxLoadLibrary(_T("plugins\\ty2.02.dll"));
	if(NULL == hDll)
	{
		AfxMessageBox(_T("DLL��̬����ʧ��"));
		return;
	}

	//����querymodule�����õ��㷨����
	CreateFunc QueryModuleFunc=(CreateFunc) GetProcAddress(hDll,"QueryModule");
	if(QueryModuleFunc!=NULL)
	{
		QueryModuleFunc();
	}
	return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString st;
	Common::GetEXEPath(st);
	return;
	//CNetworkAdapter::GetAdaptersInfosArray();
	CReg regCfg(HKEY_LOCAL_MACHINE,_T("SoftWare\\CrystSoft\\RouterManager\\PPPoE"));
	regCfg.Open();
	
	regCfg.Write(_T("Account"),_T("���")); 
	
	AfxMessageBox(regCfg.Read(_T("Account")));
	return;
	udp.sendTo(_T("192.168.1.100"),_T("e123456789abcdHello Udp!I'm King!"));
	return;
	CDialogEx::OnOK();
	

	CList<CString,CString&> m_list;
	
	m_list.RemoveAll();

	CString strTemp;	
	struct hostent *host;
	struct in_addr *ptr;	// ���IP��ַ 	


	DWORD dwScope = RESOURCE_CONTEXT;
	NETRESOURCE *NetResource = NULL;
	HANDLE hEnum;
	WNetOpenEnum( dwScope, NULL, NULL, NULL, &hEnum );

	WSADATA wsaData;
	//��ʼö��������Դ
	WSAStartup(MAKEWORD(1,1),&wsaData);

	if ( hEnum )     //��������Ч
	{
		DWORD Count = 0xFFFFFFFF;
		DWORD BufferSize = 2048;
		LPVOID Buffer = new char[2048];
		// ����WSAStartup�����WNetEnumResource����һ����ö�ٹ���
		WNetEnumResource( hEnum, &Count, Buffer, &BufferSize );
		NetResource = (NETRESOURCE*)Buffer;

		char szHostName[200];

		for ( unsigned int i = 0; i < BufferSize/sizeof(NETRESOURCE); i++, NetResource++ )	
		{
			if ( NetResource->dwUsage == RESOURCEUSAGE_CONTAINER && NetResource->dwType == RESOURCETYPE_ANY ) {
				if ( NetResource->lpRemoteName )
				{
					CString strFullName = NetResource->lpRemoteName;
					if ( 0 == strFullName.Left(2).Compare(_T("\\\\")) )	 strFullName = strFullName.Right(strFullName.GetLength()-2);

					//���������
					gethostname( szHostName, strlen( szHostName ) );
					//����������ø�����Ӧ��������Ϣ
					const char sName[100]="";
					host = gethostbyname(sName);
					USES_CONVERSION;
					strFullName.Format(_T("%s"),A2T(sName));
					if(host == NULL) continue; 
					ptr = (struct in_addr *) host->h_addr_list[0];	

					// ��ȡIP��ַ��Ϣ����ַ��ʽ���£� 211.40.35.76 	
					int a = ptr->S_un.S_un_b.s_b1;  // 211	
					int b = ptr->S_un.S_un_b.s_b2;  // 40
					int c = ptr->S_un.S_un_b.s_b3;  // 35
					int d = ptr->S_un.S_un_b.s_b4;  // 76

					strTemp.Format(_T("%s -->  %d.%d.%d.%d"),strFullName,a,b,c,d);
					// ���뵽������
					TRACE("%S\n",strTemp);
					m_list.AddTail(strTemp);
				}
			}
		}
		delete Buffer;
		// ����ö�ٹ���
		WNetCloseEnum( hEnum );	
	}

	// ж��Winsock.dll
	WSACleanup();
	*/
}


void CDebugerDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
	
	

}
