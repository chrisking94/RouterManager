#include "stdafx.h" 
#include "resource.h"
#include "Application.h"
#include "RouterManagerDlg.h"
#include "CodeString.h"
#include <Shlwapi.h>
#include "Common.h"
#include "HttpSocket.h"
#include <afxpriv.h>
#include "Ping.h"
#include "NetworkAdapter.h"
#include "Messager.h"
#include "PluginMgr.h"
#include "BroadBand.h"
#include "WinConfig.h"

CConfig Application::g_Config;
CThread* Application::s_pThread_ShowStatus;


DWORD WINAPI Application::Thread_ShowStatus( LPVOID lpParam )
{
	AfxOleInit();

	CRouterManagerDlg* mainDlg=(CRouterManagerDlg*)lpParam;

	const DWORD cdwLinkStatusIcons[]={IDI_ICON1,IDI_ICON3,IDI_ICON2};
	
	char chLastRouterLinkStatIcon=-1;
	char chLastWanLinkStatIcon=-1;
	char chCurRouterLinkStatIcon=-1;
	char chCurWanLinkStatIcon=-1;
	CRouterXml::RouterLinkStatus	nCurRouterLinkStat=CRouterXml::TimeOut;
	CRouterXml::WanLinkStat			nCurWanLinkStat= CRouterXml::UnknownFailed;

	CString szCurPPPoELinkStat;
	CString szPPPoELinkStat_last=_T("");
	CString szPPPoEConnBtnText;
	CString sTmp;

	CButton* btn_router=(CButton*)mainDlg->m_tabPage1.GetDlgItem(IDC_BUTTON1);
	CButton* btn_pppoe=(CButton*)mainDlg->m_tabPage1.GetDlgItem(IDC_BUTTON2);
	CStatic* lbl_RouterType=(CStatic*)mainDlg->m_tabPage1.GetDlgItem(IDC_ST_RouterType);
	CStatic* edi_RouterUsername=(CStatic*)mainDlg->m_tabPage1.GetDlgItem(IDC_EDIT1);
	CStatic* edi_RouterPassword=(CStatic*)mainDlg->m_tabPage1.GetDlgItem(IDC_EDIT2);
	while (TRUE)
	{
		if(g_bAllowedShowStatus)
		{
			switch(mainDlg->m_CMainTab.GetCurSel())
			{
			case 0://页面：常规
				if(Router::g_MyStat.workMode!=Router::Pause)
				{
					Router::g_MyStat.workMode=Router::QueryStat;
				}
				
				lbl_RouterType->GetWindowText(sTmp);
				if(sTmp!=Router::g_MyStat.szQueryTypeMsg)
				{//TRACE("Here%S\n---------------------",Router::g_MyStat.szQueryTypeMsg);
					lbl_RouterType->SetWindowText(Router::g_MyStat.szQueryTypeMsg);
					mainDlg->m_tabPage1.SendMessage(WM_THREAD,0,FALSE);
				}
				
				if(Router::g_MyStat.nLinkStat!=nCurRouterLinkStat)//##路由器-修改状态文本
				{
					nCurRouterLinkStat=Router::g_MyStat.nLinkStat;

					mainDlg->m_tabPage1.m_ST_RouterLinkStatus=Router::g_MyStat.szLinkStat;
					//AfxMessageBox(mainDlg->m_tabPage1.m_ST_RouterLinkStatus);
					mainDlg->m_tabPage1.SendMessage(WM_THREAD,0,FALSE);
					if(Router::g_MyStat.nLinkStat==CRouterXml::OK)
					{
						chCurRouterLinkStatIcon=0;
						btn_router->EnableWindow(FALSE);
						edi_RouterPassword->EnableWindow(FALSE);
						edi_RouterUsername->EnableWindow(FALSE);
						mainDlg->showMsg(_T("路由器已连接！"));
						//Router::g_MyStat.bInitialized = TRUE;
					}
					else if(Router::g_MyStat.nLinkStat==CRouterXml::Initilizing)
					{
						chCurRouterLinkStatIcon=1;
						btn_router->EnableWindow(FALSE);
						edi_RouterPassword->EnableWindow(TRUE);
						if(Router::g_RouterXml.m_Router.Auth.amAuth == Router::Dlg)
						{
							edi_RouterUsername->EnableWindow(TRUE);
						}
					}
					else
					{
						btn_router->EnableWindow(TRUE);
						edi_RouterPassword->EnableWindow(TRUE);
						if(Router::g_RouterXml.m_Router.Auth.amAuth == Router::Dlg)
						{
							edi_RouterUsername->EnableWindow(TRUE);
						}

						if(Router::g_MyStat.nLinkStat==CRouterXml::Denied)
						{
							g_pMainDlg->showMsg(_T("请更正路由器账号或密码然后点击 [连接] 按钮！"));
						}
						chCurRouterLinkStatIcon=2;
					}

					if(chCurRouterLinkStatIcon!=chLastRouterLinkStatIcon)//##路由器-修改状态图标
					{
						HICON hIco=AfxGetApp()->LoadIcon(cdwLinkStatusIcons[chCurRouterLinkStatIcon]);
						((CStatic*)(mainDlg->m_tabPage1.GetDlgItem(IDC_STATIC_ROUTER_STATUS_ICON)))->SetIcon(hIco);
						chLastRouterLinkStatIcon=chCurRouterLinkStatIcon;
					}
				}

				if(Router::g_MyStat.nLinkStat==CRouterXml::OK)
				{
					/* 在这里添加后续处理代码 */

				}

				//PPPoE状态
				if(nCurWanLinkStat != Router::g_MyStat.nWanLinkStat)
				{
					nCurWanLinkStat=Router::g_MyStat.nWanLinkStat;

					if(nCurWanLinkStat == CRouterXml::Connected)
					{
						chCurWanLinkStatIcon=0;
						btn_pppoe->EnableWindow(FALSE);
						szPPPoEConnBtnText=_T("(*^__^*)");
						mainDlg->showMsg(_T("连接宽带成功！"));

						/* 在这里添加联网成功之后的操作 */

						//自动重连计数清零
						Router::g_MyStat.nAutoReconnectTriedTimes = 0;
					}
					else if(nCurWanLinkStat == CRouterXml::NotConnected)
					{
						chCurWanLinkStatIcon=2;
						btn_pppoe->EnableWindow(TRUE);
						szPPPoEConnBtnText=_T("连接");
					}
					else if(nCurWanLinkStat == CRouterXml::WaitingForRouter)
					{
						btn_pppoe->EnableWindow(FALSE);
						szPPPoEConnBtnText=_T("...");
						chCurWanLinkStatIcon=1;
					}
					else
					{
						szPPPoEConnBtnText=_T("取消");
						btn_pppoe->EnableWindow(TRUE);
						chCurWanLinkStatIcon=1;
					}

					btn_pppoe->SetWindowText(szPPPoEConnBtnText);
					mainDlg->m_tabPage1.m_ST_WanLinkStatus=Router::g_MyStat.szWanLinkStat;
					mainDlg->m_tabPage1.SendMessage(WM_THREAD,0,FALSE);

					if(chCurWanLinkStatIcon!=chLastWanLinkStatIcon)//##Wan-修改状态图标
					{
						chLastWanLinkStatIcon=chCurWanLinkStatIcon;

						HICON hIco=AfxGetApp()->LoadIcon(cdwLinkStatusIcons[chCurWanLinkStatIcon]);
						((CStatic*)(mainDlg->m_tabPage1.GetDlgItem(IDC_STATIC_PPPOE_STATUS_ICON)))->SetIcon(hIco);
					}
				}
				break;
			case 1://页面：流量
				if(Router::g_MyStat.workMode!=Router::Pause)
				{
				Router::g_MyStat.workMode=Router::QueryTrafStat;
				}
				//更新列表框
				Router::TrafficStatusDef *pCurTrafStat;
				for(int i=mainDlg->m_tabPage2.m_LC_Traffic.GetItemCount();i<Router::g_TrafficStatList.GetLength();i++)
				{
					//追加空白的Item到列表
					mainDlg->m_tabPage2.m_LC_Traffic.InsertItem(i,i,_T(""),0,0,0,0);
				}
				for(int i=Router::g_TrafficStatList.GetLength();i<mainDlg->m_tabPage2.m_LC_Traffic.GetItemCount();i++)
				{
					//删除多余Item
					mainDlg->m_tabPage2.m_LC_Traffic.DeleteItem(i);
				}

				for(int i =0 ;i<Router::g_TrafficStatList.GetLength();i++)
				{
					pCurTrafStat=&Router::g_TrafficStatList[i];

					if(mainDlg->m_tabPage2.m_LC_Traffic.GetItemText(i,0)!=pCurTrafStat->szUsername)//用户名
					{
						mainDlg->m_tabPage2.m_LC_Traffic.SetItemText(i,0,pCurTrafStat->szUsername);
					}
					if(mainDlg->m_tabPage2.m_LC_Traffic.GetItemText(i,1)!=pCurTrafStat->szUpDownload)//总流量
					{
						mainDlg->m_tabPage2.m_LC_Traffic.SetItemText(i,1,pCurTrafStat->szUpDownload);
					}
					if(mainDlg->m_tabPage2.m_LC_Traffic.GetItemText(i,2)!=pCurTrafStat->szUpDownloadSpeed)//流率
					{
						mainDlg->m_tabPage2.m_LC_Traffic.SetItemText(i,2,pCurTrafStat->szUpDownloadSpeed);
					}
					if(mainDlg->m_tabPage2.m_LC_Traffic.GetItemText(i,3)!=pCurTrafStat->szOnline)//流率
					{
						mainDlg->m_tabPage2.m_LC_Traffic.SetItemText(i,3,pCurTrafStat->szOnline);
					}
				}
				//流量统计.END
			}

		}
		Sleep(Router::g_MyConfig.dwQueryInterval);
	}

	return TRUE;
}

