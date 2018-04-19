#pragma once
#include "charcontrol.h"
class CInitCharControl :
	public CCharControl
{
public:
	enum ControlFlag
	{
		_Common,//普通模式
		_SearchFunction,//搜索函数
		_Function,//接收函数名
		_Parameter,//接收形参名
		_FunctionBody,//接收函数体
		_String,//接收函数体中字符串

		_Offset,//操作完成的控制符对应，控制符的偏移量
		//如_SearchFunction+CompleteControlFlagOffset 表示_SearchFunction结束
	};//Flag+20代表
public:
	CInitCharControl(void);
	~CInitCharControl(void);

	void EnSearchFunction(BOOL bEnter=TRUE);//进入函数搜索模式，或退出
	void EnSearchFunctionEnd();
	void EnFunction(BOOL bEnter=TRUE);
	void EnFunctionEnd();//设置函数名结束标志
	void EnParameter(BOOL bEnter=TRUE);
	void EnParameterEnd();
	void EnFunctionBody(BOOL bEnter=TRUE);
	void EnFunctionBodyEnd();
	void EnString(BOOL bEnter=TRUE);
	void EnStringEnd();
};

