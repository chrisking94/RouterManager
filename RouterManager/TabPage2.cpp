// TabPage2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RouterManager.h"
#include "TabPage2.h"
#include "afxdialogex.h"
#include "Application.h"
#include "Messager.h"

// CTabPage2 �Ի���

IMPLEMENT_DYNAMIC(CTabPage2, CDialogEx)

CTabPage2::CTabPage2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPage2::IDD, pParent)
{
	
}

CTabPage2::~CTabPage2()
{
}

void CTabPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LC_Traffic);
	DDX_Control(pDX, IDC_EDIT3, m_EC_C_CommentEditor);
}


BEGIN_MESSAGE_MAP(CTabPage2, CDialogEx)
	ON_MESSAGE(WM_THREAD,OnThreadMessage)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTabPage2::OnLvnItemchangedList1)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PC_ADTRAF, &CTabPage2::OnStnClickedPcAdtraf)
//	ON_EN_CHANGE(IDC_EDIT3, &CTabPage2::OnEnChangeEdit3)
ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTabPage2::OnNMClickList1)
END_MESSAGE_MAP()

void CTabPage2::Initialize( void )
{
	m_LC_Traffic.SetExtendedStyle(m_LC_Traffic.GetExtendedStyle()
		|LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES);
	m_LC_Traffic.InsertColumn(0,_T("�û�"),0,200);
	m_LC_Traffic.InsertColumn(3,_T("������(����/�ϴ�)"),0,130);
	m_LC_Traffic.InsertColumn(4,_T("����/��(����/�ϴ�)"),0,130);
	m_LC_Traffic.InsertColumn(5,_T("״̬"),0,66);


	CEdit* pEdit=(CEdit*)this->GetDlgItem(IDC_EDIT3);
	pEdit->SetWindowText(Messager::GetMyName());
}


// CTabPage2 ��Ϣ�������
LRESULT CTabPage2::OnThreadMessage( WPARAM wParam,LPARAM lParam )
{
	return 0;
}


//void CTabPage2::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	
//	
//	
//	*pResult = 0;
//}

UINT nLVCurrentMod=0;
INT  nLVSelectedIndex=-1;
CString szLVSelectedName;
CString szLVSelectedMac;
BOOL CTabPage2::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		if(GetFocus() == GetDlgItem(IDC_EDIT3))
		{

			CString sTmp;
			m_EC_C_CommentEditor.GetWindowTextW(sTmp);
			if(sTmp.IsEmpty())
			{
				MessageBox(_T("����Ϊ�գ�"),_T("����"),MB_ICONWARNING);
			}
			if(nLVCurrentMod==0)//���ı����û��ǳ�
			{
				Messager::SetMyName(sTmp);
				sTmp=_T("�ҡ�����޸��ǳơ�(")+sTmp+_T(")");
			}
			else
			{
				Messager::SetContactRemark(szLVSelectedMac,sTmp);
				//sTmp=sTmp+_T("(")+szLVSelectedName+_T(")");
			}
			m_LC_Traffic.SetItemText(nLVSelectedIndex,0,sTmp);
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CTabPage2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	Application::g_TrafPageAdPic.DrawOnPictureCtrl();
}


void CTabPage2::OnStnClickedPcAdtraf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Application::g_pTrafAdvertise->strLinkUrl==_T("")) return;
	ShellExecute(NULL, _T("open"), Application::g_pTrafAdvertise->strLinkUrl, NULL, NULL, SW_SHOWNORMAL);
}


//void CTabPage2::OnEnChangeEdit3()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	if(bLVSelectedChange) return;//�б��ѡ����ı�
//	
//}


void CTabPage2::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	POSITION ps;
	int nIndex;

	ps=m_LC_Traffic.GetFirstSelectedItemPosition();
	nIndex=m_LC_Traffic.GetNextSelectedItem(ps);
	//TODO:��Ӷ�ѡ�Ĳ�����
	if(nIndex==-1)
	{
		//m_list.SetItemState(-1,LVNI_SELECTED,LVNI_SELECTED);

	}
	m_list.SetItemState(nIndex,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	CString str;

	str=str+m_list.GetItemText(nIndex,1);
	SetDlgItemText(IDC_EDIT_IP,str);
	*/

	CStatic* pComment=(CStatic*)this->GetDlgItem(IDC_STATIC_Comment);
	//TRACE("%d",pNMLV->iItem);
	nLVSelectedIndex=pNMItemActivate->iItem;
	if(nLVSelectedIndex==-1) return;
	szLVSelectedMac = Router::g_TrafficStatList[nLVSelectedIndex].szMac; 
	//TRACE("\n-%S-%S-\n",szMac,Application::g_NetAdpInf_Router.szMac);

	if(szLVSelectedMac==Router::g_MyNetAdpInf.szMac)
	{
		//����
		pComment->SetWindowText(_T("�ǳ�(���س�������)��"));
		m_EC_C_CommentEditor.SetWindowText(Messager::GetContactName(szLVSelectedMac));
		nLVCurrentMod=0;
	}
	else
	{
		pComment->SetWindowText(_T("��ע(���س�������)��"));
		m_EC_C_CommentEditor.SetWindowText(Messager::GetContactRemark(szLVSelectedMac));
		szLVSelectedName=Messager::GetContactName(szLVSelectedMac);
		nLVCurrentMod=1;
	}

	*pResult = 0;
}