void Application::Initialize( LPVOID pDlg )
{
	g_pMainDlg=(CRouterManagerDlg*)pDlg;
	//创建线程-状态更新线程
	s_pThread_ShowStatus=new CThread((AFX_THREADPROC)Application::Thread_ShowStatus,pDlg);
	s_pThread_ShowStatus->start();
	s_pThread_ExecuteTask=new CThread((AFX_THREADPROC)Application::Thread_ExecuteTask,NULL);
	s_pThread_ExecuteTask->start();
	s_pThread_Control=new CThread((AFX_THREADPROC)Application::Thread_Control,pDlg);
	s_pThread_Control->start();
	//创建完毕
	
}

void Application::InitializeVariable()
{
	GetCurrentDirectory(MAX_PATH,s_szStartUpPath.GetBuffer(MAX_PATH));
	s_szStartUpPath.ReleaseBuffer();

	s_RouterXmlFolder=s_szStartUpPath+_T("\\Routers\\");

	if(!PathIsDirectory(s_RouterXmlFolder))
	{
		CreateDirectory(s_RouterXmlFolder,NULL);
	}
	
	g_Config=CConfig(s_szStartUpPath+_T("\\Config.ini"),HKEY_LOCAL_MACHINE,_T("SoftWare\\CrystSoft\\RouterManager"));
	g_WinConfig=CWinConfig(s_szStartUpPath+_T("\\Config.ini"));

	g_pMainAdvertise = new User::PicAdDef;
	g_pTrafAdvertise = new User::PicAdDef;

	g_pMainAdvertise->strLinkUrl = _T("http://www.crystsoft.xyz");
	g_pTrafAdvertise->strLinkUrl = _T("http://www.crystsoft.xyz");

	InitializeModules();
}

