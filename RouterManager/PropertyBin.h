#pragma once
#include "RecordHeapFile.h"

class CPropertyBin:private CRecordHeapFile
{
public:
	typedef enum
	{
		NotConnected,//未连接
		Connected,//已连接
		Connecting,//正在连接...
		AccessDenied,//用户名或密码验证失败
		ServerNoResponse,//服务器无响应
		UnknownFailed,//未知原因失败
		WanNotConnected,//WAN口未连接！
		WaitingForRouter,//等待连接路由器
	}WanLinkStat;

	struct
	{
		LPCTSTR lpszPath;

		struct CTRL
		{
			INT nPathIndex;
			CString szAccount;
			CString szPassword;
			CString szConfirm;
			CString szConnect;
			CString szDisconnect;
			CString szWanType;
			CString szLinkType;
			CString szSave;
			CTRL():nPathIndex(0),szPassword(_T("")),szDisconnect(_T(""))
				,szConnect(_T("")),szConfirm(_T(""))
				,szAccount(_T("")){}
		}Ctrl;

		struct STAT
		{
			INT nPathIndex;
			CString szStatRegex;
			INT nStatRegSubItem;//为2
			CArrayEx<INT> nStatCodeMapList;//状态码列表，对应状态文本列表
			UINT nMapLength;
			STAT():nStatRegSubItem(2){}
		}Stat;
	}m_PPPoE;

	typedef struct
	{
		INT nPathIndex;
		struct CTRL
		{
			CString szWantype;
		}Ctrl;
		struct STAT
		{
			INT nPathIndex;
		}Stat;
	}DIPDef;

	typedef struct
	{
		struct STAT
		{
			INT nPathIndex;
		}Stat;
	}SIPDef;

	typedef struct
	{
		INT nPathIndex;
		struct CTRL
		{
			CString szMac;
			CString szSave;
		}Ctrl;
	}MacCloneDef;

	typedef struct WanDef
	{
		INT nPathIndex;
		CString		szRegex;

		PPPoEDef	PPPoE;
		DIPDef		DIP;
		SIPDef		SIP;
		MacCloneDef	MacClone;
	}WanDef;

	typedef struct WlanFilterDef
	{
		CString szPath;
		CString szFilterSwitch[2];//0:off 1:on
	}WlanFilterDef;

	typedef struct TrafficDef
	{
		INT nPathIndex;

		struct CTRL
		{
			INT nPathIndex;
			CString szStatOn;
			CString szStatOff;
		}Ctrl;

		struct STAT
		{
			INT nPathIndex;
			CString szRegex;
			int nItemPerRow;
			int pnItemIndexs[6];//IP,Mac,总流量：下载，上传，流率：下载，上传
		}Stat;
	}TrafficDef;

	typedef struct DHCPDef
	{
		INT nPathIndex;
		struct STAT
		{
			INT nPathIndex;
			CString szRegex;
			INT nItemsPerRow;
			CArrayEx<INT> nItemIndexs;
		}Stat;
	}DHCPDef;

	typedef struct 
	{
		CString szPath;
		CHttpWebConnection::Method requestMethod;
	}RequestPathDef;

	typedef struct
	{
		CArrayEx<RequestPathDef> RequestPathList;
		CStringEx::CStringArray szFormContainerList;//表单容器列表,也作为网页源码返回容器，一个path对应一个form
		WanDef			Wan;
		TrafficDef		Traf;
		DHCPDef			DHCP;
		AuthDef			Auth;
		WlanFilterDef	WlanFilter;
	}RouterDef;
public:
	CPropertyBin(void);
	~CPropertyBin(void);
};

