#include "StdAfx.h"
#include "CSOperator.h"

#define OPT_COUNT 50

CCSOperator::CCSOperator(void):m_nAsso(_Ignore),m_nMeshNum(_null)
{
	
}


CCSOperator::~CCSOperator(void)
{
}

BOOL CCSOperator::Parse( char *&s )
{
	char opt[4]={0,0,0,0};
	if(optSigns[*s])//发现运算符
	{
		int j,matchCount,matchOKIndex;
		char nID = 0;
		bool matchTable[OPT_COUNT];//用于标示匹配成功或失败
		memset(matchTable,1,OPT_COUNT);
		matchCount = OPT_COUNT;
		char c;

		//开始查找，以计算ＩＤ，运算符以两个特征标记①运算符char②运算符结合性，所以在解析之前要设置其结合性
		j=1;//考察结合性
		while(matchCount)
		{
			for(nID=0;nID<OPT_COUNT;nID++)
			{
				if(matchTable[nID])
				{
					if(j==1)//考察结合性
					{
						if(opts[nID][j] - 0x30 == _Ignore) continue;//此运算符设置不考察结合性
						c = (char)(m_nAsso+0x30);//结合性需要在解析之前设置
					}
					else
					{
						c = *s;
					}
					if(c != opts[nID][j])//当不匹配时
					{
						if(opts[nID][j]=='\0')//已经匹配到完整的，继续检测，如果没有继续匹配的，则认为这个就是要找的
						{
							matchOKIndex=nID;
						}
						matchTable[nID] = 0;//去掉其匹配权力标志
						--matchCount;
					}
				}
			}
			if(matchCount==0)//没有一个匹配的了，则完全匹配的就是解析的运算符
			{
				nID = matchOKIndex;
				break;
			}
			
			if(j++!=1) ++s;//j==1时在考察结合性，不移动字符串
		}
		
		//操作数目数
		m_nMeshNum = (Mesh_Number)(opts[nID][0]-0x30);

		//计算优先级
		for(m_nPriority=0;m_nPriority<16;m_nPriority++)
		{
			if(nID<=prioritys[m_nPriority])
				break;
		}
		//
		m_nOptID = (OPT_ID)nID;
		
		return TRUE;
	}

	return FALSE;
}

char CCSOperator::opts[OPT_COUNT][6]=
{
	//格式为 "m a opt"
	//m:操作数目数 a:结合性 opt:运算符string
	"00#",//特殊的#号 0，这是之后加进来的，后面的ID都要+1
	
	//0	    1     2    3    4    6    7    8
	"10++","10--","10[","10]","10(","10)","10.","10->"//优先级为1，后置++
	//9     10    11    12    13   14   15   16   17
	,"11-","11tc","11++","11--","11*","11&","11!","11~","11so"//-为负号，tc为强制类型转换，前置++，so为sizeof
	// 18   19   20
	,"20/","22*","20%"
	// 21   22
	,"20+","20-"
	// 23    24
	,"20<<","20>>"
	// 25   26    27   28
	,"20>","20>=","20<","20<="
	// 29    30
	,"20==","20!="
	// 31
	,"20&"
	// 32
	,"20^"
	// 33
	,"20|"
	// 34
	,"20&&"
	// 35
	,"20||"
	// 36
	,"31?:"
	// 37   38    39    40    41    42    43     44     45    46    47
	,"20=","21/=","21*=","21%=","21+=","21-=","21<<=","21>>=","21&=","21^=","12|="
	// 48
	,"00,","20@"//多目
};

#define PRIORITY_COUNT 16
char CCSOperator::prioritys[PRIORITY_COUNT]=
{
	_Sharp,_Pointer,_SizeOf,_Mod
	,_Sub,_Rshift,_LE,_NEqu
	,_BAnd,_BEOR,_BOr,_And
	,_Or,_ConditionExpression,_ABOr,_At
};

bool CCSOperator::optSigns[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};//由程序生成

int CCSOperator::ISP() const
{
	//一些运算的特殊优先级
	switch(m_nOptID)
	{
	case _Sharp://	#
		return 0;
	case _LBracket://	(
		return 1;
	case _RBracket://	)
		return PRIORITY_COUNT*2+1;
	}
	
	return PRIORITY_COUNT*2 - m_nPriority*2+1;
}

int CCSOperator::ICP() const
{
	//一些运算的特殊优先级
	switch(m_nOptID)
	{
	case _Sharp://	#
		return 0;
	case _LBracket://	(
		return PRIORITY_COUNT*2+1;
	case _RBracket://	)
		return 1;
	}
	return PRIORITY_COUNT*2 - m_nPriority*2;
}

CCSOperator::OPT_ID CCSOperator::GetID() const
{
	return m_nOptID;
}

CCSOperator::Mesh_Number CCSOperator::GetMN() const
{
	return m_nMeshNum;
}

CCSOperator::Associativity CCSOperator::GetAsso() const
{
	return m_nAsso;
}

void CCSOperator::SetAsso( const Associativity as )
{
	m_nAsso = as;
}
