#include "StdAfx.h"
#include "CSVarManager.h"
#include "CSException.h"


CCSVarManager::~CCSVarManager(void)
{
	//清除变量
	for(int i=0;i<m_nCount;i++)
	{
		delete m_lstPtr[i];
	}
	delete [] m_lstPtr;
}

CCSVar * CCSVarManager::FindVar( char *szName )
{
	//二分法查找
	int low=0,high=m_nNamedVarCount-1;
	int mid;
	char *s;
	int mi = 0,i;//匹配位置

	while(low<=high)
	{
		mid = (low+high)/2;
		s=(m_lstPtr[mid])->GetName();
		if(s[mi]==*szName)//匹配一个字符
		{
			//向上查找
			for(i= mid;i>=0&&*(m_lstPtr[i]->GetName())==*szName;--i)//第一个字符必须匹配
				if(m_lstPtr[i]->IsMe(szName))
					return m_lstPtr[i];
			//向下查找
			for(i= mid;i<=m_nNamedVarCount&&*(m_lstPtr[i]->GetName())==*szName;++i)//第一个字符必须匹配
				if(m_lstPtr[i]->IsMe(szName))
					return m_lstPtr[i];
			break;//如果没有匹配到说明变量不存在
		}
		else if(*s>*szName)//粗查找
		{
			high = mid - 1;//在左边
		}
		else
		{
			low = mid + 1;//在右边
		}
	}

	//未找到
	if(m_parentMgr) return m_parentMgr->FindVar(szName);//到父空间查找

	return NULL;
}

CCSVar::VAR_ID CCSVarManager::AddVar(  CCSVar *pV )
{
	char *s,*s_n;
	int i,j,k;
	int low=0 , high = m_nNamedVarCount-1 ,mid=0 ,mi;

	++m_nCount;
	if(m_nCount>m_lstLen)//变量数超过表长
	{
		m_lstLen *= 2;//扩大列表空间为2倍
		CCSVar **p = new CCSVar*[m_lstLen];
		//复制数据
		memcpy(p,m_lstPtr,(m_nCount-1)*sizeof(CCSVar*));
		//删除原来的指针
		delete []m_lstPtr;
		//更新指针
		m_lstPtr = p;
	}

	s_n = pV->GetName();
	if(s_n==NULL)//无名变量
	{
		//直接插入到尾部
	}
	else
	{
		++m_nNamedVarCount;
		//二分法插入
		//1 查找插入位置
		while(low<=high)
		{
			mid = (low + high)/2;
			s = m_lstPtr[mid]->GetName();

			if(*s_n == *s)
			{
				break;//在mid出插入
			}
			else if(*s_n>*s)
			{
				low = mid + 1;
			}
			else
			{
				high = mid -1;
			}
		}
	}
	//直接在low处插入
	for(i=m_nCount;i>low;i--) m_lstPtr[i] = m_lstPtr[i-1];//后移元素
	m_lstPtr[low] = pV;//插入元素

	return 0;
}

CCSVarManager::CCSVarManager(CCSVarManager *parentMgr, int nLstLen /*= 50*/ ):
m_lstLen(nLstLen),
	m_nCount(0),
	m_nNamedVarCount(0),
	m_parentMgr(parentMgr)
{
	m_lstPtr = new CCSVar*[m_lstLen];
}

void CCSVarManager::SetKeepCount( int nKeep )
{
	m_nKeep = nKeep;
}

CCSVar * CCSVarManager::operator[]( const int index )
{
	if(index<m_nCount)
	{
		return m_lstPtr[index];
	}
	else
	{
		CCSException e("CScript.VarManager.opt([])：索引 ");
		e.AppendMsg(index);
		e.AppendMsg("超过界限 Count=");
		e.AppendMsg(m_nCount);
		e.Throw();
	}

	return NULL;
}

CCSVar & CCSVarManager::Var( const int index )
{
	return *operator[](index);
}
