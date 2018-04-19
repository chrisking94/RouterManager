/********************************************
By King
2015.04.11
********************************************/
#pragma once

#include "StdAfx.h"

template  <class ClassType>
class __declspec(dllexport) CArrayEx
{
public:
	CArrayEx(void);
	CArrayEx(const ClassType* adCopy,UINT nArrayLength);
	~CArrayEx(void);
	CArrayEx( CArrayEx& caExSource);//复制构造函数
	
	UINT Append(const ClassType &adDataApp);
	UINT Append(const CArrayEx &caArrayApp);
	UINT AppendUnique(const ClassType &adDataApp);//如果列表中已经存在该元素则不添加
	ClassType* AppendNew();//追加一个空元素并返回它的指针，该元素初始值被设为Null
	void InsertAt(ClassType adData,UINT nIndex);
	BOOL RemoveAt(UINT nIndex);//移除后指针指向被移除节点的上一个节点
	void SetAt(UINT nIndex,ClassType adData);
	UINT toDataArray(ClassType* &adReturned);
	CArrayEx operator+(const CArrayEx &caArrayAdd);
	CArrayEx operator+=(const CArrayEx &caArrayAdd);
	void operator=(CArrayEx& caExSource);
	ClassType& At(UINT nIndex);
	ClassType& operator[](UINT nIndex);
	INT GetLength(void) const;
	BOOL Exists(const ClassType &adData);//如果adData未重载 == ，则调用该函数会出错，不存在返回False，存在返回(索引+1)
	void Clear();
	
	void Destroy(void);
private:
	void moveCurrentPointerTo(UINT nIndex);//移动当前指针到指定位置
private:
	typedef struct Node
	{
		ClassType aData;
		Node* pNext;
		Node():pNext(NULL){}
	}Node;

	UINT m_nArrayLength;//该长度包括最后一个空节点
	UINT m_nCurIndex;

	Node *m_pNodeFir;
	Node *m_pNodeCur;
	Node *m_pNodeEnd;

	BOOL m_bDestroyed;
};

template <class ClassType>
UINT CArrayEx<ClassType>::AppendUnique( const ClassType &adDataApp )
{
	INT nIndex = Exists(adDataApp);

	if(!nIndex)
	{
		nIndex = Append(adDataApp);
	}
	
	return nIndex;
}

template <class ClassType>
BOOL CArrayEx<ClassType>::Exists( const ClassType &adData )
{
	m_pNodeCur = m_pNodeFir;
	m_nCurIndex = 0;
	while(m_pNodeCur != m_pNodeEnd)//空节点
	{
		if(m_pNodeCur->aData == adData)
		{
			return m_nCurIndex+1;//返回索引+1
		}
		m_nCurIndex ++;
		m_pNodeCur = m_pNodeCur->pNext;
	}
	
	//重置
	m_pNodeCur = m_pNodeFir;
	m_nCurIndex = 0;
	return FALSE;
}

template <class ClassType>
void CArrayEx<ClassType>::Clear()
{
	m_pNodeCur=m_pNodeFir;

	while(m_pNodeCur->pNext)//pNext为空则代表该节点为尾节点，需要保留
	{
		m_pNodeEnd=m_pNodeCur->pNext;//暂存
		delete m_pNodeCur;
		m_pNodeCur=m_pNodeEnd;
	}
	m_pNodeFir=m_pNodeEnd;
	m_nCurIndex = 0;
	m_nArrayLength = 1;//一个空节点
}

template <class ClassType>
void CArrayEx<ClassType>::operator=( CArrayEx<ClassType>& caExSource )
{
	//清空原来的数组
	Clear();

	//复制数据
	m_nArrayLength=caExSource.m_nArrayLength;

	caExSource.m_pNodeCur=caExSource.m_pNodeFir;
	for(UINT i=0;i<m_nArrayLength;i++)
	{
		//更新数据
		m_pNodeEnd->aData=caExSource.m_pNodeCur->aData;
		//创建节点
		m_pNodeEnd->pNext=new Node;
		//移动指针
		m_pNodeEnd=m_pNodeEnd->pNext;

		caExSource.m_pNodeCur=caExSource.m_pNodeCur->pNext;
	}

	//恢复指针
	caExSource.m_pNodeCur=caExSource.m_pNodeFir;
	caExSource.m_nCurIndex=0;

	m_pNodeCur=m_pNodeFir;
	m_nCurIndex=0;
}

