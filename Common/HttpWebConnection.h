#pragma once
#include <afxinet.h>
#include "HtmlDocument.h"

class __declspec(dllexport) CHttpWebConnection
{
public:
	typedef enum Method
	{
		POST=CHttpConnection::HTTP_VERB_POST,
		GET=CHttpConnection::HTTP_VERB_GET
	}Method;

	typedef struct HeadersDef
	{
		CString szLocation;
	};
public:
	CHttpWebConnection(void);
	CHttpWebConnection(CString URL ,Method method=GET);
	~CHttpWebConnection(void);
private:
	CString m_szURL;
	CString m_szHeaders;
	CInternetSession m_session;
	CHttpConnection* m_pHttpConnection;
	CHttpFile *m_pHttpFile;
	bool m_bClosed;
	Method m_Method;
	CString m_strForm;
	CString m_szRequestBody;
public:
	void addRequestHeaderString(CString header);
	void writeForm(CString sVarName,CString sValue);
	void writeForm(CString sExpression);
	void writeRequestBody(CString szValue);
	BOOL sendRequest();
	DWORD getConnectionStatusCode(void);
	CString readResponseString(void);
	CHtmlDocument readResponseDocument();
	bool readResponseDocument(CHtmlDocument &hd);
	bool readResponseString(CString& szReturned);
	void setCookie(LPCTSTR pstrCookieName,LPCTSTR pstrCookieData);
	void close(void);
	UINT readBytes(BYTE* lpBuf,UINT nBufLen);
	void getResponseHeaders(HeadersDef &headers);
};

