#include "StdAfx.h"
#include "CScript.h"
#include "CSException.h"


CCScript::CCScript(void):
CCSNameSpace(NULL)//顶级命名空间，无父命名空间
{
	
}

CCScript::CCScript( CString szScript ):
CCSNameSpace(NULL)
{

}


CCScript::~CCScript(void)
{
}

BOOL CCScript::LoadFile( CString szFilePath )
{
	CFile rfile(szFilePath,CFile::modeRead|CFile::shareExclusive);
	ULONG fl=rfile.GetLength();
	BYTE *pBuf=new BYTE[fl+1];
	rfile.Read(pBuf,fl);
	pBuf[fl]=0;
	rfile.Close();

	//PreTreatment((char*)pBuf);
	
	m_szScript = CString(pBuf);
	//m_szScript.Append((CString::PCXSTR)pBuf,fl);

	return TRUE;
}


BOOL CCScript::Parse( CString szJS/*=_T("")*/ )
{
	char *s;
	USES_CONVERSION;
	s = T2A(m_szScript);
	
	CCharControl c;
		c.SetFlag('a','z',1);
		c.SetFlag('A','Z',1);
		c.SetFlag("_#+-@",1);

	CCSStatement *pStat;

	while(*s)
	{
		switch(c.Table[*s])
		{
		case 1://查找到语句
			if(CCSStatement::Parse(s,pStat,this))
			{
					if(pStat->GetType()==CCSStatement::_function)
					{
						m_lstCSFunction.AddTail((CCSFunction*)pStat);
					}
					else
					{
						CCSNameSpace::m_lstStat.AddTail(pStat);
					}
			}
		}

		JUMP_WHITE_CHARS;
	}

	return TRUE;
}

CCScript& CCScript::operator=( const CCScript &csSource )
{
	this->m_lstCSFunction = csSource.m_lstCSFunction;
	this->m_lstStat = csSource.m_lstStat;
	this->m_szScript = csSource.m_szScript;

	return *this;
}

CCSFunction * CCScript::FindFunction( char * szFunctionName )
{
	CCSFunction *pf=NULL;
	FindFunction(szFunctionName,pf);
	return pf;
}

BOOL CCScript::FindFunction( char * szFunctionName,CCSFunction *&pFuncRet )
{
	POSITION p=m_lstCSFunction.GetHeadPosition();

	do
	{
		pFuncRet = m_lstCSFunction.GetNext(p);
		if(pFuncRet&&pFuncRet->IsMe(szFunctionName))
		{
			return TRUE;
		}
	}while(pFuncRet);
	
	pFuncRet = NULL;
	return FALSE;
}


CCSFunction * CCScript::operator[]( char *szFunction ) 
{
	CCSFunction *pF;
	USES_CONVERSION;
	
	if(FindFunction(szFunction,pF))
	{
		return pF;
	}
	else
	{
		CCSException e("CScript：");
		e.AppendMsg("没有找到函数 ");
		e.AppendMsg(szFunction);
		e.Throw();

		return m_lstCSFunction.GetHead();//空函数
	}
}

void CCScript::PreTreatment( char *szScript )
{
	int x=0,y=0;//x为当前扫描位置，y为当前储存位置

	while(szScript[x])
	{
		++x;
	}
}
