
// UpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "UpdateDlg.h"
#include "afxdialogex.h"
#include "Common.h"
#include "resource.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpdateDlg 对话框




CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUpdateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUpdateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CUpdateDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUpdateDlg 消息处理程序

BOOL CUpdateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CListCtrl* pLC_FileInfo=(CListCtrl*) this->GetDlgItem(IDC_LIST1);
	pLC_FileInfo->SetExtendedStyle(pLC_FileInfo->GetExtendedStyle()
		|LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES);
	pLC_FileInfo->InsertColumn(0,_T("路径"),0,200);
	pLC_FileInfo->InsertColumn(1,_T("版本"),0,80);
	pLC_FileInfo->InsertColumn(2,_T("大小"),0,60);
	pLC_FileInfo->InsertColumn(3,_T("下载进度"),0,80);

	if(CString(AfxGetApp()->m_lpCmdLine)!="")
	{
		OnBnClickedButton1();
		ShowWindow(SW_HIDE);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUpdateDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CUpdateDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CWnd * btn=(CWnd*)this->GetDlgItem(IDC_BUTTON1);
	CString strTmp;
	this->GetDlgItemTextW(IDC_BUTTON1,strTmp);
	if(strTmp==_T("检查更新"))
	{
		m_pThreadUpdate=new CThread((AFX_THREADPROC)Thread_Update,this);
		m_pThreadUpdate->start();
	}
	else
	{
		CUpdateDlg::OnOK();
	}
}

