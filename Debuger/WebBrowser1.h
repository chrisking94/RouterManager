#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CHtmlView��"
#endif 

// CWebBrowser1 Html ��ͼ

class CWebBrowser1 : public CHtmlView
{
	DECLARE_DYNCREATE(CWebBrowser1)

protected:
	CWebBrowser1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWebBrowser1();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


