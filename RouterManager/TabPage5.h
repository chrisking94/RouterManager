#pragma once

// CTabPage5 �Ի���

class CTabPage5 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage5)

public:
	CTabPage5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage5();

// �Ի�������
	enum { IDD = IDD_TABPAGE5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
};
