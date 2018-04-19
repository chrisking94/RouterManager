#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CHtmlView。"
#endif 

// CWebBrowser1 Html 视图

class CWebBrowser1 : public CHtmlView
{
	DECLARE_DYNCREATE(CWebBrowser1)

protected:
	CWebBrowser1();           // 动态创建所使用的受保护的构造函数
	virtual ~CWebBrowser1();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