void Application::Destroy( void )
{
	//清理变量
	if(s_pThread_Control!=NULL)
	{
		delete s_pThread_Control;
		s_pThread_Control=NULL;
	}
	if(s_pThread_ShowStatus!=NULL)
	{
		delete s_pThread_ShowStatus;
		s_pThread_ShowStatus=NULL;
	}
	if(s_pThread_ExecuteTask)
	{
		delete s_pThread_ExecuteTask;
		s_pThread_ExecuteTask=NULL;
	}

	Messager::Destroy();
	Router::Destroy();
}


DWORD WINAPI Application::Thread_ExecuteTask( LPVOID lpParam )
{
	AfxOleInit();
	
	while(true)
	{
		if(s_TaskList.GetLength()>0)
		{
			g_pMainDlg->showMsg(_T(""),_T("正在执行任务：")+s_TaskList[0].m_strTaskName);
			s_TaskList[0].m_pTaskFuncAddr();
			g_pMainDlg->showMsg(_T(""),_T("任务完成：")+s_TaskList[0].m_strTaskName);
			s_TaskList.RemoveAt(0);
		}
		Sleep(100);
	}
}

void Application::Task_BroadBandConnect()
{
	CString sTmp;
	(g_pMainDlg->m_tabPage1.GetDlgItem(IDC_EDIT4))->GetWindowText(sTmp);
	BroadBand::SetAccount(sTmp);
	(g_pMainDlg->m_tabPage1.GetDlgItem(IDC_EDIT5))->GetWindowText(sTmp);
	BroadBand::SetPassword(sTmp);
	
	if(BroadBand::Connect())
	{
		ShowMsg(_T("正在连接..."));
	}
	else
	{
		ShowMsg(_T("连接失败."));
	}
	((CWnd*)g_pMainDlg->m_tabPage1.GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE));
}

