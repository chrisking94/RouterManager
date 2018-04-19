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
	if(optSigns[*s])//���������
	{
		int j,matchCount,matchOKIndex;
		char nID = 0;
		bool matchTable[OPT_COUNT];//���ڱ�ʾƥ��ɹ���ʧ��
		memset(matchTable,1,OPT_COUNT);
		matchCount = OPT_COUNT;
		char c;

		//��ʼ���ң��Լ���ɣģ������������������Ǣ������char�����������ԣ������ڽ���֮ǰҪ����������
		j=1;//��������
		while(matchCount)
		{
			for(nID=0;nID<OPT_COUNT;nID++)
			{
				if(matchTable[nID])
				{
					if(j==1)//��������
					{
						if(opts[nID][j] - 0x30 == _Ignore) continue;//����������ò���������
						c = (char)(m_nAsso+0x30);//�������Ҫ�ڽ���֮ǰ����
					}
					else
					{
						c = *s;
					}
					if(c != opts[nID][j])//����ƥ��ʱ
					{
						if(opts[nID][j]=='\0')//�Ѿ�ƥ�䵽�����ģ�������⣬���û�м���ƥ��ģ�����Ϊ�������Ҫ�ҵ�
						{
							matchOKIndex=nID;
						}
						matchTable[nID] = 0;//ȥ����ƥ��Ȩ����־
						--matchCount;
					}
				}
			}
			if(matchCount==0)//û��һ��ƥ����ˣ�����ȫƥ��ľ��ǽ����������
			{
				nID = matchOKIndex;
				break;
			}
			
			if(j++!=1) ++s;//j==1ʱ�ڿ������ԣ����ƶ��ַ���
		}
		
		//������Ŀ��
		m_nMeshNum = (Mesh_Number)(opts[nID][0]-0x30);

		//�������ȼ�
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
	//��ʽΪ "m a opt"
	//m:������Ŀ�� a:����� opt:�����string
	"00#",//�����#�� 0������֮��ӽ����ģ������ID��Ҫ+1
	
	//0	    1     2    3    4    6    7    8
	"10++","10--","10[","10]","10(","10)","10.","10->"//���ȼ�Ϊ1������++
	//9     10    11    12    13   14   15   16   17
	,"11-","11tc","11++","11--","11*","11&","11!","11~","11so"//-Ϊ���ţ�tcΪǿ������ת����ǰ��++��soΪsizeof
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
	,"00,","20@"//��Ŀ
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
};//�ɳ�������

int CCSOperator::ISP() const
{
	//һЩ������������ȼ�
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
	//һЩ������������ȼ�
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
