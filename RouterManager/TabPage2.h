#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Udp.h"

// CTabPage2 �Ի���

class CTabPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage2)

public:
	CTabPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage2();

// �Ի�������
	enum { IDD = IDD_TABPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnThreadMessage(WPARAM wParam,LPARAM lParam);//�̼߳�ͨ��-��Ϣ������
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

