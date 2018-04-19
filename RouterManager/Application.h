// Application.h	负责将各模块与窗口对接
#pragma once

#include "Router.h"
#include "Thread.h"
#include "Config.h"
#include "CodeString.h"
#include "Regex.h"
#include "StringEx.h"
#include "Xml.h"
#include "RouterManagerDlg.h"
#include "user.h"
#include "Picture.h"
#include "ArrayEx.h"
#include "NetworkAdapter.h"
#include "Udp.h"
#include "Reg.h"
#include "WinConfig.h"

#define  WM_THREAD WM_USER+100
#define  UM_TRAYNOTIFY WM_USER + 11

class Application
{
public:
	Application(void);
	typedef struct ThreadTask
	{
		CString m_strTaskName;
		void (*m_pTaskFuncAddr)(void);
		
		ThreadTask():m_strTaskName(_T("")),m_pTaskFuncAddr(NULL){};
		ThreadTask( void (*pTaskFuncAddr)() , CString strTaskName ):
		m_pTaskFuncAddr(pTaskFuncAddr)
			,m_strTaskName(strTaskName)
		{
			
		}
		
	}ThreadTask;

	typedef CArrayEx<ThreadTask> ThreadTaskList;

	typedef struct BroadBandDef
	{
		CString szAccount;
		CString szPassword;

		CString szAccEnc;
		CString szPswEnc;
	}BroadBandDef;
public:
	static void InitializeVariable();
	static void Initialize(LPVOID pDlg);
	static void InitializeModules();
	static void Destroy(void);
	static CString getRouterXmlFolder(){return s_RouterXmlFolder;}
	static UINT AddThreadTask(void (*pFuncAddr)() , CString strThreadName = _T("系统任务"));//添加后返回任务在列表中的位置
	static void SetStatusUpdateInterval(DWORD dwSecond);
	static bool CheckForUpdate();
	static void ShowMsg(CString szMsg1=_T(""),CString szMsg2=_T(""));

	static void SetRouterUsername(CString szVal ,BOOL bSetWhenEmpty = FALSE);//bSetWhenEmpty=TRUE表示当Username为空时才设定
	static void SetRouterPassword(CString szVal ,BOOL bSetWhenEmpty = FALSE);
public:
	static CConfig g_Config;
	static CWinConfig g_WinConfig;
	static User::PicAdDef *g_pMainAdvertise;
	static User::PicAdDef *g_pTrafAdvertise;
	static CPicture g_MainPageAdPic;
	static CPicture g_TrafPageAdPic;
	static bool g_bUpdateChecked;
	static BOOL g_bAllowedShowStatus;
	static BroadBandDef g_BroadBand;
	static CRouterManagerDlg* g_pMainDlg;
public:
	//Tread Tasks
	static void Task_BroadBandConnect();
	static void Task_BroadBandDisconnect();
	static void Task_TrafficStatusOn();
	static void Task_CheckForUpdate();
	static void Task_DownloadRouterXmls();
	static void Task_InitRouter();
	static void Task_InitMessager();
	static void Task_InitUser();
	static void Task_InitPluginMgr();

	//Thread Control
	static void Control_MainDlgShowAdvertise();
	static void Control_TrafDlgShowAdvertise();
private:
	/* 3条核心线程 */
	static DWORD WINAPI Thread_ShowStatus( LPVOID lpParam );//状态更新服务线程
	static DWORD WINAPI Thread_ExecuteTask( LPVOID lpParam);//要执行任务时使用AddThreadTask
	static DWORD WINAPI Thread_Control(LPVOID lpParam);//杂类任务控制线程
	static CThread *s_pThread_ShowStatus;
	static CThread *s_pThread_ExecuteTask;
	static CThread *s_pThread_Control;
	static CString s_szStartUpPath;
	static CString s_RouterXmlFolder;
	static ThreadTaskList s_TaskList;
};

