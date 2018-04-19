#pragma once

#include "StringEx.h"
#include "HttpWebConnection.h"
#include "RouterXml.h"
#include "Thread.h"
#include "NetworkAdapter.h"

namespace Router
{
	enum WorkMode
	{
		Pause,
		QueryStat,
		QueryTrafStat
	};

	typedef enum
	{
		Null,//��ȡ����
		Dlg,//�Ի���������
		Page//��ҳ������
	}AuthMethod;

	typedef enum
	{
		PPPoE,
		DIP,
		SIP
	}WanMode;

	typedef struct RouterStatDef
	{
		BOOL							bInitialized;

		CRouterXml::RouterLinkStatus	nLinkStat;
		CString							szLinkStat;

		CRouterXml::WanLinkStat			nWanLinkStat;
		CString							szWanLinkStat;

		WorkMode						workMode;

		BOOL							bXmlLoaded;

		CString							szQueryTypeMsg;//��ȡ·�����ͺŽ��̵�״̬��Ϣ

		INT								nAutoReconnectTriedTimes;//�����ѳ��Դ���
		RouterStatDef():
			bXmlLoaded(FALSE)
			,bInitialized(FALSE)
			,nLinkStat(CRouterXml::Initilizing)
			,szLinkStat(_T("��ʼ��..."))
			,szQueryTypeMsg(_T("��ʼ��..."))
			,nWanLinkStat(CRouterXml::UnknownFailed)
			,szWanLinkStat(_T("��ʼ��..."))
			,nAutoReconnectTriedTimes(0)
			{}
	}RouterStatDef;

	typedef struct RouterInfoDef
	{
		CString szType;
		CString szIP;
		CString szURL;
		CString szUsername;
		CString szPassword;
		CString szBasicAuthorize;
		const char* pcszIP;
		AuthMethod authMethod;
		RouterInfoDef():szType(_T("")),authMethod(Null){}
	};

	typedef struct TrafficStatusDef
	{
		CString szIP;
		CString szMac;
		CString szUpDownload;
		CString szUpDownloadSpeed;
		CString szUsername;
		UINT nIndex;
		BOOL bOnline;
		CString szOnline;
	}TrafficStatusDef;

	typedef struct
	{
		CString szClientName;
		CString szMac;
		CString szIP;
	}DHCPClientDef;

	typedef CArrayEx<TrafficStatusDef>	TrafficStatusList;
	typedef CArrayEx<DHCPClientDef>		DHCPClientList;

	typedef struct ConfigDef
	{
		DWORD dwQueryInterval;
		BOOL bAutoReconnect;
		BOOL bPPPoEEncryptAcount;//PPPoE�����˺��Ƿ����
		WanMode wanMode;
		ConfigDef():dwQueryInterval(1000),bAutoReconnect(FALSE),bPPPoEEncryptAcount(TRUE),wanMode(PPPoE){}
	}ConfigDef;
	
	typedef struct RouterTaskDef
	{
		BOOL bTaskCompleted;
		CString szTaskName;
		RouterTaskDef():bTaskCompleted(TRUE){}
	}RouterTaskDef;

	
	RouterInfoDef s_MyInfo;
	CThread*	pTh_QueryStat;
	CString* s_pszCurrentForm;

	void Initialize();
	void Destroy();

	BOOL Login();
	BOOL PPPoE_Dial();
	BOOL PPPoE_Disc();
	BOOL MacFilter_On(BOOL bOn=TRUE);
	BOOL Traffic_TurnOn();
	BOOL ConfigRouter();//����·����
	
	void SetIP(CString szIP);
	void SetAuthorize(CString Username = _T(""), CString Password = _T(""));

	CString GetRouterType(){return s_MyInfo.szType;}

	BOOL createConnection(CHttpWebConnection* &pHwcRet,CRouterXml::RequestPathDef requestPath );
	void closeConnection(CHttpWebConnection* &pHwcRet);
	BOOL loadXml( CString strParam = _T("") );
	BOOL setCurrentForm(INT nFormIndex);
	void writeCurrentForm(CString szExpression);
	void writeCurrentForm(CString szName,CString szValue);
	
	DWORD WINAPI Thread_QueryRouterStatus( LPVOID lpParam );//״̬���·����߳�

	RouterStatDef g_MyStat;
	TrafficStatusList	g_TrafficStatList;
	DHCPClientList		g_DHCPClientList;
	CNetworkAdapter::AdapterInfoDef g_MyNetAdpInf;//��·����������������������Ϣ
	ConfigDef g_MyConfig;
	CString g_szDefaultUsername;
	RouterTaskDef g_WanTask;
	RouterTaskDef g_TrafficTask;
	CRouterXml	g_RouterXml;
};
