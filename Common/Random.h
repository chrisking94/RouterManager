#pragma once
class __declspec(dllexport) CRandom
{
public:
	CRandom(void);
	~CRandom(void);

	static void seed(UINT nSeed);
	static void seedByTime();//使用当前时间作为随机数种子
	static int getNum(int nFrom,int nTo);//获取从nFrom~nTo范围内的随机数
};

