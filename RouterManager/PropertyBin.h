#pragma once
#include "RecordHeapFile.h"

class CPropertyBin:private CRecordHeapFile
{
public:
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

	struct
	{
		LPCTSTR lpszPath;

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
	}m_PPPoE;

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
public:
	CPropertyBin(void);
	~CPropertyBin(void);
};

