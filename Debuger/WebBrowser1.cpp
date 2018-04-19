// WebBrowser1.cpp : 实现文件
//

#include "stdafx.h"
#include "Debuger.h"
#include "WebBrowser1.h"


// CWebBrowser1

IMPLEMENT_DYNCREATE(CWebBrowser1, CHtmlView)

CWebBrowser1::CWebBrowser1()
{

}

CWebBrowser1::~CWebBrowser1()
{
}

void CWebBrowser1::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebBrowser1, CHtmlView)
END_MESSAGE_MAP()


// CWebBrowser1 诊断

#ifdef _DEBUG
void CWebBrowser1::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebBrowser1::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CWebBrowser1 消息处理程序
