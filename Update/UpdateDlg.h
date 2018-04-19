
// UpdateDlg.h : ͷ�ļ�
//

#pragma once
#include "Thread.h"
#include "../Common/Update.h"

// CUpdateDlg �Ի���
class CUpdateDlg : public CDialogEx
{
// ����
public:
	CUpdateDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	static DWORD WINAPI Thread_Update(LPVOID lpParam);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
private:
	CThread* m_pThreadUpdate;
	static CUpdate::UpdateCmdString ucs;
	static BOOL m_bDownloading;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
