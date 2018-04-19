
// RouterManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RouterManager.h"
#include "RouterManagerDlg.h"
#include "afxdialogex.h"
#include "Application.h"
#include "CodeString.h"
#include "Regex.h"
#include "Common.h"
#include "PluginMgr.h"
#include "BroadBand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOWTASK WM_USER+10
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRouterManagerDlg �Ի���




CRouterManagerDlg::CRouterManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRouterManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCloseFromNotifyIcon=false;
}

void CRouterManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_CMainTab);
}

BEGIN_MESSAGE_MAP(CRouterManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CRouterManagerDlg::OnTcnSelchangeTabMain)
	ON_MESSAGE(WM_THREAD,OnThreadMessage)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)
END_MESSAGE_MAP()



// CRouterManagerDlg ��Ϣ�������
static UINT indicators[]=
{
	 ID_INDICATOR_CAPS,
	 ID_SEPARATOR,
	 ID_INDICATOR_NUM   
};
BOOL CRouterManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AfxOleInit();//MSXml
	Application::InitializeVariable();
	//---------------------------����ͼ��---------------------------------//
	
	m_NotifyIcon.cbSize  = (DWORD)sizeof(NOTIFYICONDATA);
	m_NotifyIcon.hWnd    = GetSafeHwnd();
	m_NotifyIcon.uID     = IDR_MAINFRAME;
	m_NotifyIcon.uFlags  = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	m_NotifyIcon.uCallbackMessage = UM_TRAYNOTIFY;             // �Զ������Ϣ����
	m_NotifyIcon.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	CString strToolTip = _T("·�ɹ�����");
	_tcsncpy_s(m_NotifyIcon.szTip, strToolTip, strToolTip.GetLength());

	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);                // �����������ͼ��
	// TabControl
	//!ע�⣺��Ҫ��m_tabPage1~5��style��������ΪChild�������ܰ󶨵�TabControl
	m_CMainTab.InsertItem(0,(LPCTSTR)_T("����"));
	m_tabPage1.Create(IDD_TABPAGE1,&m_CMainTab); 
	m_CMainTab.InsertItem(0,(LPCTSTR)_T("����"));
	m_tabPage2.Create(IDD_TABPAGE2,&m_CMainTab);
	m_CMainTab.InsertItem(0,(LPCTSTR)_T("����"));
	m_tabPage3.Create(IDD_TABPAGE3,&m_CMainTab);
	m_CMainTab.InsertItem(0,(LPCTSTR)_T("����"));
	m_tabPage4.Create(IDD_TABPAGE4,&m_CMainTab);
	m_CMainTab.InsertItem(0,(LPCTSTR)_T("����"));
	m_tabPage5.Create(IDD_TABPAGE5,&m_CMainTab);

	m_pTabPage[0]=&m_tabPage1;
	m_pTabPage[1]=&m_tabPage2;
	m_pTabPage[2]=&m_tabPage3;
	m_pTabPage[3]=&m_tabPage4;
	m_pTabPage[4]=&m_tabPage5;
	
	CRect rs;
	m_CMainTab.GetClientRect(&rs);
	rs.top+=23; 
	rs.bottom-=2; 
	rs.left+=1; 
	rs.right-=2;

	for(int i=0;i<5;i++)
	{
		m_pTabPage[i]->MoveWindow(&rs);
	}

	m_tabPage1.ShowWindow(TRUE);
	m_CMainTab.SetCurSel(0);

	//StatusBar,״̬��
	m_wndStatusBar.CreateEx(this,SBT_TOOLTIPS,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,AFX_IDW_STATUS_BAR);
	m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
	CRect   rect;
	GetClientRect(&rect);
	m_wndStatusBar.SetPaneInfo(0,ID_INDICATOR_CAPS,SBPS_NORMAL,3*rect.Width()/5);
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_CAPS,SBPS_NORMAL,2*rect.Width()/5);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_NUM);
	//m_wndStatusBar.GetStatusBarCtrl().SetBkColor(RGB(220,220,220));

	m_tabPage2.Initialize();
	/*===TabPage1===*/
	//
	
	/*===TabPage5===*/

	//CStatic *mPicture = (CStatic *)m_tabPage5.GetDlgItem(IDC_PC_TRADEMARK);  
	//HICON hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 128, 128, 1);     
	//mPicture->ModifyStyle(0x0,SS_ICON|SS_CENTERIMAGE);    
	//mPicture->SetIcon(hIcon);  

	//HICON hIco=AfxGetApp()->LoadIcon(IDI_ICON4);
	//((CStatic*)(m_tabPage5.GetDlgItem(IDC_PC_TRADEMARK)))->SetIcon(hIco);

	/*===��������===*/
	m_tabPage3.m_CB_AutoStartUp=FALSE;
	CReg reg(HKEY_LOCAL_MACHINE,_T("software\\microsoft\\windows\\currentversion\\run"));
	if(reg.Open())
	{
		if(reg.Read(_T("RouterManager"))==Common::GetEXEPath())
		{
			m_tabPage3.m_CB_AutoStartUp=TRUE;
		}
	}
	m_tabPage3.m_EC_StatusUpdateInterval=Application::g_WinConfig.ReadFromConfig((CEdit*)m_tabPage3.GetDlgItem(IDC_EDIT2));
	if(_ttoi(m_tabPage3.m_EC_StatusUpdateInterval)<1)
	{
		m_tabPage3.m_EC_StatusUpdateInterval=_T("2");
		m_tabPage3.UpdateData(FALSE);
	}
	m_tabPage3.UpdateData(TRUE);
	Application::g_WinConfig.ReadFromConfig((CButton*)m_tabPage3.GetDlgItem(IDC_CHECK2));
	Application::g_WinConfig.ReadFromConfig((CButton*)m_tabPage3.GetDlgItem(IDC_CHECK3));
	Application::g_WinConfig.ReadFromConfig((CIPAddressCtrl*)m_tabPage3.GetDlgItem(IDC_IPADDRESS1));
	Application::SetStatusUpdateInterval(_ttoi(Common::GetWindownTextById(IDC_EDIT2,&m_tabPage3)));
	
	Application::g_Config.setDefaultSection(_T("Router"));
	Application::g_Config.read(_T("Router.Username"),m_tabPage1.m_EC_RouterAccount,FALSE,CConfig::Reg);
	Application::g_Config.read(_T("Router.Password"),m_tabPage1.m_EC_RouterPassword,TRUE,CConfig::Reg);
	//Application::g_Config.read(_T("RouterType"),Application::g_strRouterType);
	//��ʼ��·����

	Application::g_Config.setDefaultSection(_T("BroadBand"));
	Application::g_Config.read(_T("BB.Accout"),m_tabPage1.m_EC_PPPoEAcount,FALSE,CConfig::Reg);
	Application::g_Config.read(_T("BB.Password"),m_tabPage1.m_EC_PPPoEPassword,TRUE,CConfig::Reg);
	Application::g_Config.readBool(_T("AutoReconnect"),Router::g_MyConfig.bAutoReconnect);
	m_tabPage3.m_CB_AutoReconn.SetCheck(Router::g_MyConfig.bAutoReconnect);
	

	//��ʼ��telecom
	BroadBand::SetAccount(m_tabPage1.m_EC_PPPoEAcount);
	BroadBand::SetPassword(m_tabPage1.m_EC_PPPoEPassword);
	//��ʼ��·����
	Router::SetAuthorize(m_tabPage1.m_EC_RouterAccount, m_tabPage1.m_EC_RouterPassword);
	
	m_tabPage1.UpdateData(FALSE);
	m_tabPage3.UpdateData(FALSE);

	Application::SetStatusUpdateInterval(_ttoi(m_tabPage3.m_EC_StatusUpdateInterval));
	Application::Initialize(this);

	m_tabPage3.Initialize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRouterManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRouterManagerDlg::OnPaint()
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
HCURSOR CRouterManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRouterManagerDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static int curSel=0;
	m_pTabPage[curSel]->ShowWindow(FALSE);
	curSel=m_CMainTab.GetCurSel();
	m_pTabPage[curSel]->ShowWindow(TRUE);
	
	*pResult = 0;
}

