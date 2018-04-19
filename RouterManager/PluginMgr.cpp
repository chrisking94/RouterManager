#include "StdAfx.h"
#include "PluginMgr.h"
#include "Application.h"
#include "BroadBand.h"

PluginMgr::PluginMgr(void)
{
}


PluginMgr::~PluginMgr(void)
{
}

void PluginMgr::Initialize()
{
	s_pPluginsXml= new CXml("Root");
	s_pPluginsXml->LoadFile(_bstr_t(Common::GetStartUpPath()+_T("Plugins\\plugins.xml")));
	ReadPluginsList();
	g_bInitialized = TRUE;
}

void PluginMgr::Destroy()
{

}

void PluginMgr::ReadPluginsList( )
{
	CString szCurPluginName;

	g_PluginList.Clear();
	Application::g_Config.setDefaultSection(_T("BroadBand"));
	Application::g_Config.read(_T("Plugin"),szCurPluginName);

	s_pPluginsXml->Root();
	while(s_pPluginsXml->EnumerateChildElement(0))
	{
		PluginDef *p_plugin=g_PluginList.AppendNew();

		s_pPluginsXml->Read(p_plugin->szName);
		if(p_plugin->szName==szCurPluginName)
		{
			g_pCurPlugin=p_plugin;
		}

		s_pPluginsXml->Read(p_plugin->szDll,"cmd");//还需添加验证
		p_plugin->connMod=(ConnMod)(s_pPluginsXml->ReadNumber("mod"));
		switch(p_plugin->connMod)
		{
		case PPPoE:
		case PPPoE_AT:
		case PPPoE_Encypted:
			p_plugin->wanMod=PPPoE;
			break;
		case _802_1:
			p_plugin->wanMod=DHCP;
			break;
		}
	}
	if(g_pCurPlugin==NULL)
	{
		if(g_PluginList.GetLength()>0)
		{
			g_pCurPlugin=&g_PluginList[0];
		}
	}
}

void PluginMgr::SetCurPlugin( CString szPluginName )
{
	for(int i=0;i<g_PluginList.GetLength();i++)
	{
		g_pCurPlugin = &g_PluginList[i];
		if(g_pCurPlugin->szName == szPluginName)
		{
			break;
		}
	}
}


void PluginMgr::Deal()
{
	BroadBand::Reset();

	if(g_pCurPlugin->connMod==PPPoE_AT)
	{
		BroadBand::g_szAccount+=g_pCurPlugin->szDll;
	}
	else if(g_pCurPlugin->connMod==PPPoE_Encypted||g_pCurPlugin->connMod==_802_1)
	{
		HINSTANCE hDll = AfxLoadLibrary(_T("plugins\\")+g_pCurPlugin->szDll+_T(".dll"));
		if(NULL == hDll)
		{
			AfxMessageBox(_T("插件加载失败！"));
			return;
		}

		//查找pluginDeal函数得到算法描述
		pluginDeal pDealFunc=(pluginDeal) GetProcAddress(hDll,"Deal");
		if(pDealFunc!=NULL)
		{
			CCodeString(BroadBand::g_szAccount).KEncrypt();
			CCodeString(BroadBand::g_szPassword).KEncrypt();
			pDealFunc(BroadBand::g_szAccount,BroadBand::g_szPassword);
		}
		else
		{
			AfxMessageBox(_T("插件加载失败！"));
		}
	}
}

BOOL PluginMgr::g_bInitialized(FALSE);

PluginMgr::PluginDef *PluginMgr::g_pCurPlugin(NULL);

PluginMgr::PluginArray PluginMgr::g_PluginList;

CXml *PluginMgr::s_pPluginsXml(NULL);
