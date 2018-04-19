#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Udp.h"

// CTabPage2 对话框

class CTabPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage2)

public:
	CTabPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPage2();

// 对话框数据
	enum { IDD = IDD_TABPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnThreadMessage(WPARAM wParam,LPARAM lParam);//线程间通信-消息处理函数
	void Initialize(void);
	CListCtrl m_LC_Traffic;
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_EC_C_CommentEditor;
//	afx_msg void OnEnChangeEdit3();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPcAdtraf();
//	afx_msg void OnEnChangeEdit3();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};

