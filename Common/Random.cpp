#include "StdAfx.h"
#include "Random.h"
#include <stdlib.h>
#include <time.h>

CRandom::CRandom(void)
{
}


CRandom::~CRandom(void)
{
}

void CRandom::seed( UINT nSeed )
{
	srand(nSeed);
}

void CRandom::seedByTime()
{
	srand((unsigned)time( NULL ));
}

int CRandom::getNum( int nFrom,int nTo )
{
	return rand()%(nTo-nFrom);
}

