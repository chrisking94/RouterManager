#include "StdAfx.h"
#include "RouterXml.h"

CRouterXml::CRouterXml(void)
{
}

CRouterXml::CRouterXml( _bstr_t bszFilePath ):
CXml("Root")//�������ʹ�ò����б��ʼ���������������⣨ԭ�������ں����г�ʼ�����������ʼ��ʱ���û��๹�캯���������б���û��ʱ����Ĭ�Ϲ��캯����...
{
	m_bXmlLoaded=CXml::LoadFile(bszFilePath);
}


CRouterXml::~CRouterXml(void)
{
}

bool CRouterXml::IsRouterXml()
{
	if(!m_bXmlLoaded) return false;

	_bstr_t *pNode = RXmlStruct;
	BOOL bAttr = FALSE;
	CString szTmp;

	if(Root("Root"))
	{
		for(;;)
		{
			CString szTmp((*pNode).GetBSTR());
			if(szTmp[0]=='*')//����
			{
				bAttr=!bAttr;
			}
			else if(szTmp[0]==0)//��β
			{
				return true;
			}
			else
			{
				if(bAttr)
				{
					if(!ExistsAttribute(*pNode))
					{
						return false;
					}
				}
				else
				{
					if(szTmp[0]=='-')
					{
						CD("..");//�����ϼ�
					}
					else
					{
						CD(*pNode);
					}
				}
			}

			++pNode;
		};
	}

	return false;
}


int CRouterXml::ReadSupportedRouters( CStringEx::CStringArray& szaReturned )
{
	if(Root("Root"))
	{
		return CStringEx(CXml::Read("supp")).split(_T(","),szaReturned);
	}else return 0;
}

_bstr_t CRouterXml::RXmlStruct[]=
{
"Root",
		"Path",
			"-",
		"Auth",
			"*",//����
			"accessOK",
			"PW",
			"UN",
			"cookie",
			"path",
			"statRegex",
			"defaultPW",
			"defaultUN",
			"*",//����END
			"-",
		"Wan",//����
			"*",
			"path",//�����ŵ�������
			"regex",
			"*",
				"PPPoE",
				"*",
				"path",
				"*",
					"ctrl",
						"*",
						"path",
						"psw",
						"conf",
						"conn",
						"acc",
						"dconn",
						"wantype",
						"linktype",//�Զ����ֶ�...
						"sav",
						"*",
						"-",//������һ��
					"stat",
						"*",
						"path",
						"regex",
						"*",
						"-",
					"-",
				"DIP",
					"*",
					"path",
					"*",
						"ctrl",
							"*",
							"wantype",
							"*",
							"-",
						"-",
				"SIP",
				"Mac",
					"*",
					"path",
					"*",
					"ctrl",
						"*",
						"sav",
						"mac",
						"*",
						"-",
					"-",
				"-",
			"TrafStat",
				"*",
				"path",
				"*",
				"ctrl",
					"*",
					"path",
					"statOff",
					"statOn",
					"*",
					"-",
				"stat",
					"*",
					"path",
					"regex",
					"ipr",//itemsPerRow
					"*",
					"-",
				"-",
			"DHCP",
				"*",
				"path",
				"*",
				"stat",
					"*",
					"path",
					"regex",
					"ipr",
					"*",
					"-",
				"-",
			"-",
		""//����
};


