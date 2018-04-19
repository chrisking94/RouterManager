#pragma once
#include "udp.h"
#include "ArrayEx.h"
#include "Xml.h"

class Messager 
{
public:
	typedef enum
	{
		QueryUsername='a',//�����ñ����û��ǳ�
		QueryUsernameRecved,//RequestUsername�����õ��û�����Ϣ
		QueryOnline,//��ѯ�Ƿ�����
		QueryOnlineRecved,//��ѯ�������
		SimpleMsg//ͨ����Ϣ
	}MsgType;

	typedef struct ContactDef//��ϵ��
	{
		CString szIP;
		CString szID;//Mac��ʽ������Ϊ�û�ID
		CString szName;
		CString szRemark;
		BOOL bOnline;
		BOOL bOnlineTimeOut;//�����ӳټ�⣬����ʱ��bOnlineTimeOut��ΪFALSE����bOnlieҲ��ΪFALSE
		ContactDef():bOnline(FALSE)
		{

		}
	}ContactDef;

	typedef struct UDPMsgDef
	{
		MsgType msgType;
		CString szMsg;
		ContactDef senderInfo;
	}UDPMsgDef;
public:
	Messager(void);
	~Messager(void);

	static BOOL Initialize(CString szStartUpPath, CString szMyMac,CString szMyIP);
	static UINT AddContact(ContactDef &ctct);//������дctct �� szID��Mac����������ID��ȡ���ر�ע
	static BOOL RemoveContact(CString szMac);
	static BOOL RemoveContact(UINT index);
	static INT FindContact(CString szMac);//�������б��е�����
	static BOOL SelectContact(CString szMac);
	static BOOL SelectContact(UINT index);//����ϵ�������е�����
	static BOOL SendMsg(CString szMsg, MsgType msgType = SimpleMsg);
	static BOOL SendMsg(CString szMsg,CString szIP, MsgType msgType = SimpleMsg);
	static BOOL ReadMsg(UDPMsgDef& msg);//������Ϣʱ����True
	static INT GetContact(ContactDef &contactRet);//ʹ��ʱ���Ƚ�ID����
	static CString GetContactName(CString szMac);//Mac��Ϊ�û�ID
	static CString GetContactRemark(CString szMac);//��ȡ��ע
	static BOOL SetContactRemark(CString szMac,CString szRemark);
	static void SetMyName(CString szName);
	static CString GetMyName();
	static void Destroy(void);
private:
	static void FormatMacToID(CString &szMac);//��Macת��ΪID��ʽ
	static DWORD WINAPI Thread_QueryOnline( LPVOID lpParam );
private:
	static CUdp s_udp;
	static CUdp::ListenParamsDef s_MsgRecvr;
	static ContactDef s_curContact;
	static CArrayEx<ContactDef> s_ContactList;//��ϵ���б�
	static UINT s_IDLength;
	static ContactDef s_MyInfo;//����(��ϵ��)��Ϣ
	static CThread* pTh_QueryOnline;
public:
	static CXml* s_pContactsRemarkXml;
};

