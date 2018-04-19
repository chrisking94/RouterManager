#include "StdAfx.h"
#include "CSNameSpace.h"
#include "CScript.h"


CCSNameSpace::~CCSNameSpace(void)
{
}



BOOL CCSNameSpace::Parse( char *&s )
{
	CCSStatement *pStat;
	CCSVar *pVarRet;

	if(*s!='{')//单语句，如 if(a==1) c=2;
	{
		if(CCSStatement::Parse(s,pStat,this))
		{
			m_lstStat.AddTail(pStat);
		}
		
		return TRUE;
	}
	++s;//跳过 {
	CCharControl c;
		c.SetFlag('a','z',1);
		c.SetFlag('A','Z',1);
		c.SetFlag("_#=+@",1);

	while(*s!='}')
	{
		switch(c.Table[*s])
		{
			case 1://查找到语句
				if(CCSStatement::Parse(s,pStat,this))
				{
					if(m_statCount++<m_nKeep)//这些是函数的形参declare语句，只运行，不加入语句表
					{
						pStat->Execute(pVarRet);
						delete pStat;//运行后删除
					}
					else
					{
						m_lstStat.AddTail(pStat);
					}
				}
		}

		JUMP_WHITE_CHARS;
	}
	++s;//跳过}

	return TRUE;
}

void CCSNameSpace::AppendStat( CCSStatement *pStat )
{
	m_lstStat.AddTail(pStat);
}

CCSNameSpace::CCSNameSpace( CCSNameSpace *parentNS ):
m_parentNameSpace(m_parentNameSpace)
	,m_statCount(0)
	,CCSVarManager(parentNS)
{

}

SRCode CCSNameSpace::Execute( CCSVar *&pVarRet )
{
	CCSStatement *pStat;
	SRCode code = _SR_TRUE;
	POSITION p = m_lstStat.GetHeadPosition();
	//执行语句
	m_lstStat.AppendNew();//这个节点无意义，用于标示列表尾 p==pEnd
	POSITION posEnd = CCSNameSpace::m_lstStat.GetTailPosition();

	while(p!=posEnd)
	{
		pStat = m_lstStat.GetNext(p);

		code = pStat->Execute(pVarRet);
		switch(code)
		{
		case _SR_BREAK:
		case _SR_CONTINUE:
		case _SR_RETURN:
			m_lstStat.RemoveTail();//移除新加的无意义节点
			return code;
		}
	}

	m_lstStat.RemoveTail();//移除新加的无意义节点

	return code;
}

CCSNameSpace * CCSNameSpace::GetTopNamespace()
{
	if(m_parentNameSpace)
	{
		return this;
	}
	else
	{
		return m_parentNameSpace->GetTopNamespace();
	}
}