bool CRouterXml::ReadAll()
{
	bool bRet=true;

	m_pStructNode = RXmlStruct;
	m_Router.RequestPathList.Clear();
	m_Router.szFormContainerList.Clear();
	if(Root(*m_pStructNode++))//Root
	{
		if(CD(*m_pStructNode++))//��ȡ·���б�
		{
			CStringEx::CStringArray szArr;
			CStringEx(Read()).split(_T("\n"),szArr);

			for(INT i=0;i<szArr.GetLength();i++)
			{
				if(szArr[i].IsEmpty()) continue;

				RequestPathDef *pPath = m_Router.RequestPathList.AppendNew();

				pPath->requestMethod = CHttpWebConnection::Method(szArr[i][0]-0x30);
				szArr[i].Delete(0,1);

				pPath->szPath = szArr[i];
			}
			m_pStructNode++;//-
			CD("..");
		}else return false;

		if(CD(*m_pStructNode++))//Auth
		{
			++m_pStructNode;//����"*"
			ReadComm(m_Router.Auth.szAccessOk);
			ReadComm(m_Router.Auth.szPassword);
			ReadComm(m_Router.Auth.szUsername);
			ReadComm(m_Router.Auth.szCookie);
			ReadComm(m_Router.Auth.nPathIndex);
			ReadComm(m_Router.Auth.szStatRegex);
			ReadComm(m_Router.Auth.szDefalutPassword);
			ReadComm(m_Router.Auth.szDefalutUsername);
			*m_pStructNode++;//*
			*m_pStructNode++;//-
			CD("..");
		}else return false;

		if(CD(*m_pStructNode++))//Wan
		{
			*m_pStructNode++;//*
			ReadComm(m_Router.Wan.nPathIndex);
			ReadComm(m_Router.Wan.szRegex);
			*m_pStructNode++;//*
			if(CD(*m_pStructNode++))//Wan\PPPoE
			{
				++m_pStructNode;//����"*"
				ReadComm(m_Router.Wan.PPPoE.nPathIndex);
				++m_pStructNode;

				if(CD(*m_pStructNode++))//Wan\PPPoE\ctrl
				{
					++m_pStructNode;
					ReadComm(m_Router.Wan.PPPoE.Ctrl.nPathIndex);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szPassword);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szConfirm);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szConnect);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szAccount);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szDisconnect);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szWanType);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szLinkType);
					ReadComm(m_Router.Wan.PPPoE.Ctrl.szSave);
					++m_pStructNode;
					++m_pStructNode;//����"-"
					CD("..");//Wan\PPPoE\stat
				}else return false;
				if(CD(*m_pStructNode++))//stat
				{
					CStringEx(Read()).split(_T(","),m_Router.Wan.PPPoE.Stat.nStatCodeMapList);

					++m_pStructNode;
					ReadComm(m_Router.Wan.PPPoE.Stat.nPathIndex);
					ReadComm(m_Router.Wan.PPPoE.Stat.szStatRegex);
					if(m_Router.Wan.PPPoE.Stat.szStatRegex.IsEmpty())
					{
						//�̳�Wan
						m_Router.Wan.PPPoE.Stat.szStatRegex = m_Router.Wan.szRegex;
					}
					++m_pStructNode;
					++m_pStructNode;
					CD("..");
				}else return false;
				++m_pStructNode;
				CD("..");
			}else return false;

			if(CD(*m_pStructNode++))//Wan\DIP
			{
				++m_pStructNode;
				ReadComm(m_Router.Wan.DIP.nPathIndex);
				++m_pStructNode;
				if(CD(*m_pStructNode++))
				{
					m_pStructNode++;
					ReadComm(m_Router.Wan.DIP.Ctrl.szWantype);
					m_pStructNode++;
					m_pStructNode++;
					CD("..");
				}
				m_pStructNode++;
				CD("..");
			}

			if(CD(*m_pStructNode++))//Wan\SIP
			{
				CD("..");
			}else return false;

			if(CD(*m_pStructNode++))//Wan\MAC Clone
			{
				m_pStructNode++;
				ReadComm(m_Router.Wan.MacClone.nPathIndex);
				m_pStructNode++;
				if(CD(*m_pStructNode++))//Wan\MAC Clone\Ctrl
				{
					m_pStructNode++;
					ReadComm(m_Router.Wan.MacClone.Ctrl.szSave);
					ReadComm(m_Router.Wan.MacClone.Ctrl.szMac);
					m_pStructNode++;
					m_pStructNode++;
					CD("..");
				}else return false;
				m_pStructNode++;
				CD("..");
			}else return false;
			m_pStructNode++;
			CD("..");//--Wan End
		}

		if(CD(*m_pStructNode++))//TrafStat
		{
			++m_pStructNode;//����"*"
			ReadComm(m_Router.Traf.nPathIndex);
			++m_pStructNode;

			if(CD(*m_pStructNode++))//  TrafStat\Ctrl
			{
				++m_pStructNode;

				ReadComm(m_Router.Traf.Ctrl.nPathIndex);
				ReadComm(m_Router.Traf.Ctrl.szStatOff);
				ReadComm(m_Router.Traf.Ctrl.szStatOn);
				
				++m_pStructNode;
				++m_pStructNode;//����"-"
				CD("..");
			}else return false;
			if(CD(*m_pStructNode++))// TrafStat\Stat
			{
				CStringEx::CStringArray szArr;
				CStringEx(Read()).split(_T(","),szArr);
				for(int i=0;i<6;i++)
				{
					if(i<szArr.GetLength())
					{
						m_Router.Traf.Stat.pnItemIndexs[i]=_ttoi(szArr[i]);
					}
					else
					{
						m_Router.Traf.Stat.pnItemIndexs[i]=-1;
					}
				}

				++m_pStructNode;
				ReadComm(m_Router.Traf.Stat.nPathIndex);
				ReadComm(m_Router.Traf.Stat.szRegex);
				ReadComm(m_Router.Traf.Stat.nItemPerRow);
				++m_pStructNode;
				++m_pStructNode;
				CD("..");
			}else return false;
			++m_pStructNode;
			CD("..");
			//Traf End
		}else return false;

		if(CD(*m_pStructNode++))//DHCP
		{
			m_pStructNode++;
			ReadComm(m_Router.DHCP.nPathIndex);
			m_pStructNode++;
			if(CD(*m_pStructNode++))//stat
			{
				CStringEx(Read()).split(_T(","),m_Router.DHCP.Stat.nItemIndexs);

				m_pStructNode++;
				ReadComm(m_Router.DHCP.Stat.nPathIndex);
				ReadComm(m_Router.DHCP.Stat.szRegex);
				ReadComm(m_Router.DHCP.Stat.nItemsPerRow);
				m_pStructNode++;
				m_pStructNode++;
				CD("..");
			}
			m_pStructNode++;
			CD("..");
		}else return false;
	}else return false;

	//��չFormContainerά��
	for(int i = 0;i<m_Router.RequestPathList.GetLength() ; i++)
	{
		m_Router.szFormContainerList.AppendNew();
	}

	m_Router.Auth.mRequestMethod = m_Router.RequestPathList[m_Router.Auth.nPathIndex].requestMethod;

	return bRet;
}


