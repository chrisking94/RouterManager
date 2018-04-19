#pragma once



// CThread：工作者线程

class __declspec(dllexport) CThread : public CWinThread
{
	DECLARE_DYNCREATE(CThread)
public:
	CThread(void);
	CThread(AFX_THREADPROC pfnThreadProc,LPVOID pParam=NULL);
	virtual ~CThread();

	void start(void);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