DWORD WINAPI CUpdateDlg::Thread_Update( LPVOID lpParam )
{
	AfxOleInit();

	CString strCmdLine=AfxGetApp()->m_lpCmdLine;
	BOOL bCmdLineNull=FALSE;
	if(strCmdLine=="")
	{
		bCmdLineNull=TRUE;
	}

	CUpdateDlg* pUdlg=(CUpdateDlg*) lpParam;
	CListCtrl* pLC_FileInfo=(CListCtrl*) pUdlg->GetDlgItem(IDC_LIST1);
	CStatic* VersionCommet=(CStatic*)pUdlg->GetDlgItem(IDC_STATIC_VERCOM);
	CProgressCtrl* CurrentProgressCtrl=(CProgressCtrl*)pUdlg->GetDlgItem(IDC_PROGRESS1);
	CButton* btn= (CButton*)pUdlg->GetDlgItem(IDC_BUTTON1);

	btn->EnableWindow(FALSE);
	pUdlg->SetWindowText(_T("Update - 正在获取版本信息..."));

	if(bCmdLineNull)
	{
		strCmdLine=(CIniFile(Common::GetStartUpPath()+_T("\Config.ini")).read(_T("Config"),_T("Project")));
	}
	if(strCmdLine==_T(""))
	{
		strCmdLine=_T("RouterManager");
	}

	CString sTmp(_T("http://www.crystsoft.xyz/products/")+strCmdLine+_T("/"));
	//AfxMessageBox(sTmp);
	LPCTSTR lpszUpdateWebFolder=sTmp;
	CUpdate upd(lpszUpdateWebFolder);
	CString strLocalFolder=Common::GetStartUpPath();
	CUpdate::FileVersionArray fva;
	CUpdate::FileVersionDef* pFvWeb;
	CString strComment;
	CString strCmds;
	upd.ReadWebFileVersions(fva,strComment);
	upd.ReadCmds(strCmds);
	
	pUdlg->SetWindowText(_T("Update"));

	if(strComment==_T("err"))
	{
		if(!bCmdLineNull)
		{
			exit(0);
		}

		btn->EnableWindow(TRUE);
		pUdlg->MessageBox(_T("无法连接服务器，请检查网络后重试！"),_T("错误"),MB_OK|MB_ICONSTOP);
		
		return 0;
	}
	
	UINT newVFileCount=upd.GetAvailableNewVersions(fva);

	CurrentProgressCtrl->SetRange(0,100);
	VersionCommet->SetWindowText(strComment);
	if(newVFileCount==0)
	{
		if(!bCmdLineNull)
		{
			exit(0);
		}

		btn->EnableWindow(TRUE);
		btn->SetWindowText(_T("确定"));
		pUdlg->MessageBox(_T("您现在使用的版本已为最新！"),_T("提示"),MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	pUdlg->ShowWindow(SW_SHOWNORMAL);
	
	CString strTmp;
	for(UINT i=0 ; i<newVFileCount;i++)//显示列表
	{
		pFvWeb=&fva[i];

		pLC_FileInfo->InsertItem(i,pFvWeb->strFilePath);
		strTmp.Format(_T("%d.%d.%d.%d"),pFvWeb->btVer_Major,pFvWeb->btVer_Minor,pFvWeb->btVer_Revision,0);
		pLC_FileInfo->SetItemText(i,1,strTmp);
		pLC_FileInfo->SetItemText(i,2,CCodeString::ConvertUnits(pFvWeb->ulFileSize));
		pLC_FileInfo->SetItemText(i,3,_T("等待..."));
	}
	//开始下载

	for(UINT i=0 ; i<newVFileCount;i++)
	{
		pFvWeb=&fva[i];

		ULONG fileSize=0;
		ULONG lenPerPercent=0;//进度条每个单位(1，总共100)所带表的文件大小
		ULONG lenProcess=0;//下载的长度，达到lenPerPercent时进度条+1；lenProcess-=lenPerPercent;
		BYTE pBuffer[128];
		UINT nRead=0;
		CString curLocalFilePath;
		//初始化
		pLC_FileInfo->SetSelectionMark(i);
		CurrentProgressCtrl->SetPos(0);

		lenPerPercent=pFvWeb->ulFileSize/100;

		CHttpWebConnection hwc(lpszUpdateWebFolder+pFvWeb->strFilePath);
		hwc.sendRequest();

		curLocalFilePath=strLocalFolder+pFvWeb->strFilePath;
		CFile dlFile(curLocalFilePath+_T(".upd"),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);

		m_bDownloading=TRUE;
		while(nRead=hwc.readBytes(pBuffer,128))//下载文件
		{
			dlFile.Write(pBuffer,nRead);
			lenProcess+=nRead;

			if(lenProcess>lenPerPercent)
			{
				lenProcess-=lenPerPercent;
				CurrentProgressCtrl->OffsetPos(1);
				CString strTmp;
				strTmp.Format(_T("%d%%"),CurrentProgressCtrl->GetPos());
				pLC_FileInfo->SetItemText(i,3,strTmp);
			}
		}
		
		pLC_FileInfo->SetItemText(i,3,_T("已下载"));

		dlFile.Close();
		hwc.close();

		CString strBatWriteOut;
		if(pFvWeb->bExeOrDll)
		{
			ucs.addExcutionFile(pFvWeb->strFilePath);
		}
		ucs.addFile(pFvWeb->strFilePath);
	}
	ucs.addCmds(strCmds);
	USES_CONVERSION;
	ucs.writeOut();
	pUdlg->SetWindowText(_T("Update - 下载完成！"));
	btn->SetWindowText(_T("下一步"));

	btn->EnableWindow(TRUE);

	CurrentProgressCtrl->SetPos(100);

	m_bDownloading=FALSE;

	return 0;
}


void CUpdateDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//运行bat
	if(m_bDownloading)
	{
		this->ShowWindow(SW_SHOWMINIMIZED);
	}
	else
	{
		ucs.runBat();
		CDialogEx::OnClose();
	}
}

void CUpdateDlg::OnOK()
{
	if(m_bDownloading)
	{
		this->ShowWindow(SW_SHOWMINIMIZED);
	}
	else
	{
		ucs.runBat();
		CDialogEx::OnOK();
	}
}

BOOL CUpdateDlg::m_bDownloading=FALSE;

//创建update.bat
CUpdate::UpdateCmdString CUpdateDlg::ucs(Common::GetStartUpPath()+_T("update.bat"));
