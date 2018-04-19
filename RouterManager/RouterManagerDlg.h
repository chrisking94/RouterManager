// RouterManagerDlg.h : 头文件
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
// CRouterManagerDlg 对话框
class CRouterManagerDlg : public CDialogEx
{
// 构造
public:
	CRouterManagerDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_ROUTERMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnThreadMessage(WPARAM wParam,LPARAM lParam);//线程间通信-消息处理函数
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	// 主界面
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
