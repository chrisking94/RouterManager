#pragma once
#include "explorer1.h"


// CTabPage4 �Ի���

class CTabPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage4)

public:
	CTabPage4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage4();

// �Ի�������
	enum { IDD = IDD_TABPAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_WebBrowser_Help;
};
