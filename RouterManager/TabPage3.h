#pragma once
#include "afxwin.h"


// CTabPage3 �Ի���

class CTabPage3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage3)

public:
	CTabPage3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage3();

// �Ի�������
	enum { IDD = IDD_TABPAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
