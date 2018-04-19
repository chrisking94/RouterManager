#include "StdAfx.h"
#include "CSStatement.h"
#include "CharControl.h"
#include "CSFunction.h"
#include "CScript.h"
#include "CSStringParser.h"
#include "CSException.h"
#include "CScript.h"

class CCSFunction;

#define JUMP_SPAC_TAB while(*s==' '||*s=='	') s++ //�����ո���Ʊ��
#define GO_UNTIL_WHITE_CHARS while(*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n')//ֱ�����ַ�
#define JUMP_TO_STATEND while(*s!=';') ++s;++s;//������������;�����һ���ַ�
#define GO_UNTIL_STATEND_OR_WHITE_CHARS while(*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n'&&*s!=';')//ֱ�����ַ���������
#define GO_TO_STAT_END while(*s!=';')
#define NOT_WHITE_CHARS (*s!=' '&&*s!='	'&&*s!='\r'&&*s!='\n')
CCSStatement::CCSStatement(void):m_parentNS(NULL)
{
	
}

CCSStatement::CCSStatement( CCSNameSpace *parentNameSpace ):m_parentNS(parentNameSpace)
{

}


CCSStatement::~CCSStatement(void)
{
}

BOOL CCSStatement::Parse( char *&s,CCSStatement *&p_statRet ,CCSNameSpace *pNS,char endWithChar)
{
	int k;
	CString sz,sz1;

	p_statRet =NULL;

	CStrParallelCmp spc(s);
#define STAT_TYPE_COUNT 12
	char *spcLst[STAT_TYPE_COUNT]=
	{
		"typedef",//0
		"if",//1
		"while",//2
		"do",//3
		"for",//4
		"typedef",//5
		"struct",//6
		"enum",//7
		"union",//8
		"return",//9
		"break",//10
		"continue",//11
	};
	spc.SetList(spcLst,STAT_TYPE_COUNT);
	spc.SetEndWith(" (={+-	;*\r\n");

	char *sBckup;
	USES_CONVERSION;

	if(*s == '/')//ע��
	{
		if(*++s == '/')
		{
			while(*s++!='\n');//������һ��
		}
		else if(*s++ == '*')
		{
			while(!(*s++=='*' && *s++!='/'));//������һ��ע�ͷ��ĺ���
		}
	}
	else
	{
		k=spc.Compare(sBckup);
		switch(k)
		{
		case -1://ƥ��ʧ��
			JUMP_WHITE_CHARS;//�������ַ�

			if(*s=='(')//��������
			{
				p_statRet = new CCSStatInvoke(pNS);
				((CCSStatInvoke*)(p_statRet))->Parse(s);
			}
			else
			{
				if(pNS->GetTypeByString(spc.GetComparedString()) == CCSVarType::_nullType)
				{
					//��ͨ���
					p_statRet = new CCSStatExp(pNS);
					s = sBckup;
					((CCSStatExp*)p_statRet)->Parse(s);
				}
				else
				{
					//����
					CCSStatDeclare *pdlc = new CCSStatDeclare(pNS);
					s=sBckup;//����
					pdlc->Parse(s);
					if(pdlc->GetType() == _function)
					{
						p_statRet = new CCSFunction(pNS,pdlc);
						delete pdlc;
						((CCSFunction*)p_statRet)->Parse(s);
					}
					else
					{
						p_statRet = pdlc;
					}
				}
			}
			break;
		case 0://typedef
			break;
		case 1://if
			p_statRet = new CCSStatIf(pNS);
			((CCSStatIf*)p_statRet)->Parse(s);
			break;
		case 2://while
			p_statRet = new CCSStatWhile(pNS);
			((CCSStatWhile*)p_statRet)->Parse(s);
			break;
		case 3://do
			p_statRet = new CCSStatDo(pNS);
			((CCSStatDo*)p_statRet)->Parse(s);
			break;
		case 4://for
			p_statRet = new CCSStatFor(pNS);
			((CCSStatFor*)p_statRet)->Parse(s);
			break;
		case 5://"typedef",//9
		case 6://"struct",//10
		case 7://"enum",//11
		case 8://"union",//12	
			break;
		case 9:
			JUMP_WHITE_CHARS;
			p_statRet = new CCSStatReturn(pNS);
			((CCSStatReturn*)p_statRet)->Parse(s);
			break;
		case 10:
			p_statRet = new CCSStatBreak(pNS);
			((CCSStatBreak*)p_statRet)->Parse(s);
			break;
		case 11:
			p_statRet = new CCSStatBreak(pNS);
			((CCSStatBreak*)p_statRet)->Parse(s);
			break;
		}
	}

	if(p_statRet)
	{
		return TRUE;
	}
	else return FALSE;
}