DWORD WINAPI Application::Thread_Control( LPVOID lpParam )
{
	CRouterManagerDlg* mainDlg=(CRouterManagerDlg*)lpParam;
	DWORD dwMainPageAdCounter=0;
	DWORD dwTrafPageAdCounter=0;
	CButton * btn=(CButton *)mainDlg->m_tabPage1.GetDlgItem(IDC_CHECK1);
	CStatic * routerlnkStat=(CStatic *)mainDlg->m_tabPage1.GetDlgItem(IDC_ST_ROUTER_LINKSTATE);
	CStatic * pppoelnkStat=(CStatic *)mainDlg->m_tabPage1.GetDlgItem(IDC_ST_PPPOE_LINKSTATE);
	CListCtrl * pTrafLV = (CListCtrl*)mainDlg->m_tabPage2.GetDlgItem(IDC_LIST1);
	Messager::UDPMsgDef MsgReciever;

	while(true)
	{
		if(Router::g_MyStat.workMode!=Router::Pause)
		{
			if(User::g_Info.bAdvertiseDownloaded)
			{
				if(dwMainPageAdCounter<g_pMainAdvertise->dwInterval)
				{
					dwMainPageAdCounter++;
				}
				else
				{
					dwMainPageAdCounter=0;

					Control_MainDlgShowAdvertise();
				}

				if(dwTrafPageAdCounter<g_pTrafAdvertise->dwInterval)
				{
					dwTrafPageAdCounter++;
				}
				else
				{
					dwTrafPageAdCounter=0;

					Control_TrafDlgShowAdvertise();
				}
			}
		}

		if(Messager::ReadMsg(MsgReciever))//有新消息
		{
			switch(MsgReciever.msgType)
			{
			case Messager::SimpleMsg:
				mainDlg->MessageBox(MsgReciever.szMsg,_T("来自  ")+MsgReciever.senderInfo.szRemark+_T("(")+MsgReciever.senderInfo.szName+_T(")  的消息："),0);
				break;
			}
		}

		Sleep(5);
	}
}

UINT Application::AddThreadTask( void (*pFuncAddr)() , CString strThreadName /*= _T("无名称")*/ )
{
	return s_TaskList.Append(ThreadTask(pFuncAddr,strThreadName));
}

void Application::Task_BroadBandDisconnect()
{
	if(BroadBand::Disconnect())
	{
		ShowMsg(_T("正在执行."));
	}
	
	((CWnd*)g_pMainDlg->m_tabPage1.GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE));
}


void Application::SetStatusUpdateInterval( DWORD dwSecond )
{
	Router::g_MyConfig.dwQueryInterval=dwSecond*1000;
}

