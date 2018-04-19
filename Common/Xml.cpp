#include "StdAfx.h"
#include "Xml.h"
#include <Shlwapi.h>
#include "StringEx.h"
#include "ArrayEx.h"

using namespace MSXML2;

CXml::CXml(void)
{
	
}

CXml::CXml( _bstr_t bszRoot ):
	m_pEleRoot(NULL)
	,m_pEleCur(NULL)
	,m_bszRoot(bszRoot)
	,m_pCurChildCount(NULL)
	,m_dwLastDeepth(0)
	,m_pCurChildIndex(NULL)
{
	HRESULT hr=m_XMLDoc.CreateInstance(_uuidof(::DOMDocument30));
	if(!SUCCEEDED(hr))
	{
		AfxMessageBox(_T(" 无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库! " )); 
		return;
	}
}

CXml::~CXml(void)
{
	Destroy();
}

bool CXml::LoadFile( _bstr_t bszFilePath )
{
	m_bszFilePath=bszFilePath;
	if(PathFileExists(bszFilePath))
	{
		CFile rfile(bszFilePath,CFile::modeRead|CFile::shareExclusive);
		BYTE *pBuf=new BYTE[rfile.GetLength()+1];
		rfile.Read(pBuf,rfile.GetLength());
		pBuf[rfile.GetLength()]=0;
		rfile.Close();
		CString sRead(pBuf);
		delete pBuf;

		if(sRead.GetLength()>2)
		{
			if(sRead[1]=='!') return false;//网页文档
		}
		else
		{
			return false;
		}

		if(!m_XMLDoc->load(m_bszFilePath))
		{
			//AfxMessageBox(CString(_T("加载XML文件："))+bszFilePath.GetBSTR()+_T(" 失败！"));
			return false;
		}
	}else return false;
	
	return Root(m_bszRoot);
}

bool CXml::LoadString( LPCTSTR lpszXmlString )
{
	if(!m_XMLDoc->loadXML(lpszXmlString))
	{
		//AfxMessageBox(CString(_T("加载XML：\n"))+lpszXmlString+_T("\n 失败！"));
		return false;
	}
	return Root(m_bszRoot);
}

CString CXml::Read(_bstr_t szAttributeName)
{
	CString szRet;
	Read(szRet,szAttributeName);

	return szRet;
}


long CXml::ReadNumber( _bstr_t szAttributeName/*=""*/ )
{
	long lRet;

	Read(lRet,szAttributeName);

	return lRet;
}

bool CXml::Read( CString &szValueReturned , _bstr_t szAttributeName)
{
	_variant_t vt="";
	bool bRet = Read(vt,szAttributeName);
	szValueReturned.Format(_T("%s"),vt.bstrVal);

	return bRet;
}

