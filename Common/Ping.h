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
    //���ܣ�pingĳ����ַ����Ip�����ؽ��
    //������v_pszDesAddress-Ŀ����������IP
    //������v_usTimes-��Ҫping�Ĵ���
    //������v_ulTimeout-��ʱʱ�䣬��λ����
    //����ֵ��-1-�����쳣,�������ڵ���0��ֵ��ʾδ��ʱ�Ĵ���
    int Ping(const char *v_pszDesAdress, USHORT v_usTimes, ULONG v_ulTimeout);
};
