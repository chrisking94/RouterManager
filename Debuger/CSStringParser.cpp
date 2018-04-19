#include "StdAfx.h"
#include "CSStringParser.h"


CCSStringParser::CCSStringParser()
{
	
}


CCSStringParser::~CCSStringParser(void)
{
}

BOOL CCSStringParser::Parse( char *buff ,int *startIndex)
{
	if(**m_ps!='"') return FALSE;//不是字符串

	++(*m_ps);//跳过"
	while(**m_ps!='"')
	{
		if(**m_ps=='\\')
		{
			switch(*++(*m_ps))
			{
			case 'n':
				buff[(*startIndex)++] = '\n';
				break;;
			case 'r':
				buff[(*startIndex)++] = '\r';
				break;;
			default:
				buff[(*startIndex)++] = *(*m_ps);
				break;
			}
			++(*m_ps);
		}
		else
		{
			buff[(*startIndex)++] = *(*m_ps)++;
		}
	}
	++(*m_ps);//跳过"
	buff[(*startIndex)++] = '\0';

	return TRUE;
}

BOOL CCSStringParser::Parse( char *buff , int startIndex )
{
	return Parse(buff,&startIndex);
}

void CCSStringParser::SetParseObject( char *&ps )
{
	m_ps = &ps;
}

BOOL CCSStringParser::ParseBracket( char *buff , int &startIndex )
{
	if(**m_ps!='(') return FALSE;//不是字符串

	char stack[20];
	int top=-1;

	stack[++top] = *(*m_ps)++;//(	入栈
	while(top>=0&&**m_ps)
	{
		if(**m_ps=='"')
		{
			buff[startIndex++] = '"';
			Parse(buff,startIndex);
			buff[startIndex++] = '"';
		}
		else if(**m_ps == '(')
		{
			stack[++top] = *(*m_ps);//(	入栈
		}
		else if(**m_ps == ')')
		{
			stack[top--] ;//(	出栈
		}
		buff[startIndex++] = *(*m_ps)++;
	}
	buff[startIndex-1] = '\0';

	return TRUE;
}

BOOL CCSStringParser::ParseStatement( char *buff ,int * startIndex)
{
	while(**m_ps&&**m_ps!=';')
	{
		if(**m_ps=='"')
		{
			buff[(*startIndex)++] = '"';
			Parse(buff,startIndex);
			buff[(*startIndex)++] = '"';
		}
		buff[(*startIndex)++] = **m_ps;
		(*m_ps)++;
	}

	return TRUE;
}

BOOL CCSStringParser::Listen( char *buff,int *startIndex )
{
	if(**m_ps!='"') return FALSE;//不是字符串

	buff[(*startIndex)++] = *(*m_ps)++;//存入"
	while(**m_ps!='"')
	{
		if(**m_ps=='\\')
		{
			buff[(*startIndex)++] = *(*m_ps)++;
		}
		buff[(*startIndex)++] = *(*m_ps)++;
	}
	buff[(*startIndex)++] = *(*m_ps)++;//存入"
	buff[(*startIndex)++] = '\0';

	return TRUE;
}
