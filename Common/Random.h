#pragma once
class __declspec(dllexport) CRandom
{
public:
	CRandom(void);
	~CRandom(void);

	static void seed(UINT nSeed);
	static void seedByTime();//ʹ�õ�ǰʱ����Ϊ���������
	static int getNum(int nFrom,int nTo);//��ȡ��nFrom~nTo��Χ�ڵ������
};

