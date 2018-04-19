
// RXmlEditorDlg.h : ͷ�ļ�
//

#pragma once
#include "RouterXml.h"
#include "Config.h"
#include "afxwin.h"
#include "afxcmn.h"

// CRXmlEditorDlg �Ի���
class CRXmlEditorDlg : public CDialogEx
{
// ����
public:
	CRXmlEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RXMLEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	void ListFiles();

	CString m_strFolderPath;
	CConfig m_cfg;
	CRouterXml *m_pXml;
	CRouterXml::PPPoEDef m_PPPoE;
	CRouterXml::TrafficDef m_Traffic;
	CRouterXml::WlanFilterDef m_WlanFilter;
	CRouterXml::AuthDef m_RouterAuth;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnSelchangeList3();
	CListBox m_LB_FileList;
	CEdit m_ED_Folder;
	CListBox m_LB_Project;
	CListBox m_LB_Attribute;
	afx_msg void OnLbnSelchangeList5();
};
