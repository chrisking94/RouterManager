#include "StdAfx.h"
#include "CodeString.h"
#include <afxpriv.h>
#include "MD5.h"


CCodeString::CCodeString(void):
m_nSourceLength(0)
{
}

CCodeString::CCodeString( LPCTSTR lpszSourceString ):
m_szSourceString(lpszSourceString)
,m_nSourceLength(m_szSourceString.GetLength())
{
	
}


CCodeString::~CCodeString(void)
{
}

CString CCodeString::B64Encode( void )
{
	const BYTE Base64ValTab[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	if(0==m_nSourceLength)
	{
		return _T("");
	}

	int i,sourceLen;
	char tmp;
	CString dst;
	sourceLen=m_nSourceLength;

	for(i=0;i<=sourceLen-3;i+=3)
	{
		tmp=m_szSourceString[i]>>2;
		dst.AppendChar(Base64ValTab[tmp]);
		tmp=((m_szSourceString[i]&0x03)<<4)+(m_szSourceString[i+1]>>4);
		dst.AppendChar(Base64ValTab[tmp]);
		tmp=((m_szSourceString[i+1]&0x0f)<<2)+(m_szSourceString[i+2]>>6);
		dst.AppendChar(Base64ValTab[tmp]);
		tmp=m_szSourceString[i+2]&0x3f;
		dst.AppendChar(Base64ValTab[tmp]);
	}

	sourceLen%=3;

	if(sourceLen>0)
	{
		tmp=m_szSourceString[i]>>2;
		dst.AppendChar(Base64ValTab[tmp]);
		tmp=(m_szSourceString[i]&0x03)<<4;
		if(sourceLen==1)
		{
			dst.AppendChar(Base64ValTab[tmp]);
			dst.AppendChar('=');
		}
		if(sourceLen==2)
		{
			tmp+=m_szSourceString[i+1]>>4;
			dst.AppendChar(Base64ValTab[tmp]);
			tmp=(m_szSourceString[i+1]&0x0f)<<2;
			dst.AppendChar(Base64ValTab[tmp]);
		}
		dst.AppendChar('=');
	}

	return dst;
}

CString CCodeString::B64Decode( void )
{
	const BYTE Base64IdxTab[128] = 
	{
		255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
		255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
		255,255,255,255,  255,255,255,255,  255,255,255,62,   255,255,255,63,
		52,53,54,55,      56,57,58,59,      60,61,255,255,    255,255,255,255,
		255,0,1,2,        3,4,5,6,          7,8,9,10,         11,12,13,14,
		15,16,17,18,      19,20,21,22,      23,24,25,255,     255,255,255,255,
		255,26,27,28,     29,30,31,32,      33,34,35,36,      37,38,39,40,
		41,42,43,44,      45,46,47,48,      49,50,51,255,     255,255,255,255
	};

	if(0==m_nSourceLength)
	{
		return _T("");
	}

	int sourceLen,i,j=0;
	char tmp,tmp1,tmp2,tmp3;
	CString dst;

	sourceLen=m_nSourceLength;

	while(m_szSourceString[sourceLen-1]=='=')
	{
		j++;
		sourceLen--;
	}

	for(i=0;i<=sourceLen-4;i+=4)
	{
		tmp=Base64IdxTab[m_szSourceString[i]];
		tmp1=Base64IdxTab[m_szSourceString[i+1]];
		tmp2=Base64IdxTab[m_szSourceString[i+2]];
		tmp3=Base64IdxTab[m_szSourceString[i+3]];

		tmp=(tmp<<2)+(tmp1>>4);
		dst.AppendChar(tmp);
		tmp=(tmp1<<4)+(tmp2>>2);
		dst.AppendChar(tmp);
		tmp=(tmp2<<6)+tmp3;
		dst.AppendChar(tmp);
	}

	if(j>0)
	{
		tmp=Base64IdxTab[m_szSourceString[i]];
		tmp1=Base64IdxTab[m_szSourceString[i+1]];

		tmp=(tmp<<2)+(tmp1>>4);
		dst.AppendChar(tmp);
		if(j==1)
		{
			tmp2=Base64IdxTab[m_szSourceString[i+2]];

			tmp=(tmp1<<4)+(tmp2>>2);
			dst.AppendChar(tmp);
		}
	}

	return dst;
}

CString CCodeString::KEncrypt( void )
{
	UINT i;
	int j=0,k=1;
	CString dst;

	for(i=0;i<m_nSourceLength;i++)
	{
		if(j<k)
		{
			j++;
		}
		else
		{
			j=1;
			k++;
		}
		dst.AppendChar(m_szSourceString[i]+(j+k)*(j%2==0?-1:1));
	}

	dst=CCodeString(dst).B64Encode();
	dst.Replace('=','.');

	return dst;
}

CString CCodeString::KDecrypt( void )
{
	UINT i,sLen;
	int j=0,k=1;
	CString dst;

	dst=m_szSourceString;
	dst.Replace('.','=');
	dst=CCodeString(dst).B64Decode();

	sLen=dst.GetLength();

	for(i=0;i<sLen;i++)
	{
		if(j<k)
		{
			j++;
		}
		else
		{
			j=1;
			k++;
		}
		dst.SetAt(i,dst[i]+(j+k)*(j%2==0?1:-1));
	}

	return dst;
}

CString CCodeString::Md5Encrypt( void )
{
	MD5 md5;                 //定义MD5的类
	md5.reset();
	USES_CONVERSION;
	md5.update(T2A(m_szSourceString.GetBuffer(0)));//T2A:wchar*转char*
	m_szSourceString.ReleaseBuffer();
	return (CString)md5.toString().c_str();     //toString()函数获得加密字符串，c_str();函数重新转换成CString类型
}

CString CCodeString::ToHexString( ULONG num )
{
	const char hexTable[]="0123456789abcdef";//由于最后少了一个f在这里浪费了很多时间，汲取教训

	CString dst=_T("");
	int dstLen=0,dstHalfLen;
	wchar_t tmp;

	while(num>0)
	{
		dst.AppendChar(hexTable[num%16]);
		dstLen++;
		num>>=4;
	}

	dstHalfLen=dstLen/2;
	
	for(int i=0;i<dstHalfLen;i++)
	{
		tmp=dst[i];
		dst.SetAt(i,dst[dstLen-i-1]);
		dst.SetAt(dstLen-i-1,tmp);
	}
	
	return dst;
}

CString CCodeString::ConvertUnits( ULONG nNumber )
{
	const char chUnitsTable[]={'B','K','M','G','T'};

	CString szReturn;
	byte i=0;
	double nTmp=nNumber;

	while(nTmp>1023.0)
	{
		nTmp/=1024;
		i++;
	}

	szReturn.Format(_T("%.1lf"),nTmp);
	szReturn.AppendChar(chUnitsTable[i]);

	return szReturn;
}
