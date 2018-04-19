#pragma once
#include "explorer1.h"


// CTabPage4 对话框

class CTabPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage4)

public:
	CTabPage4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPage4();

// 对话框数据
	enum { IDD = IDD_TABPAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_WebBrowser_Help;
};