void CRouterXml::ConvertRouterLinkStatStrToCode( CString szCode,RouterLinkStatus &codeRet )
{
	if(szCode==m_Router.Auth.szAccessOk)
	{
		codeRet=OK;
	}
	else
	{
		codeRet=Denied;
	}
}

void CRouterXml::ConvertWanLinkStatStrCodeToCode( CString szCode,WanLinkStat &codeRet )
{
	INT nCode = _ttoi(szCode);

	for(int i=0;i<m_Router.Wan.PPPoE.Stat.nStatCodeMapList.GetLength();i++)
	{
		if(nCode==m_Router.Wan.PPPoE.Stat.nStatCodeMapList[i])
		{
			codeRet = (WanLinkStat)i;
			return ;
		}
	}

	codeRet = UnknownFailed;
}

void CRouterXml::ConvertWanLinkStatCodeToStr( WanLinkStat code,CString &szCodeRet )
{
	const CString cszStringList[]={_T("δ����"),_T("������"),_T("��������..."),_T("�û�����������֤ʧ��"),_T("����������Ӧ"),_T("δ֪ԭ��ʧ��"),_T("WAN��δ���ӣ�"),_T("�ȴ�����·����")};
	szCodeRet = cszStringList[code];
}

void CRouterXml::ConvertRouterLinkStatCodeToStr( RouterLinkStatus code,CString &szCodeRet )
{
	const CString cszList[]={_T("��ʼ��"),_T("δ����"),_T("����Ӧ"),_T("������"),_T("��֤ʧ��")};

	INT nCode = code;
	if(code == OK)
	{
		nCode = 3;
	}
	else
	{
		nCode = 4;
	}

	szCodeRet = cszList[nCode];
}

bool CRouterXml::ReadComm( CString &szRet )
{
	return Read(szRet,*m_pStructNode++);
}

bool CRouterXml::ReadComm( long &lRet )
{
	return Read(lRet,*m_pStructNode++);
}

bool CRouterXml::ReadComm( AuthMethod &amRet )
{
	long lRet;

	bool bRet = Read(lRet,*m_pStructNode++);
	amRet = (AuthMethod)lRet;

	return bRet;
}

bool CRouterXml::ReadComm( CHttpWebConnection::Method &mRet )
{
	long lRet;

	bool bRet = Read(lRet,*m_pStructNode++);
	mRet = (CHttpWebConnection::Method)lRet;

	return bRet;
}

bool CRouterXml::ReadComm( int &iRet )
{
	long lRet;

	bool bRet = Read(lRet,*m_pStructNode++);
	iRet = (int)lRet;

	return bRet;
}

void CRouterXml::ResetContainer( void )
{
	for(int i =0 ;i<m_Router.szFormContainerList.GetLength() ;i++)
	{
		m_Router.szFormContainerList[i] = _T("");
	}
}

