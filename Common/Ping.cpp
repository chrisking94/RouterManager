#include "StdAfx.h"
#include "ping.h"

//���캯��
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
//��������
CPing::~CPing(void)
{
    FreeLibrary(hIcmp);
}
//���ܣ�pingĳ����ַ����Ip�����ؽ��
//������v_pszDesAddress-Ŀ����������IP
//������v_usTimes-��Ҫping�Ĵ���
//������v_ulTimeout-��ʱʱ�䣬��λ����
//����ֵ��-1-�����쳣,�������ڵ���0��ֵ��ʾδ��ʱ�Ĵ���
int CPing::Ping(const char *v_pszDesAdress, USHORT v_usTimes, ULONG v_ulTimeout)
{
    ULONG ulDesIP = 0;
    int ret = 0;
 
    // �����ݵĲ���Ϊ����ʱ��������������IP
    ulDesIP = inet_addr(v_pszDesAdress);
    //����
    if(ulDesIP == INADDR_NONE)
    {
        // ��������������IP
        PHOSTENT pHostent = gethostbyname(v_pszDesAdress);
        if (pHostent == NULL)
        {
            return -1;
        }
        ulDesIP = *(DWORD *)pHostent->h_addr;
    }
 
    // ���뷢�ͻ�����
    void *sendBuffer = new char[32];
    if (sendBuffer == NULL)
    {
        return -1;
    }
 
    // ������ջ�����
    unsigned long replySize = sizeof(ICMP_ECHO_REPLY) + 32;
    void *replyBuffer = new char[replySize];
    if (replyBuffer == NULL)
    {
        delete [] sendBuffer;
        sendBuffer = NULL;
        return -1;
    }
    PICMP_ECHO_REPLY pIcmpEchoReply = (PICMP_ECHO_REPLY)replyBuffer;
 
    // ��ȡICMP echo������
    HANDLE hIcmpFile = pIcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        delete [] sendBuffer;
        sendBuffer = NULL;
        delete [] replyBuffer;
        replyBuffer = NULL;
        return -1;
    }
 
    // ����IcmpSendEcho����ȡPingֵ
    for (int i=0; i<v_usTimes; i++)
    {
        pIcmpSendEcho(hIcmpFile, ulDesIP, sendBuffer, 32, NULL, replyBuffer, replySize, v_ulTimeout);
 
        if (pIcmpEchoReply->Status == 0)    // δ��ʱ
        {
            ret++;
        }
    }
 
    //�ͷŻ��������رվ��
    delete[] replyBuffer;
    delete[] sendBuffer;
    pIcmpCloseHandle(hIcmpFile);
 
    return ret;
}
