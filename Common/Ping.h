#pragma once
#include "stdafx.h"
#include <IPHlpApi.h>

#pragma comment(lib, "iphlpapi.lib")
 
class __declspec(dllexport) CPing
{
public:
    CPing(void);
    ~CPing(void);
private:
    HINSTANCE hIcmp;
    typedef HANDLE (WINAPI *PIcmpCreateFile)(VOID);
    typedef DWORD  (WINAPI *PIcmpSendEcho)(HANDLE, IPAddr, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
    typedef BOOL   (WINAPI *PIcmpCloseHandle)(HANDLE);
    PIcmpCreateFile        pIcmpCreateFile;
    PIcmpSendEcho        pIcmpSendEcho;
    PIcmpCloseHandle    pIcmpCloseHandle;
public:
    //功能：ping某个地址或者Ip，返回结果
    //参数：v_pszDesAddress-目标域名或者IP
    //参数：v_usTimes-需要ping的次数
    //参数：v_ulTimeout-超时时间，单位毫秒
    //返回值：-1-发生异常,其他大于等于0的值表示未超时的次数
    int Ping(const char *v_pszDesAdress, USHORT v_usTimes, ULONG v_ulTimeout);
};
