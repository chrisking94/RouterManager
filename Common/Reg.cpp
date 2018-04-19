#include "StdAfx.h"
#include "Reg.h"
#include "Common.h"

CReg::CReg(void)
{
	if(Common::IsSystemWow64())
	{
		m_samDesired64_32=KEY_WOW64_32KEY;
	}
}

CReg::CReg( HKEY hKey,LPCTSTR lpszSubKey ):
m_RootKey(hKey)
	,m_lpszSubKey(lpszSubKey)
{
	CReg();
}

CReg::CReg( PHKEY pHKey ):
m_HKEY(*pHKey)
{
	CReg();
}


CReg::~CReg(void)
{
	CRegKey::Close();
}

BOOL CReg::Read( LPCTSTR lpszValueName, DWORD dwKeyType, LPBYTE lpData,/* data buffer�������ѯ����Ļ����� */ DWORD dwDataBufferLen )
{
	if(CRegKey::QueryValue(lpszValueName,&dwKeyType,lpData,&dwDataBufferLen)==ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CReg::Read( LPCTSTR lpszValueName,CString& szReturned )
{
	ULONG ulCount=1024;
	
	if(CRegKey::QueryStringValue(lpszValueName,szReturned.GetBuffer(ulCount),&ulCount)==ERROR_SUCCESS)
	{
		szReturned.ReleaseBuffer();//�����ReleaseBuffer����õ���CString�����д��󣬷��ֵĴ���Ϊ��s1="1",s2="2"��������s1+s2="2"
		//GetBuffer()
		return TRUE;
	}
	szReturned.ReleaseBuffer();
	return FALSE;
}

CString CReg::Read( LPCTSTR lpszValueName )
{
	CString szRet;
	Read(lpszValueName,szRet);
	return szRet;
}

BOOL CReg::Write( LPCTSTR lpszValueName,DWORD dwKeyType, CONST BYTE *lpData,/* value data Ҫд������� */ DWORD dwDataBufLen )
{
		if(CRegKey::SetValue( lpszValueName,  dwKeyType, lpData, dwDataBufLen)==ERROR_SUCCESS)
		{
			return TRUE;
		}
	
	return FALSE;
}

BOOL CReg::Write( LPCTSTR lpszValueName,CONST  CString szValue)
{
	return Write(lpszValueName,REG_SZ,(const BYTE *)(LPCTSTR)szValue,szValue.GetLength()*2);
}

BOOL CReg::Write( LPCTSTR lpszValueName,CONST DWORD dwValue )
{
	return Write(lpszValueName,REG_DWORD,(const BYTE *)dwValue,4);
}

BOOL CReg::Delete( LPCTSTR lpszValueName )
{
	if(CRegKey::DeleteValue(lpszValueName)==ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
	
}

BOOL CReg::CreateKey( LPCTSTR lpSubKey )
{
	if(CRegKey::Create(m_RootKey,lpSubKey,0,0,KEY_CREATE_SUB_KEY|m_samDesired64_32)==ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CReg::DeleteKey( LPCTSTR lpSubKey )
{
	if(CRegKey::DeleteSubKey(lpSubKey)==ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CReg::Open( REGSAM samDesired )
{
	samDesired|=m_samDesired64_32;//���౻32λ����ʹ��

	if(CRegKey::Open(m_RootKey,m_lpszSubKey,samDesired)==ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		if(CRegKey::Create(m_RootKey,m_lpszSubKey)==ERROR_SUCCESS)
			return TRUE;
	}
	
	return FALSE;
}
