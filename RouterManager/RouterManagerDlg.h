// RouterManagerDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"
#include "resource.h"
#include "TabPage1.h"
#include "TabPage2.h"
#include "TabPage3.h"
#include "TabPage4.h"
#include "TabPage5.h"

class CRouterManagerDlg;
// CRouterManagerDlg �Ի���
class CRouterManagerDlg : public CDialogEx
{
// ����
public:
	CRouterManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_ROUTERMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnThreadMessage(WPARAM wParam,LPARAM lParam);//�̼߳�ͨ��-��Ϣ������
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	// ������
	CTabCtrl m_CMainTab;
	CTabPage1 m_tabPage1;
	CTabPage2 m_tabPage2;
	CTabPage3 m_tabPage3;
	CTabPage4 m_tabPage4;
	CTabPage5 m_tabPage5;
	CDialogEx *m_pTabPage[5];
	NOTIFYICONDATA m_NotifyIcon;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	CStatusBar m_wndStatusBar;
	bool m_bCloseFromNotifyIcon;
public:
	void showMsg(LPCTSTR msg1,LPCTSTR msg2=_T(""));
	afx_msg void OnBnClickedButton2();
};
