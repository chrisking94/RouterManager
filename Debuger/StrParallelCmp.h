#pragma once
//�ַ������бȽ���
class CStrParallelCmp
{
public:
	~CStrParallelCmp(void);
	//CStrParallelCmp(char *s);//������������⣬����������ʵ�ζ���һ����
	CStrParallelCmp(char *&s);//����Ҫ�Ƚϵ��ַ�����ָ�룬�ȽϽ����ƶ���ָ��

	void SetList(char **lst,int nListLen);//���ò��бȽ��б�
	void SetEndWith(char *endWithChar);//�ȽϽ�����־
	int Compare(char *&pchMatchStart);//��ʼ���бȽϣ�����endWith��β�����endWithΪ0������ƥ�䵽һ�����б��е��ַ���֮��
	char *GetComparedString() const;
	//���ַ���β��ʱ����
	//ƥ��ʧ���򷵻�-1
	//pchMathStart��End����֮��Ϊһ���ַ���

private:
	char **m_ps;
	char **m_lst;
	int m_lstLen;
	char *m_pchEnd;
	char m_sCmped[1024];
};

