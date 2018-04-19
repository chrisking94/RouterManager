#import <MsXml6.dll>

#pragma once
#include "ArrayEx.h"

class __declspec(dllexport) CXml
{
public:
	CXml(void);
	CXml(_bstr_t lpszRoot);//可以将在xml文件或内存中的字符串转换为CXml
	~CXml(void);

	bool LoadFile(_bstr_t bszFilePath);
	bool LoadString(LPCTSTR lpszXmlString);

	bool ExistsAttribute(_bstr_t szAttributeName="");
	CString Read(_bstr_t szAttributeName="");
	long ReadNumber(_bstr_t szAttributeName="");
	bool Read(CString &szValueReturned , _bstr_t szAttributeName="");
	bool Read(_variant_t &varRet , _bstr_t szAttributeName="");
	bool Read(long &nRet , _bstr_t szAttributeName="");
	bool Write(_bstr_t szValue,_bstr_t szAttributeName="");//写当前

	bool CD(_bstr_t szPath , bool bCreate=false);//与CMD的CD类似,如果路径存在则将指针指向该路径末端并返回true，否则返回false。不同的是，如果路径不存在且bCreate==true，则会创建这个路径并返回true。
	bool GotoParent();//如果已到根节点，则不执行操作并返回false，否则返回true
	bool GotoChild(_bstr_t szNodeName);//将m_pEleCur移动到其ChildNode，如果不存在则创建并返回false，存在则返回true。
	void AppendChild(_bstr_t szNodeName);//添加child并将指针移动到它
	bool Root(_bstr_t szRootName = "Root");//切换根Element，如果不存在则创建并返回false，已经存在则返回true
	MSXML2::IXMLDOMElementPtr getCurrentElement(){return m_pEleCur;}
	bool save(LPCTSTR lpszFilePath = nullptr);
	void Destroy(void);

	bool EnumerateChildElement(DWORD dwDeepth = 0);//枚举子节点，并将指针移动到子节点，且调用一次子节点索引+1，枚举完子节点后返回false
	// dwDeepth：枚举深度，为了使该函数能嵌套调用，使用嵌套时深度必须依次递增1，从0开始，见示例
	/*
	示例用法：
	while(xm.EnumerateChild())//将指针(记为pA)移动到一个Child，必须使用循环直到其返回false，否则可能出错
	{
		xm.Read();//这时当前指针为一个Child
		.
		.
		.
		while(xm.EnumerateChild(1))
		{
		}
	}
	*/
protected:
	_bstr_t m_bszFilePath;
	_bstr_t m_bszPath;
	_bstr_t m_bszRoot;
	MSXML2::IXMLDOMDocumentPtr m_XMLDoc;
	MSXML2::IXMLDOMElementPtr m_pEleRoot;
	MSXML2::IXMLDOMElementPtr m_pEleCur;

private:
	CArrayEx<MSXML2::IXMLDOMNodeListPtr> ListPtrArray;
	CArrayEx<int> nChildIndexArray;
	CArrayEx<long> lChildCountArray;

	/* 以下变量用于枚举函数 */
	MSXML2::IXMLDOMNodeListPtr m_CurNodeListPtr;//当前深度child列表指针，指向当前深度列表(ListPtrArray[dwDeepth])
	//如果使用IXMLDOMNodeListPtr*，则后面使用时会用到&取地址，而智能指针的&运算符会将其所有资源释放掉，引起错误
	int* m_pCurChildIndex;//当前深度child索引号指针
	long* m_pCurChildCount;//当前深度child数目指针
	DWORD m_dwLastDeepth;
};