template <class ClassType>
CArrayEx<ClassType>::CArrayEx(  CArrayEx<ClassType>& caExSource )
{
	operator=(caExSource);
}

template <class ClassType>
ClassType& CArrayEx<ClassType>::At( UINT nIndex )
{
	return CArrayEx::operator[](nIndex);
}

template <class ClassType>
ClassType* CArrayEx<ClassType>::AppendNew()
{
	//更新数据
	ClassType* pRet=(ClassType*)&m_pNodeEnd->aData;
	//加(ClassType*)是为了兼容智能指针xxxPtr型数组
	//如果没有，&会将智能指针xxxPtr转换为**xxx

	//创建节点
	m_pNodeEnd->pNext=new Node;
	//移动指针
	m_pNodeEnd=m_pNodeEnd->pNext;
	m_pNodeCur=m_pNodeEnd;
	//更新属性
	m_nCurIndex=m_nArrayLength;//移动到新节点
	m_nArrayLength++;

	return pRet;
}

template <class ClassType>
INT CArrayEx<ClassType>::GetLength( void ) const
{
	return m_nArrayLength-1;//去掉一个虚节点
}

template <class ClassType>
CArrayEx<ClassType>::CArrayEx( void ):
m_pNodeFir(new Node)
	,m_pNodeCur(m_pNodeFir)
	,m_pNodeEnd(m_pNodeFir)
	,m_nCurIndex(0)
	,m_nArrayLength(1)
	,m_bDestroyed(FALSE)
{

}

template <class ClassType>
CArrayEx<ClassType>::CArrayEx( const ClassType* adCopy,UINT nArrayLength )
{
	for(UINT i=0;i<nArrayLength;i++)
	{
		m_pNodeCur->aData=adCopy[i];
		m_pNodeCur->pNext=new Node;
		m_pNodeCur=m_pNodeCur->pNext;
	}

	m_nArrayLength=nArrayLength;
	m_nCurIndex=m_nArrayLength-1;
}

template <class ClassType>
CArrayEx<ClassType>::~CArrayEx( void )
{
	if(m_bDestroyed) return;
	Clear();//Clear不会删除最后一个空节点
	delete m_pNodeEnd;
}


template <class ClassType>
void CArrayEx<ClassType>::moveCurrentPointerTo( UINT nIndex )
{
	ASSERT(nIndex<m_nArrayLength);//去掉尾部虚节点

	UINT nOffset=nIndex;

	//计算偏移量及偏移起始指针
	if(nOffset>=m_nCurIndex)
	{
		nOffset-=m_nCurIndex;
	}
	else
	{
		//移动指针到第一个节点
		m_pNodeCur=m_pNodeFir;
	}
	//更新属性
	m_nCurIndex=nIndex;
	//移动指针到指定索引
	if(nOffset>0)
	{
		while(nOffset--)
		{
			m_pNodeCur=m_pNodeCur->pNext;
		}
	}
}


template <class ClassType>
void CArrayEx<ClassType>::Destroy( void )
{
	CArrayEx<ClassType>::~CArrayEx();
	m_bDestroyed=TRUE;
}

template <class ClassType>
ClassType& CArrayEx<ClassType>::operator[]( UINT nIndex )
{
	moveCurrentPointerTo(nIndex);
	return m_pNodeCur->aData;
}

template <class ClassType>
CArrayEx<ClassType> CArrayEx<ClassType>::operator+=( const CArrayEx<ClassType> &caArrayAdd )
{
	this->Append(caArrayAdd);
	return *this;
}

