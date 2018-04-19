#pragma once
#include "Thread.h"
#include "StringEx.h"

class __declspec(dllexport)  CUdp
{
public:
	typedef enum
	{
		UnListening,
		Listening
	}ListeningStatDef;//监听状态
	
	typedef struct ListenParamsDef
	{
		CStringEx::CStringArray		szRecvLstRet;
		CStringEx::CStringArray		szSenderIpLstRet;
		BOOL				bNewRecved;
		UINT				nPort;//如果为0则使用构建类时的port
		ListenParamsDef(void):nPort(0)
		{

		}
	}ListenParamsDef;
private:
	UINT		m_nDefaultPort;
	CThread*	m_pThListen;
public:
	CUdp(void);
	CUdp(UINT nDefaultPort);
	~CUdp(void);

	BOOL sendTo(CString szIpAddr,CString szSend,UINT nPort = 0);//nPort=0则使用m_nDefaultPort
	void listen(ListenParamsDef &params);

	static DWORD WINAPI Thread_Listen(LPVOID lpParam);
	static ListeningStatDef g_ListenStat;
};

