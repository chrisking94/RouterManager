#include "StdAfx.h"
#include "CSVar.h"
#include "CSException.h"
#include "math.h"


CCSVar::CCSVar( ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_void)
{
	
}

CCSVar::CCSVar( const CCSVar &v):CCSVarType(v)
{
	m_szName = NULL;
	m_nID = s_nGlobalID++;
	operator=(v);
}

CCSVar::CCSVar( const char v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_char)
{
	m_ptr = v;
}

CCSVar::CCSVar( const double v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_double)
{
	m_ptr = (long) new double;
	*(double*)m_ptr = v;//double使用指针存储
}

CCSVar::CCSVar( const float v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_double)
{
	m_ptr = (long) new double;
	*(double*)m_ptr = v;//double使用指针存储
}

CCSVar::CCSVar( const int v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_int)
{
	m_ptr = v;
}

CCSVar::CCSVar( const long v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_int)
{
	m_ptr = v;
}

CCSVar::CCSVar( const CCSVar *v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_ptr)
{
		m_ptr = (long)v;
}

CCSVar::CCSVar( const short v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_int)
{
		m_ptr = v;
}

CCSVar::CCSVar( const char * v ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(CCSVarType::_string)
{
	m_strlen = strlen(v);
	m_ptr = (long) new char[m_strlen+1];//复制字符串
	((char*)m_ptr)[m_strlen]=0;
	strcpy((char*)m_ptr,v);
}

CCSVar::CCSVar( const CCSVarType::TYPE t ):m_nID(s_nGlobalID++),m_ptr(NULL),m_szName(NULL),CCSVarType(t)
{

}


CCSVar::~CCSVar( void )
{
	switch(CCSVarType::m_type)
	{
	case  CCSVarType::_double://double 存储在另外的地方，删除其指针
		if(m_ptr!=NULL)
		{
			delete (double*)m_ptr;
		}
		break;
	case CCSVarType::_string:
		if(m_ptr!=NULL)
		{
			delete [] (char*)m_ptr;
		}
		break;
	}
}

CCSVar CCSVar::operator+( const CCSVar &v ) const
{
	int n;
	char *s ,*s1,*s2;
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,5,6,6,0,6},//_string,
	};
	switch(CCT[m_type][v.m_type])
	{
	case 1://char,int,ptr
		return m_ptr + v.m_ptr;
	case 2://char int + double
		return m_ptr + *(double*)v.m_ptr;
	case 3://double + char int
		return *(double*)m_ptr + v.m_ptr;
	case 4://double + double
		return *(double*)m_ptr + *(double*)v.m_ptr;
	case 5://string + char
		s = new char[m_strlen+2];
		s[m_strlen+1]='\0';
		strcpy(s,(char*)m_ptr);//将原来的字符串拷贝进来
		s[m_strlen]=(char)v.m_ptr;
		return s;
	case 6://string + double int string
		s1 = v.StrVal();
		n=strlen(s1);
		s = new char[m_strlen+n+1];
		strcpy(s,(char*)m_ptr);//拷贝原来的字符串
		s[m_strlen+n+1]='\0';
		s2 = &s[m_strlen];//定位到s1的末尾
		strcpy(s2,s1);//拷贝 转换出来的字符串，接到s1上
		return s;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(+)：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();

	return 0;
}

CCSVar CCSVar::operator-( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};
	switch(CCT[m_type][v.m_type])
	{
	case 1://char,int,ptr
		return m_ptr - v.m_ptr;
	case 2://char int - double
		return m_ptr - *(double*)v.m_ptr;
	case 3://double - char int
		return *(double*)m_ptr - v.m_ptr;
	case 4://double - double
		return *(double*)m_ptr - *(double*)v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(-)：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();

	return 0;
}

CCSVar CCSVar::operator*( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};
	switch(CCT[m_type][v.m_type])
	{
	case 1://char,int,ptr
		return m_ptr * v.m_ptr;
	case 2://char int * double
		return m_ptr * *(double*)v.m_ptr;
	case 3://double * char int
		return *(double*)m_ptr * v.m_ptr;
	case 4://double * double
		return *(double*)m_ptr * *(double*)v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(*)：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();

	return 0;
}

CCSVar & CCSVar::operator*()
{
	if(m_type == CCSVarType::_ptr)
	{
		return *(CCSVar*)m_ptr;
	}

	CCSException e(CCSException::_BadPointer);
	e.AppendMsg("CScript.CSVar.opt(*_ptr)：");
	e.AppendMsg("错误的指针");
	e.Throw();
	return *(CCSVar*)m_ptr;
}

