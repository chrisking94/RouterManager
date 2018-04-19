#include "StdAfx.h"
#include "HttpWebConnection.h"

CHttpWebConnection::CHttpWebConnection(void)
{
}

CHttpWebConnection::CHttpWebConnection( CString URL ,Method method):
m_szURL(URL)
	,m_session(_T("session"),0,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD)     //设置不缓冲
	,m_bClosed(false)
	,m_pHttpConnection(NULL)
	,m_pHttpFile(NULL)
	,m_Method(method)
{
	
}

CHttpWebConnection::~CHttpWebConnection( void )
{
	if(m_bClosed) return;
	close();
}

BOOL CHttpWebConnection::sendRequest( )//使用其后的请求函数必须保证sendRequest成功
{
	CString strServer, strObject;
	INTERNET_PORT wPort;
	DWORD dwType;
	BOOL bReturn=FALSE;

	CString strTmp=_T("");
	if(m_Method==GET)
	{
		strTmp=m_strForm;
	}
	if(AfxParseURL(m_szURL+strTmp, dwType, strServer, strObject, wPort))
	{
		m_pHttpConnection = m_session.GetHttpConnection(strServer, wPort);
		m_pHttpFile = m_pHttpConnection->OpenRequest(m_Method, strObject,NULL,1UL,NULL,NULL,INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD);
		
		try
		{
			if(m_Method==POST)
			{
				m_pHttpFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
				m_pHttpFile->AddRequestHeaders(m_szHeaders);
				
				USES_CONVERSION;
				BYTE *pBuf = (BYTE *)T2A(m_strForm);
				UINT len=strlen((const char*)pBuf);

				bReturn = m_pHttpFile->SendRequestEx(len);
				
				m_pHttpFile->Write(pBuf,len);
				//m_pHttpFile->SendRequest(m_szRequestBody.GetBuffer(),m_szRequestBody.GetLength());
				m_pHttpFile->EndRequest();
			}
			else
			{
				//m_pHttpFile->WriteString(m_szRequestBody);

				bReturn = m_pHttpFile->SendRequest(m_szHeaders);
			}
		}
		catch (CException* e)
		{
			e->Delete();
			bReturn=FALSE;
		}
	}
	
	return bReturn;
}

CString CHttpWebConnection::readResponseString( void )
{
	CString sRet=_T("");
	readResponseString(sRet);
	//TRACE(sRet);
	return sRet;
}


bool CHttpWebConnection::readResponseString( CString& szReturned )
{
	szReturned=_T("");
	if(getConnectionStatusCode() == HTTP_STATUS_OK)
	{
		char pszBuffer[1024];
		UINT nRead;

		while(nRead=m_pHttpFile->Read(pszBuffer, 1024))//注意：假设中途出现nRead<1024的情况则可能出现错误
		{
			szReturned.Append(CString((pszBuffer),nRead));
		}
		m_pHttpFile->Close();
		delete m_pHttpFile;
		m_pHttpFile=NULL;
		return true;
		//m_pHttpFile->EndRequest();
		/*
		警告：这种读取方式会导致多使用一次m_pHttpFile->Read（），进而导致http堵塞而发送不了下一次request
		..被这个错误困扰了很长一段时间，终于解决了。2015-04-15 20:26
		do
		{
			nRead = m_pHttpFile->Read(pszBuffer, 1024);
			sRet+=CString(pszBuffer);
		}while (nRead > 0);
		delete []pszBuffer;
		*/
	}
	return false;
}


void CHttpWebConnection::addRequestHeaderString( CString header )
{
	m_szHeaders+=header+_T("\r\n");
}

DWORD CHttpWebConnection::getConnectionStatusCode( void )
{
	DWORD dwRet;

	m_pHttpFile->QueryInfoStatusCode(dwRet);

	return dwRet;
}

void CHttpWebConnection::setCookie( LPCTSTR pstrCookieName,LPCTSTR pstrCookieData )
{
	m_session.SetCookie(m_szURL,pstrCookieName,pstrCookieData);
}

void CHttpWebConnection::close( void )
{
		m_session.Close();
		if(m_pHttpFile != NULL)
		{
			if((int)m_pHttpFile==-842150451)//错误的指针
			{
				TRACE("------------- m_pHttpFile ---- Error! void CHttpWebConnection::close( void ) ---");
				m_bClosed=true;
				return;
			}
			m_pHttpFile->Close();
			delete m_pHttpFile;
			m_pHttpFile = 0;
		}
		if(m_pHttpConnection != NULL)
		{
			m_pHttpConnection->Close();
			delete m_pHttpConnection;
			m_pHttpConnection = 0;
		}
		
		m_bClosed=true;
}

CHtmlDocument CHttpWebConnection::readResponseDocument()
{
	return CHtmlDocument(readResponseString());
}

bool CHttpWebConnection::readResponseDocument( CHtmlDocument &hd )
{
	CString szTmp;
	bool bRet=readResponseString(szTmp);
	hd=CHtmlDocument(szTmp);
	return bRet;
}

UINT CHttpWebConnection::readBytes( BYTE* lpBuf,UINT nBufLen )
{
	return m_pHttpFile->Read(lpBuf, nBufLen);
}

void CHttpWebConnection::writeForm( CString sExpression )
{
	if(sExpression==_T("")) return;
	if(m_strForm==_T(""))
	{
		if(m_Method==GET)
		{
			m_strForm+=_T("?")+sExpression;
		}
		else
		{
			m_strForm+=sExpression;
		}
	}
	else
	{
		m_strForm+=_T("&")+sExpression;
	}
}

void CHttpWebConnection::writeForm( CString sVarName,CString sValue )
{
	writeForm(sVarName+_T("=")+sValue);
}

void CHttpWebConnection::getResponseHeaders( HeadersDef &headers )
{
	CString szHeader ;
	DWORD dwStat=0;
	m_pHttpFile-> QueryInfo( HTTP_QUERY_LOCATION,szHeader) ;
	
	
	TRACE("--------------%d\n------------",dwStat);
	m_pHttpFile-> QueryInfo( HTTP_QUERY_RAW_HEADERS_CRLF,szHeader) ;
	TRACE("--------------%S\n------------",szHeader);
	
}

void CHttpWebConnection::writeRequestBody( CString szValue )
{
	m_szRequestBody = szValue;
}