bool Application::CheckForUpdate()
{
	g_pMainDlg->showMsg(_T("尝试检查更新..."));
	CUpdate upd(Common::GetWebFolder());
	CString strComment;
	if(upd.NewVersionAvailable(strComment))
	{
		if(strComment==_T("err"))
		{
			g_pMainDlg->showMsg(_T("无法检查更新！"));
		}
		else
		{
			g_bUpdateChecked=true;
			if(g_pMainDlg->MessageBox(_T("有新版本可用，是否立即更新？\n\n新版信息：\n")+strComment,_T("更新"),MB_YESNO|MB_ICONQUESTION)==6)
			{
				if(PathFileExists(Common::GetStartUpPath()+_T("update.exe")))
				{
					ShellExecute(NULL,_T("open"),Common::GetStartUpPath()+_T("update.exe"),_T("RouterManager"),NULL,SW_SHOW);
				}
				else
				{
					g_pMainDlg->MessageBox(_T("无法启动Update.exe！文件不存在。"),_T("错误"),MB_YESNO|MB_ICONSTOP);
				}   
			}
		}
	}
	else
	{
		g_bUpdateChecked=true;
		g_pMainDlg->showMsg(_T("当前版本为最新！"));
		return false;
	}
	g_pMainDlg->showMsg(_T("检查更新完毕！"));
	return true;
}

void Application::Control_MainDlgShowAdvertise()
{
	static int i=0;
	CString sTmp;
	Common::GetMyPicturesPath(sTmp);
	if(i==g_pMainAdvertise->strArrFiles.GetLength())
	{
		i=0;//循环
		return;
	}
	
	sTmp+=g_pMainAdvertise->strArrFiles[i++];
	if(g_MainPageAdPic.LoadFile(sTmp))
	{
		g_MainPageAdPic.SetDrawPictureCtrl(&(g_pMainDlg->m_tabPage1),g_pMainDlg->m_tabPage1.GetDlgItem(IDC_PC_ADMAIN));
		g_MainPageAdPic.DrawOnPictureCtrl();
	}
}

void Application::Control_TrafDlgShowAdvertise()
{
	static int i=0;
	CString sTmp;
	Common::GetMyPicturesPath(sTmp);
	if(i==g_pTrafAdvertise->strArrFiles.GetLength())
	{
		i=0;//循环
		return;
	}
	sTmp+=g_pTrafAdvertise->strArrFiles[i++];
	if(g_TrafPageAdPic.LoadFile(sTmp))
	{
		g_TrafPageAdPic.SetDrawPictureCtrl(&(g_pMainDlg->m_tabPage2),g_pMainDlg->m_tabPage2.GetDlgItem(IDC_PC_ADTRAF));
		g_TrafPageAdPic.DrawOnPictureCtrl();
	}
}

void Application::Task_TrafficStatusOn()
{
	Router::Traffic_TurnOn();
}

void Application::Task_CheckForUpdate()
{
	CheckForUpdate();
}

