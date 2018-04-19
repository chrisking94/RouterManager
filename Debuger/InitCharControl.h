#pragma once
#include "charcontrol.h"
class CInitCharControl :
	public CCharControl
{
public:
	enum ControlFlag
	{
		_Common,//��ͨģʽ
		_SearchFunction,//��������
		_Function,//���պ�����
		_Parameter,//�����β���
		_FunctionBody,//���պ�����
		_String,//���պ��������ַ���

		_Offset,//������ɵĿ��Ʒ���Ӧ�����Ʒ���ƫ����
		//��_SearchFunction+CompleteControlFlagOffset ��ʾ_SearchFunction����
	};//Flag+20����
public:
	CInitCharControl(void);
	~CInitCharControl(void);

	void EnSearchFunction(BOOL bEnter=TRUE);//���뺯������ģʽ�����˳�
	void EnSearchFunctionEnd();
	void EnFunction(BOOL bEnter=TRUE);
	void EnFunctionEnd();//���ú�����������־
	void EnParameter(BOOL bEnter=TRUE);
	void EnParameterEnd();
	void EnFunctionBody(BOOL bEnter=TRUE);
	void EnFunctionBodyEnd();
	void EnString(BOOL bEnter=TRUE);
	void EnStringEnd();
};

