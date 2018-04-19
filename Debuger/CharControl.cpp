#include "StdAfx.h"
#include "CharControl.h"


CCharControl::CCharControl(void)
{
	for(int i=0;i<256;i++) {Table[i]=0;m_CompareTable[i]=0;}
}


CCharControl::~CCharControl(void)
{
}

void CCharControl::SetFlag( char ch,char flag )
{
	m_CompareTable[ch]=Table[ch];
	Table[ch]=flag;
}

void CCharControl::SetFlag( char from,char to,char flag )
{
	unsigned char m=from,n=to;

	for(int i=m;i<=n;i++)
	{
		m_CompareTable[i]=Table[i];
		Table[i]=flag;
	}
}

void CCharControl::RecoverFlag( char flag )
{
	for(int i=0;i<256;i++)
	{
		if(Table[i]==flag)
		{
			Table[i]=m_CompareTable[i];
		}
	}
}

void CCharControl::SetFlag( char *s,char flag )
{
	while(*s)
	{
		m_CompareTable[*s]=Table[*s];
		Table[*s++]=flag;
	}
}

void CCharControl::FlagCharactor( const char flag )
{
	SetFlag('0','9',1);
}

void CCharControl::FlagNumber( const char flag )
{
	SetFlag('A','Z',1);
	SetFlag('a','z',1);
}

void CCharControl::FlagVarName( const char flag )
{
	FlagCharactor(flag);
	FlagNumber(flag);
	SetFlag('_',flag);
}
