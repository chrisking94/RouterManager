#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CTabPage1 对话框

class CTabPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage1)

public:
	CTabPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPage1();
// 对话框数据
	enum { IDD = IDD_TABPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnThreadMessage(WPARAM wParam,LPARAM lParam);//线程间通信-消息处理函数
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	// PPPoE连接状态指示PictureControl
	CStatic m_PC_PPPoEStatusIcon;
	// 路由器连接状态指示PictureControl
	CStatic m_PC_RouterLinkStatusIcon;
	// 路由器账号
	CString m_EC_RouterAccount;
	// 路由器密码
	CString m_EC_RouterPassword;
	// 宽带账号
	CString m_EC_PPPoEAcount;
	// 宽带密码
	CString m_EC_PPPoEPassword;
	// //PPPoE连接状态显示
	CString m_ST_WanLinkStatus;
	// 路由器连接状态Static Text
	CString m_ST_RouterLinkStatus;
	//路由器连接状态指示
	CStatic m_STC_RouterLinkStatustics;

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnStnClickedPcAdmain();
	afx_msg void OnStnClickedStaticGetsupport();
	afx_msg void OnStnClickedPcMainqq();
	afx_msg void OnBnClickedButton3();
	CComboBox m_CBB_Plugins;
};
