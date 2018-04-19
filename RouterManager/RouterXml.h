#pragma once
#include "xml.h"
#include "StringEx.h"
#include "HttpWebConnection.h"

class CRouterXml :
	private CXml
{
public:
	typedef enum
	{
		Null,//读取错误
		Dlg,//对话框中输入
		Page//网页中输入
	}AuthMethod;

	enum RouterLinkStatus
	{
		Initilizing,//初始化中
		TimeOut,//未连接
		NoResponse, //能ping通但无响应
		OK=HTTP_STATUS_OK,
		Denied=HTTP_STATUS_DENIED
	};

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

	typedef struct AuthDef//如果外部使用该类结构体时使用的末尾}xxx;的结构体别名声明，则调用的构造函数也是别名，如果struct 后面的名称和别名不一样，则会导致不调用构造函数
	{
		INT nPathIndex;
		CString szCookie;
		AuthMethod amAuth;
		CString szPassword;//表单中的元素名称，如psw   =
		CString szUsername;
		CString szDefalutUsername;
		CString szDefalutPassword;
		CHttpWebConnection::Method mRequestMethod;
		CString szStatRegex;//状态码正则,Post方式为验证成功的重定向页面， 用此方式来验证是否登陆成功
		INT nStatRegSubItem;//正则表达结果的中状态码所在子元素位置，默认为2
		CString szAccessOk;
		AuthDef(void):amAuth(Null),mRequestMethod(CHttpWebConnection::GET),nStatRegSubItem(2){
		}
	}AuthDef;

	typedef struct PPPoEDef
	{
		INT nPathIndex;

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
	}PPPoEDef;

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

	RouterDef m_Router;
public:
	CRouterXml(void);
	CRouterXml(_bstr_t bszFilePath);
	~CRouterXml(void);
	bool IsRouterXml();
	bool ReadAll();
	int ReadSupportedRouters(CStringEx::CStringArray &szaReturned);//获取该配置的支持的路由器
	void ResetContainer(void);//将表单容器内容清除

	void ConvertRouterLinkStatStrToCode(CString szCode,RouterLinkStatus &codeRet);
	void ConvertRouterLinkStatCodeToStr(RouterLinkStatus code,CString &szCodeRet);
	void ConvertWanLinkStatStrCodeToCode(CString szCode,WanLinkStat &codeRet);//将状态码字符 0,1,...转换为linkStat code
	void ConvertWanLinkStatCodeToStr(WanLinkStat code,CString &szCodeRet);//将状态码转换为 "未连接","已连接",...
private:
	bool m_bXmlLoaded;
	_bstr_t * m_pStructNode;
	_bstr_t   m_bszStructPath;
	
	bool ReadComm(CString &szRet);
	bool ReadComm(long &lRet);
	bool ReadComm(AuthMethod &amRet);
	bool ReadComm(CHttpWebConnection::Method &mRet);
	bool ReadComm(int &iRet);
	//void InheritAttribute(CString szParentAttribute,CString &szChildAttribute);

	static _bstr_t RXmlStruct[];
};

