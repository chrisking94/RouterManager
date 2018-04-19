#include "StdAfx.h"
#include "StringEx.h"


CStringEx::CStringEx(void):
	CString(_T(""))
	,m_szLower(_T(""))
{
}

CStringEx::CStringEx( LPCTSTR lpszStr ):
	CString(lpszStr)
	,m_szLower(CString(lpszStr).MakeLower())//������ܳ������⣬���CString��Copy��ǳ�����Ļ�
{
	
}

CStringEx::CStringEx( CString szStr ):
	CString(szStr)
	,m_szLower(szStr.MakeLower())
{
	
}


CStringEx::~CStringEx(void)
{
}

CString CStringEx::glors(CString szToken,LeftOrRight lorPosition, BOOL bIgnoreCase /*= TRUE*/)
{
	if(lorPosition==LEFT)
	{
		return mid(_T(""),szToken);
	}
	else
	{
		return mid(szToken,_T(""));
	}
}

UINT CStringEx::split( CString szToken,CStringEx::CStringArray &szArrayReturned , BOOL bIgnoreCase )
{
	CString szSbuf;

	UINT nTokenMatchX=0,nReturnLength=1,nTmpX;//xΪ��άret��x,fitxΪ��ǰƥ��ı�־��x����
	CString szSource=CString::GetString();
	CString szCompare;

	if(bIgnoreCase)
	{
		szCompare=m_szLower;
		szToken.MakeLower();
	}
	else
	{
		szCompare=szSource;
	}

	for(UINT i=0;i<CString::GetLength();i++){
		nTmpX=i;
		nTokenMatchX=0;
		while(szToken[nTokenMatchX]==szCompare[nTmpX]&&szToken[nTokenMatchX]!='\0')
		{
			nTokenMatchX++;
			nTmpX++;
			if('\0'==szToken[nTokenMatchX])
			{
				i=nTmpX;
				szArrayReturned.Append(szSbuf);
				szSbuf=_T("");
				nReturnLength++;
				if(i==CString::GetLength()) break;//�ַ���ĩβ�зָ�����˳�ѭ��
			}
		}
		szSbuf.AppendChar(szSource[i]);
	}
	
	szArrayReturned.Append(szSbuf);

	return nReturnLength;
}

UINT CStringEx::split( CString szToken,CString* &szArrayReturned, BOOL bIgnoreCase /*= TRUE*/ )
{
	CStringArray szArr;
	UINT nRet = split(szToken,szArr,bIgnoreCase);
	szArr.toDataArray(szArrayReturned);

	return nRet;
}

UINT CStringEx::split( CString szToken,INT* &nArrayReturned, BOOL bIgnoreCase /*= TRUE*/ )
{
	CArrayEx<INT> nArr;
	UINT nRet = split(szToken,nArr,bIgnoreCase);

	nArr.toDataArray(nArrayReturned);

	return nRet;
}

UINT CStringEx::split( CString szToken,CArrayEx<INT> &nArrayReturned, BOOL bIgnoreCase /*= TRUE*/ )
{
	CStringArray szArr;
	UINT nRet = split(szToken,szArr,bIgnoreCase);

	nArrayReturned.Clear();

	for(int i=0;i<szArr.GetLength();i++)
	{
		nArrayReturned.Append(_ttoi(szArr[i]));
	}

	return nRet;
}

CString CStringEx::mid( CString strLeftToken,CString strRightToken,UINT nStartIndex/*=0*/, BOOL bIgnoreCase /*= TRUE*/ )
{
	CString strRet;
	mid(strLeftToken,strRightToken,strRet,nStartIndex);
	return strRet;
}

UINT CStringEx::mid( CString strLeftToken,CString strRightToken,CString& strRet,UINT nStartIndex/*=0*/ , BOOL bIgnoreCase )
{
	strRet=_T("");
	BOOL bStarted=FALSE;
	UINT i=0,j=0,k=0;
	if(strLeftToken[0]==0) bStarted=TRUE;
	if(nStartIndex<CString::GetLength())
	{
		i=nStartIndex;
	}
	else
	{
		return 0;
	}

	CString szSource=CString::GetString();
	CString szCompare;
	
	if(bIgnoreCase)
	{
		/*�ջ�
		��CString����ת��ΪLPCTSTRʱ��
		CString���������ݵ�ַ��ֵ��LPCTSTR��
		���Ե�CString��������ʱ����ֵ��LPCTSTR������ָ���ڴ�����Ҳ���ͷţ�
		��ʱLPCTSTR����ָ��Ĳ�����ת���������ַ����׵�ַ������һ����CString�ͷŵ�����
		AfxMessageBox(LPCTSTR������ʾ�ľ���һ�������ַ�
		*/
		szCompare=m_szLower;
		strLeftToken.MakeLower();
		strRightToken.MakeLower();
	}
	else
	{
		szCompare=szSource;
	}

	
	while(szCompare[i])
	{
		if(!bStarted)
		{
			j=0;
			k=i;
			while(szCompare[k]==strLeftToken[j])
			{
				if(szCompare[k]==0) return szCompare.GetLength();
				j++;
				k++;
			}

			if(strLeftToken[j]==0)
			{
				i+=j;
				if(strRightToken[0]==0)
				{
					j=i;
					while(szCompare[i])
					{
						strRet.AppendChar(szSource[i++]);
					}
					return i-j;
				}
				i--;
				bStarted=TRUE;
			}
		}
		else
		{
			j=0;
			k=i;
			while(szCompare[k]==strRightToken[j])
			{
				if(szCompare[k]==0) break;
				j++;
				k++;
			}
			if(strRightToken[j]==0)
			{
				if(strRightToken[0]==0)
				{
					j=i;
					while(szCompare[i])
					{
						strRet.AppendChar(szSource[i++]);
					}
					return i-j;
				}
				return k;
			}
			strRet.AppendChar(szSource[i]);
		}
		i++;
	}
	return i;
}

void CStringEx::RemoveChineseChars( CString &szRet )
{
	szRet=_T("");
	TCHAR tch;
	for(int i=0;i<GetLength();i++)
	{
		tch = CString::GetAt(i);
		if(!(tch>255))
		{
			szRet.AppendChar(tch);
		}
	}
}
