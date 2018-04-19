// TabPage5.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RouterManager.h"
#include "TabPage5.h"
#include "afxdialogex.h"
#include "Application.h"
#include "Common.h"


// CTabPage5 �Ի���

IMPLEMENT_DYNAMIC(CTabPage5, CDialogEx)

CTabPage5::CTabPage5(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPage5::IDD, pParent)
{

}

CTabPage5::~CTabPage5()
{
}

void CTabPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPage5, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabPage5::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CTabPage5::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


// CTabPage5 ��Ϣ�������


void CTabPage5::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Application::AddThreadTask(Application::Task_CheckForUpdate,_T("������"));
} 


void CTabPage5::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� 
	Common::SetFont(GetDlgItem(IDC_STATIC_RMTEXT),_T("΢���ź�"),20);
	Common::SetFont(GetDlgItem(IDC_STATIC_COPYRIGHT),_T("΢���ź�"),18);
}