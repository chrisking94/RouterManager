
// DebugerDlg.h : ͷ�ļ�
//

#pragma once

// CDebugerDlg �Ի���
class CDebugerDlg : public CDialogEx
{
private:
	
// ����
public:
	CDebugerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DEBUGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:

	typedef struct
	{
		char o;
		float f;
		char p;
		double d;
		char q;
	}Nest;
	typedef struct 
	{
		char *sz;
		char a;
		short s;
		char b;
		int i;
		char c;
		long long l;
		char d;
		Nest *pNest;
		char e;
		//LPCTSTR lpsz;
	}TestStru;
};
