#include "StdAfx.h"
#include "ping.h"

//构造函数
CPing::CPing(void)
{
    hIcmp = LoadLibrary(_T("icmp.dll"));
    if(NULL != hIcmp)
    {
        pIcmpCreateFile = (PIcmpCreateFile)GetProcAddress(hIcmp, "IcmpCreateFile");
        pIcmpSendEcho = (PIcmpSendEcho)GetProcAddress(hIcmp, "IcmpSendEcho");
        pIcmpCloseHandle = (PIcmpCloseHandle)GetProcAddress(hIcmp, "IcmpCloseHandle");
    }
}
//析构函数
CPing::~CPing(void)
{
    FreeLibrary(hIcmp);
}
//功能：ping某个地址或者Ip，返回结果
//参数：v_pszDesAddress-目标域名或者IP
//参数：v_usTimes-需要ping的次数
//参数：v_ulTimeout-超时时间，单位毫秒
//返回值：-1-发生异常,其他大于等于0的值表示未超时的次数
int CPing::Ping(const char *v_pszDesAdress, USHORT v_usTimes, ULONG v_ulTimeout)
{
    ULONG ulDesIP = 0;
    int ret = 0;
 
    // 当传递的参数为域名时，解析该域名的IP
    ulDesIP = inet_addr(v_pszDesAdress);
    //域名
    if(ulDesIP == INADDR_NONE)
    {
        // 解析传入域名的IP
        PHOSTENT pHostent = gethostbyname(v_pszDesAdress);
        if (pHostent == NULL)
        {
            return -1;
        }
        ulDesIP = *(DWORD *)pHostent->h_addr;
    }
 
    // 申请发送缓冲区
    void *sendBuffer = new char[32];
    if (sendBuffer == NULL)
    {
        return -1;
    }
 
    // 申请接收缓冲区
    unsigned long replySize = sizeof(ICMP_ECHO_REPLY) + 32;
    void *replyBuffer = new char[replySize];
    if (replyBuffer == NULL)
    {
        delete [] sendBuffer;
        sendBuffer = NULL;
        return -1;
    }
    PICMP_ECHO_REPLY pIcmpEchoReply = (PICMP_ECHO_REPLY)replyBuffer;
 
    // 获取ICMP echo所需句柄
    HANDLE hIcmpFile = pIcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        delete [] sendBuffer;
        sendBuffer = NULL;
        delete [] replyBuffer;
        replyBuffer = NULL;
        return -1;
    }
 
    // 调用IcmpSendEcho，获取Ping值
    for (int i=0; i<v_usTimes; i++)
    {
        pIcmpSendEcho(hIcmpFile, ulDesIP, sendBuffer, 32, NULL, replyBuffer, replySize, v_ulTimeout);
 
        if (pIcmpEchoReply->Status == 0)    // 未超时
        {
            ret++;
        }
    }
 
    //释放缓冲区，关闭句柄
    delete[] replyBuffer;
    delete[] sendBuffer;
    pIcmpCloseHandle(hIcmpFile);
 
    return ret;
}
