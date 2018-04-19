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
	CArrayEx( CArrayEx& caExSource);//���ƹ��캯��
	
	UINT Append(const ClassType &adDataApp);
	UINT Append(const CArrayEx &caArrayApp);
	UINT AppendUnique(const ClassType &adDataApp);//����б����Ѿ����ڸ�Ԫ�������
	ClassType* AppendNew();//׷��һ����Ԫ�ز���������ָ�룬��Ԫ�س�ʼֵ����ΪNull
	void InsertAt(ClassType adData,UINT nIndex);
	BOOL RemoveAt(UINT nIndex);//�Ƴ���ָ��ָ���Ƴ��ڵ����һ���ڵ�
	void SetAt(UINT nIndex,ClassType adData);
	UINT toDataArray(ClassType* &adReturned);
	CArrayEx operator+(const CArrayEx &caArrayAdd);
	CArrayEx operator+=(const CArrayEx &caArrayAdd);
	void operator=(CArrayEx& caExSource);
	ClassType& At(UINT nIndex);
	ClassType& operator[](UINT nIndex);
	INT GetLength(void) const;
	BOOL Exists(const ClassType &adData);//���adDataδ���� == ������øú�������������ڷ���False�����ڷ���(����+1)
	void Clear();
	
	void Destroy(void);
private:
	void moveCurrentPointerTo(UINT nIndex);//�ƶ���ǰָ�뵽ָ��λ��
private:
	typedef struct Node
	{
		ClassType aData;
		Node* pNext;
		Node():pNext(NULL){}
	}Node;

	UINT m_nArrayLength;//�ó��Ȱ������һ���սڵ�
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
	while(m_pNodeCur != m_pNodeEnd)//�սڵ�
	{
		if(m_pNodeCur->aData == adData)
		{
			return m_nCurIndex+1;//��������+1
		}
		m_nCurIndex ++;
		m_pNodeCur = m_pNodeCur->pNext;
	}
	
	//����
	m_pNodeCur = m_pNodeFir;
	m_nCurIndex = 0;
	return FALSE;
}

template <class ClassType>
void CArrayEx<ClassType>::Clear()
{
	m_pNodeCur=m_pNodeFir;

	while(m_pNodeCur->pNext)//pNextΪ�������ýڵ�Ϊβ�ڵ㣬��Ҫ����
	{
		m_pNodeEnd=m_pNodeCur->pNext;//�ݴ�
		delete m_pNodeCur;
		m_pNodeCur=m_pNodeEnd;
	}
	m_pNodeFir=m_pNodeEnd;
	m_nCurIndex = 0;
	m_nArrayLength = 1;//һ���սڵ�
}

template <class ClassType>
void CArrayEx<ClassType>::operator=( CArrayEx<ClassType>& caExSource )
{
	//���ԭ��������
	Clear();

	//��������
	m_nArrayLength=caExSource.m_nArrayLength;

	caExSource.m_pNodeCur=caExSource.m_pNodeFir;
	for(UINT i=0;i<m_nArrayLength;i++)
	{
		//��������
		m_pNodeEnd->aData=caExSource.m_pNodeCur->aData;
		//�����ڵ�
		m_pNodeEnd->pNext=new Node;
		//�ƶ�ָ��
		m_pNodeEnd=m_pNodeEnd->pNext;

		caExSource.m_pNodeCur=caExSource.m_pNodeCur->pNext;
	}

	//�ָ�ָ��
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
	//��������
	ClassType* pRet=(ClassType*)&m_pNodeEnd->aData;
	//��(ClassType*)��Ϊ�˼�������ָ��xxxPtr������
	//���û�У�&�Ὣ����ָ��xxxPtrת��Ϊ**xxx

	//�����ڵ�
	m_pNodeEnd->pNext=new Node;
	//�ƶ�ָ��
	m_pNodeEnd=m_pNodeEnd->pNext;
	m_pNodeCur=m_pNodeEnd;
	//��������
	m_nCurIndex=m_nArrayLength;//�ƶ����½ڵ�
	m_nArrayLength++;

	return pRet;
}

template <class ClassType>
INT CArrayEx<ClassType>::GetLength( void ) const
{
	return m_nArrayLength-1;//ȥ��һ����ڵ�
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
	Clear();//Clear����ɾ�����һ���սڵ�
	delete m_pNodeEnd;
}


template <class ClassType>
void CArrayEx<ClassType>::moveCurrentPointerTo( UINT nIndex )
{
	ASSERT(nIndex<m_nArrayLength);//ȥ��β����ڵ�

	UINT nOffset=nIndex;

	//����ƫ������ƫ����ʼָ��
	if(nOffset>=m_nCurIndex)
	{
		nOffset-=m_nCurIndex;
	}
	else
	{
		//�ƶ�ָ�뵽��һ���ڵ�
		m_pNodeCur=m_pNodeFir;
	}
	//��������
	m_nCurIndex=nIndex;
	//�ƶ�ָ�뵽ָ������
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
	if(m_nArrayLength==1) return FALSE;//���ٱ���һ���ڵ㣨��ڵ㣩

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

	//����nIndex=1����m_pNodeCurָ��0�ţ����nIndex=0��ҲCurҲָ��0�ţ��������⴦��
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
	//��������
	m_pNodeCur->aData=adData;
	//��������
	m_nArrayLength++;
	m_nCurIndex=nIndex;
}

template <class ClassType>
UINT CArrayEx<ClassType>::Append( const CArrayEx<ClassType> &caArrayApp )
{
	if(NULL!=m_pNodeEnd->pNext)//ɾ��β���սڵ�
	{
		delete m_pNodeEnd->pNext;
	}
	//���ӽڵ�
	m_pNodeEnd->pNext=caArrayApp.m_pNodeFir;
	//�ƶ�ָ��
	m_pNodeEnd=caArrayApp.m_pNodeEnd;
	m_pNodeCur=m_pNodeEnd;
	//��������
	m_nArrayLength+=caArrayApp.m_nArrayLength-1;
	m_nCurIndex=m_nArrayLength-1;

	return m_nArrayLength;
}

template <class ClassType>
UINT CArrayEx<ClassType>::Append(const ClassType &adDataApp )
{
	//��������
	m_pNodeEnd->aData=adDataApp;
	//�����ڵ�
	m_pNodeEnd->pNext=new Node;
	//�ƶ�ָ��
	m_pNodeEnd=m_pNodeEnd->pNext;
	m_pNodeCur=m_pNodeEnd;
	//��������
	m_nCurIndex=m_nArrayLength;//�ƶ����½ڵ�
	m_nArrayLength++;
	
	return m_nArrayLength;
}
