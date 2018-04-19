#include "StdAfx.h"
#include "StrParallelCmp.h"


CStrParallelCmp::CStrParallelCmp( char *&s )
{
	m_ps=&s;
	m_pchEnd = NULL;
	m_lstLen = 0;
}


CStrParallelCmp::~CStrParallelCmp(void)
{
}

void CStrParallelCmp::SetList( char **lst ,int nListLen)
{
	m_lst=lst;
	m_lstLen = nListLen;
}

int CStrParallelCmp::Compare( char *&pchMatchStart )
{
	//����������㷨�����ɿ�
	pchMatchStart = *m_ps;
	bool *matchList = new bool[m_lstLen];//���ڱ���Ѿ�ƥ��ʧ�ܵ���Ŀ
	memset(matchList,1,m_lstLen);
	int j=0 , nMatchedI = -1,k;

	while(**m_ps)
	{
		if(m_pchEnd)
		{
			for(k=0;m_pchEnd[k]&&m_pchEnd[k]!=**m_ps;k++);
			if(m_pchEnd[k]!='\0')//ƥ�䵽������
			{
				break;
			}
		}
		for(int i=0;i<m_lstLen;i++)
		{
			if(matchList[i])
			{
				if(m_lst[i][j]==**m_ps)
				{
					nMatchedI = i;
				}
				else
				{
					matchList[i] = false;//���Ϊƥ��ʧ��
					if(nMatchedI == i) nMatchedI = -1;//ȥ������Ϊƥ����Ȩ��
				}
			}
			
		}

		m_sCmped[j++] = *(*m_ps)++;//�洢ƥ��ʧ�ܵ��ַ�
	}
	m_sCmped[j] = '\0';

	if(nMatchedI!=-1 && m_lst[nMatchedI][j] != '\0') nMatchedI = -1;//û����ȫƥ��

	return nMatchedI;
}
/*
int CStrParallelCmp::Compare( char *&pchMatchStart )
{
	pchMatchStart = *m_ps;
	int *jumpList = new int[m_lstLen];
	memset(jumpList,0,sizeof(int)*m_lstLen);
	int nFirstMatchFailed = -1 ,j=0 , nMatchedI = -1,k;
	BOOL bMatchComplete = FALSE;

	while(**m_ps)
	{
		if(m_pchEnd)
		{
			for(k=0;m_pchEnd[k]&&m_pchEnd[k]!=**m_ps;k++);
			if(m_pchEnd[k]!='\0')//ƥ�䵽������
			{
				bMatchComplete = TRUE;
				break;
			}
		}
		for(int i=0;i<m_lstLen;i++)
		{
			if(jumpList[i]!=0)
			{
				nFirstMatchFailed=i;
				while(i<m_lstLen&&jumpList[i]!=0)
				{
					i+=jumpList[i];
				}
			}
			if(m_lst[i][j]==**m_ps)
			{
				nMatchedI = i;
				if(nFirstMatchFailed!=-1)//����һ��ƥ��ʧ�ܵ�λ��ָ�����ֱ��������
				{
					jumpList[nFirstMatchFailed] = i - nFirstMatchFailed;
					nFirstMatchFailed=-1;//����
				}
				else
				{
					if(nFirstMatchFailed==-1) nFirstMatchFailed=i;
				}
			}
			else
			{
				if(nMatchedI == i) nMatchedI = -1;//�����ǰ��¼��ƥ�������������ʧ�ܺ�����Ϊ-1
			}
		}
		if(nFirstMatchFailed!=-1)
		{
			jumpList[nFirstMatchFailed] = m_lstLen - nFirstMatchFailed-1;//ֱ������β��
			nFirstMatchFailed=-1;//����
		}

		m_sCmped[j++] = *(*m_ps)++;//�洢ƥ��ʧ�ܵ��ַ�
	}
	m_sCmped[j] = '\0';

	delete [] jumpList;
	return nMatchedI;
	///*
	int *bMatched = new int[m_lstLen];
	memset(bMatched,1,m_lstLen*sizeof(int));//��ʾÿ���ַ����Ƿ񻹿��Լ�������ƥ��
	int j=0,matchedIndex=0,k;
	pchMatchStart = *m_ps;

	while(**m_ps)//ƥ��ֱ���ַ���β��������һ�����Ѿ�ƥ�����
	{
		if(m_pchEnd!=NULL)//����Ƿ�ƥ�䵽������
		{
			//TRACE("%c-----",**m_ps);
			k=0;//����Ƿ�ƥ�䵽������
			while(m_pchEnd[k]&&m_pchEnd[k++]!=*(*m_ps));
			if(m_pchEnd[k]!='\0') break;//ƥ�䵽������
		}
		
		for(int i=0;i<m_lstLen;i++)
		{
			if(bMatched[i])//0��ʾ�Ѿ�ƥ��ʧ��
			{
				if(m_lst[i][j]!=**m_ps)
				{
					bMatched[i]=0;
				}
				else
				{
					//TRACE("%c",m_lst[i][j]);
					matchedIndex = i;
				}
			}
		}

		//TRACE("%c+++++",**m_ps);
		(*m_ps)++;//�ƶ�ָ��

		j++;
	}
	//TRACE("------------\r\n");
	if(bMatched[matchedIndex]&&m_lst[matchedIndex][j]=='\0')
	{
		delete bMatched;
		return matchedIndex;
	}
	else
	{
		return -1;
	}
	///
}
*/

void CStrParallelCmp::SetEndWith( char *endWithChar )
{
	m_pchEnd = endWithChar;
}

char * CStrParallelCmp::GetComparedString() const
{
	return (char*)m_sCmped;
}
