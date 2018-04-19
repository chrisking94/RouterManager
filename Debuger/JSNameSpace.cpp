#include "StdAfx.h"
#include "JSNameSpace.h"
#include "FuncCharCtrl.h"
#include "Stack.h"


CJSNameSpace::CJSNameSpace(void)
{
}


CJSNameSpace::~CJSNameSpace(void)
{
}

void CJSNameSpace::Execute()
{
	POSITION p=m_lstStatement.GetHeadPosition();

	while(p)
	{
		m_lstStatement.GetNext(p).Execute();
	}
}

int CJSNameSpace::AddVar( const CJSVar &var )
{
	m_lstVar.AddTail(var);

	return m_lstVar.GetCount();
}

CJSVar* CJSNameSpace::AppendNewVar()
{
	return m_lstVar.AppendNew();
}

CJSVar* CJSNameSpace::GetVar( CString szName )
{
	POSITION p=m_lstVar.GetHeadPosition();
	CJSVar *pRet;

	while(p)
	{
		pRet = &m_lstVar.GetNext(p);
		if(pRet->szName==szName)
		{
			return pRet;
		}
	}

	return NULL;
}

CJSVar* CJSNameSpace::GetVar( int nIndex )
{
	return &m_lstVar[nIndex];
}

void CJSNameSpace::Parse( CString szSegment )
{
	USES_CONVERSION;
	char* s=T2A(szSegment);
	CString sTmp=_T("");

	CFuncCharCtrl fc;
	CFuncCharCtrl::ControlFlag lastFlag=CFuncCharCtrl::_Common;
	CJSStatement* pCurJSStat;
	CStack<char> stack;
	char c;

	fc.EnSearchStatement();

	while(*s)
	{
		switch(fc.Table[*s])
		{
			case fc._SearchStatement:
				sTmp.AppendChar(*s);
				if(lastFlag!=fc._SearchStatement) fc.EnSearchStatementEnd();
				break;
			case fc._SearchStatement+fc._Offset://search end
				fc.EnSearchStatement(FALSE);
				fc.EnSearchStatement();
				
				if(sTmp=="var")
				{
					m_lstStatement.AddTail(*new CJSStatVar(this));
					fc.EnReceiveStatData(CJSStatement::_var);
				}
				else if(sTmp=="if")
				{
					m_lstStatement.AddTail(*new CJSStatIf(this));
					fc.EnReceiveStatData(CJSStatement::_if);
				}
				else if(sTmp=="while")
				{
					m_lstStatement.AddTail(*new CJSStatWhile(this));
					fc.EnReceiveStatData(CJSStatement::_while);
				}
				else if(sTmp=="do")
				{
					m_lstStatement.AddTail(*new CJSStatDo(this));
					fc.EnReceiveStatData(CJSStatement::_do);
				}
				else
				{
					m_lstStatement.AddTail(*new CJSStatInvoke(this,sTmp));
					fc.EnReceiveStatData(CJSStatement::_invoke);
				}
				pCurJSStat=&m_lstStatement.GetTail();
				sTmp=_T("");

				break;
			case fc._SearchStatement+fc._Offset*2://	赋值语句
				fc.EnNameSpace();
				fc.EnNameSpaceEnd();
				
				m_lstStatement.AddTail(*new CJSStatAssign(this));
				pCurJSStat=&m_lstStatement.GetTail();
				((CJSStatAssign*)pCurJSStat)->szVar = sTmp;
				fc.EnReceiveStatData(CJSStatement::_assign);

				sTmp=_T("");
				break;
			case fc._NameSpace:
				sTmp.AppendChar(*s);
				break;
			case fc._NameSpace+fc._Offset:// {
				stack.Push(*s);
				sTmp.AppendChar(*s);
				break;
			case fc._NameSpace+fc._Offset*2:// }
				if(!stack.Pop(c))
				{
					AfxMessageBox(_T("语法错误!"));
				}
				else
				{
					if(stack.IsEmpty())
					{
						fc.EnNameSpace(FALSE);//退出Body接收模式
						fc.EnSearchStatement();//重新开始搜索函数
						sTmp=_T("");
					}
					else
					{
						sTmp.AppendChar(*s);
					}
				}
				break;
			case fc._NameSpace+fc._Offset*3:// "
				sTmp.AppendChar(*s);
				fc.EnString();
				fc.EnStringEnd();
				break;
			case fc._String:
				sTmp.AppendChar(*s);
				break;
			case fc._String+fc._Offset://	"
				fc.EnString(FALSE);
				sTmp.AppendChar(*s);
				break;
			case fc._String+fc._Offset*2://	\转意符

				sTmp.AppendChar(*s);
				sTmp.AppendChar(*++s);//总是两个一起出现
				break;
		}
		lastFlag=(CFuncCharCtrl::ControlFlag)fc.Table[*s];

		s++;
	}

	int g=0;
}

void CJSNameSpace::SetParent( CJSNameSpace *parent )
{
	m_parent = parent;
}
