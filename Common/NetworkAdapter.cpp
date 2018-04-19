#include "StdAfx.h"
#include "NetworkAdapter.h"
#include "shlobj.h"

#pragma comment(lib, "Iphlpapi.lib")//解决error LNK2019: 无法解析的外部符号 _GetAdaptersInfo@8，该符号在函数 ...（作用相当于引入了.cpp）


CNetworkAdapter::CNetworkAdapter(void)
{
}


CNetworkAdapter::~CNetworkAdapter(void)
{
}


BOOL CNetworkAdapter::GetAdaptersInfosArray( AdapterInfoArray& adpsInfoRet )
{
	AdapterInfoDef adpInfo;
	///*
	CString szTmp=_T("");
	u_char pMac[6];
	PIP_ADAPTER_INFO pAdp = NULL;
	ULONG uLong=0;
	//为适配器申请内存

	::GetAdaptersInfo(pAdp,&uLong);
	pAdp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//取得本地适配器结构信息
	if(::GetAdaptersInfo(pAdp,&uLong) == ERROR_SUCCESS)
	{
		_IP_ADDR_STRING * pGateWay=NULL;
		while(pAdp != NULL)
		{
			memcpy(pMac,pAdp->Address,6);
			adpInfo.szMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),pMac[0],pMac[1],pMac[2],pMac[3],pMac[4],pMac[5]);
			pGateWay=&pAdp->GatewayList;// 网关列表，最多4个
			
			adpInfo.nGateWayCount=0;
			while(pGateWay!=NULL&&adpInfo.nGateWayCount<4)
			{
				adpInfo.szGateWays[adpInfo.nGateWayCount].Format(_T("%S"),pGateWay->IpAddress.String);
				adpInfo.nGateWayCount++;
				pGateWay=pGateWay->Next;
			}

			adpInfo.szIp.Format(_T("%S"),pAdp->IpAddressList.IpAddress.String);//IP
			adpInfo.szIpMask.Format(_T("%S"),pAdp->IpAddressList.IpMask.String);//子网掩码 
			adpInfo.szDHCPServer.Format(_T("%S"),pAdp->DhcpServer.IpAddress.String);//DHCP服务器
			adpInfo.szName.Format(_T("%S"),pAdp->AdapterName);

			//DNS
			IP_PER_ADAPTER_INFO* pPerAdapt	= NULL;
			ULONG ulLen = 0;
			int err = GetPerAdapterInfo( pAdp->Index, pPerAdapt, &ulLen);
			if( err == ERROR_BUFFER_OVERFLOW )
			{
				pPerAdapt = ( IP_PER_ADAPTER_INFO* ) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulLen);
				err = GetPerAdapterInfo( pAdp->Index, pPerAdapt, &ulLen );
				if( err == ERROR_SUCCESS )
				{
					IP_ADDR_STRING* pNext = &( pPerAdapt->DnsServerList );
					INT curIndex=0;

					//获取Dns列表
					while(pNext)
					{
						adpInfo.szDnsServer[curIndex].Format(_T("%s"),CStringW(pNext->IpAddress.String));
						curIndex++;
						pNext = pNext->Next;
					}
				}
				HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pPerAdapt);
			}
			adpsInfoRet.Append(adpInfo);

			pAdp=pAdp->Next;
		}
		
		//strErrorMsg = _T("获取网络配置信息成功");
		return TRUE;
	}
	return FALSE;

	//*/
}

