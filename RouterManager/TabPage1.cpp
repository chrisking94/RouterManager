// TabPage1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HttpWebConnection.h"
#include "RouterManager.h"
#include "TabPage1.h"
#include "afxdialogex.h"
#include "Application.h"
#include "StringEx.h"
#include "Common.h"
#include "RouterManagerDlg.h"
#include "Random.h"
#include "PluginMgr.h"
#include "BroadBand.h"


// CTabPage1 �Ի���

IMPLEMENT_DYNAMIC(CTabPage1, CDialogEx)

CTabPage1::CTabPage1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPage1::IDD, pParent)
	, m_EC_RouterAccount(_T(""))
	, m_EC_RouterPassword(_T(""))
	, m_EC_PPPoEAcount(_T(""))
	, m_EC_PPPoEPassword(_T(""))
	, m_ST_WanLinkStatus(_T(""))
	, m_ST_RouterLinkStatus(_T(""))
{

}

CTabPage1::~CTabPage1()
{

}

void CTabPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PPPOE_STATUS_ICON, m_PC_PPPoEStatusIcon);
	DDX_Text(pDX, IDC_EDIT1, m_EC_RouterAccount);
	DDX_Text(pDX, IDC_EDIT2, m_EC_RouterPassword);
	DDX_Text(pDX, IDC_EDIT4, m_EC_PPPoEAcount);
	DDX_Text(pDX, IDC_EDIT5, m_EC_PPPoEPassword);
	DDX_Control(pDX, IDC_STATIC_ROUTER_STATUS_ICON, m_PC_RouterLinkStatusIcon);
	DDX_Text(pDX, IDC_ST_PPPOE_LINKSTATE, m_ST_WanLinkStatus);
	DDX_Text(pDX, IDC_ST_ROUTER_LINKSTATE, m_ST_RouterLinkStatus);
	DDX_Control(pDX, IDC_COMBO1, m_CBB_Plugins);
}


BEGIN_MESSAGE_MAP(CTabPage1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabPage1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTabPage1::OnBnClickedButton2)
	ON_MESSAGE(WM_THREAD,OnThreadMessage)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTabPage1::OnCbnSelchangeCombo1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK2, &CTabPage1::OnBnClickedCheck2)
	ON_STN_CLICKED(IDC_PC_ADMAIN, &CTabPage1::OnStnClickedPcAdmain)
	ON_STN_CLICKED(IDC_STATIC_GETSUPPORT, &CTabPage1::OnStnClickedStaticGetsupport)
	ON_STN_CLICKED(IDC_PC_MAINQQ, &CTabPage1::OnStnClickedPcMainqq)
END_MESSAGE_MAP()


void CTabPage1::OnPaint()
{
	CDialogEx::OnPaint();
	/*
	CPaintDC dc(this);
	dc.SelectObject(CreatePen(PS_DASHDOTDOT,4,RGB(25,50,80)));
	dc.MoveTo(0,0);
	dc.LineTo(100,0);
	*/
	Common::SetFont(GetDlgItem(IDC_STATIC_CR),_T("΢���ź�"),20);
	Common::SetFont(GetDlgItem(IDC_ST_RouterType),_T("΢���ź�"),40);

	Application::g_MainPageAdPic.DrawOnPictureCtrl();
}

// CTabPage1 ��Ϣ�������
void CTabPage1::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������;
	UpdateData(TRUE);//�������ݵ�����
	Router::SetAuthorize(m_EC_RouterAccount, m_EC_RouterPassword);
	Application::InitializeModules();
	//Router::g_MyStat.workMode=Router::QueryStat;//������ѯ
	
	//Application::AddThreadTask(Application::Task_GetRouterInfo,_T("��ȡ·������Ϣ"));
	
}


void CTabPage1::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton * btn=(CButton *) GetDlgItem(IDC_BUTTON2);
	btn->EnableWindow(FALSE);
	CString btnText;
	btn->GetWindowText(btnText);
	UpdateData(TRUE);
	if(btnText==_T("����"))
	{
		Application::AddThreadTask(Application::Task_BroadBandConnect,_T("PPPoE����"));
	}
	else
	{
		Application::AddThreadTask(Application::Task_BroadBandDisconnect,_T("�Ͽ�PPPoE"));
	}
}

enum StatusRefreshOperation
{
	Update,
	SetIcon
};
LRESULT CTabPage1::OnThreadMessage( WPARAM wParam,LPARAM lParam )
{
	StatusRefreshOperation Operation=(StatusRefreshOperation)wParam;
	
	switch(Operation)
	{
	case Update:
		UpdateData((BOOL)lParam);
		break;
	}
	OnPaint();
	
	return 0;
}



void CTabPage1::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szTmp;

	m_CBB_Plugins.GetLBText(m_CBB_Plugins.GetCurSel(),szTmp);
	PluginMgr::SetCurPlugin(szTmp);

	//PluginMgr::Deal();
}


HBRUSH CTabPage1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_CR){//IDC_STATIC_TEST�ǿؼ���ID
		pDC->SetTextColor(RGB(20,150,220));//��RGB��ı���ɫ        
	}
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_GETSUPPORT){//IDC_STATIC_TEST�ǿؼ���ID
		pDC->SetTextColor(RGB(0,51,255));//��RGB��ı���ɫ        
	}
	if(pWnd->GetDlgCtrlID() == IDC_ST_RouterType){//IDC_STATIC_TEST�ǿؼ���ID
		CRandom::seedByTime();
		pDC->SetTextColor(RGB(CRandom::getNum(0,255),CRandom::getNum(0,255),CRandom::getNum(0,255)));//��RGB��ı���ɫ        
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CTabPage1::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

BOOL CTabPage1::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		if(GetFocus() == GetDlgItem(IDC_EDIT1)||GetFocus() == GetDlgItem(IDC_EDIT2))
		{
			OnBnClickedButton1();
		}
		else if(GetFocus() == GetDlgItem(IDC_EDIT4)||GetFocus() == GetDlgItem(IDC_EDIT5))
		{
			OnBnClickedButton2();
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CTabPage1::OnStnClickedPcAdmain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Application::g_pMainAdvertise->strLinkUrl==_T("")) return;
	ShellExecute(NULL, _T("open"), Application::g_pMainAdvertise->strLinkUrl, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CTabPage1::OnInitDialog()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON,0,0,TRUE); 
	CStatic *picCtrl=(CStatic *)GetDlgItem(IDC_PC_MAINQQ);
	picCtrl->SetIcon(hIcon); 

	return CDialogEx::OnInitDialog();
}


void CTabPage1::OnStnClickedStaticGetsupport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL, _T("open"), _T("http://wpa.qq.com/msgrd?uin=1030500851"), NULL, NULL, SW_HIDE);
}


void CTabPage1::OnStnClickedPcMainqq()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnStnClickedStaticGetsupport();
}


void CTabPage1::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Application::AddThreadTask(Application::Task_DownloadRouterXmls,_T("���·�����ͺ�֧��"));
}

