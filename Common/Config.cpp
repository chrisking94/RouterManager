#include "StdAfx.h"
#include "Config.h"
#include "CodeString.h"


CConfig::CConfig(void):CIniFile()
{
	
}

CConfig::CConfig( LPCTSTR szFilePath,HKEY hkey,LPCTSTR lpszSubKey )://这里如果用CCString而不用(LPCTSTR)会导致基类初始化不正确，导致其数据成员值为";"
CIniFile(szFilePath)
	,CReg(hkey,lpszSubKey)
{
	CReg::Open();
}


CConfig::~CConfig(void)
{
}

void CConfig::read( LPCTSTR szKey,CString &szReturned,BOOL bDecrypt/*=FALSE*/ ,Type typ)
{
	CString tmp;
	switch(typ)
	{
	case Ini:
		CIniFile::read(szKey,tmp);
		break;
	case Reg:
		if(!CReg::Read(szKey,tmp))//读取Reg失败则读取Ini文件
		{
			CIniFile::read(szKey,tmp);
		}
		break;
	}
	
	if(bDecrypt)
	{    
		tmp=CCodeString((LPCTSTR)tmp).KDecrypt();
	}
	szReturned=tmp;
}

CString CConfig::read( LPCTSTR lpszKey ,Type typ)
{
	switch(typ)
	{
	case Ini:
		return CIniFile::read(lpszKey);
		break;
	case Reg:
		return CReg::Read(lpszKey);
		break;
	}

	return _T("");
}

BOOL CConfig::write( LPCTSTR lpszKey,LPCTSTR lpszValue,BOOL bEncrypt/*=FALSE*/ ,Type typ)
{
	CString writeVal=CString(lpszValue);
	if(bEncrypt)
	{
		writeVal=CCodeString(lpszValue).KEncrypt();//不能直接用lpszValue=...；否则会出错
	}

	switch(typ)
	{
	case Ini:
		return CIniFile::write(lpszKey,writeVal);
		break;
	case Reg:
		if(!CReg::Write(lpszKey,writeVal))
		{
			return CIniFile::write(lpszKey,writeVal);
		}
		break;
	}

	return FALSE;
}
