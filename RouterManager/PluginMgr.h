#pragma once
#include "Common.h"

//����Ĺ������¡����� ģ��

class PluginMgr
{
private:
	static CXml *s_pPluginsXml;
public:
	enum ConnMod
	{
		PPPoE_Default,
		PPPoE_Encypted,
		PPPoE_AT,//@
		_802_1,
	};
	enum WanMod
	{
		PPPoE,
		DHCP,
		STATIC_IP
	};
	typedef struct PluginDef
	{
		WanMod wanMod;
		ConnMod connMod;
		CString szDll;
		CString szFunction;
		BOOL bCloneMac;
		CString szName;
	}PluginDef;

	typedef CArrayEx<PluginDef> PluginArray;

	PluginMgr(void);
	~PluginMgr(void);


	typedef void (*pluginDeal)(CString &szAccount,CString &szPassword);//�����Deal���� ����
public:
	static void Initialize();
	static void Destroy();

	static void ReadPluginsList();
	static void SetCurPlugin(CString szPluginName);

	static void Deal();//���ò����Deal����

	static PluginArray g_PluginList;
	static PluginDef *g_pCurPlugin;
	static BOOL g_bInitialized;
};

