#include "StdAfx.h"
#include "Messager.h"
#include "Application.h"
#include "Common.h"
#include "Udp.h"

Messager::Messager(void)
{
}


Messager::~Messager(void)
{
}

BOOL Messager::Initialize( CString szStartUpPath, CString szMyMac,CString szMyIP)
{
	s_pContactsRemarkXml=new CXml("Root");
	s_pContactsRemarkXml->LoadFile((_bstr_t)(szStartUpPath+_T("\\Comments.xml")));
	//初始化本机信息
	s_MyInfo.szID=szMyMac;
	FormatMacToID(s_MyInfo.szID);
	s_MyInfo.szIP=szMyIP;
	Application::g_Config.setDefaultSection(_T("User"));
	Application::g_Config.read(_T("Name"),s_MyInfo.szName);
	if(s_MyInfo.szName==_T(""))//Name为空则使用本计算机名作为Name
	{
		SetMyName(Common::GetMyComputerName());
	}
	//监听
	//if(CUdp::g_ListenStat!=CUdp::Listening)
	{
		
	}
	
	//查询是否在线 _线程
	if(pTh_QueryOnline==NULL)
	{
		//监听
		s_udp.listen(s_MsgRecvr);
		pTh_QueryOnline=new CThread(AFX_THREADPROC(Thread_QueryOnline));
		pTh_QueryOnline->start();
	}
	
	return TRUE;
}

BOOL Messager::SendMsg(CString szMsg,CString szIP, MsgType msgType /*= SimpleMsg*/ )
{
	szMsg=s_MyInfo.szID+szMsg;//写入ID
	szMsg.Insert(0,(TCHAR)msgType);//写入信息类型
	return s_udp.sendTo(szIP,szMsg);
}

BOOL Messager::SendMsg( CString szMsg , MsgType msgType /*= SimpleMsg*/ )
{
	return SendMsg(szMsg,s_curContact.szIP,msgType);
}

BOOL Messager::SelectContact( UINT index )
{
	if(index>s_ContactList.GetLength()-1) return FALSE;
	s_curContact=s_ContactList[index];
	return TRUE;
}

BOOL Messager::SelectContact( CString szMac )
{
	return SelectContact(FindContact(szMac));
}

INT Messager::FindContact( CString szMac )
{
	FormatMacToID(szMac);
	szMac.MakeLower();
	CString szID;
	for(int i=0 ;i<s_ContactList.GetLength();i++)
	{
		szID=s_ContactList[i].szID;
		if(szMac==szID)
		{
			return i;
		}
	}

	return -1;
}

BOOL Messager::RemoveContact( CString szMac )
{
	INT nFound = FindContact(szMac);
	if(nFound!=-1)
	{
		return RemoveContact(nFound);
	}

	return FALSE;
}

BOOL Messager::RemoveContact( UINT index )
{
	if(s_ContactList.GetLength()>index+1)
	{
		s_ContactList.RemoveAt(index);
	}
	return FALSE;
}

UINT Messager::AddContact( ContactDef &ctct )
{
	if(FindContact(ctct.szID)==-1)//没有才添加，避免重复
	{
		if(s_pContactsRemarkXml->Root("Root"))
		{
			FormatMacToID(ctct.szID);
			if(ctct.szID==s_MyInfo.szID)//本机
			{
				ctct.szRemark=_T("我【点击修改昵称】");
				ctct.szName=s_MyInfo.szName;
			}
			else
			{
				if(s_pContactsRemarkXml->CD((_bstr_t)ctct.szID))
				{
					ctct.szRemark = s_pContactsRemarkXml->Read();
				}
				else
				{
					ctct.szRemark=_T("");
				}
				ctct.szName=_T("");
			}
		}
		return s_ContactList.Append(ctct);
	}
	
	return -1;
}