CCSStatement::SType CCSStatement::GetType()
{
	return m_statType;
}

SRCode CCSStatement::Execute(CCSVar *&pVarRet)
{
	CCSException e(CCSException::_ExcuteError);
	e.AppendMsg("CScript.Statement��void Execute() ���ô��󣬵����˴�������");
	e.Throw(); 

	return _SR_FALSE;
}

CCSStatDeclare::CCSStatDeclare(CCSNameSpace *parentNameSpace):CCSStatement(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_declare;
}

CCSStatDeclare::CCSStatDeclare( const CCSStatDeclare &d )
{
	CCSException e(CCSException::_InternalError);
	e.AppendMsg("CScript.Declare.���ƹ��캯�����ô���");
	e.Throw();
}

BOOL CCSStatDeclare::Parse( char *&s )
{
	char buff[1024];
	int i=0;
	DeclNode **ppNode = &m_lstDelc;
	//��ʽ���ַ���
	//buff[i++]='#';

	CCSStringParser::SetParseObject(s);

	while(NOT_WHITE_CHARS)
	{
		buff[i++]=*s++;
	}
	buff[i++]='\0';
	m_vt = CCSVarType::GetTypeByString(buff);

	CCharControl cc;
	cc.SetFlag('A','Z',1);
	cc.SetFlag('a','z',1);
	cc.SetFlag('0','9',1);
	cc.SetFlag('_',1);
	//��ʼ����������
	while(*s != ';')
	{
		JUMP_WHITE_CHARS;
		(*ppNode) = new DeclNode;
		(*ppNode)->nArrSize = 0;
		(*ppNode)->nPtrLevel = 0;
		(*ppNode)->pAssiExpr = NULL;
		(*ppNode)->pNext = NULL;
		(*ppNode)->szVName = NULL;
		while(*s == '*')//��ȡָ�����
		{
			++(*ppNode)->nPtrLevel;
			++s;
			JUMP_WHITE_CHARS;
		}
		//��ȡ������
		i=0;
		while(cc.Table[*s]==1)
		{
			buff[i++] = *s++;
		}
		buff[i] = 0;
		(*ppNode)->szVName = new char[i];
		strcpy((*ppNode)->szVName,buff);

		JUMP_WHITE_CHARS;
		if(*s == '(')//����
		{
			m_statType = CCSStatement::_function;
			return TRUE;
		}
		//��ȡ������
		cc.SetFlag('0','9',2);
		char nbuff[20];
		if(*s == '[')
		{
			++s;//����[
			JUMP_WHITE_CHARS;
			int k = 0;
			while(cc.Table[*s])
			{
				nbuff[k++] = *s++;
			}
			nbuff[k] = 0;
			(*ppNode)->nArrSize = atoi(nbuff);
			JUMP_WHITE_CHARS;
			++s;//����]
		}
		JUMP_WHITE_CHARS;

		//��ȡ��ֵ���ʽ
		if(*s == '=')
		{
			char *s_std = buff;

			while(*s != ','&&*s != ';')
			{
				buff[i++] = *s++;
			}
			buff[i] = 0;
			//�������ʽ
			(*ppNode)->pAssiExpr = new CCSExpression(this->m_parentNS);
			(*ppNode)->pAssiExpr->Parse(s_std);
		}
		if(*s == ',') ++s;

		ppNode = &((*ppNode)->pNext);
	}
	++s;//����;

	return TRUE;
}