bool CXml::Read( _variant_t &varRet , _bstr_t szAttributeName/*=""*/ )
{
	try
	{
		if(szAttributeName==_bstr_t(""))
		{
			varRet = m_pEleCur->Gettext();
		}
		else
		{
			varRet = m_pEleCur->getAttribute(szAttributeName);
		}
		if(varRet == nullptr)
		{
			return false;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		e=NULL;
		return false;
	}
	return true;
}

bool CXml::Read( long &nRet , _bstr_t szAttributeName/*=""*/ )
{
	_variant_t vt=0;
	bool bRet = Read(vt,szAttributeName);

	vt.ChangeType(VT_BSTR);
	if(vt.bstrVal[0]!=0)
	{
		vt.ChangeType(VT_I8);
		nRet = vt.lVal;
	}
	else
	{
		nRet = 0;
		bRet = false;
	}

	return bRet;
}

bool CXml::Write(_bstr_t szValue,_bstr_t szAttributeName)
{
	HRESULT hr;

	if(_bstr_t("")==szAttributeName)
	{
		hr=SUCCEEDED(m_pEleCur->put_text(szValue));
	}
	else
	{
		hr=m_pEleCur->setAttribute(szAttributeName,szValue);
	}

	if(SUCCEEDED(hr))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CXml::save(LPCTSTR lpszFilePath)
{
	if(CString(lpszFilePath)!=_T(""))//这里如果使用lpszFilePath!=  (NULL,0,_T(""))的方式，都得不到正确的结果，原因暂不明
	{
		m_bszFilePath=lpszFilePath;
	}

	if(m_bszFilePath==(_bstr_t)"")
	{
		return false;
	}

		if(SUCCEEDED(m_XMLDoc->save(m_bszFilePath))) 
		{
			return true;
		}
		else
		{
			return false;
		}
}

bool CXml::CD( _bstr_t szPath , bool bCreate)
{
	if(_bstr_t("")==szPath) return false;
	if(_bstr_t("..")==szPath)
	{
		GotoParent();
		return true;
	}
	CStringEx::CStringArray  szArray;
	_bstr_t bszTmp;
	UINT nPathLength=CStringEx(szPath).split(_T("\\"),szArray);

	for(UINT j=0;j<nPathLength;j++)
	{
		bszTmp=szArray[j];
		if(bszTmp==_bstr_t(".."))
		{
			GotoParent();
		}
		else
		{
			if(!GotoChild(bszTmp))
			{
				if(bCreate) AppendChild(bszTmp);
				else return false;
			}
			
		}	
	}
	
	return true;
}

bool CXml::GotoParent()
{
	if(m_pEleRoot==m_pEleCur) return false;
	m_pEleCur=m_pEleCur->GetparentNode();
	return true;
}

bool CXml::GotoChild( _bstr_t szNodeName )
{
	MSXML2::IXMLDOMNodeListPtr pNodeList;
	MSXML2::IXMLDOMNodePtr pNode;
	long nNodesCount;

	_bstr_t szNodNam;
	m_pEleCur->get_childNodes(&pNodeList);
	pNodeList->get_length(&nNodesCount);
	for(long i=0;i<nNodesCount;i++)
	{
		pNodeList->get_item(i,&pNode);
		szNodNam=pNode->GetnodeName();

		if(szNodNam==szNodeName)
		{
			m_pEleCur=pNode;
			return true;
			break;
		}
	}

	return false;
}


void CXml::AppendChild( _bstr_t szNodeName )
{
	MSXML2::IXMLDOMElementPtr pEle=m_XMLDoc -> createElement((_bstr_t) szNodeName );
	m_pEleCur->appendChild(pEle);
	m_pEleCur=pEle;
}

bool CXml::Root( _bstr_t szRootName )
{
	if(m_pEleRoot==NULL)
	{
		m_pEleRoot=m_XMLDoc->GetdocumentElement();
		if(m_pEleRoot==NULL)//不存在则创建
		{
			m_pEleRoot=m_XMLDoc->createElement(szRootName);
			m_XMLDoc->appendChild(m_pEleRoot);
		}
	}
	if(m_pEleRoot==NULL) return false;
	m_pEleCur=m_pEleRoot;
	return true;
}

void CXml::Destroy( void )
{
	static bool bDestroyed=false;
	if(bDestroyed) return;
	bDestroyed=true;
	
	//这里添加退出代码
	m_XMLDoc=NULL;
	m_pEleRoot=NULL;
	m_pEleCur=NULL;
	m_CurNodeListPtr=NULL;
	m_pCurChildCount=NULL;
	m_pCurChildIndex=NULL;
	ListPtrArray.Destroy();
	nChildIndexArray.Destroy();
	lChildCountArray.Destroy();
}

bool CXml::EnumerateChildElement(DWORD dwDeepth)
{	
	if(dwDeepth>m_dwLastDeepth||m_pCurChildCount==NULL)//新建深度（层）并初始化，以pCurChildCount为起始标记
	{
		m_pCurChildIndex=nChildIndexArray.AppendNew();
		m_pCurChildCount=lChildCountArray.AppendNew();

		m_pEleCur->get_childNodes(&m_CurNodeListPtr);
		m_CurNodeListPtr->get_length(m_pCurChildCount);
		(*m_pCurChildIndex)=0;
		ListPtrArray.Append(m_CurNodeListPtr);
	}
	m_dwLastDeepth=dwDeepth;
	
	MSXML2::IXMLDOMNodePtr pNode;
	do//这里如果使用递归，静态变量在递归调用中每次都会重新开辟空间，所以会导致结果出错
	{
		if((*m_pCurChildIndex)==(*m_pCurChildCount))
		{
			ListPtrArray.RemoveAt(dwDeepth);//处理完成后移除当前层   *pNodeListPtr=NULL;
			nChildIndexArray.RemoveAt(dwDeepth);
			lChildCountArray.RemoveAt(dwDeepth);
			//调试收获：
			//静态的智能指针（com）要在CoUnitialize之前释放，否则会出错
			//释放方法：将其赋值为NULL，如果使用->Release，不能将其置零，导致程序结束时(static变量)再次调用其Release而导致错误
			//如果->Release再置零则会出现重复释放错误，因此智能指针置零等于释放（重载了=），也可能对其赋值就等于释放
			if(dwDeepth==0)
			{
				GotoParent();//顶层枚举完成之后指针返回到被枚举的节点
				m_pCurChildCount=NULL;//枚举完成后重置起始标志
				return false;
			}

			m_CurNodeListPtr=ListPtrArray[dwDeepth-1];
			//调试收获：
			//这种逐层使用的变量在处理完成后返回上层时要将所有相关指针移回上层
			//code! TRACE("强制转换前：&(ListPtrArray[dwDeepth-1])=%d\n",(ListPtrArray[dwDeepth-1]));
			//code!ListPtrArray[dwDeepth-1];
			//code!pCurNodeListPtr=(MSXML2::IXMLDOMNodeListPtr*)lpConv;//将指针移回上一层
			//code!TRACE("强制转换后：&(ListPtrArray[dwDeepth-1])=%d\n",(ListPtrArray[dwDeepth-1]));
			//code!pCurNodeListPtr=(MSXML2::IXMLDOMNodeListPtr*)&(ListPtrArray[dwDeepth-1]);此处转换后，CurNodeListPtr的m_pInterface变为0，出现错误
			//调试发现：
			//强制转换(IXMLDOMNodeListPtr*)，会导致智能指针为NULL
			//----------------------------------------------------
			//查找相关文档：
			////_com_ptr_t的实现
			//Interface** operator&() throw()
			//{
			//	_Release();   //这个函数会将原来指针所指向的资源释放掉
			//	m_pInterface = NULL;
			//	return &m_pInterface;
			//}
			//如果一个智能指针在取地址操作时释放掉原有的COM资源(_com_ptr_t)，那它将不能做传入[in]的参数上
			//介于上述情况复杂，&操作符一般会在指针非空的情况下做一个断言。禁止你返回一个非空的接口指针。这样&操作符就被限定使用在仅仅用于传出的函数参数中。
			//然而，若将此智能指针取地址后传入到一个传入或传入传出功能的函数参数上。他却将智能指针持有的资源隐式释放了，为程序崩溃埋下了伏笔。
			//如果这种情况出现了，你的脑子里充满了一大堆的疑问，满世界找原因。
			//--------------------------------------------------------------
			//这里确实卡了很长时间，但也算一种很大的收获

			m_pCurChildIndex=&(nChildIndexArray[dwDeepth-1]);
			m_pCurChildCount=&(lChildCountArray[dwDeepth-1]);

			return false;
		}
		m_CurNodeListPtr->get_item((*m_pCurChildIndex),&pNode);
		(*m_pCurChildIndex)++;
	} while (pNode->GetnodeType()!=MSXML2::NODE_ELEMENT);//保证Node类型为Element
	m_pEleCur=pNode;
	
	return true;
}

bool CXml::ExistsAttribute( _bstr_t bszAttributeName/*=""*/ )
{
	if(m_pEleCur==NULL) return false;
	if(bszAttributeName.length() == 0)
	{
		BSTR *text;
		if(m_pEleCur->get_text(text)==ERROR_SUCCESS)
		{
			return true;
		}
	}
	else
	{
		if(m_pEleCur->getAttribute(bszAttributeName)!=nullptr)
		{
			return true;
		}
	}
	
	return false;
}
