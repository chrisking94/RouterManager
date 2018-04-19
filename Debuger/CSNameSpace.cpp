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

	if(*s!='{')//����䣬�� if(a==1) c=2;
	{
		if(CCSStatement::Parse(s,pStat,this))
		{
			m_lstStat.AddTail(pStat);
		}
		
		return TRUE;
	}
	++s;//���� {
	CCharControl c;
		c.SetFlag('a','z',1);
		c.SetFlag('A','Z',1);
		c.SetFlag("_#=+@",1);

	while(*s!='}')
	{
		switch(c.Table[*s])
		{
			case 1://���ҵ����
				if(CCSStatement::Parse(s,pStat,this))
				{
					if(m_statCount++<m_nKeep)//��Щ�Ǻ������β�declare��䣬ֻ���У�����������
					{
						pStat->Execute(pVarRet);
						delete pStat;//���к�ɾ��
					}
					else
					{
						m_lstStat.AddTail(pStat);
					}
				}
		}

		JUMP_WHITE_CHARS;
	}
	++s;//����}

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
	//ִ�����
	m_lstStat.AppendNew();//����ڵ������壬���ڱ�ʾ�б�β p==pEnd
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
			m_lstStat.RemoveTail();//�Ƴ��¼ӵ�������ڵ�
			return code;
		}
	}

	m_lstStat.RemoveTail();//�Ƴ��¼ӵ�������ڵ�

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

