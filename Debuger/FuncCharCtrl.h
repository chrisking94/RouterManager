#pragma once
#include "CharControl.h"
#include "JSStatement.h"

class CFuncCharCtrl:public CCharControl
{
public:
	enum ControlFlag
	{
		_Common,//��ͨģʽ
		_SearchStatement,//�������
		_String,
		_NameSpace,
		_RecieveStatData,
		_Offset,//������ɵĿ��Ʒ���Ӧ�����Ʒ���ƫ����
		//��_SearchFunction+CompleteControlFlagOffset ��ʾ_SearchFunction����
	};//Flag+20����
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