BOOL Messager::ReadMsg( UDPMsgDef& msg )
{
	if(s_MsgRecvr.szRecvLstRet.GetLength()>0)
	{
		//读取消息
		msg.szMsg=s_MsgRecvr.szRecvLstRet[0];
		//读取消息类型
		msg.msgType=(MsgType)msg.szMsg[0];//第一个字符为消息类型
		msg.szMsg.Delete(0,1);//删除类型符
		//获取发送方ID
		msg.senderInfo.szID=msg.szMsg.Mid(0,s_IDLength);
		msg.szMsg.Delete(0,s_IDLength);
		//获取发送方IP
		msg.senderInfo.szIP=s_MsgRecvr.szSenderIpLstRet[0];

		switch(msg.msgType)
		{
		case QueryUsername://申请传输本机用户的昵称					'a'
			SendMsg(s_MyInfo.szName,msg.senderInfo.szIP,QueryUsernameRecved);
			break;
		case QueryUsernameRecved://收到已申请的目标主机的用户名称	'b'
			for(int i = 0;i<s_ContactList.GetLength();i++)
			{
				if(s_ContactList[i].szID==msg.senderInfo.szID)
				{
					//更新内存中联系人列表信息
					s_ContactList[i].szName=msg.szMsg;
					msg.senderInfo.szName=msg.szMsg;
					if(s_ContactList[i].szRemark==_T(""))
					{
						SetContactRemark(s_ContactList[i].szID,msg.szMsg);//备注为空则使用用户名作为备注
					}
					break;
				}
			}
			//break;
		case SimpleMsg://								'e'
			msg.senderInfo.szName=GetContactName(msg.senderInfo.szID);
			msg.senderInfo.szRemark=GetContactRemark(msg.senderInfo.szID);
			break;
		case QueryOnline://被查询						'c'
			SendMsg(_T(""),msg.senderInfo.szIP,QueryOnlineRecved);
			break;
		case QueryOnlineRecved://查询其他主机返回的结果	'd'
			INT nFound=FindContact(msg.senderInfo.szID);
			if(nFound!=-1)
			{
				if(s_ContactList[nFound].bOnlineTimeOut==FALSE)
				{
					//每隔一段时间查询一次用户名
					SendMsg(_T(""),s_ContactList[nFound].szIP,QueryUsername);
					s_ContactList[nFound].bOnlineTimeOut=TRUE;
				}
				
				if(s_ContactList[nFound].bOnline==FALSE)
				{
					s_ContactList[nFound].bOnline=TRUE;
				}
			}
		}

		s_MsgRecvr.szRecvLstRet.RemoveAt(0);
		s_MsgRecvr.szSenderIpLstRet.RemoveAt(0);

		return TRUE;
	}

	return FALSE;
}

CString Messager::GetContactName( CString szMac )
{
	INT nFound=FindContact(szMac);
	if(nFound!=-1)
	{
		return s_ContactList[nFound].szName;
	}

	return _T("");
}

CString Messager::GetContactRemark( CString szMac )
{
	INT nFound=FindContact(szMac);
	if(nFound!=-1)
	{
		return s_ContactList[nFound].szRemark;
	}

	return _T("");
}

BOOL Messager::SetContactRemark( CString szMac,CString szRemark )
{
	//更新内存中列表
	INT nFound=FindContact(szMac);
	if(nFound!=-1)
	{
		s_ContactList[nFound].szRemark=szRemark;
	}
	//更新文件
	if(s_pContactsRemarkXml->Root("Root"))
	{
		FormatMacToID(szMac);
		if(s_pContactsRemarkXml->CD((_bstr_t)szMac,true))
		{
			if(s_pContactsRemarkXml->Write((_bstr_t)szRemark))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void Messager::FormatMacToID( CString &szMac )
{
	szMac.Remove(_T('-'));
	szMac.Remove(_T(' '));
	szMac.MakeLower();
	if(szMac[0]!='m')
	{
		szMac=_T("m")+szMac;
	}
}

void Messager::SetMyName( CString szName )
{
	//更新内存
	s_MyInfo.szName=szName;
	//更[新文件
	Application::g_Config.setDefaultSection(_T("User"));
	Application::g_Config.write(_T("Name"),szName);
}

CString Messager::GetMyName()
{
	return s_MyInfo.szName;
}

DWORD WINAPI Messager::Thread_QueryOnline( LPVOID lpParam )
{
	UINT j=0;
	while(TRUE)
	{
		for(int i =0 ;i<s_ContactList.GetLength();i++)
		{
			if(j==2)
			{
				if(s_ContactList[i].bOnlineTimeOut==FALSE)
				{
					s_ContactList[i].bOnline=FALSE;//超时未收到回应，标记为离线
				}
			}
			s_ContactList[i].bOnlineTimeOut=FALSE;//如果送出去的查询收到回应，则bOnlineTimeOut将被置为TRUE
			//查询列表中所有的联系人
			SendMsg(_T(""),s_ContactList[i].szIP,QueryOnline);
		}
		if(j==2) j=0;
		j++;
		Sleep(3000);//3s查询一次
	}
}

INT Messager::GetContact( ContactDef &contactRet )
{
	FormatMacToID(contactRet.szID);
	contactRet.szName=_T("");
	contactRet.szRemark=_T("");
	contactRet.bOnline=FALSE;
	INT nFound=FindContact(contactRet.szID);
	if(nFound!=-1)
	{
		contactRet=s_ContactList[nFound];
	}

	return nFound;
}

void Messager::Destroy( void )
{
	//在这里添加退出代码

	//
	if(s_pContactsRemarkXml!=NULL)
	{
		s_pContactsRemarkXml->save();
	}
	
}

CThread* Messager::pTh_QueryOnline=NULL;

CXml* Messager::s_pContactsRemarkXml=NULL;

Messager::ContactDef Messager::s_MyInfo;

UINT Messager::s_IDLength=13;

CArrayEx<Messager::ContactDef> Messager::s_ContactList;

Messager::ContactDef Messager::s_curContact;

CUdp::ListenParamsDef Messager::s_MsgRecvr;

CUdp Messager::s_udp(9896);

