// Thread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return FALSE;//����false����Ϊ�������̣߳���ִ��run
}

int CThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

void CThread::start( void )
{
	CWinThread::CreateThread();
}

BEGIN_MESSAGE_MAP(CThread, CWinThread)
END_MESSAGE_MAP()


// CThread ��Ϣ�������
