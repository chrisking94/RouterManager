#include "StdAfx.h"
#include "CSFunction.h"

#define JUMP_WHITE_CHARS while(*s==' '||*s=='	'||*s=='\r'||*s=='\n') s++ //�����հ��ַ�
#define JUMP_SPAC_TAB while(*s==' '||*s=='	') s++ //�����ո���Ʊ��
#define GO_UNTIL_WHITE_CHARS while(*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n')//ֱ�����ַ�
#define JUMP_TO_STATEND while(*s!=';') ++s;++s;//������������;�����һ���ַ�
#define GO_UNTIL_STATEND_OR_WHITE_CHARS while(*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n'&&*s!=';')//ֱ�����ַ���������
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

BOOL CCSFunction::Parse( char *&s )//��ʼ�ַ�Ϊ(
{
	//��ʽ��
	char buff[1024],*s_std = buff;
	int i=0,nParaCount = 0;

	//#�����β�
	++s;//����(
	JUMP_WHITE_CHARS;
	if(*s != ')')// ==')'��������β�
	{
		buff[i++] = '{';
		nParaCount = 1;//����һ��
		while(*s!=')')
		{
			if(*s==',')
			{
				buff[i++] = ';';//ת���������ռ���ʽ��ʽ
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
		CCSNameSpace::SetKeepCount(nParaCount);//�βα�
		CCSNameSpace::Parse(s_std);
	}
	++s;//����)

	//#���������ռ�
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

	//ѹ���β�
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
