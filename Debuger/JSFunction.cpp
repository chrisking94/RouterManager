#include "StdAfx.h"
#include "JSFunction.h"
#include "FuncCharCtrl.h"


CJSFunction::CJSFunction(void)
{
}


CJSFunction::~CJSFunction(void)
{
}

CJSFunction& CJSFunction::operator=( const CJSFunction& jsfSource )
{
		szFunction=jsfSource.szFunction;
		szReturn=jsfSource.szReturn;
		arrParameters=jsfSource.arrParameters;
		szBody=jsfSource.szBody;
		CJSNameSpace(jsfSource);

		return *this;
}

void CJSFunction::Invoke( char *para,... )
{
	va_list args;

	va_start(args,szFunction);
	//ѹ��ʵ��
	//�βα���NameSpace�У���0��ʼ����
	int i=0;
	while(*para)
	{
		if(*para=='%'||*para=='&')
		{
			switch(*++para)
			{
			case 's'://�ַ�����
				if(*para=='&')
					(*(CJSNameSpace::GetVar(i++))).SetPointer((CString*)va_arg(args,CString*));
				else
					*CJSNameSpace::GetVar(i++)=((CString)va_arg(args,CString));
				break;
			case 'n'://��
				if(*para=='&')
					(*(CJSNameSpace::GetVar(i++))).SetPointer((int*)va_arg(args,int*));
				else
					*CJSNameSpace::GetVar(i++)=((int)va_arg(args,int));
				break;
			}
		}
	}
	va_end(args);

	//ִ��
	CJSNameSpace::Execute();
}

CJSVar* CJSFunction::AppendParameter()
{
	return CJSNameSpace::AppendNewVar();
}

void CJSFunction::Parse()
{
	CJSNameSpace::Parse(szBody);
}