void Application::Task_DownloadRouterXmls()
{
	g_pMainDlg->showMsg(_T("正在联网检查支持的路由器型号"));
	CUpdate upd(Common::GetWebFolder()+_T("Routers/"),Common::GetStartUpPath()+_T("Routers\\"));
	CUpdate::FileVersionArray fva;
	CString szComment;
	upd.ReadWebFileVersions(fva,szComment);
	upd.GetAvailableNewVersions(fva);
	INT i;
	CString sSupport;
	
	for(i=0;i<fva.GetLength();i++)
	{
		CHttpWebConnection hwc(Common::GetWebFolder()+_T("Routers/")+fva[i].strFilePath);
		CString sLocalPath(Common::GetStartUpPath()+_T("Routers\\")+fva[i].strFilePath);
		if(hwc.sendRequest())
		{
			BYTE buf[128];
			CFile dlFile(sLocalPath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
			while(UINT nRead=hwc.readBytes(buf,128))
			{
				dlFile.Write(buf,nRead);
			}
			dlFile.Close();
		}
		hwc.close();
	}
	if(i>0)
	{
		CString sTmp;
		sTmp.Format(_T("%d"),i);
		g_pMainDlg->showMsg(_T("新增 ")+sTmp+_T(" 个文件"));
		//s_pMainDlg->MessageBox(_T("点击组合框查看新支持的路由器\r\n\r\n说明：\r\n")+szComment,sTmp+_T("个新文件已下载"),MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		g_pMainDlg->showMsg(_T("没有获取到新支持的型号"));
	}
}

void Application::Task_InitRouter()
{
	Router::Initialize();
}

void Application::Task_InitMessager()
{
	if(Router::g_MyStat.bInitialized)
	{
		Messager::Initialize(Application::s_szStartUpPath,Router::g_MyNetAdpInf.szMac,Router::g_MyNetAdpInf.szIp);
	}
}

void Application::Task_InitPluginMgr()
{
	if(!PluginMgr::g_bInitialized)
	{
		PluginMgr::Initialize();
		//更新窗口数据
		g_pMainDlg->m_tabPage1.m_CBB_Plugins.Clear();
		for(int i=0;i<PluginMgr::g_PluginList.GetLength();i++)
		{
			g_pMainDlg->m_tabPage1.m_CBB_Plugins.AddString(PluginMgr::g_PluginList[i].szName);
		}
		g_pMainDlg->m_tabPage1.m_CBB_Plugins.SelectString(0,PluginMgr::g_pCurPlugin->szName);
	}
}

void Application::InitializeModules()
{
	AddThreadTask(Task_InitRouter,_T("初始化路由器."));
	AddThreadTask(Task_InitMessager,_T("初始化Messager."));
	AddThreadTask(Task_InitUser,_T("初始化用户"));
	AddThreadTask(Task_InitPluginMgr,_T("初始化插件管理器"));
}

void Application::ShowMsg( CString szMsg1/*=_T("")*/,CString szMsg2/*=_T("")*/ )
{
	g_pMainDlg->showMsg(szMsg1,szMsg2);
}

void Application::Task_InitUser()
{
	if(Router::g_MyStat.bInitialized)
	{
		User::Initialize();
	}
}

void Application::SetRouterUsername( CString szVal ,BOOL bSetWhenEmpty /*= FALSE*/ )
{
	CStatic* pEdit_RouterUsername=(CStatic*)g_pMainDlg->m_tabPage1.GetDlgItem(IDC_EDIT1);
	CString szTmp;

	if(szVal.IsEmpty())
	{
		return;
	}
	
	if(bSetWhenEmpty)
	{
		if(Router::g_RouterXml.m_Router.Auth.amAuth != CRouterXml::Dlg)//必须是默认用户名，admin之类
		{
			pEdit_RouterUsername->EnableWindow(FALSE);
			bSetWhenEmpty = FALSE;
		}
		else
		{
			pEdit_RouterUsername->GetWindowText(szTmp);
			if(szTmp.IsEmpty())
			{
				bSetWhenEmpty = FALSE;
			}
		}
	}

	if(!bSetWhenEmpty)
	{
		pEdit_RouterUsername->SetWindowText(szVal);
	}
}

void Application::SetRouterPassword( CString szVal ,BOOL bSetWhenEmpty /*= FALSE*/ )
{
	CStatic* pEdit_RouterPassword=(CStatic*)g_pMainDlg->m_tabPage1.GetDlgItem(IDC_EDIT2);
	CString szTmp;

	if(szVal.IsEmpty())
	{
		return;
	}

	if(bSetWhenEmpty)
	{
		pEdit_RouterPassword->GetWindowText(szTmp);
		if(szTmp.IsEmpty())
		{
			bSetWhenEmpty = FALSE;
		}
	}

	if(!bSetWhenEmpty)
	{
		pEdit_RouterPassword->SetWindowText(szVal);
	}
}

CWinConfig Application::g_WinConfig;



BOOL Application::g_bAllowedShowStatus=TRUE;

bool Application::g_bUpdateChecked=false;

Application::ThreadTaskList Application::s_TaskList;

CPicture Application::g_TrafPageAdPic;

CPicture Application::g_MainPageAdPic;

User::PicAdDef *Application::g_pTrafAdvertise;

User::PicAdDef *Application::g_pMainAdvertise;

CRouterManagerDlg* Application::g_pMainDlg;

CThread * Application::s_pThread_Control;

CThread * Application::s_pThread_ExecuteTask;

CString Application::s_RouterXmlFolder;

CString Application::s_szStartUpPath;


