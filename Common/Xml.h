#import <MsXml6.dll>

#pragma once
#include "ArrayEx.h"

class __declspec(dllexport) CXml
{
public:
	CXml(void);
	CXml(_bstr_t lpszRoot);//���Խ���xml�ļ����ڴ��е��ַ���ת��ΪCXml
	~CXml(void);

	bool LoadFile(_bstr_t bszFilePath);
	bool LoadString(LPCTSTR lpszXmlString);

	bool ExistsAttribute(_bstr_t szAttributeName="");
	CString Read(_bstr_t szAttributeName="");
	long ReadNumber(_bstr_t szAttributeName="");
	bool Read(CString &szValueReturned , _bstr_t szAttributeName="");
	bool Read(_variant_t &varRet , _bstr_t szAttributeName="");
	bool Read(long &nRet , _bstr_t szAttributeName="");
	bool Write(_bstr_t szValue,_bstr_t szAttributeName="");//д��ǰ

	bool CD(_bstr_t szPath , bool bCreate=false);//��CMD��CD����,���·��������ָ��ָ���·��ĩ�˲�����true�����򷵻�false����ͬ���ǣ����·����������bCreate==true����ᴴ�����·��������true��
	bool GotoParent();//����ѵ����ڵ㣬��ִ�в���������false�����򷵻�true
	bool GotoChild(_bstr_t szNodeName);//��m_pEleCur�ƶ�����ChildNode������������򴴽�������false�������򷵻�true��
	void AppendChild(_bstr_t szNodeName);//���child����ָ���ƶ�����
	bool Root(_bstr_t szRootName = "Root");//�л���Element������������򴴽�������false���Ѿ������򷵻�true
	MSXML2::IXMLDOMElementPtr getCurrentElement(){return m_pEleCur;}
	bool save(LPCTSTR lpszFilePath = nullptr);
	void Destroy(void);

	bool EnumerateChildElement(DWORD dwDeepth = 0);//ö���ӽڵ㣬����ָ���ƶ����ӽڵ㣬�ҵ���һ���ӽڵ�����+1��ö�����ӽڵ�󷵻�false
	// dwDeepth��ö����ȣ�Ϊ��ʹ�ú�����Ƕ�׵��ã�ʹ��Ƕ��ʱ��ȱ������ε���1����0��ʼ����ʾ��
	/*
	ʾ���÷���
	while(xm.EnumerateChild())//��ָ��(��ΪpA)�ƶ���һ��Child������ʹ��ѭ��ֱ���䷵��false��������ܳ���
	{
		xm.Read();//��ʱ��ǰָ��Ϊһ��Child
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

	/* ���±�������ö�ٺ��� */
	MSXML2::IXMLDOMNodeListPtr m_CurNodeListPtr;//��ǰ���child�б�ָ�룬ָ��ǰ����б�(ListPtrArray[dwDeepth])
	//���ʹ��IXMLDOMNodeListPtr*�������ʹ��ʱ���õ�&ȡ��ַ��������ָ���&������Ὣ��������Դ�ͷŵ����������
	int* m_pCurChildIndex;//��ǰ���child������ָ��
	long* m_pCurChildCount;//��ǰ���child��Ŀָ��
	DWORD m_dwLastDeepth;
};

