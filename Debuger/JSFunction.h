#pragma once
#include "JSNameSpace.h"

class CJSFunction:private CJSNameSpace//�����������Ϊ�����ռ�
{
public:
	CString szFunction;//������
	CString szReturn;//����ֵ
	CListEx<CJSVar> arrParameters;//�βα�
	CString szBody;//������
	
public:
	CJSFunction(void);
	~CJSFunction(void);

	CJSFunction& operator=(const CJSFunction& jsfSource);
	void Invoke(char *para,...);//������"%s%n &s&nΪ��������
	CJSVar* AppendParameter();//׷���ββ�����ָ��
	void Parse();//��szBody����Ϊ�����ռ�ṹ
};

