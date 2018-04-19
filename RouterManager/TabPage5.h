#pragma once

// CTabPage5 对话框

class CTabPage5 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage5)

public:
	CTabPage5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPage5();

// 对话框数据
	enum { IDD = IDD_TABPAGE5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
};
