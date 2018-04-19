#pragma once
#include "Common.h"

//插件的管理、更新、调用 模块

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


	typedef void (*pluginDeal)(CString &szAccount,CString &szPassword);//插件的Deal函数 类型
public:
	static void Initialize();
	static void Destroy();

	static void ReadPluginsList();
	static void SetCurPlugin(CString szPluginName);

	static void Deal();//调用插件的Deal函数

	static PluginArray g_PluginList;
	static PluginDef *g_pCurPlugin;
	static BOOL g_bInitialized;
};

