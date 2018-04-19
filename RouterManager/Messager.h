#pragma once
#include "udp.h"
#include "ArrayEx.h"
#include "Xml.h"

class Messager 
{
public:
	typedef enum
	{
		QueryUsername='a',//请求获得本机用户昵称
		QueryUsernameRecved,//RequestUsername申请获得的用户名信息
		QueryOnline,//查询是否在线
		QueryOnlineRecved,//查询结果返回
		SimpleMsg//通用消息
	}MsgType;

	typedef struct ContactDef//联系人
	{
		CString szIP;
		CString szID;//Mac格式化后作为用户ID
		CString szName;
		CString szRemark;
		BOOL bOnline;
		BOOL bOnlineTimeOut;//用于延迟检测，当超时后bOnlineTimeOut仍为FALSE，则将bOnlie也置为FALSE
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
	static UINT AddContact(ContactDef &ctct);//必须填写ctct 的 szID（Mac），才能以ID读取本地备注
	static BOOL RemoveContact(CString szMac);
	static BOOL RemoveContact(UINT index);
	static INT FindContact(CString szMac);//返回在列表中的索引
	static BOOL SelectContact(CString szMac);
	static BOOL SelectContact(UINT index);//在联系人数组中的索引
	static BOOL SendMsg(CString szMsg, MsgType msgType = SimpleMsg);
	static BOOL SendMsg(CString szMsg,CString szIP, MsgType msgType = SimpleMsg);
	static BOOL ReadMsg(UDPMsgDef& msg);//有新消息时返回True
	static INT GetContact(ContactDef &contactRet);//使用时须先将ID填入
	static CString GetContactName(CString szMac);//Mac作为用户ID
	static CString GetContactRemark(CString szMac);//获取备注
	static BOOL SetContactRemark(CString szMac,CString szRemark);
	static void SetMyName(CString szName);
	static CString GetMyName();
	static void Destroy(void);
private:
	static void FormatMacToID(CString &szMac);//将Mac转换为ID形式
	static DWORD WINAPI Thread_QueryOnline( LPVOID lpParam );
private:
	static CUdp s_udp;
	static CUdp::ListenParamsDef s_MsgRecvr;
	static ContactDef s_curContact;
	static CArrayEx<ContactDef> s_ContactList;//联系人列表
	static UINT s_IDLength;
	static ContactDef s_MyInfo;//本机(联系人)信息
	static CThread* pTh_QueryOnline;
public:
	static CXml* s_pContactsRemarkXml;
};

