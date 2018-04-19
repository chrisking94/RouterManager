#include "StdAfx.h"
#include "Udp.h"
#include "winsock2.h"
#include <afxpriv.h>

CUdp::CUdp(void):
m_nDefaultPort(9000)
{
}

CUdp::CUdp( UINT nDefaultPort )
{
	m_nDefaultPort=nDefaultPort;
}


CUdp::~CUdp(void)
{
}

BOOL CUdp::sendTo( CString szIpAddr ,CString szSend ,UINT nPort )
{
	CSocket socket;

	if(nPort==0) nPort=m_nDefaultPort;
	
	if(!socket.Create(m_nDefaultPort+1,SOCK_DGRAM,NULL))
	{
		socket.Close();
		return FALSE;
	}
	else
	{
		static char *send;
		USES_CONVERSION;
		send=T2A(szSend.GetBuffer());
		socket.SendTo(send,strlen(send),nPort,szIpAddr,0);
		szSend.ReleaseBuffer();
	}
	socket.Close();
	return TRUE;
}

void CUdp::listen(ListenParamsDef &params)
{
	if(params.nPort==0)
	{
		params.nPort=m_nDefaultPort;
	}
	m_pThListen = new CThread((AFX_THREADPROC)Thread_Listen,&params);
	m_pThListen->start();
}

DWORD WINAPI CUdp::Thread_Listen( LPVOID lpParam )
{
	CSocket socket;
	ListenParamsDef* pLParam=(ListenParamsDef*)lpParam;

	if(socket.Create(pLParam->nPort,SOCK_DGRAM,NULL)==0)
	{
		CString strErrCode;
		DWORD nErr=GetLastError();
		if(nErr==10048)
		{
			strErrCode=_T("端口被占用！");
		}
		else
		{
			strErrCode.Format(_T("错误码：%d"),strErrCode);
		}
		
		AfxMessageBox(_T("UDP 服务启动失败，")+strErrCode);
		return 0;
	}

	g_ListenStat=Listening;
	while(TRUE)
	{
		SOCKADDR_IN ClntAddr;
		int clntAddrLen = sizeof(ClntAddr);
		char recvBuffer[1024]="";
		static CString save;


		//阻塞
		int recvSize = socket.ReceiveFrom(recvBuffer,sizeof(recvBuffer),(SOCKADDR*)&ClntAddr,&clntAddrLen,0);
		
		if(recvSize>0)
		{
			//获取发送方IP
			char *addr = inet_ntoa(ClntAddr.sin_addr);
			CString sTmp;
			USES_CONVERSION;
			sTmp.Format(_T("%s"),A2T(addr));
			pLParam->szSenderIpLstRet.Append(sTmp);
			sTmp.Format(_T("%s"),A2T(recvBuffer));
			pLParam->szRecvLstRet.Append(sTmp);
			pLParam->bNewRecved=TRUE;
		}
	}
	socket.Close();

	return 0;
}

CUdp::ListeningStatDef CUdp::g_ListenStat=UnListening;

