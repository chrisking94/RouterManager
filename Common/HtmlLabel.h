#pragma once
#include "stdafx.h"

#define JUMP_WHITE_CHARS while(*s==' '||*s=='	'||*s=='\r'||*s=='\n') s++

class CHtmlLabel
{
public:
	CHtmlLabel(void);
	~CHtmlLabel(void);

	BOOL Parse(TCHAR *&s);
private:
	typedef struct Attribute
	{
		LPCTSTR lpszName;
		LPCTSTR lpszVal;
		Attribute *pNext;
	}Attribute;

	static void ReadTo(TCHAR *&s,TCHAR end,LPCTSTR &szRet);//read to from current position to TCHAR and jump it,and jump white TCHARs
	static void ReadString(TCHAR *&s,LPCTSTR &ret);
	static void CopyString(TCHAR *buffer,LPCTSTR &dst);//allocate memory and assign to dst
	static int LenStrT(TCHAR *buffer);
	static void ReadInnerHtml(TCHAR *&s,LPCTSTR &szRet);
	static void ReadString(TCHAR *&s,TCHAR *&buffer);
	static void ReadComment(TCHAR *&s);//comment
	
public:
	LPCTSTR m_lpszType;
	Attribute *m_pFirstAttr;
	LPCTSTR m_lpszInnerHtml;
	CHtmlLabel *m_firstChild;
	CHtmlLabel *m_nextSibling;
};