CCSStatDeclare::~CCSStatDeclare()
{
	DeclNode *pNode = m_lstDelc;

	while(pNode)
	{
		//if(pNode->szVName) delete (pNode->szVName);//��֪Ϊ������delete�����
		if(pNode->pAssiExpr) delete pNode->pAssiExpr;
		pNode = pNode->pNext;
	}
}

SRCode CCSStatDeclare::Execute(CCSVar *&pVarRet)
{
	DeclNode *pn = m_lstDelc;
	CCSVar *pVar;

	while(pn)
	{
		if(pn->nArrSize>0)
		{
			CCSVar *pVar = new CCSVar(CCSVarType::_ptr);
			(*pVar)	=  new CCSVar[pn->nArrSize];//���飬��pVarΪָ�룬ָ������ͷ��
		}
		else
		{
			pVar = new CCSVar(m_vt);
		}
		
		pVar->SetPtrLevel(pn->nPtrLevel);
		pVar->SetName(pn->szVName);
		m_parentNS->AddVar(pVar);//��ӱ������б�
		if(pn->pAssiExpr) pn->pAssiExpr->Execute();//���и�ֵ���ʽ
		pn = pn->pNext;
	}

	return _SR_TRUE;
}

BOOL CCSStatIf::Parse( char *&s )
{
	CCSStatJudge::Parse(s);
	
	JUMP_WHITE_CHARS;
	m_pNS->Parse(s);

	char *sBkup;
	//else if
	JUMP_WHITE_CHARS;
	CStrParallelCmp spc(s);
	char *scmp[1]=
	{
		"else"
	};
	spc.SetList(scmp,1);
	spc.SetEndWith(" \r\n	{(");

	CCSStatIf **pIf = &m_pElse;
	
	while(spc.Compare(sBkup)!=-1)//�������� else
	{
		JUMP_WHITE_CHARS;
		*pIf = new CCSStatIf(m_parentNS);
		if(*s=='{')//else
		{
			(*pIf)->m_pNS->Parse(s);//else ֻ��pNsû���ж����
		}
		else if(*s=='i'&&*(s+1)=='f')//else if
		{
			s+=2;
			JUMP_WHITE_CHARS;
			(*pIf)->Parse(s);//����һ��if
		}
	}
	s = sBkup;//����ָ��

	return TRUE;
}

CCSStatIf::CCSStatIf(CCSNameSpace *parentNameSpace):CCSStatJudge(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_if;
}

CCSStatIf::~CCSStatIf()
{

}

SRCode CCSStatIf::Execute(CCSVar *&pVarRet)
{
	if(CCSStatJudge::Execute(TRUE))
	{
		return m_pNS->Execute(pVarRet);
	}
	else if(m_pElse)
	{
		return m_pElse->Execute(pVarRet);
	}

	return _SR_FALSE;
}

BOOL CCSStatFor::Parse( char *&s )
{
	char buff[1024],*s_std=buff;
	int i=0;
	JUMP_WHITE_CHARS;
	CCSStringParser::SetParseObject(s);
	CCSStringParser::ParseBracket(buff,i);
	--i;//�˵�����λ��
	buff[i++] = ';';
	m_ExpInit.Parse(s_std);//��ʼ�����
	CCSStatExp::Parse(s_std);//�ж����
	m_ExpChange.Parse(s_std);//�������
	JUMP_WHITE_CHARS;
	//���������ռ�
	m_pNS->Parse(s);

	return TRUE;
}

CCSStatFor::CCSStatFor(CCSNameSpace *parentNameSpace):CCSStatJudge(parentNameSpace),m_ExpInit(parentNameSpace),m_ExpChange(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_for;
}

CCSStatFor::~CCSStatFor()
{

}

