#include "StdAfx.h"
#include "CSTypeManager.h"


CCSTypeManager::CCSTypeManager(void)
{
}


CCSTypeManager::~CCSTypeManager(void)
{
}

CCSVarType::TYPE CCSTypeManager::GetTypeByString( char *s )
{
	CCSVarType::TYPE t = CCSVarType::GetTypeByString(s);
	if(t == CCSVarType::_void)//�ǻ�������
	{
		return CCSVarType::_void;
	}
	else
	{
		return t;
	}
}
