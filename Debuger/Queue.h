#pragma once
#include "LinkList.h"

/*********
队列类
**********/
template  <class ElemType>
class CQueue:private CLinkList<ElemType>
{
public:
	CQueue(void);
	~CQueue(void);
	void EnQueue(ElemType e);//入队
	void EnQueue(ElemType *pE,int nCount);//从数组批量入队
	BOOL DeQueue(ElemType e);//出队，空队则返回false
	BOOL DeQueue(ElemType *pE);//全部批量输出到数组
	ElemType DeQueue();
	BOOL IsEmpty();
	BOOL Compare(CQueue &q);
	void Clear();
};

template  <class ElemType>
void CQueue<ElemType>::Clear()
{
	CLinkList<ElemType>::Clear();
}

template  <class ElemType>
BOOL CQueue<ElemType>::Compare( CQueue &q )
{
	return CLinkList<ElemType>::Compare(q);
}

template  <class ElemType>
BOOL CQueue<ElemType>::IsEmpty()
{
	return CLinkList<ElemType>::IsEmpty();
}

template  <class ElemType>
CQueue<ElemType>::~CQueue( void )
{

}

template  <class ElemType>
CQueue<ElemType>::CQueue( void )
{

}

template  <class ElemType>
BOOL CQueue<ElemType>::DeQueue( ElemType *pE )
{
	int i=0;
	ElemType e;

	while(DeQueue(e))
	{
		pE[i]=e;
	}

	return TRUE;
}

template  <class ElemType>
void CQueue<ElemType>::EnQueue( ElemType *pE,int nCount )
{
	CLinkList::Append(pE,nCount);
}

template  <class ElemType>
BOOL CQueue<ElemType>::DeQueue( ElemType e )
{
	BOOL bRet = CLinkList::GetHead(e);
	CLinkList::RemoveHead();

	return bRet;
}

template  <class ElemType>
void CQueue<ElemType>::EnQueue( ElemType e )
{
	CLinkList::Append(e);
}