void CRouterManagerDlg::showMsg( LPCTSTR msg1,LPCTSTR msg2/*=_T("")*/ )
{
	if(_T("")!=msg1)
	{
		SendMessage(WM_THREAD,0,(LPARAM)msg1);
	}
	
	if(_T("")!=msg2)
	{
		SendMessage(WM_THREAD,1,(LPARAM)msg2);
	}
}

LRESULT CRouterManagerDlg::OnThreadMessage( WPARAM wParam,LPARAM lParam )
{
	switch(wParam)
	{
	case 0:
	case 1:
		m_wndStatusBar.SetPaneText(wParam,(LPTSTR)lParam);
		break;
	}
	return 0;
}

void CRouterManagerDlg::OnClose()
{
	UpdateData(TRUE);
	m_tabPage1.UpdateData(TRUE);
	m_tabPage3.UpdateData(TRUE);
	
	Router::g_MyStat.workMode=Router::QueryStat;
	ShowWindow(SW_HIDE); //����������
	if(m_bCloseFromNotifyIcon)
	{
		//Application::Destroy();
		CRouterManagerDlg::OnCancel();
	}
}

void CRouterManagerDlg::OnDestroy()
{
	Application::g_Config.setDefaultSection(_T("Router"));
	Application::g_Config.write(_T("Router.Username"),m_tabPage1.m_EC_RouterAccount,FALSE,CConfig::Reg);
	Application::g_Config.write(_T("Router.Password"),m_tabPage1.m_EC_RouterPassword,TRUE,CConfig::Reg);
	Application::g_Config.write(_T("RouterType"),Router::GetRouterType());
	Application::g_Config.setDefaultSection(_T("BroadBand"));
	Application::g_Config.write(_T("BB.Accout"),m_tabPage1.m_EC_PPPoEAcount,FALSE,CConfig::Reg);
	Application::g_Config.write(_T("BB.Password"),m_tabPage1.m_EC_PPPoEPassword,TRUE,CConfig::Reg);
	Application::g_Config.writeBool(_T("AutoReconnect"),Router::g_MyConfig.bAutoReconnect);
	Application::g_Config.write(_T("Plugin"),PluginMgr::g_pCurPlugin->szName);
	
	Application::Destroy();
	
	m_NotifyIcon.hIcon=NULL;
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon);//�������ͼ��
	
	CDialogEx::OnDestroy();
}

