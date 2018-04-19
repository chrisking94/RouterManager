
// RXmlEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RXmlEditor.h"
#include "RXmlEditorDlg.h"
#include "afxdialogex.h"
#include "RouterXml.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"Common.lib")

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRXmlEditorDlg �Ի���




CRXmlEditorDlg::CRXmlEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRXmlEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRXmlEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_LB_FileList);
	DDX_Control(pDX, IDC_EDIT1, m_ED_Folder);
	DDX_Control(pDX, IDC_LIST5, m_LB_Project);
	DDX_Control(pDX, IDC_LIST6, m_LB_Attribute);
}

BEGIN_MESSAGE_MAP(CRXmlEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CRXmlEditorDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST3, &CRXmlEditorDlg::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST5, &CRXmlEditorDlg::OnLbnSelchangeList5)
END_MESSAGE_MAP()


// CRXmlEditorDlg ��Ϣ�������

BOOL CRXmlEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AfxOleInit();

	m_cfg=CConfig(Common::GetStartUpPath()+_T("\\RXmlEditorConfig.ini"));
	CString sTmp;
	m_cfg.setDefaultSection(_T("Config"));
	m_strFolderPath=m_cfg.read(_T("Folder"));
	SetDlgItemText(IDC_EDIT1,m_strFolderPath);
	if(!m_strFolderPath.IsEmpty())
	{
		ListFiles();
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRXmlEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRXmlEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRXmlEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRXmlEditorDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString    strFolderPath;
	BROWSEINFO broInfo = {0};
	TCHAR       szDisName[MAX_PATH] = {0};

	broInfo.hwndOwner = this->m_hWnd;
	broInfo.pidlRoot  = NULL;
	broInfo.pszDisplayName = szDisName;
	broInfo.lpszTitle = _T("ѡ�񱣴�·��");
	broInfo.ulFlags   = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN
		| BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	broInfo.lpfn      = NULL;
	broInfo.lParam    = NULL;
	broInfo.iImage    = IDR_MAINFRAME;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&broInfo);
	if (pIDList != NULL)
	{
		memset(szDisName, 0, sizeof(szDisName));
		SHGetPathFromIDList(pIDList, szDisName);
		strFolderPath = szDisName;
	}
	if(strFolderPath!=_T(""))
	{
		SetDlgItemText(IDC_EDIT1,strFolderPath);
	}

	ListFiles();
}

void CRXmlEditorDlg::ListFiles()
{
	CListBox * pLC=(CListBox*)GetDlgItem(IDC_LIST3);
	CString sTmp;
	WIN32_FIND_DATA fd;

	GetDlgItemText(IDC_EDIT1,sTmp);
	sTmp+=_T ("\\");
	
	pLC->ResetContent();
	HANDLE hFind = ::FindFirstFile (sTmp+_T("*.config"), &fd);  
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if(CRouterXml(_bstr_t(sTmp+fd.cFileName)).IsRouterXml())
			{
				pLC->AddString(fd.cFileName);
			}
		} while (::FindNextFile (hFind, &fd));
		::FindClose (hFind); 
	}
}

void CRXmlEditorDlg::OnClose()
{
	CString sTmp;
	GetDlgItemText(IDC_EDIT1,sTmp);
	m_cfg.setDefaultSection(_T("Config"));
	m_cfg.write(_T("Folder"),sTmp);
	
	CRXmlEditorDlg::OnCancel();
}


void CRXmlEditorDlg::OnLbnSelchangeList3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szFileName,sTmp;
	m_LB_FileList.GetText(m_LB_FileList.GetCurSel(),szFileName);
	m_ED_Folder.GetWindowText(sTmp);
	szFileName=sTmp+_T("\\")+szFileName;
	m_pXml =  new CRouterXml((_bstr_t)szFileName);
	
	//���ص�TreeView
	m_LB_Project.ResetContent();
	if(m_pXml->ReadAuthorizeAttibute(m_RouterAuth))
	{
			m_LB_Project.AddString(_T("��֤"));
	}
	if(m_pXml->ReadPPPoEAttribute(m_PPPoE))
	{
		m_LB_Project.AddString(_T("PPPoE����"));
	}

	if(m_pXml->ReadTrafficAttribute(m_Traffic))
	{
		m_LB_Project.AddString(_T("����ͳ��"));
	}
}


void CRXmlEditorDlg::OnLbnSelchangeList5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sTmp;
	m_LB_Project.GetText(m_LB_Project.GetCurSel(),sTmp);

	if(sTmp=="��֤")
	{
		m_LB_Attribute.AddString(_T("��֤��ʽ"));
	}
}
