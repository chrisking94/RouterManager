
// UpdateDlg.h : 头文件
//

#pragma once
#include "Thread.h"
#include "../Common/Update.h"

// CUpdateDlg 对话框
class CUpdateDlg : public CDialogEx
{
// 构造
public:
	CUpdateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	static DWORD WINAPI Thread_Update(LPVOID lpParam);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
