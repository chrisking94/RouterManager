#pragma once
#include "CharControl.h"
#include "JSStatement.h"

class CFuncCharCtrl:public CCharControl
{
public:
	enum ControlFlag
	{
		_Common,//普通模式
		_SearchStatement,//搜索语句
		_String,
		_NameSpace,
		_RecieveStatData,
		_Offset,//操作完成的控制符对应，控制符的偏移量
		//如_SearchFunction+CompleteControlFlagOffset 表示_SearchFunction结束
	};//Flag+20代表
public:
	CFuncCharCtrl(void);
	~CFuncCharCtrl(void);

	void EnSearchStatement(BOOL bEnter=TRUE);
	void EnSearchStatementEnd();
	void EnReceiveStatData(CJSStatement::SType t,BOOL bEnter = TRUE);
	void EnString(BOOL bEnter=TRUE);
	void EnStringEnd();
	void EnNameSpace(BOOL bEnter=TRUE);
	void EnNameSpaceEnd();
};

