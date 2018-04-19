#pragma once
#include "afxwin.h"


// CTabPage3 对话框

class CTabPage3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage3)

public:
	CTabPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPage3();

// 对话框数据
	enum { IDD = IDD_TABPAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_CB_AutoStartUp;
	CString m_EC_StatusUpdateInterval;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck1();

	void Initialize();
	afx_msg void OnBnClickedCheck2();
	CButton m_CB_AutoReconn;
	afx_msg void OnBnClickedCheck3();
};
