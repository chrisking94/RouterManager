// TabPage4.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RouterManager.h"
#include "TabPage4.h"
#include "afxdialogex.h"


// CTabPage4 �Ի���

IMPLEMENT_DYNAMIC(CTabPage4, CDialogEx)

CTabPage4::CTabPage4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPage4::IDD, pParent)
{

}

CTabPage4::~CTabPage4()
{
}

void CTabPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser_Help);
}


BEGIN_MESSAGE_MAP(CTabPage4, CDialogEx)
END_MESSAGE_MAP()

BOOL CTabPage4::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


// CTabPage4 ��Ϣ�������
