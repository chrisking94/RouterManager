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
	}ListeningStatDef;//����״̬
	
	typedef struct ListenParamsDef
	{
		CStringEx::CStringArray		szRecvLstRet;
		CStringEx::CStringArray		szSenderIpLstRet;
		BOOL				bNewRecved;
		UINT				nPort;//���Ϊ0��ʹ�ù�����ʱ��port
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

	BOOL sendTo(CString szIpAddr,CString szSend,UINT nPort = 0);//nPort=0��ʹ��m_nDefaultPort
	void listen(ListenParamsDef &params);

	static DWORD WINAPI Thread_Listen(LPVOID lpParam);
	static ListeningStatDef g_ListenStat;
};

