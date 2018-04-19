#pragma once
#include "JSNameSpace.h"

class CJSFunction:private CJSNameSpace//将函数体抽象为命名空间
{
public:
	CString szFunction;//函数名
	CString szReturn;//返回值
	CListEx<CJSVar> arrParameters;//形参表
	CString szBody;//函数体
	
public:
	CJSFunction(void);
	~CJSFunction(void);

	CJSFunction& operator=(const CJSFunction& jsfSource);
	void Invoke(char *para,...);//参数表"%s%n &s&n为引用类型
	CJSVar* AppendParameter();//追加形参并返回指针
	void Parse();//将szBody解析为命名空间结构
};

