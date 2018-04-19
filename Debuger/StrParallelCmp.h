#pragma once
//字符串并行比较类
class CStrParallelCmp
{
public:
	~CStrParallelCmp(void);
	//CStrParallelCmp(char *s);//这里可能有问题，这两个函数实参都是一样的
	CStrParallelCmp(char *&s);//设置要比较的字符串的指针，比较将会移动该指针

	void SetList(char **lst,int nListLen);//设置并行比较列表
	void SetEndWith(char *endWithChar);//比较结束标志
	int Compare(char *&pchMatchStart);//开始并行比较，并以endWith结尾，如果endWith为0，则在匹配到一个在列表中的字符串之后
	char *GetComparedString() const;
	//或到字符串尾部时结束
	//匹配失败则返回-1
	//pchMathStart和End两者之间为一个字符串

private:
	char **m_ps;
	char **m_lst;
	int m_lstLen;
	char *m_pchEnd;
	char m_sCmped[1024];
};

