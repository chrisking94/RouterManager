#pragma once
#include "CSVarType.h"

class CCSVarType;

class CCSVar:public CCSVarType
{
public:
	typedef char* ptrSZ;//字符串指针
	typedef unsigned int VAR_ID;
public:
	CCSVar();
	~CCSVar(void);
	CCSVar(const CCSVar &v);//拷贝构造
	const CCSVar &operator=(const CCSVar &v);//如果不重载=，则 在使用=时会调用默认拷贝构造函数，导致问题
	
	//从类型创建
	CCSVar(const CCSVarType::TYPE t);
	//基本变量类型
	CCSVar(const char v);
	CCSVar(const double v);
	CCSVar(const float v);
	CCSVar(const int v);
	CCSVar(const long v);
	CCSVar(const CCSVar *v);//指针型变量
	CCSVar(const short v);
	CCSVar(const char * v);//深拷贝创建
	

	//运算符
	CCSVar operator+(const CCSVar &v) const;
	CCSVar operator-(const CCSVar &v) const;
	CCSVar operator*(const CCSVar &v) const;
	CCSVar operator/(const CCSVar &v) const;
	CCSVar operator%(const CCSVar &v) const;
	CCSVar operator^(const CCSVar &v) const;
	//CCSVar operator&(const CCSVar &v) const;
	//CCSVar operator|(const CCSVar &v) const;

	//自边运算符
	CCSVar operator+=(const CCSVar &v);
	CCSVar operator-=(const CCSVar &v);
	CCSVar operator*=(const CCSVar &v);
	CCSVar operator/=(const CCSVar &v);
	CCSVar operator%=(const CCSVar &v);
	CCSVar operator^=(const CCSVar &v);
	//CCSVar operator&=(const CCSVar &v);
	//CCSVar operator|=(const CCSVar &v);
	CCSVar operator++(const int);
	CCSVar operator++();//这个才是前置++
	CCSVar operator--(const int);
	CCSVar operator--();

	//比较运算符
	bool operator>(const CCSVar &v) const;
	bool operator>=(const CCSVar &v) const;
	bool operator<(const CCSVar &v) const;
	bool operator<=(const CCSVar &v) const;
	bool operator==(const CCSVar &v) const;
	bool operator!=(const CCSVar &v) const;

	CCSVar &operator[](const int v);//字符串：取char；类似访问a[i]
	CCSVar &operator*();//解析
	CCSVar operator&();//取地址，返回ptr类型var

	//外部接口函数
	char *StrVal() const;
	long NumVal() const;

	CCSVar &ToNumber();//支持16进制
	CCSVar &ToString();//转换为String

	bool IsMe(char *szName) const;//与自己的m_szName比较
	void SetName(char *name);//设置名称
	char *GetName() const;//读取名称
	long GetPtr() const;//返回m_ptr

	char &At(const int v);//字符串的操作，因为[]重载给指针了，所以字符串采用这种方式

	CCSVar *GetPtrVar();//指针型(_ptr)变量用，用于访问到指针最终指向的变量
	BOOL IsBadPtr() const; //_ptr，非_ptr则返回本对象this
	void SetPtrLevel(int nLevel);//指针层数
	CCSVar *GetMyAddr();//返回本变量的地址
private:
	void Realse();//释放存储变量申请的内存空间
protected:
	long m_ptr;//数据所在的内存位置指针
private:
	VAR_ID m_nID;//全局唯一ID
	static VAR_ID s_nGlobalID;
	int m_strlen;//只对字符串类型变量
	char* m_szName;

	//内部函数使用
	char *buff;
};

