#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CTabPage1 �Ի���

class CTabPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage1)

public:
	CTabPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage1();
// �Ի�������
	enum { IDD = IDD_TABPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnThreadMessage(WPARAM wParam,LPARAM lParam);//�̼߳�ͨ��-��Ϣ������
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	// PPPoE����״ָ̬ʾPictureControl
	CStatic m_PC_PPPoEStatusIcon;
	// ·��������״ָ̬ʾPictureControl
	CStatic m_PC_RouterLinkStatusIcon;
	// ·�����˺�
	CString m_EC_RouterAccount;
	// ·��������
	CString m_EC_RouterPassword;
	// ����˺�
	CString m_EC_PPPoEAcount;
	// �������
	CString m_EC_PPPoEPassword;
	// //PPPoE����״̬��ʾ
	CString m_ST_WanLinkStatus;
	// ·��������״̬Static Text
	CString m_ST_RouterLinkStatus;
	//·��������״ָ̬ʾ
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
