#include "StdAfx.h"
#include "IniFile.h"


CIniFile::CIniFile(LPCTSTR filePath):
m_szFilePath(filePath),
m_lpszDefaultSection(_T("Config"))
{
	
}

CIniFile::CIniFile( void )
{

}

CIniFile::~CIniFile(void)
{
}

BOOL CIniFile::write(LPCTSTR lpszSection,LPCTSTR lpszKey,LPCTSTR lpszValue)
{
	return ::WritePrivateProfileString(lpszSection,lpszKey,lpszValue,m_szFilePath);
}

BOOL CIniFile::write( LPCTSTR lpszKey,LPCTSTR lpszValue )
{
	return write(m_lpszDefaultSection,lpszKey,lpszValue);
}

CString CIniFile::read(LPCTSTR lpszSection,LPCTSTR lpszKey,LPCTSTR lpszDefault)
{
	CString Buffer;
	read(lpszSection,lpszKey,Buffer,lpszDefault);
	return Buffer;
}

DWORD CIniFile::read( LPCTSTR lpszSection,LPCTSTR lpszKey,CString& csReturned,LPCTSTR lpszDefault/*=_T("")*/ )
{
	DWORD dwCopied; 
	dwCopied=::GetPrivateProfileString(lpszSection,lpszKey,lpszDefault,csReturned.GetBuffer(MAX_PATH),MAX_PATH,m_szFilePath);
	csReturned.ReleaseBuffer();
	return dwCopied;
}

CString CIniFile::read( LPCTSTR lpszKey)
{
	return read(m_lpszDefaultSection,lpszKey);
}

DWORD CIniFile::read( LPCTSTR lpszKey,CString &szReturned )
{
	return read(m_lpszDefaultSection,lpszKey,szReturned);
}

void CIniFile::setDefaultSection( LPCTSTR lpszDefaultSection )
{
	m_lpszDefaultSection=lpszDefaultSection;
}

BOOL CIniFile::writeBool( LPCTSTR lpszKey,BOOL bValue )
{
	if(bValue)
	{
		return write(lpszKey,_T("1"));
	}
	else
	{
		return write(lpszKey,_T("0"));
	}
}

void CIniFile::readBool( LPCTSTR lpszKey,BOOL &bReturned )
{
	if(read(lpszKey)=="1")
	{
		bReturned=TRUE;
	}
	else
	{
		bReturned=FALSE;
	}
}
