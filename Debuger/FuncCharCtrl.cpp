#include "StdAfx.h"
#include "FuncCharCtrl.h"


CFuncCharCtrl::CFuncCharCtrl(void)
{
}


CFuncCharCtrl::~CFuncCharCtrl(void)
{
}

void CFuncCharCtrl::EnSearchStatement( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag('a','z',_SearchStatement);
		CCharControl::SetFlag('A','Z',_SearchStatement);
		CCharControl::SetFlag('_',_SearchStatement);
		CCharControl::SetFlag(':',_SearchStatement);//APP::
	}
	else
	{
		CCharControl::RecoverFlag(_SearchStatement);
		CCharControl::RecoverFlag(_SearchStatement+_Offset);
		CCharControl::RecoverFlag(_SearchStatement+_Offset*2);
	}
}

void CFuncCharCtrl::EnSearchStatementEnd()
{
	CCharControl::SetFlag(0,255,_SearchStatement);
	CCharControl::SetFlag(' ',(ControlFlag)(_SearchStatement+_Offset));//var a=1
	CCharControl::SetFlag('=',(ControlFlag)(_SearchStatement+_Offset*2));//��ֵ���ʽ
	CCharControl::SetFlag('(',(ControlFlag)(_SearchStatement+_Offset));//if while ...
}

void CFuncCharCtrl::EnString( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag(0,255,_String);//���������ַ�
		CCharControl::SetFlag('\\',(ControlFlag)(_String+_Offset*2));//��б����ת���
	}
	else
	{
		CCharControl::RecoverFlag(_String);
		CCharControl::RecoverFlag(_String+_Offset);
		CCharControl::RecoverFlag(_String+_Offset*2);
	}
}

void CFuncCharCtrl::EnStringEnd()
{
	CCharControl::SetFlag('"',(ControlFlag)(_String+_Offset));
}

void CFuncCharCtrl::EnNameSpace( BOOL bEnter/*=TRUE*/ )
{
	if(bEnter)
	{
		CCharControl::SetFlag(0,255,_NameSpace);//������֧�������ַ�
		CCharControl::SetFlag('{',(ControlFlag)(_NameSpace+_Offset));
		CCharControl::SetFlag('"',(ControlFlag)(_NameSpace+_Offset*3));//�����ַ���
	}
	else
	{
		CCharControl::RecoverFlag(_NameSpace);
		CCharControl::RecoverFlag(_NameSpace+_Offset);
	}
}

void CFuncCharCtrl::EnNameSpaceEnd()
{
	CCharControl::SetFlag('}',(ControlFlag)(_NameSpace+_Offset*2));
}

void CFuncCharCtrl::EnReceiveStatData( CJSStatement::SType t ,BOOL bEnter )
{
	
	if(bEnter)
	{
		switch(t)
		{
		case CJSStatement::_var://��������
			CCharControl::SetFlag('a','z',(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_var));
			CCharControl::SetFlag('A','Z',(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_var));
			CCharControl::SetFlag('_',(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_var));
			break;
		case CJSStatement::_if:
		case CJSStatement::_while:

		case CJSStatement::_assign://��ֵ���
			CCharControl::SetFlag(0,255,(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_assign));//��ֵ���֧�������ַ���������С�����ַ���
			CCharControl::SetFlag(';',(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_assign));
			CCharControl::SetFlag('\r',(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_assign));
			CCharControl::SetFlag('\n',(ControlFlag)(_RecieveStatData+_Offset*CJSStatement::_assign));
			CCharControl::SetFlag('"',_String);//�ַ���ͳһ����
			break;
		}
	}
	else
	{
		CCharControl::RecoverFlag(_NameSpace);
		CCharControl::RecoverFlag(_NameSpace+_Offset);
	}
}