BOOL CRouterManagerDlg::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CRouterManagerDlg::OnTrayNotify( WPARAM wParam, LPARAM lParam )
{
	UINT uMsg = (UINT)lParam;

	switch(uMsg)
	{
	case WM_RBUTTONUP:
		{
			//�Ҽ�����
			CMenu menuTray;
			CPoint point;
			int id;
			GetCursorPos(&point);

			menuTray.LoadMenu(IDR_MENU_Notify);
			SetForegroundWindow();//�������������̵����˵�ʧȥ�����Զ��ر�
			id = menuTray.GetSubMenu(0)->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
			
			switch(id){
			case ID_NOTIFYMENU_Mainframe:
			case ID_NOTIFYMENU_Traffic:
			case ID_NOTIFYMENU_Setting:
			case  ID_NOTIFYMENU_Help:
			case ID_NOTIFYMENU_About:
				Application::g_bAllowedShowStatus=true;

				m_CMainTab.SetCurSel(id-ID_NOTIFYMENU_Mainframe);

				NMHDR   nmhdr; 
				nmhdr.code = TCN_SELCHANGE;       
				nmhdr.hwndFrom =   m_CMainTab.GetSafeHwnd();       
				nmhdr.idFrom= m_CMainTab.GetDlgCtrlID(); 
				m_CMainTab.SendMessage(WM_NOTIFY,TCN_SELCHANGE,(LPARAM)(&nmhdr));

				//����ǰ����ʾ
				SetForegroundWindow();
				ShowWindow(SW_SHOWNORMAL);
				break;
			case ID_NOTIFYMENU_Exit:
				m_bCloseFromNotifyIcon=true;
				OnClose();
			default:
				break;
			}
			break;
		}
	case WM_LBUTTONDOWN:
		SetForegroundWindow();
		ShowWindow(SW_SHOWNORMAL);
		Application::g_bAllowedShowStatus=true;
		break;
	default:
		break;
	}
	return 0;
}

