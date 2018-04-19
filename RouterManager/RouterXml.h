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
		Null,//��ȡ����
		Dlg,//�Ի���������
		Page//��ҳ������
	}AuthMethod;

	enum RouterLinkStatus
	{
		Initilizing,//��ʼ����
		TimeOut,//δ����
		NoResponse, //��pingͨ������Ӧ
		OK=HTTP_STATUS_OK,
		Denied=HTTP_STATUS_DENIED
	};

	typedef enum
	{
		NotConnected,//δ����
		Connected,//������
		Connecting,//��������...
		AccessDenied,//�û�����������֤ʧ��
		ServerNoResponse,//����������Ӧ
		UnknownFailed,//δ֪ԭ��ʧ��
		WanNotConnected,//WAN��δ���ӣ�
		WaitingForRouter,//�ȴ�����·����
	}WanLinkStat;

	typedef struct AuthDef//����ⲿʹ�ø���ṹ��ʱʹ�õ�ĩβ}xxx;�Ľṹ���������������õĹ��캯��Ҳ�Ǳ��������struct ��������ƺͱ�����һ������ᵼ�²����ù��캯��
	{
		INT nPathIndex;
		CString szCookie;
		AuthMethod amAuth;
		CString szPassword;//���е�Ԫ�����ƣ���psw   =
		CString szUsername;
		CString szDefalutUsername;
		CString szDefalutPassword;
		CHttpWebConnection::Method mRequestMethod;
		CString szStatRegex;//״̬������,Post��ʽΪ��֤�ɹ����ض���ҳ�棬 �ô˷�ʽ����֤�Ƿ��½�ɹ�
		INT nStatRegSubItem;//������������״̬��������Ԫ��λ�ã�Ĭ��Ϊ2
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
			INT nStatRegSubItem;//Ϊ2
			CArrayEx<INT> nStatCodeMapList;//״̬���б���Ӧ״̬�ı��б�
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
			int pnItemIndexs[6];//IP,Mac,�����������أ��ϴ������ʣ����أ��ϴ�
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
		CStringEx::CStringArray szFormContainerList;//�������б�,Ҳ��Ϊ��ҳԴ�뷵��������һ��path��Ӧһ��form
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
	int ReadSupportedRouters(CStringEx::CStringArray &szaReturned);//��ȡ�����õ�֧�ֵ�·����
	void ResetContainer(void);//���������������

	void ConvertRouterLinkStatStrToCode(CString szCode,RouterLinkStatus &codeRet);
	void ConvertRouterLinkStatCodeToStr(RouterLinkStatus code,CString &szCodeRet);
	void ConvertWanLinkStatStrCodeToCode(CString szCode,WanLinkStat &codeRet);//��״̬���ַ� 0,1,...ת��ΪlinkStat code
	void ConvertWanLinkStatCodeToStr(WanLinkStat code,CString &szCodeRet);//��״̬��ת��Ϊ "δ����","������",...
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

