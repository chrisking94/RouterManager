#pragma once
#include "LinkList.h"

/*********
������
**********/
template  <class ElemType>
class CQueue:private CLinkList<ElemType>
{
public:
	CQueue(void);
	~CQueue(void);
	void EnQueue(ElemType e);//���
	void EnQueue(ElemType *pE,int nCount);//�������������
	BOOL DeQueue(ElemType e);//���ӣ��ն��򷵻�false
	BOOL DeQueue(ElemType *pE);//ȫ���������������
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