SRCode CCSStatFor::Execute(CCSVar *&pVarRet)
{
	for(m_ExpInit.Execute(pVarRet);CCSStatJudge::Execute(TRUE);m_ExpChange.Execute(pVarRet))
	{
		switch(m_pNS->Execute(pVarRet))
		{
		case _SR_BREAK:
			return _SR_FALSE;
		case _SR_RETURN:
			return _SR_RETURN;
		}
	}

	return _SR_TRUE;
}

CCSStatWhile::CCSStatWhile(CCSNameSpace *parentNameSpace):CCSStatJudge(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_while;
}

CCSStatWhile::~CCSStatWhile()
{

}

BOOL CCSStatWhile::Parse( char *&s )
{
	CCSStatJudge::Parse(s);
	JUMP_WHITE_CHARS;
	return m_pNS->Parse(s);
}

SRCode CCSStatWhile::Execute(CCSVar *&pVarRet)
{
	while(CCSStatJudge::Execute(TRUE))
	{
		switch(m_pNS->Execute(pVarRet))
		{
		case _SR_BREAK:
			return _SR_FALSE;
		case _SR_RETURN:
			return _SR_RETURN;
		}
	}

	return _SR_TRUE;
}

CCSStatDo::CCSStatDo(CCSNameSpace *parentNameSpace):CCSStatJudge(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_do;
}

CCSStatDo::~CCSStatDo()
{
	
}

BOOL CCSStatDo::Parse( char *&s )
{
	JUMP_WHITE_CHARS;
	CCSStatJudge::m_pNS->Parse(s);
	JUMP_WHITE_CHARS;
	while(*s!='(') ++s;//����(
	CCSStatJudge::Parse(s);
	JUMP_TO_STATEND;

	return TRUE;
}

SRCode CCSStatDo::Execute(CCSVar *&pVarRet)
{
	do 
	{
		switch(m_pNS->Execute(pVarRet))
		{
		case _SR_BREAK:
			return _SR_FALSE;
		case _SR_RETURN:
			return _SR_RETURN;
		}
	} while (CCSStatJudge::Execute(TRUE));

	return _SR_TRUE;
}

CCSStatTypedef::CCSStatTypedef()
{
	CCSStatement::m_statType = CCSStatement::_typedef;
}

CCSStatTypedef::~CCSStatTypedef()
{

}

BOOL CCSStatTypedef::Parse( char *&s )
{
	return TRUE;
}

CCSStatTypeStruct::CCSStatTypeStruct()
{
	CCSStatTypedef::m_type = CCSStatTypedef::_struct;
}

CCSStatTypeStruct::~CCSStatTypeStruct()
{

}

BOOL CCSStatTypeStruct::Parse( char *&s )
{
	return TRUE;
}

CCSStatTypeEnum::CCSStatTypeEnum()
{
	CCSStatTypedef::m_type = CCSStatTypedef::_enum;
}

CCSStatTypeEnum::~CCSStatTypeEnum()
{

}

BOOL CCSStatTypeEnum::Parse( char *&s )
{
	return TRUE;
}

CCSStatInvoke::CCSStatInvoke( CCSNameSpace *parentNameSpace ):
CCSStatement(parentNameSpace)
	,CCSExpression(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_invoke;
}

CCSStatInvoke::~CCSStatInvoke()
{

}

BOOL CCSStatInvoke::Parse( char *&s )
{
	char buff[1024];//��ʽ��
	int i=0,j=0;
	
	//��ȡ������
	CCharControl cc;
	cc.FlagVarName(1);
	while(cc.Table[*s] == 1)
	{
		buff[i++] = *s++;
	}
	JUMP_WHITE_CHARS;

	//��ӵ����������@����������
	buff[i++] = '@';

	CCSStringParser::ParseBracket(buff,i);
	++s;//���� (
	JUMP_WHITE_CHARS;

	//��ȡ�����б�
	
	while(*s != ')')
	{
		CCSStringParser::Listen(buff,&i);
		buff[i++] = *s++;
	}

	m_parentNS->GetTopNamespace();
	
	return TRUE;
}

