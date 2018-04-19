// Thread.cpp : 实现文件
//

#include "stdafx.h"
#include "Thread.h"


// CThread

IMPLEMENT_DYNCREATE(CThread, CWinThread)

CThread::CThread()
{
	
}

CThread::CThread( AFX_THREADPROC pfnThreadProc,LPVOID pParam/*=NULL*/ ):
CWinThread(pfnThreadProc,pParam)
{
	
}

CThread::~CThread()
{
	
}

BOOL CThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return FALSE;//返回false代表为工作者线程，不执行run
}

int CThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

void CThread::start( void )
{
	CWinThread::CreateThread();
}

BEGIN_MESSAGE_MAP(CThread, CWinThread)
END_MESSAGE_MAP()


// CThread 消息处理程序
