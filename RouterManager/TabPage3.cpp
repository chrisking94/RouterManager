// TabPage3.cpp : 实现文件
//

#include "stdafx.h"
#include "RouterManager.h"
#include "TabPage3.h"
#include "afxdialogex.h"
#include "Application.h"
#include "RouterManagerDlg.h"
#include "Reg.h"
#include "Common.h"


// CTabPage3 对话框

IMPLEMENT_DYNAMIC(CTabPage3, CDialogEx)

CTabPage3::CTabPage3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPage3::IDD, pParent)
	, m_CB_AutoStartUp(FALSE)
	, m_EC_StatusUpdateInterval(_T(""))
{

}

CTabPage3::~CTabPage3()
{
}

void CTabPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_CB_AutoStartUp);
	DDX_Text(pDX, IDC_EDIT2, m_EC_StatusUpdateInterval);
	DDX_Control(pDX, IDC_CHECK2, m_CB_AutoReconn);
}


BEGIN_MESSAGE_MAP(CTabPage3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabPage3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTabPage3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CTabPage3::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTabPage3::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CTabPage3::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CTabPage3 消息处理程序


void CTabPage3::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Application::SetStatusUpdateInterval(_ttoi(m_EC_StatusUpdateInterval));

	//保存数据
	//Application::g_WinConfig.setDefaultSection(_T("FSDF"));
	Application::g_WinConfig.WriteToConfig((CEdit*)GetDlgItem(IDC_EDIT2));
	//Application::g_Config.setDefaultSection(_T("Config"));
	//Application::g_Config.write(_T("SUI"),m_EC_StatusUpdateInterval);
	Application::g_WinConfig.WriteToConfig((CButton*)GetDlgItem(IDC_CHECK2));
	Application::g_WinConfig.WriteToConfig((CButton*)GetDlgItem(IDC_CHECK3));
	Application::g_WinConfig.WriteToConfig((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1));

	CRouterManagerDlg *pMPD = (CRouterManagerDlg*)AfxGetMainWnd(); //获取主窗口指针
	pMPD->showMsg(_T("###已应用###"));
}

void CTabPage3::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
}

BOOL CTabPage3::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void CTabPage3::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CReg rg(HKEY_LOCAL_MACHINE,_T("software\\microsoft\\windows\\currentversion\\run"));
	if(rg.Open())
	{
		if(m_CB_AutoStartUp)
		{
			rg.Write(_T("RouterManager"),Common::GetEXEPath());
		}
		else
		{
			rg.Delete(_T("RouterManager"));
		}
	}
}


void CTabPage3::Initialize()
{
	
}


void CTabPage3::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	Router::g_MyConfig.bAutoReconnect=m_CB_AutoReconn.GetCheck();
}


void CTabPage3::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();

	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(bCheck);

	if(bCheck)
	{
		Router::SetIP(Common::GetWindownTextById(IDC_IPADDRESS1,this)) ;
	}
}
