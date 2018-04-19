#include "StdAfx.h"
#include "HtmlLabel.h"

CHtmlLabel::CHtmlLabel(void):
m_firstChild(NULL)
	,m_lpszInnerHtml(NULL)
	,m_lpszType(NULL)
	,m_nextSibling(NULL)
	,m_pFirstAttr(NULL)   
{
}


CHtmlLabel::~CHtmlLabel(void)
{
}

BOOL CHtmlLabel::Parse(TCHAR *&s)
{
	Attribute **ppatt;
	CHtmlLabel **pplbl;
	s++;//jump <
	if(*s=='!')	//comment
	{
		ReadComment(s);
		return TRUE;
	}
	ReadTo(s,'>',m_lpszType);

	//read attribute
	JUMP_WHITE_CHARS;
	
	ppatt = &m_pFirstAttr;
	while(*s!='>')
	{
		(*ppatt) = new Attribute;
		ReadTo(s,'=',(*ppatt)->lpszName);
		JUMP_WHITE_CHARS;
		if(*s=='"')
		{
			ReadString(s,(*ppatt)->lpszVal);
		}
		else
		{
			ReadTo(s,'>',(*ppatt)->lpszVal);
		}
		JUMP_WHITE_CHARS;

		ppatt = &((*ppatt)->pNext);
	}
	(*ppatt) = NULL;

	//meta dont have chidren and dont have </meta
	if(_T("meta")==CString(m_lpszType))
	{
		s++;//jump >
		JUMP_WHITE_CHARS;
		return TRUE;
	}

	//---<a href="" />
	if(*(s-1) != '/')//not end
	{
		
		s++;//jump >

		JUMP_WHITE_CHARS;
		if(*s!='<')//innerHtml
		{
			ReadInnerHtml(s,m_lpszInnerHtml);
		}
		
		pplbl = &m_firstChild;
		while(*(s+1)!='/')//read children
		{
			//---<a href="" >abc<img ...
			(*pplbl) = new CHtmlLabel;
			(*pplbl)->Parse(s);
			pplbl = &(*pplbl)->m_nextSibling;
		}//else ---<a href="">abc</
		(*pplbl) = NULL;
	}
	
	while(*s++!='>');
	JUMP_WHITE_CHARS;//to next label

	return TRUE;
}

void CHtmlLabel::ReadTo(TCHAR *&s,TCHAR end,LPCTSTR &szRet)
{
	static TCHAR buffer[1024];
	int i=0;
	while(*s!=' '&&*s!='	'&&*s!=end&&*s!=0) buffer[i++] = *s++;
	buffer[i] = 0;
	CopyString(buffer,szRet);
}

void CHtmlLabel::ReadString(TCHAR *&s,LPCTSTR &ret)
{
	static TCHAR buffer[1024];
	int i=0;
	s++;//jump "
	while(*s!='"')
	{
		if(*s=='\\') s++;//escape TCHAR
		buffer[i++]=*s++;
	}
	buffer[i] = 0;
	CopyString(buffer,ret);
}

void CHtmlLabel::ReadString(TCHAR *&s,TCHAR *&buffer)
{
	*buffer++ = *s++;// "
	while(*s!='"')
	{
		if(*s=='\\') s++;
		*buffer++ = *s++;
	}
	*buffer++ = *s++;// "
}

void CHtmlLabel::CopyString(TCHAR *buffer,LPCTSTR &dst)
{
	TCHAR *p = buffer;
	while(*p++);
	int len = p-buffer;//contains the '\0'
	dst = (LPCTSTR)new TCHAR[len];
	memcpy((byte*)dst,buffer,len*sizeof(TCHAR));
}

int CHtmlLabel::LenStrT(TCHAR *buffer)
{
	TCHAR *p = buffer;
	while(*p++);
	return (p-buffer);
}

void CHtmlLabel::ReadInnerHtml(TCHAR *&s,LPCTSTR &szRet)
{
	TCHAR buffer[65535];
	TCHAR *p=buffer;
	int i=0;
	while(*s!='<')
	{
		if(*s == '"')
		{
			ReadString(s,p);
		}
		else
			*p++ = *s++;
	}

	*p = 0;
	CopyString(buffer,szRet);
}

void CHtmlLabel::ReadComment(TCHAR *&s)
{
	while(!(*s=='-'&&*(s+1)=='-'&&*(s+2)=='>')) s++;
	s+=3;//jump -->
}