CCSVar CCSVar::operator/( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};
	switch(CCT[m_type][v.m_type])
	{
	case 1://char,int,ptr
		return m_ptr / v.m_ptr;
	case 2://char int / double
		return m_ptr / *(double*)v.m_ptr;
	case 3://double / char int
		return *(double*)m_ptr / v.m_ptr;
	case 4://double / double
		return *(double*)m_ptr / *(double*)v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt( / )：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();

	return 0;
}

CCSVar CCSVar::operator%( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,0,1,0,0},//_char,
		{0,0,0,0,0,0},//_double,
		{0,1,0,1,0,0},//_int,
		{0,0,0,0,0,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};
	switch(CCT[m_type][v.m_type])
	{
	case 1://char,int,ptr
		return m_ptr % v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt( % )：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();

	return 0;
}

CCSVar CCSVar::operator^( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,0,1,0,0},//_char,
		{0,0,0,0,0,0},//_double,
		{0,1,0,1,0,0},//_int,
		{0,0,0,0,0,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};
	switch(CCT[m_type][v.m_type])
	{
	case 1://char,int,ptr
		return m_ptr ^ v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt( ^ )：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();

	return 0;
}

CCSVar CCSVar::operator+=( const CCSVar &v )
{
	return *this = *this + v;
}

CCSVar CCSVar::operator-=( const CCSVar &v )
{
	return *this = *this - v;
}

CCSVar CCSVar::operator*=( const CCSVar &v )
{
	return *this = *this * v;
}

CCSVar CCSVar::operator/=( const CCSVar &v )
{
	return *this = *this / v;
}

CCSVar CCSVar::operator%=( const CCSVar &v )
{
	return *this = *this % v;
}

CCSVar CCSVar::operator^=( const CCSVar &v )
{
	return *this = *this ^ v;
}

CCSVar CCSVar::operator++( const int )
{
	switch(CCSVarType::m_type)
	{
	case CCSVarType::_char:
	case CCSVarType::_int:
	case CCSVarType::_ptr:
		return m_ptr ++ ;
		return (*(float*)&m_ptr) ++ ;
	case CCSVarType::_string:

		break;
	case CCSVarType::_double://都为64位
		return (*(double*)m_ptr) ++ ;
		break;
	}
	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(a++)：");
	e.AppendMsg(GetTypeString());
	e.AppendMsg("不支持此操作");
	e.Throw();
}

