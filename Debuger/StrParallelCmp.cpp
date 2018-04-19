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
	//下面的跳表算法不够可靠
	pchMatchStart = *m_ps;
	bool *matchList = new bool[m_lstLen];//用于标记已经匹配失败的条目
	memset(matchList,1,m_lstLen);
	int j=0 , nMatchedI = -1,k;

	while(**m_ps)
	{
		if(m_pchEnd)
		{
			for(k=0;m_pchEnd[k]&&m_pchEnd[k]!=**m_ps;k++);
			if(m_pchEnd[k]!='\0')//匹配到结束符
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
					matchList[i] = false;//标记为匹配失败
					if(nMatchedI == i) nMatchedI = -1;//去掉其作为匹配项权力
				}
			}
			
		}

		m_sCmped[j++] = *(*m_ps)++;//存储匹配失败的字符
	}
	m_sCmped[j] = '\0';

	if(nMatchedI!=-1 && m_lst[nMatchedI][j] != '\0') nMatchedI = -1;//没有完全匹配

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
			if(m_pchEnd[k]!='\0')//匹配到结束符
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
				if(nFirstMatchFailed!=-1)//将第一个匹配失败的位置指向这里，直接跳过来
				{
					jumpList[nFirstMatchFailed] = i - nFirstMatchFailed;
					nFirstMatchFailed=-1;//重置
				}
				else
				{
					if(nFirstMatchFailed==-1) nFirstMatchFailed=i;
				}
			}
			else
			{
				if(nMatchedI == i) nMatchedI = -1;//如果当前记录的匹配项是这个，则失败后将其置为-1
			}
		}
		if(nFirstMatchFailed!=-1)
		{
			jumpList[nFirstMatchFailed] = m_lstLen - nFirstMatchFailed-1;//直接跳到尾部
			nFirstMatchFailed=-1;//重置
		}

		m_sCmped[j++] = *(*m_ps)++;//存储匹配失败的字符
	}
	m_sCmped[j] = '\0';

	delete [] jumpList;
	return nMatchedI;
	///*
	int *bMatched = new int[m_lstLen];
	memset(bMatched,1,m_lstLen*sizeof(int));//表示每个字符串是否还可以继续进行匹配
	int j=0,matchedIndex=0,k;
	pchMatchStart = *m_ps;

	while(**m_ps)//匹配直到字符串尾部，或有一个串已经匹配完成
	{
		if(m_pchEnd!=NULL)//检查是否匹配到结束符
		{
			//TRACE("%c-----",**m_ps);
			k=0;//检查是否匹配到结束符
			while(m_pchEnd[k]&&m_pchEnd[k++]!=*(*m_ps));
			if(m_pchEnd[k]!='\0') break;//匹配到结束符
		}
		
		for(int i=0;i<m_lstLen;i++)
		{
			if(bMatched[i])//0表示已经匹配失败
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
		(*m_ps)++;//移动指针

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
