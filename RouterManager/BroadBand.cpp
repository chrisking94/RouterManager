
#include "StdAfx.h"
#include "BroadBand.h"
#include "PluginMgr.h"
#include "Router.h"

BroadBand::BroadBand(void)
{
}


BroadBand::~BroadBand(void)
{
}


BOOL BroadBand::Connect()
{
	switch(PluginMgr::g_pCurPlugin->connMod)
	{
		case PluginMgr::PPPoE_Encypted:
		case PluginMgr::PPPoE_AT:
			PluginMgr::Deal();
			return Router::PPPoE_Dial();
			break;
	}

	return FALSE;
}

BOOL BroadBand::Disconnect()
{
	switch(PluginMgr::g_pCurPlugin->connMod)
	{
	case PluginMgr::PPPoE_Encypted:
	case PluginMgr::PPPoE_Default:
	case PluginMgr::PPPoE_AT:
		return Router::PPPoE_Disc();
		break;
	}

	return FALSE;
}

void BroadBand::ConfigRouter()
{

}

CString BroadBand::g_szPassword;

CString BroadBand::g_szAccount;

CString BroadBand::s_szPassword;

CString BroadBand::s_szAccount;
