#pragma once

#include "stdafx.h"
#include "ArrayEx.h"
#include "StringEx.h"

#pragma comment(lib,"Netapi32.lib")

class __declspec(dllexport) CNetworkAdapter
{
public:
	enum AdapterInfoType
	{
		Mac,
		GateWay,//网关
		IP,
		SubnetMask//子网掩码
	};

	typedef struct AdapterInfoDef
	{
		CString szName;
		CString szGateWays[4];
		INT nGateWayCount;
		CString szIp;
		CString szMac;
		CString szIpMask;
		BOOL bDHCPEnable;
		CString szDHCPServer;
		CString szDnsServer[3];//最多3个
	}AdapterInfoDef;

	typedef CArrayEx<AdapterInfoDef> AdapterInfoArray;
public:
	CNetworkAdapter(void);
	~CNetworkAdapter(void);

	static BOOL GetAdaptersInfosArray(AdapterInfoArray& adpsInfoRet);
};