CCSStatJudge::CCSStatJudge(CCSNameSpace *parentNameSpace):CCSStatExp(parentNameSpace)
{
	m_pNS = new CCSNameSpace(m_parentNS);
	CCSStatement::m_statType = CCSStatement::_null;
}

CCSStatJudge::~CCSStatJudge()
{

}

BOOL CCSStatJudge::Parse( char *&s )
{
	char buff[1024],*s_std = buff;
	int i = 0;
	JUMP_WHITE_CHARS;//�����հ׷�
	CCSStringParser::SetParseObject(s);
	CCSStringParser::ParseBracket(buff,i);//��ȡ�����еı��ʽ
	//buff[i] = ';';
	CCSExpression::Parse(s_std);

	return TRUE;
}

BOOL CCSStatJudge::Execute(BOOL bJudge)
{
	if(CCSExpression::m_expTree == NULL) return TRUE;//if�����else��û��������ֱ��return true
	return CCSExpression::Execute()->GetPtr();
}

CM::SRCode CCSStatJudge::Execute( CCSVar *&pVarRet )
{
	return _SR_FALSE;
}

SRCode CCSStatReturn::Execute(CCSVar *&pVarRet)
{
	pVarRet = CCSExpression::Execute();
	return _SR_RETURN;
}

BOOL CCSStatReturn::Parse( char *&s )
{
	return CCSExpression::Parse(s);
}

CCSStatReturn::~CCSStatReturn()
{

}

CCSStatReturn::CCSStatReturn(CCSNameSpace *parentNameSpace):
CCSStatement(parentNameSpace)
	,CCSExpression(parentNameSpace)
{
	CCSStatement::m_statType = CCSStatement::_return;
}

BOOL CCSStatExp::Parse( char *&s )
{
	/*
	CCharControl cc;
	cc.FlagVarName(1);
	cc.SetFlag('_',2);
	BOOL bVar = FALSE;
	char buff[1024],*s_std = buff;
	int i = 0;
	CCSStringParser::SetParseObject(s);

	//��ʽ��
	while(*s != ';')
	{
		CCSStringParser::Listen(buff,&i);
		switch(cc.Table[*s])
		{
		case 1:
			bVar = TRUE;
		case 2:
			if(bVar)//�����������һ�� (�������Ǻ�������
			{
				buff[i++] = '@';//��ӵ��÷���
			}
		default:
			bVar = FALSE;
		}
		buff[i++] = *s++;
	}
	++s;//����;
	*/

	CCSExpression::Parse(s);
	
	return TRUE;
}

CCSStatExp::CCSStatExp( CCSNameSpace *parentNameSpace ):CCSStatement(parentNameSpace),CCSExpression(parentNameSpace)
{
	m_statType = CCSStatement::_expr;
}

CCSStatExp::~CCSStatExp( void )
{

}

SRCode CCSStatExp::Execute(CCSVar *&pVarRet)
{
	CCSExpression::Execute();

	return _SR_TRUE;
}

CM::SRCode CCSStatBreak::Execute( CCSVar *&pVarRet )
{
	return _SR_BREAK;
}

BOOL CCSStatBreak::Parse( char *&s )
{
	JUMP_TO_STATEND;
	return TRUE;
}

CCSStatBreak::~CCSStatBreak()
{

}

CCSStatBreak::CCSStatBreak( CCSNameSpace *parentNameSpace )
{

}

CCSStatContinue::~CCSStatContinue()
{

}

CCSStatContinue::CCSStatContinue( CCSNameSpace *parentNameSpace )
{

}

CM::SRCode CCSStatContinue::Execute( CCSVar *&pVarRet )
{
	return _SR_CONTINUE;
}

BOOL CCSStatContinue::Parse( char *&s )
{
	JUMP_TO_STATEND;
	return TRUE;
}
