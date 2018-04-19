#include "StdAfx.h"
#include "CSFunction.h"

#define JUMP_WHITE_CHARS while(*s==' '||*s=='	'||*s=='\r'||*s=='\n') s++ //跳过空白字符
#define JUMP_SPAC_TAB while(*s==' '||*s=='	') s++ //跳过空格和制表符
#define GO_UNTIL_WHITE_CHARS while(*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n')//直到白字符
#define JUMP_TO_STATEND while(*s!=';') ++s;++s;//跳到语句结束，;后面的一个字符
#define GO_UNTIL_STATEND_OR_WHITE_CHARS while(*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n'&&*s!=';')//直到白字符或语句结束
#define GO_TO_STAT_END while(*s!=';')

CCSFunction::CCSFunction( CCSNameSpace *parentNS, CCSStatDeclare *pdlc ):
	CCSNameSpace(parentNS)
{
	m_func.SetName(pdlc->GetFirstVarNamer());
	m_func.ForceChangeType(pdlc->GetVarType());
	m_func.SetPtrLevel(pdlc->GetFirstVarPtrLevel());
	m_statType = _function;
}


CCSFunction::~CCSFunction(void)
{
}

BOOL CCSFunction::Parse( char *&s )//起始字符为(
{
	//格式化
	char buff[1024],*s_std = buff;
	int i=0,nParaCount = 0;

	//#解析形参
	++s;//跳过(
	JUMP_WHITE_CHARS;
	if(*s != ')')// ==')'则代表无形参
	{
		buff[i++] = '{';
		nParaCount = 1;//至少一个
		while(*s!=')')
		{
			if(*s==',')
			{
				buff[i++] = ';';//转换成命名空间表达式形式
				++nParaCount;
			}
			else
			{
				buff[i++] = *s;
			}

			++s;
		}
		buff[i++] = ';';
		buff[i++] = '}';
		buff[i++] = '\0';
		CCSNameSpace::SetKeepCount(nParaCount);//形参表
		CCSNameSpace::Parse(s_std);
	}
	++s;//跳过)

	//#解析命名空间
	JUMP_WHITE_CHARS;
 	return CCSNameSpace::Parse(s);
}

CCSVar *CCSFunction::Invoke( char * szParams,... )
{
	POSITION p = m_lstStat.GetHeadPosition();
	CString sz;
	CCSStatement *pStat;
	CCSVar *pVar;
	int i=0;

	//压入形参
	va_list args;
	va_start(args,szParams);
	USES_CONVERSION;
	while(*szParams)
	{
		switch(*szParams)
		{
		case 'c'://char
			Var(i++) = ((char)va_arg(args,char));
			break;
		case 's'://string
			Var(i++) = ((char*)va_arg(args,char*));
			break;
		case 'd'://int
			Var(i++) = ((int)va_arg(args,int));
			break;
		case 'f'://float
			Var(i++) = ((float)va_arg(args,float));
			break;
		case 'b'://double
			Var(i++) = ((double)va_arg(args,double));
			break;
		default:
			break;
		}

		szParams++;
	}
	va_end(args);
	
	CCSNameSpace::Execute(pVar);

	return pVar;
}

BOOL CCSFunction::IsMe( char *szName )
{
	return m_func.IsMe(szName);
}
