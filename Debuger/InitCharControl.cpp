#include "StdAfx.h"
#include "InitCharControl.h"


CInitCharControl::CInitCharControl(void)
{
}


CInitCharControl::~CInitCharControl(void)
{
}

void CInitCharControl::EnSearchFunction( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag('a','z',_SearchFunction);//java的function声明符只允许小写
	}
	else
	{
		CCharControl::RecoverFlag(_SearchFunction);
		CCharControl::RecoverFlag(_SearchFunction+_Offset);
	}
}

void CInitCharControl::EnFunction( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag('a','z',_Function);
		CCharControl::SetFlag('A','Z',_Function);
		CCharControl::SetFlag('0','9',_Function);
		CCharControl::SetFlag('_',_Function);
	}
	else
	{
		CCharControl::RecoverFlag(_Function);
		CCharControl::RecoverFlag(_Function+_Offset);
	}
}

void CInitCharControl::EnFunctionEnd()
{
	CCharControl::SetFlag('(',(ControlFlag)(_Function+_Offset));
	CCharControl::SetFlag(' ',(ControlFlag)(_Function+_Offset));
}

void CInitCharControl::EnSearchFunctionEnd()
{
	CCharControl::SetFlag(' ',(ControlFlag)(_SearchFunction+_Offset));//
}

void CInitCharControl::EnParameter( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag('a','z',_Parameter);
		CCharControl::SetFlag('A','Z',_Parameter);
		CCharControl::SetFlag('0','9',_Parameter);
		CCharControl::SetFlag('_',_Parameter);
	}
	else
	{
		CCharControl::RecoverFlag(_Parameter);
		CCharControl::RecoverFlag(_Parameter+_Offset);
	}
}

void CInitCharControl::EnParameterEnd()
{
	CCharControl::SetFlag(' ',(ControlFlag)(_Parameter+_Offset));
	CCharControl::SetFlag(',',(ControlFlag)(_Parameter+_Offset));
	CCharControl::SetFlag(')',(ControlFlag)(_Parameter+_Offset*2));
}

void CInitCharControl::EnFunctionBody( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag(0,255,_FunctionBody);//函数体支持所有字符
		CCharControl::SetFlag('{',(ControlFlag)(_FunctionBody+_Offset));
		CCharControl::SetFlag('"',(ControlFlag)(_FunctionBody+_Offset*3));//遇到字符串
	}
	else
	{
		CCharControl::RecoverFlag(_FunctionBody);
		CCharControl::RecoverFlag(_FunctionBody+_Offset);
	}
}

void CInitCharControl::EnFunctionBodyEnd()
{
	CCharControl::SetFlag('}',(ControlFlag)(_FunctionBody+_Offset*2));
}

void CInitCharControl::EnString( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag(0,255,_String);//接收所有字符
		CCharControl::SetFlag('\\',(ControlFlag)(_String+_Offset*2));//右斜杠是转意符
	}
	else
	{
		CCharControl::RecoverFlag(_String);
		CCharControl::RecoverFlag(_String+_Offset);
		CCharControl::RecoverFlag(_String+_Offset*2);
	}
}

void CInitCharControl::EnStringEnd()
{
	CCharControl::SetFlag('"',(ControlFlag)(_String+_Offset));
}

