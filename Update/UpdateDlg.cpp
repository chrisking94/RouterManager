
// UpdateDlg.cpp : ʵ���ļ�
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


// CUpdateDlg �Ի���




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


// CUpdateDlg ��Ϣ�������

BOOL CUpdateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CListCtrl* pLC_FileInfo=(CListCtrl*) this->GetDlgItem(IDC_LIST1);
	pLC_FileInfo->SetExtendedStyle(pLC_FileInfo->GetExtendedStyle()
		|LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES);
	pLC_FileInfo->InsertColumn(0,_T("·��"),0,200);
	pLC_FileInfo->InsertColumn(1,_T("�汾"),0,80);
	pLC_FileInfo->InsertColumn(2,_T("��С"),0,60);
	pLC_FileInfo->InsertColumn(3,_T("���ؽ���"),0,80);

	if(CString(AfxGetApp()->m_lpCmdLine)!="")
	{
		OnBnClickedButton1();
		ShowWindow(SW_HIDE);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpdateDlg::OnPaint()
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
HCURSOR CUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUpdateDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}


void CUpdateDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CWnd * btn=(CWnd*)this->GetDlgItem(IDC_BUTTON1);
	CString strTmp;
	this->GetDlgItemTextW(IDC_BUTTON1,strTmp);
	if(strTmp==_T("������"))
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
	pUdlg->SetWindowText(_T("Update - ���ڻ�ȡ�汾��Ϣ..."));

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
		pUdlg->MessageBox(_T("�޷����ӷ�������������������ԣ�"),_T("����"),MB_OK|MB_ICONSTOP);
		
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
		btn->SetWindowText(_T("ȷ��"));
		pUdlg->MessageBox(_T("������ʹ�õİ汾��Ϊ���£�"),_T("��ʾ"),MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	pUdlg->ShowWindow(SW_SHOWNORMAL);
	
	CString strTmp;
	for(UINT i=0 ; i<newVFileCount;i++)//��ʾ�б�
	{
		pFvWeb=&fva[i];

		pLC_FileInfo->InsertItem(i,pFvWeb->strFilePath);
		strTmp.Format(_T("%d.%d.%d.%d"),pFvWeb->btVer_Major,pFvWeb->btVer_Minor,pFvWeb->btVer_Revision,0);
		pLC_FileInfo->SetItemText(i,1,strTmp);
		pLC_FileInfo->SetItemText(i,2,CCodeString::ConvertUnits(pFvWeb->ulFileSize));
		pLC_FileInfo->SetItemText(i,3,_T("�ȴ�..."));
	}
	//��ʼ����

	for(UINT i=0 ; i<newVFileCount;i++)
	{
		pFvWeb=&fva[i];

		ULONG fileSize=0;
		ULONG lenPerPercent=0;//������ÿ����λ(1���ܹ�100)��������ļ���С
		ULONG lenProcess=0;//���صĳ��ȣ��ﵽlenPerPercentʱ������+1��lenProcess-=lenPerPercent;
		BYTE pBuffer[128];
		UINT nRead=0;
		CString curLocalFilePath;
		//��ʼ��
		pLC_FileInfo->SetSelectionMark(i);
		CurrentProgressCtrl->SetPos(0);

		lenPerPercent=pFvWeb->ulFileSize/100;

		CHttpWebConnection hwc(lpszUpdateWebFolder+pFvWeb->strFilePath);
		hwc.sendRequest();

		curLocalFilePath=strLocalFolder+pFvWeb->strFilePath;
		CFile dlFile(curLocalFilePath+_T(".upd"),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);

		m_bDownloading=TRUE;
		while(nRead=hwc.readBytes(pBuffer,128))//�����ļ�
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
		
		pLC_FileInfo->SetItemText(i,3,_T("������"));

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
	pUdlg->SetWindowText(_T("Update - ������ɣ�"));
	btn->SetWindowText(_T("��һ��"));

	btn->EnableWindow(TRUE);

	CurrentProgressCtrl->SetPos(100);

	m_bDownloading=FALSE;

	return 0;
}


void CUpdateDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����bat
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

//����update.bat
CUpdate::UpdateCmdString CUpdateDlg::ucs(Common::GetStartUpPath()+_T("update.bat"));