CCSVar CCSVar::operator++()
{
	switch(CCSVarType::m_type)
	{
	case CCSVarType::_char:
	case CCSVarType::_int:
	case CCSVarType::_ptr:
		return  ++ m_ptr;
	case CCSVarType::_double://都为64位
		return  ++ (*(double*)m_ptr);
		break;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(++a)：");
	e.AppendMsg(GetTypeString());
	e.AppendMsg("不支持此操作");
	e.Throw();
}



bool CCSVar::IsMe( char *szName ) const
{
	char *s=m_szName;
	if(s)
	{
		while(*szName++==*s++&&*szName!='\0');
		if(!*s)return true;
	}
	return false;
}

void CCSVar::SetName( char *name )
{
	if(m_szName)
	{
		delete []m_szName;
	}
	m_szName = new char[strlen(name)+1];
	strcpy(m_szName,name);
}

char * CCSVar::GetName() const
{
	return m_szName;
}

char * CCSVar::StrVal() const
{
	static char s[20];s[19] = 0;
	int i,n;
	switch(m_type)
	{
	case CCSVarType::_char:
		s[0] = (char)m_ptr;
		return s;
	case CCSVarType::_int:
		_itoa_s((int)m_ptr,s,10);
		return s;
	case CCSVarType::_string:
		return (char*)m_ptr;
		//break;
	case CCSVarType::_double://都为64位
		gcvt(*(double*)m_ptr,16,s);//double有16位有效数据,加一个.
		for(i=15,n=0;i>=0;i--)
		{//下面代码的作用为删除多余的无意义的0
			if(s[i]=='0')
			{
				if(n++>=3)//从后向前有3个连续得零
				{
					s[i] = '\0';//将其从这里截断
				}
			}
			else
			{
				if(n>=3)
				{
					break;//代表已经删除过一次了
				}
				else
				{
					n=0;
				}
			}
		}
		return s;
	}

	return s;
}

const CCSVar &CCSVar::operator=( const CCSVar &v )
{
	static char CCT[][6]=
	{
		{0,1,2,1,1,3},//_void,
		{0,1,2,1,1,3},//_char,
		{0,1,2,1,1,3},//_double,
		{0,1,2,1,1,3},//_int,
		{0,1,2,1,1,3},//_ptr,
		{0,1,2,1,1,3},//_string,
	};
	if(CCT[m_type][v.m_type])
	{
		Realse();//释放空间
		switch(CCT[m_type][v.m_type])
		{
		case 1://直接复制类型
			m_ptr = v.m_ptr;
			break;
		case 2://to double
			m_ptr = (long) new double;
			*(double*)m_ptr = *(double*)v.m_ptr;
			break;
		case 3://to string
			if(v.m_ptr)
			{
				m_strlen = strlen((char*)v.m_ptr);
				m_ptr = (long) new char[m_strlen+1];
				memcpy((char*)m_ptr,(char*)v.m_ptr,m_strlen+1);
			}
			break;
		}

		m_type = v.m_type;
	}
	else
	{
		CCSVar *p;
		CCSException e(CCSException::_VarTypeConvert);
		e.AppendMsg("CScript.CSVar.opt( = )：");
		e.AppendMsg("不能转换 ");
		e.AppendMsg(v.GetTypeString());
		e.AppendMsg(" 为 ");
		e.AppendMsg(GetTypeString());
		e.Throw();
	}
	
	return *this;
}



CCSVar CCSVar::operator--( const int )
{
	switch(CCSVarType::m_type)
	{
	case CCSVarType::_char:
	case CCSVarType::_int:
	case CCSVarType::_ptr:
		return -- m_ptr;
	case CCSVarType::_double://都为64位
		return -- (*(double*)m_ptr);
		break;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(--a)：");
	e.AppendMsg(GetTypeString());
	e.AppendMsg("不支持此操作");
	e.Throw();
}

CCSVar CCSVar::operator--()
{
	switch(CCSVarType::m_type)
	{
	case CCSVarType::_char:
	case CCSVarType::_int:
	case CCSVarType::_ptr:
		return  m_ptr --;
		//break;
	case CCSVarType::_double://都为64位
		return  (*(double*)m_ptr) --;
		break;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(a--)：");
	e.AppendMsg(GetTypeString());
	e.AppendMsg("不支持此操作");
	e.Throw();
}

bool CCSVar::operator>( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};

	switch(CCT[m_type][v.m_type])
	{
	case 1://char int ptr
		return m_ptr > v.m_ptr;
	case 2:// char int > double
		return m_ptr > *(double*)v.m_ptr;
	case 3://double > char int
		return *(double*)m_ptr > v.m_ptr;
	case 4://double > double
		return *(double*)m_ptr > *(double*)v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(>)：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();
}

bool CCSVar::operator>=( const CCSVar &v ) const
{
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,0,0,0,0,0},//_string,
	};

	switch(CCT[m_type][v.m_type])
	{
	case 1://char int ptr
		return m_ptr >=v.m_ptr;
	case 2:// char int >=double
		return m_ptr >=*(double*)v.m_ptr;
	case 3://double >=char int
		return *(double*)m_ptr >=v.m_ptr;
	case 4://double >=double
		return *(double*)m_ptr >=*(double*)v.m_ptr;
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(>=)：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();
}

bool CCSVar::operator<( const CCSVar &v ) const
{
	return !(*this >= v);
}

bool CCSVar::operator<=( const CCSVar &v ) const
{
	return !(*this > v);
}

bool CCSVar::operator==( const CCSVar &v ) const
{
	char *s1,*s2;
	static char CCT[][6]=
	{
		{0,0,0,0,0,0},//_void,
		{0,1,2,1,1,0},//_char,
		{0,3,4,3,0,0},//_double,
		{0,1,2,1,1,0},//_int,
		{0,1,0,1,1,0},//_ptr,
		{0,0,0,0,0,5},//_string,
	};

	switch(CCT[m_type][v.m_type])
	{
	case 1://char int ptr
		return m_ptr == v.m_ptr;
	case 2:// char int == double
		return m_ptr == *(double*)v.m_ptr;
	case 3://double == char int
		return *(double*)m_ptr > v.m_ptr;
	case 4://double == double
		return *(double*)m_ptr > *(double*)v.m_ptr;
	case 5://string == stringr
		s1 = (char*)m_ptr;
		s2 = (char*)v.m_ptr;
		while(*s1++==*s2++&&*s1!='\0');
		return *s2=='\0';
	}

	CCSException e(CCSException::_VarTypeConvert);
	e.AppendMsg("CScript.CSVar.opt(==)：");
	e.AppendMsg("不能转换 ");
	e.AppendMsg(v.GetTypeString());
	e.AppendMsg(" 为 ");
	e.AppendMsg(GetTypeString());
	e.Throw();
}

bool CCSVar::operator!=( const CCSVar &v ) const
{
	return !(*this == v);
}

CCSVar & CCSVar::ToNumber()
{
	CCSVar::TYPE numType;
	int n=0;
	bool bFloat = false;
	char *s = (char*)m_ptr;
	char *sDel = s;
	float fBuf;
	if(m_type == CCSVarType::_string)
	{
		if(*s=='0'&&*(s+1)=='x')//16进制
		{
			s+=2;
			while(*s) ++s;//定位到尾部
			n=1;
			m_ptr = 0;
			while(--s >= sDel+2)//去掉0x
			{
				if(*s>='0'&&*s<='9')
				{
					m_ptr += ((*s) - 0x30)*n;
				}
				else if(*s>='A'&&*s<='F')		
				{
					m_ptr += ((*s) - 55)*n;
				}
				else if(*s>='a'&&*s<='f')		
				{
					m_ptr += ((*s) - 87)*n;
				}
				n*=16;
			}

			CCSVarType::SetType(CCSVarType::_int);
		}
		else
		{
			while(*s>='0'&&*s<='9')
			{
				if(*s=='.')
				{
					bFloat = true;
					s++;
				}

				n++;//记录位数
				s++;
			}

			s = (char*)m_ptr;

			if(bFloat)
			{
				m_ptr = (long) new double;
				*(double*)m_ptr = atof(s);
				CCSVarType::SetType(CCSVarType::_double);
			}
			else
			{
				CCSVarType::SetType(CCSVarType::_int);

				m_ptr = atol(s);
			}
		}

		delete []sDel;//删除原来的字符串
	}
	else
	{
		CCSException e(CCSException::_VarTypeConvert);
		e.AppendMsg("CScript.CSVar.ToNumber：");
		e.AppendMsg(GetTypeString());
		e.AppendMsg(" 不能转换为");
		e.Throw();
	}

	return *this;
}

CCSVar & CCSVar::ToString()
{
	char *s;
	int n;
	switch(m_type)
	{
	case CCSVarType::_char:
	case CCSVarType::_int:
	case CCSVarType::_double://都为64位
		s = StrVal();
		n = strlen(s);
		m_ptr = (long) new char[n+1];
		strcpy((char*)m_ptr,s);
		break;
	}
	m_type = CCSVarType::_string;

	return *this;
}

long CCSVar::NumVal() const
{
	return m_ptr;
}

void CCSVar::Realse()
{
	switch(m_type)
	{
	case CCSVarType::_double:
		if(!IsBadPtr()) delete (double*)m_ptr;
		return;
	case CCSVarType::_string:
		if(!IsBadPtr()) delete [] (char*)m_ptr;
		return;
	}
}

long CCSVar::GetPtr() const
{
	return m_ptr;
}

CCSVar * CCSVar::GetPtrVar()
{
	if(m_ptr == CCSVarType::_ptr)
	{
		if(IsBadPtr())//判断是否指向一个错误的内存地址
		{
			m_ptr = NULL;//指针置空
			return NULL;//指针错误
		}
		else
		{
			return (CCSVar*)m_ptr;
		}
	}
	else
	{
		return this;
	}
}

BOOL CCSVar::IsBadPtr() const
{
	return IsBadWritePtr((CCSVar*)m_ptr , sizeof(CCSVar));
}

CCSVar & CCSVar::operator[]( const int v )
{
	switch(m_type)
	{
	case CCSVarType::_ptr:
		if(!IsBadPtr())
		{
			return ((CCSVar*)m_ptr)[v];
		}
	case CCSVarType::_string:
		return * new CCSVar(((char*)m_ptr)[v]);//对于字符串，这种操作不太好的赶脚
	}

	CCSException e("CScript.Var.[]：该类型不支持此操作");
	e.Throw();
	THROW(e);

	return *this;
}

char & CCSVar::At( const int v )
{
	switch(m_type)
	{
	case CCSVarType::_string:
		return ((char*)m_ptr)[v];
	}
}

void CCSVar::SetPtrLevel( int nLevel )
{
	m_strlen = nLevel;
}

CCSVar CCSVar::operator&()
{
	return this;
}

CCSVar * CCSVar::GetMyAddr()
{
	return this;
}

unsigned int CCSVar::s_nGlobalID=0;
