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
		AfxMessageBox(_T(" �޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�! " )); 
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
			if(sRead[1]=='!') return false;//��ҳ�ĵ�
		}
		else
		{
			return false;
		}

		if(!m_XMLDoc->load(m_bszFilePath))
		{
			//AfxMessageBox(CString(_T("����XML�ļ���"))+bszFilePath.GetBSTR()+_T(" ʧ�ܣ�"));
			return false;
		}
	}else return false;
	
	return Root(m_bszRoot);
}

bool CXml::LoadString( LPCTSTR lpszXmlString )
{
	if(!m_XMLDoc->loadXML(lpszXmlString))
	{
		//AfxMessageBox(CString(_T("����XML��\n"))+lpszXmlString+_T("\n ʧ�ܣ�"));
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
	if(CString(lpszFilePath)!=_T(""))//�������ʹ��lpszFilePath!=  (NULL,0,_T(""))�ķ�ʽ�����ò�����ȷ�Ľ����ԭ���ݲ���
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
		if(m_pEleRoot==NULL)//�������򴴽�
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
	
	//��������˳�����
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
	if(dwDeepth>m_dwLastDeepth||m_pCurChildCount==NULL)//�½���ȣ��㣩����ʼ������pCurChildCountΪ��ʼ���
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
	do//�������ʹ�õݹ飬��̬�����ڵݹ������ÿ�ζ������¿��ٿռ䣬���Իᵼ�½������
	{
		if((*m_pCurChildIndex)==(*m_pCurChildCount))
		{
			ListPtrArray.RemoveAt(dwDeepth);//������ɺ��Ƴ���ǰ��   *pNodeListPtr=NULL;
			nChildIndexArray.RemoveAt(dwDeepth);
			lChildCountArray.RemoveAt(dwDeepth);
			//�����ջ�
			//��̬������ָ�루com��Ҫ��CoUnitialize֮ǰ�ͷţ���������
			//�ͷŷ��������丳ֵΪNULL�����ʹ��->Release�����ܽ������㣬���³������ʱ(static����)�ٴε�����Release�����´���
			//���->Release�������������ظ��ͷŴ����������ָ����������ͷţ�������=����Ҳ���ܶ��丳ֵ�͵����ͷ�
			if(dwDeepth==0)
			{
				GotoParent();//����ö�����֮��ָ�뷵�ص���ö�ٵĽڵ�
				m_pCurChildCount=NULL;//ö����ɺ�������ʼ��־
				return false;
			}

			m_CurNodeListPtr=ListPtrArray[dwDeepth-1];
			//�����ջ�
			//�������ʹ�õı����ڴ�����ɺ󷵻��ϲ�ʱҪ���������ָ���ƻ��ϲ�
			//code! TRACE("ǿ��ת��ǰ��&(ListPtrArray[dwDeepth-1])=%d\n",(ListPtrArray[dwDeepth-1]));
			//code!ListPtrArray[dwDeepth-1];
			//code!pCurNodeListPtr=(MSXML2::IXMLDOMNodeListPtr*)lpConv;//��ָ���ƻ���һ��
			//code!TRACE("ǿ��ת����&(ListPtrArray[dwDeepth-1])=%d\n",(ListPtrArray[dwDeepth-1]));
			//code!pCurNodeListPtr=(MSXML2::IXMLDOMNodeListPtr*)&(ListPtrArray[dwDeepth-1]);�˴�ת����CurNodeListPtr��m_pInterface��Ϊ0�����ִ���
			//���Է��֣�
			//ǿ��ת��(IXMLDOMNodeListPtr*)���ᵼ������ָ��ΪNULL
			//----------------------------------------------------
			//��������ĵ���
			////_com_ptr_t��ʵ��
			//Interface** operator&() throw()
			//{
			//	_Release();   //��������Ὣԭ��ָ����ָ�����Դ�ͷŵ�
			//	m_pInterface = NULL;
			//	return &m_pInterface;
			//}
			//���һ������ָ����ȡ��ַ����ʱ�ͷŵ�ԭ�е�COM��Դ(_com_ptr_t)������������������[in]�Ĳ�����
			//��������������ӣ�&������һ�����ָ��ǿյ��������һ�����ԡ���ֹ�㷵��һ���ǿյĽӿ�ָ�롣����&�������ͱ��޶�ʹ���ڽ������ڴ����ĺ��������С�
			//Ȼ��������������ָ��ȡ��ַ���뵽һ��������봫�����ܵĺ��������ϡ���ȴ������ָ����е���Դ��ʽ�ͷ��ˣ�Ϊ������������˷��ʡ�
			//���������������ˣ���������������һ��ѵ����ʣ���������ԭ��
			//--------------------------------------------------------------
			//����ȷʵ���˺ܳ�ʱ�䣬��Ҳ��һ�ֺܴ���ջ�

			m_pCurChildIndex=&(nChildIndexArray[dwDeepth-1]);
			m_pCurChildCount=&(lChildCountArray[dwDeepth-1]);

			return false;
		}
		m_CurNodeListPtr->get_item((*m_pCurChildIndex),&pNode);
		(*m_pCurChildIndex)++;
	} while (pNode->GetnodeType()!=MSXML2::NODE_ELEMENT);//��֤Node����ΪElement
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
