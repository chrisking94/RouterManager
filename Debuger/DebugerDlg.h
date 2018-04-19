
// DebugerDlg.h : 头文件
//

#pragma once

// CDebugerDlg 对话框
class CDebugerDlg : public CDialogEx
{
private:
	
// 构造
public:
	CDebugerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DEBUGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