template <class ClassType>
CArrayEx<ClassType> CArrayEx<ClassType>::operator+( const CArrayEx<ClassType> &caArrayAdd )
{
	CArrayEx ArrayReturn(*this);
	return ArrayReturn->Append(caArrayAdd);
}

template <class ClassType>
UINT CArrayEx<ClassType>::toDataArray( ClassType* &adReturned )
{
	UINT nLengthTmp=m_nArrayLength-1;
	adReturned = new ClassType[nLengthTmp];
	Node* pTmp=m_pNodeFir;
	
	for(UINT i=0 ; i<nLengthTmp; i++)
	{
		adReturned[i]=pTmp->aData;
		pTmp=pTmp->pNext;
	}
	
	return nLengthTmp;
}

template <class ClassType>
void CArrayEx<ClassType>::SetAt( UINT nIndex ,ClassType adData)
{
	moveCurrentPointerTo(nIndex);

	m_pNodeCur->aData=adData;
}

template <class ClassType>
BOOL CArrayEx<ClassType>::RemoveAt( UINT nIndex )
{
	if(m_nArrayLength==1) return FALSE;//至少保留一个节点（虚节点）

	Node *pTmp;
	if(nIndex==0)
	{
		pTmp=m_pNodeFir->pNext;
		delete m_pNodeFir;
		m_pNodeFir=pTmp;
		m_pNodeCur=m_pNodeFir;
	}
	else
	{
		moveCurrentPointerTo(nIndex-1);
		pTmp=m_pNodeCur->pNext;
		m_pNodeCur->pNext=pTmp->pNext;
		delete pTmp;
	}

	m_nCurIndex=nIndex;
	m_nArrayLength--;

	return TRUE;
}

template <class ClassType>
void CArrayEx<ClassType>::InsertAt( ClassType adData,UINT nIndex )
{
	if(nIndex>0)
	{
		moveCurrentPointerTo(nIndex-1);
	}

	//例：nIndex=1，则m_pNodeCur指向0号，如果nIndex=0，也Cur也指向0号，需做特殊处理
	if(nIndex==0)
	{
		Node* pTmp=m_pNodeFir;
		m_pNodeFir=new Node;
		m_pNodeCur=m_pNodeFir;
		m_pNodeCur->pNext=pTmp;
	}
	else
	{
		Node* pTmp;
		pTmp=m_pNodeCur->pNext;
		m_pNodeCur->pNext=new Node;
		m_pNodeCur=m_pNodeCur->pNext;
		m_pNodeCur->pNext=pTmp;
	}
	//更新数据
	m_pNodeCur->aData=adData;
	//更新属性
	m_nArrayLength++;
	m_nCurIndex=nIndex;
}

template <class ClassType>
UINT CArrayEx<ClassType>::Append( const CArrayEx<ClassType> &caArrayApp )
{
	if(NULL!=m_pNodeEnd->pNext)//删除尾部空节点
	{
		delete m_pNodeEnd->pNext;
	}
	//连接节点
	m_pNodeEnd->pNext=caArrayApp.m_pNodeFir;
	//移动指针
	m_pNodeEnd=caArrayApp.m_pNodeEnd;
	m_pNodeCur=m_pNodeEnd;
	//更新属性
	m_nArrayLength+=caArrayApp.m_nArrayLength-1;
	m_nCurIndex=m_nArrayLength-1;

	return m_nArrayLength;
}

template <class ClassType>
UINT CArrayEx<ClassType>::Append(const ClassType &adDataApp )
{
	//更新数据
	m_pNodeEnd->aData=adDataApp;
	//创建节点
	m_pNodeEnd->pNext=new Node;
	//移动指针
	m_pNodeEnd=m_pNodeEnd->pNext;
	m_pNodeCur=m_pNodeEnd;
	//更新属性
	m_nCurIndex=m_nArrayLength;//移动到新节点
	m_nArrayLength++;
	
	return m_nArrayLength;
}
