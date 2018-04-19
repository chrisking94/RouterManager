#pragma once
#include "IniFile.h"
#include "Reg.h"

class __declspec(dllexport) CConfig:public CIniFile,public CReg
{
public:
	typedef enum 
	{
		Ini,
		Reg
	}Type;

	CConfig(void);
	CConfig(LPCTSTR szFilePath,HKEY hkey,LPCTSTR lpszSubKey);
	~CConfig(void);
public:
	void read(LPCTSTR szKey,CString &szReturned,BOOL bDecrypt=FALSE,Type typ = Ini);
	BOOL write(LPCTSTR lpszKey,LPCTSTR lpszValue,BOOL bEncrypt=FALSE,Type typ = Ini);
	CString read(LPCTSTR lpszKey ,Type typ = Ini);
};
