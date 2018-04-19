#include "StdAfx.h"
#include "Router.h"
#include "Application.h"
#include "Ping.h"
#include "HtmlDocument.h"
#include "Messager.h"
#include "NetworkAdapter.h"
#include "BroadBand.h"
#include "Common.h"


BOOL Router::createConnection( CHttpWebConnection* &pHwcRet,CRouterXml::RequestPathDef requestPath   )
{
	requestPath.szPath=s_MyInfo.szURL+requestPath.szPath;
	pHwcRet =new CHttpWebConnection(requestPath.szPath,requestPath.requestMethod);
	if(s_MyInfo.authMethod == Dlg)
	{
		pHwcRet->addRequestHeaderString(_T("Authorization:Basic ")+s_MyInfo.szBasicAuthorize);
	}
	if(requestPath.requestMethod == CHttpWebConnection::POST)
	{
		pHwcRet->writeRequestBody(_T("0"));
	}
	pHwcRet->addRequestHeaderString(_T("Referer: ")+requestPath.szPath);
	
	return TRUE;
}

BOOL Router::PPPoE_Dial()
{
	if(g_WanTask.bTaskCompleted)
	{
		if(setCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.nPathIndex))
		{
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szAccount,BroadBand::g_szAccount);
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szPassword,BroadBand::g_szPassword);
			if(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szConfirm!=_T(""))
			{
				writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szConfirm,BroadBand::g_szPassword);
			}
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szWanType);//����WanTypeΪPPPoE
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szLinkType);//�����ֶ�����
			//s_pTaskHwc->writeForm(s_RouterXml.m_Router.Wan.PPPoE.szSave);//��������
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szConnect);
			g_WanTask.szTaskName=_T("����");

			g_WanTask.bTaskCompleted=FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Router::PPPoE_Disc()
{
	if(g_WanTask.bTaskCompleted)
	{
		if(setCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.nPathIndex))
		{
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szDisconnect);
			g_WanTask.szTaskName=_T("�Ͽ�����");

			g_WanTask.bTaskCompleted=FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Router::MacFilter_On( BOOL bOn/*=TRUE*/ )
{
	//CHttpWebConnection *pHwc;
	//createConnection(pHwc,g_RouterXml.m_Router.WlanFilter.szPath);
	//pHwc->writeForm(g_RouterXml.m_Router.WlanFilter.szFilterSwitch[bOn]);
	//DWORD dwRet=pHwc->sendRequest();
	//closeConnection(pHwc);
	return 1;
}

void Router::closeConnection( CHttpWebConnection* &pHwcRet )
{
	if(NULL!=pHwcRet)
	{
		pHwcRet->close();
		delete pHwcRet;
		pHwcRet=NULL;
	}
}

BOOL Router::loadXml(  CString strParam /*= _T("") */ )
{
	/* ---�����ļ�--- */
	int j=0;
	BOOL bXmlMatched=FALSE;
	CString strFileName,strMatch=_T(""),szRouterType=s_MyInfo.szType;
	szRouterType.MakeLower();

	if(szRouterType==_T("")) return false;
	//����
	strParam.MakeLower();
	szRouterType+=strParam;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile (Application::getRouterXmlFolder()+_T ("*.config"), &fd);  
	if (hFind != INVALID_HANDLE_VALUE) {  
		do {
			j = 0;
			strMatch = _T("");

			strFileName=fd.cFileName;
			strFileName.MakeLower();
			while(strFileName[j])
			{
				if(strFileName[j]=='x')//xΪͨ���
				{
					strMatch.AppendChar(szRouterType[j]);
					j++;
				}
				else if (strFileName[j]==szRouterType[j])
				{
					j++;
				}
				else
				{
					break;
				}
				//TRACE("%S\n",szRouterType);
				if(szRouterType[j]==0)//ƥ��ɹ�
				{
					//�����ļ��ڲ�ƥ��
					g_RouterXml=CRouterXml((_bstr_t)(Application::getRouterXmlFolder()+strFileName));
					if(g_RouterXml.IsRouterXml())
					{
						CStringEx::CStringArray csa;
						int c=g_RouterXml.ReadSupportedRouters(csa);
						for(int i=0;i<c;i++)
						{
							int j=0;
							CString strSupportedType=csa[i];

							while(strSupportedType[j])
							{
								if(strSupportedType[j]==_T('x')||strSupportedType[j]==strMatch[j])//'x'Ϊͨ���
								{
									j++;
									if(strMatch[j]==0)//��ȫƥ�䣬����XML
									{
										bXmlMatched=TRUE;
										break;
									}
								}
								else
								{
									break;
								}
							}
							if(strMatch[0]==0)//��ȫƥ�䣬����XML
							{
								bXmlMatched=TRUE;
							}
							if(bXmlMatched) break;
						}
					}
				}
			}
			if(bXmlMatched) break;
		} while (::FindNextFile (hFind, &fd));
		::FindClose (hFind);  
	}
	/* ---���ҽ���--- */


	if(bXmlMatched)
	{
		//TRACE("%d----\n",m_RouterAuth.amAuth);
		bXmlMatched = g_RouterXml.ReadAll();

		if(bXmlMatched)
		{
			Application::SetRouterUsername(g_RouterXml.m_Router.Auth.szDefalutUsername,TRUE);
			Application::SetRouterPassword(g_RouterXml.m_Router.Auth.szDefalutPassword,TRUE);
		}
	};
	
	return bXmlMatched;
}

void Router::SetAuthorize( CString Username, CString Password )
{
	if(!Username.IsEmpty()) s_MyInfo.szUsername=Username;
	if(!Password.IsEmpty()) s_MyInfo.szPassword=Password;
	s_MyInfo.szBasicAuthorize=CCodeString(s_MyInfo.szUsername+_T(":")+s_MyInfo.szPassword).B64Encode();
}

BOOL Router::Traffic_TurnOn()
{
	if(g_TrafficTask.bTaskCompleted)
	{
		if(setCurrentForm(g_RouterXml.m_Router.Traf.Ctrl.nPathIndex))
		{
			writeCurrentForm(g_RouterXml.m_Router.Traf.Ctrl.szStatOn);

			g_TrafficTask.bTaskCompleted=FALSE;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL Router::ConfigRouter()
{
	return TRUE;
}

DWORD WINAPI Router::Thread_QueryRouterStatus( LPVOID lpParam )
{
	AfxOleInit();

	CPing routerPing;
	UINT nTimeOutCounter=0;
	CArrayEx<INT> nCurPathIndexList;

	while(TRUE)
	{
		if(g_MyStat.workMode!= Pause)
		{
			USES_CONVERSION;
			if(routerPing.Ping((const char*)T2A(s_MyInfo.szIP.GetBuffer()),1,200)==1)
			{
				//���ó�ʱ������
				nTimeOutCounter=0;

				//ɸѡ����ǰ��Ҫ�õ�����
				nCurPathIndexList.Clear();
				nCurPathIndexList.AppendUnique(g_RouterXml.m_Router.Wan.PPPoE.Stat.nPathIndex);
				if(!g_WanTask.bTaskCompleted)
				{
					nCurPathIndexList.AppendUnique(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.nPathIndex);
				}
				if(!g_TrafficTask.bTaskCompleted)
				{
					nCurPathIndexList.AppendUnique(g_RouterXml.m_Router.Traf.Ctrl.nPathIndex);
				}
				if(g_MyStat.workMode == QueryTrafStat)
				{
					nCurPathIndexList.AppendUnique(g_RouterXml.m_Router.Traf.Stat.nPathIndex);
					nCurPathIndexList.AppendUnique(g_RouterXml.m_Router.DHCP.Stat.nPathIndex);
				}
				
				//����Html
				CRouterXml::RouterLinkStatus nLSTmp;
				INT nCurPathIndex ;
				CString szTask;
				CStringEx::CStringArray szFormContainer ;
				//�����ջ�
				//���ʹ��CStringEx::CStringArray szFormContainer = ...;
				//��szFormContainer�Ŀ�ʵ�����ᴴ����CStringEx::CStringArray szFormContainerֱ��Ϊ...
				//��ʹ��
				//CStringEx::CStringArray szFormContainer;
				//szFormContainer = s_RouterXml.m_Router.szFormContainerList;
				//�Ż��ȴ���һ����ʵ��
				//������д�����ǲ�һ����
				szFormContainer = g_RouterXml.m_Router.szFormContainerList;//��������������
				
				//s_RouterXml.ResetContainer();//���ñ�����
				for(int i=0;i<nCurPathIndexList.GetLength();i++)
				{
					CHttpWebConnection *pHwc;
					nCurPathIndex = nCurPathIndexList[i];
					if(createConnection(pHwc,g_RouterXml.m_Router.RequestPathList[nCurPathIndex]))
					{
						pHwc->writeForm(szFormContainer[nCurPathIndex]);
						if(pHwc->sendRequest())
						{
							nLSTmp = (CRouterXml::RouterLinkStatus)pHwc->getConnectionStatusCode();
							if(i==0)
							{
								//------#########-----------��ȡ·��������״̬��,&������ҳԴ��
								g_MyStat.nLinkStat = nLSTmp;
								//ת��Ϊ�ı���ʽ
								g_RouterXml.ConvertRouterLinkStatCodeToStr(g_MyStat.nLinkStat ,g_MyStat.szLinkStat);
								//------#########-----------��ȡ·��������״̬��.END
							}

							if(nLSTmp == CRouterXml::OK)
							{
								//����Դ��
								pHwc->readResponseString(szFormContainer[nCurPathIndex]);
								szTask=_T("���.");
							}
							else
							{
								szTask=_T("ʧ��.");
							}

							//����״̬����
							if(nCurPathIndex == g_RouterXml.m_Router.Wan.PPPoE.Ctrl.nPathIndex)
							{
								if(!g_WanTask.bTaskCompleted)
								{
									g_WanTask.bTaskCompleted=TRUE;//���������Ƿ�ɹ�
									szTask = g_WanTask.szTaskName + szTask;
									Application::ShowMsg(szTask);//��ʾ��ʾ��Ϣ
								}
							}
							else if(nCurPathIndex == g_RouterXml.m_Router.Traf.Ctrl.nPathIndex)
							{
								if(!g_TrafficTask.bTaskCompleted)
								{
									g_TrafficTask.bTaskCompleted=TRUE;
									szTask = g_TrafficTask.szTaskName;
								}
							}
						}
					}
					pHwc->close();
				}


				//---------------------##############----Wan----##############---------------------
				CString *pszHtml; 
				CRegex regexHtml;
				CString *pszStatRegex;
				static INT* pnWanStatPathIndexs[] = {
					&g_RouterXml.m_Router.Wan.PPPoE.Stat.nPathIndex
					,&g_RouterXml.m_Router.Wan.DIP.Stat.nPathIndex
					,&g_RouterXml.m_Router.Wan.SIP.Stat.nPathIndex
				};
				static CString* pszWanStatRegexIndexs[] = {
					&g_RouterXml.m_Router.Wan.PPPoE.Stat.szStatRegex
				};


				pszHtml = &szFormContainer[*pnWanStatPathIndexs[g_MyConfig.wanMode]];//��ҳHtml��String��ʽ
				pszStatRegex = pszWanStatRegexIndexs[g_MyConfig.wanMode];

				regexHtml = CRegex(*pszHtml);
				//------#########-----------��ȡWan����״̬��
				CStringEx::CStringArray szArr;
				CString sStatCode;

				regexHtml.match(*pszStatRegex);
				if(regexHtml.getMatchString(sStatCode,g_RouterXml.m_Router.Wan.PPPoE.Stat.nStatRegSubItem))
				{
					g_RouterXml.ConvertWanLinkStatStrCodeToCode(sStatCode,g_MyStat.nWanLinkStat);
					g_RouterXml.ConvertWanLinkStatCodeToStr(g_MyStat.nWanLinkStat,g_MyStat.szWanLinkStat);
				}
				else
				{
					g_MyStat.nWanLinkStat =  CRouterXml::UnknownFailed;
				}

				if(g_MyStat.nWanLinkStat == CRouterXml::NotConnected)
				{
					if(!g_WanTask.bTaskCompleted)
					{
						if(g_WanTask.szTaskName==_T("�Ͽ�����"))
						{
							g_WanTask.bTaskCompleted=TRUE;
							Application::ShowMsg(g_WanTask.szTaskName+_T("���."));
						}
					}
					if(g_MyConfig.bAutoReconnect)//��������
					{
						if(g_MyStat.nAutoReconnectTriedTimes<3)//����3���Զ�����
						{
							Application::AddThreadTask(Application::Task_BroadBandConnect,_T("Wan����"));
							g_MyStat.nAutoReconnectTriedTimes++;
							continue;
						}
					}
				}
				else
				{
					if(!g_WanTask.bTaskCompleted)//�����������
					{
						g_WanTask.bTaskCompleted=TRUE;
						Application::ShowMsg(g_WanTask.szTaskName+_T("���."));
					}
				}



				//---------------------##############----����ͳ��----##############---------------------
				if(g_MyStat.workMode==QueryTrafStat)
				{
					pszHtml = &szFormContainer[g_RouterXml.m_Router.Traf.Stat.nPathIndex];//��ҳHtml��String��ʽ
					pszStatRegex = &g_RouterXml.m_Router.Traf.Stat.szRegex;
					CStringEx::CStringArray szArrTraf;
					CString szTmp;

					regexHtml = CRegex(*pszHtml);
					regexHtml.match(*pszStatRegex);
					if(regexHtml.getMatchString(szTmp,1))
					{
						CStringEx(szTmp).split(_T(","),szArrTraf);
					}

					int nItemIndex=0,nArraySubItemIndex=0,nSubItemIndex=0;
					BOOL bUpdateData=FALSE;
					Messager::ContactDef curContact;
					TrafficStatusDef* pCurTrafStat;

					for(INT i=0;i<szArrTraf.GetLength()/g_RouterXml.m_Router.Traf.Stat.nItemPerRow*g_RouterXml.m_Router.Traf.Stat.nItemPerRow;i++,nArraySubItemIndex++)
					{
						if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.nItemPerRow)//���
						{
							nArraySubItemIndex=0;
						}

						if(nArraySubItemIndex==0)//��ţ��µ�Item
						{
							//TRACE("%d---%d---\n",s_RouterXml.m_Router.TrafStatList.GetLength()-1);
							if(nItemIndex>g_TrafficStatList.GetLength()-1)
							{
								pCurTrafStat=g_TrafficStatList.AppendNew();//���һ���հ�Ԫ��
							}
							else
							{
								pCurTrafStat=&g_TrafficStatList[nItemIndex];
							}
							nItemIndex++;

							pCurTrafStat->nIndex=nSubItemIndex;
							nSubItemIndex=0;
						}
						else
						{
							if(	 nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[0])//IP
							{
								pCurTrafStat->szIP=szArrTraf[i];
								pCurTrafStat->szIP.Replace('\"',' ');
								pCurTrafStat->szIP.Trim();
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[1])//Mac
							{
								CString szMac=szArrTraf[i];

								szMac.Replace('\"',' ');
								szMac.Trim();

								curContact.szID=szMac;
								if(pCurTrafStat->szMac!=szMac)
								{
									pCurTrafStat->szMac=szMac;
									curContact.szIP=pCurTrafStat->szIP;
									Messager::AddContact(curContact);
								}

								Messager::GetContact(curContact);
								pCurTrafStat->bOnline=curContact.bOnline;
								if(pCurTrafStat->bOnline)
								{
									pCurTrafStat->szOnline=_T("����");
								}
								else
								{
									pCurTrafStat->szOnline=_T("����");
								}

								if(pCurTrafStat->szMac==g_MyNetAdpInf.szMac)//����
								{
									pCurTrafStat->szUsername=_T("�ҡ�����޸��ǳơ�");
								}
								else
								{
									if(curContact.szRemark==_T(""))
									{
										pCurTrafStat->szUsername=_T("����ӱ�ע��");
									}
									else
									{
										pCurTrafStat->szUsername=curContact.szRemark;
									}
								}

								if(curContact.szName!=_T(""))
								{
									pCurTrafStat->szUsername+=_T("(")+curContact.szName+_T(")");
								}
								else
								{
									pCurTrafStat->szUsername+=_T("(")+pCurTrafStat->szMac+_T(")");
								}
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[2])//������,����
							{
								pCurTrafStat->szUpDownload=CCodeString::ConvertUnits(_ttol(szArrTraf[i]));
								pCurTrafStat->szUpDownload.AppendChar('/');
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[3])
							{
								pCurTrafStat->szUpDownload+=CCodeString::ConvertUnits(_ttol(szArrTraf[i]));
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[4])//����
							{
								pCurTrafStat->szUpDownloadSpeed=CCodeString::ConvertUnits(_ttol(szArrTraf[i]));
								pCurTrafStat->szUpDownloadSpeed.AppendChar('/');
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[5])
							{
								pCurTrafStat->szUpDownloadSpeed+=CCodeString::ConvertUnits(_ttol(szArrTraf[i]));
							}
						}
					}
					for(int i=nItemIndex;i<g_TrafficStatList.GetLength();i++)
					{
						//ɾ������Item
						g_TrafficStatList.RemoveAt(i);
					}

					//DHCP
					pszHtml = &szFormContainer[g_RouterXml.m_Router.DHCP.Stat.nPathIndex];//��ҳHtml��String��ʽ
					pszStatRegex = &g_RouterXml.m_Router.DHCP.Stat.szRegex;
					CStringEx::CStringArray szArrDHCP;
					
					regexHtml = CRegex(*pszHtml);
					regexHtml.match(*pszStatRegex);
					if(regexHtml.getMatchString(szTmp,1))
					{
						CStringEx(szTmp).split(_T(","),szArrDHCP);
					}

					if(g_RouterXml.m_Router.DHCP.Stat.nItemIndexs.GetLength()>=3&&g_RouterXml.m_Router.DHCP.Stat.nItemsPerRow>0)
					{
						DHCPClientDef *pCurDC;
						INT nRowX=0;

						g_DHCPClientList.Clear();
						for (int i=0;i<szArrDHCP.GetLength();i++)
						{
							if(nRowX==g_RouterXml.m_Router.DHCP.Stat.nItemIndexs[0])
							{
								pCurDC = g_DHCPClientList.AppendNew();
								pCurDC->szClientName = szArrDHCP[i];
							}else if(nRowX==g_RouterXml.m_Router.DHCP.Stat.nItemIndexs[1])
							{
								pCurDC->szMac  = szArrDHCP[i];
							}else if(nRowX==g_RouterXml.m_Router.DHCP.Stat.nItemIndexs[2])
							{
								pCurDC->szIP  = szArrDHCP[i];
							}

							nRowX++;
							if(nRowX == g_RouterXml.m_Router.DHCP.Stat.nItemsPerRow)
							{
								nRowX = 0;
							}
						}
					}
					
				}
				//---------------------##############----����ͳ��----##############---------------------END



			}
			else//��ʱ
			{
				if(nTimeOutCounter>4)//����ʱ�Ĵ���
				{
					g_MyStat.nLinkStat=CRouterXml::TimeOut;
					g_MyStat.szLinkStat=_T("���ӳ�ʱ");
					g_MyStat.szQueryTypeMsg=_T("���ȼ�����");
					AfxMessageBox(_T("�޷����ӵ�·���� [")+s_MyInfo.szIP+_T("] ���ô����������������ԭ���£�\n��δ����·����\n��·�������ȣ��뽫·�����ϵ���ȴ2���ӣ�"),MB_OK|MB_ICONWARNING);
				}
			}
		}

		Sleep(g_MyConfig.dwQueryInterval);
	}
}

void Router::SetIP( CString szIP )
{
	if(szIP==_T("")) return;
	s_MyInfo.szIP=szIP;
	USES_CONVERSION;
	szIP.ReleaseBuffer();
	s_MyInfo.pcszIP=T2A(szIP.GetBuffer());
	/*
	USES_CONVERSION;
	m_pcszRouterIP=(const char*)T2A(m_szRouterIP.GetBuffer());
	szIP.ReleaseBuffer();
	AfxMessageBox(A2T(m_pcszRouterIP));
	/*
	m_pcszRouterIP=(const char*)T2A(szIP.GetBuffer());
	//-CString ����ʱ���Զ�����ReleaseBuffer()������ָ��ָ��ĵ�ַ���ݱ����
	����m_szRouterIP.GetBuffer()��������ڴ�ֻ��SetIP()��Χ����Ч
	szIP.ReleaseBuffer();
	*/
	s_MyInfo.szURL=_T("http://")+szIP+_T("/");
}

void Router::Destroy()
{
	//pTh_QueryStat->ExitInstance();
}

void Router::Initialize()
{
	//��ȡIP
	CNetworkAdapter::AdapterInfoArray adpInfoArr;
	CNetworkAdapter::GetAdaptersInfosArray(adpInfoArr);
	CString strRouterIP;
	CPing routerPing;

	BOOL bCheck = ((CButton*)Application::g_pMainDlg->m_tabPage3.GetDlgItem(IDC_CHECK3))->GetCheck();

	if(bCheck)
	{
		strRouterIP = Common::GetWindownTextById(IDC_IPADDRESS1,&Application::g_pMainDlg->m_tabPage3);
		Router::SetIP(strRouterIP) ;
	}
	else//�Զ�����IP
	{
		for(int i=0;i<adpInfoArr.GetLength();i++)
		{
			for(int j=0;j<adpInfoArr[i].nGateWayCount;j++)
			{
				strRouterIP=adpInfoArr[i].szGateWays[j];//��ȡ���أ�·����IP��

				USES_CONVERSION;//������뽫wcharת��Ϊchar�����ûת����Ȼ���ᱨ���������ȷ  ��ע����������
				if(routerPing.Ping((const char *)T2A(strRouterIP.GetBuffer()),1,200)==1)
				{
					//AfxMessageBox(strRouterIP);
					SetIP(strRouterIP);
					//-------------*�������·�������Ӻ�ĳ�ʼ������*-----------------------

					//

					g_MyNetAdpInf=adpInfoArr[i];

					bCheck = TRUE;
				}
			}
		}
	}


	if(bCheck)
	{
		if(Login())//���Ե�½·����
		{
			ConfigRouter();//����·����
			Traffic_TurnOn();

			//ʵ��������Ҫ���¹������ӣ��磺����pHwc.url="http://192.168.1.1",�ǲ���ȷ��
			//��Ҫ��Ϊ"http://192.168.1.1/userRpm/..."

			if(pTh_QueryStat==NULL)
			{
				pTh_QueryStat = new CThread(AFX_THREADPROC(Thread_QueryRouterStatus));
				pTh_QueryStat->start();
			}
			Router::g_MyStat.workMode=Router::QueryStat;//������ѯ
			g_MyStat.bInitialized=TRUE;//���Ϊ�ѳ�ʼ��
		}
		else
		{
			if(g_MyStat.nLinkStat == CRouterXml::Denied)
			{
				g_RouterXml.ConvertRouterLinkStatCodeToStr(g_MyStat.nLinkStat,g_MyStat.szLinkStat);

				if(g_RouterXml.m_Router.Auth.szDefalutUsername==_T(""))
				{
					g_MyStat.szQueryTypeMsg=_T("������˺�����");
				}
				else
				{
					g_MyStat.szQueryTypeMsg=_T("���������");
				}

				g_MyStat.workMode=Pause;//��ͣ��ȡ���ȴ��û��޸��˻�����
			}
		}
	}
	else
	{
		g_MyStat.szQueryTypeMsg=_T("������IP");
		g_MyStat.workMode=Pause;//��ͣ��ȡ���ȴ��û��޸��˻�����
	}
}

BOOL Router::Login()
{
	CString szHtml;
	CRegex regexHtml;
	CPing routerPing;

	USES_CONVERSION;
	if(routerPing.Ping((const char*)T2A(s_MyInfo.szIP.GetBuffer()),2,200)>0)
	{
		//------#########-----------��ȡ·������½��ʽ&��ȡ�ͺ�
		if(s_MyInfo.authMethod==Null)
		{
			CRouterXml::RouterLinkStatus dwRouterStatCode=CRouterXml::TimeOut;
			CHttpWebConnection testHwc(s_MyInfo.szURL,CHttpWebConnection::GET);//����һ�������û��������������
			//m_pHwc->addRequestHeaderString(_T("Referer: ")+m_szRouterURL);
			testHwc.sendRequest();
			dwRouterStatCode=(CRouterXml::RouterLinkStatus)testHwc.getConnectionStatusCode();
			if(dwRouterStatCode==CRouterXml::Denied)
			{
				s_MyInfo.authMethod=Dlg;//���Denied����˵����½��ʽΪ�Ի���ʽ
				//---��ȡDlg���html
				CHttpWebConnection hwcTitle(s_MyInfo.szURL);
				hwcTitle.addRequestHeaderString(_T("Authorization:Basic ")+s_MyInfo.szBasicAuthorize);
				if(hwcTitle.sendRequest())
				{
					g_MyStat.nLinkStat = (CRouterXml::RouterLinkStatus)hwcTitle.getConnectionStatusCode();
					if(g_MyStat.nLinkStat == CRouterXml::OK)
					{
						hwcTitle.readResponseString(szHtml);
					}
				}
				hwcTitle.close();
			}
			else if(dwRouterStatCode==CRouterXml::OK)
			{
				s_MyInfo.authMethod=Page;//�����½��ʽΪ��ҳʽ�������ȶ�ȡ·�����ͺ�
				//��ȡԴ��
				g_MyStat.nLinkStat = (CRouterXml::RouterLinkStatus)testHwc.getConnectionStatusCode();
				testHwc.readResponseString(szHtml);
				testHwc.close();
			}
			g_RouterXml.m_Router.Auth.amAuth=(CRouterXml::AuthMethod)s_MyInfo.authMethod;

			if(g_MyStat.nLinkStat == CRouterXml::OK)
			{
				//------#########-----------��ȡ·�����ͺ�
				g_MyStat.szQueryTypeMsg=_T("���ڶ�ȡ�ͺ�...");
				s_MyInfo.szType=CStringEx(szHtml).mid(_T("<title>"),_T("</title>"));
				CStringEx(s_MyInfo.szType).RemoveChineseChars(s_MyInfo.szType);//ȥ������;

				//------#########-----------����Xml
				if(s_MyInfo.szType!=_T(""))
				{
					BOOL bXmlDownloaded = FALSE;
					while(TRUE)
					{
						g_MyStat.szQueryTypeMsg=s_MyInfo.szType;
						if(s_MyInfo.authMethod==Dlg)
						{
							g_MyStat.bXmlLoaded=loadXml(_T("A"));

						}
						else if(s_MyInfo.authMethod==Page)
						{
							g_MyStat.bXmlLoaded=loadXml(_T("B"));
						}

						if(g_MyStat.bXmlLoaded)
						{
							bXmlDownloaded = TRUE;
						}

						if(bXmlDownloaded) break;

						if(!g_MyStat.bXmlLoaded)
						{
							//������֧�ָ�·�������ļ�����������
							CHttpWebConnection hwc(_T("http://www.crystsoft.xyz"));//����Ƿ�����ͨ

							if(hwc.sendRequest())
							{
								g_MyStat.szQueryTypeMsg=_T("�������������ͺ�");
								static BOOL bDownloaded=FALSE;
								if(!bDownloaded)
								{
									bDownloaded=TRUE;
									Application::Task_DownloadRouterXmls();
									g_MyStat.szQueryTypeMsg=s_MyInfo.szType;
									g_MyStat.nLinkStat=CRouterXml::Initilizing;//����״̬��ֱ����������·����xml�ĵ�
									s_MyInfo.szType=_T("");
								}
								else
								{
									g_MyStat.szQueryTypeMsg=s_MyInfo.szType+_T(" *");
								}
							}
							else
							{
								static BOOL bMsgBoxShowed=FALSE;
								if(!bMsgBoxShowed)
								{
									bMsgBoxShowed=TRUE;
									AfxMessageBox(_T("δ�ҵ�֧�� ")+s_MyInfo.szType+_T(" �������ļ����������������򽫻������������ص������ļ���"),MB_ICONINFORMATION);
								}
								g_MyStat.nLinkStat=CRouterXml::Initilizing;//����״̬��ֱ����������·����xml�ĵ�
								s_MyInfo.szType=_T("");
							}
							bXmlDownloaded = TRUE;
						}
					}

					g_MyStat.szQueryTypeMsg = s_MyInfo.szType;
					if(g_MyStat.bXmlLoaded)
					{
						/*-----------------�������·����ʵ������Ĳ���-------------------------*/

						g_szDefaultUsername=g_RouterXml.m_Router.Auth.szDefalutUsername;
						SetAuthorize(g_szDefaultUsername);//һЩ·����ֻ��ʹ��Ĭ���û���
					}
				}
				else
				{
					//AfxMessageBox(_T("�޷���ȡ·����[")+s_MyInfo.szIP+_T("]�ͺţ�\n���Ƚ���ȷ��·�����˺���������·�������ж�Ӧ�ı���\nȻ������Ӧ�á���ť��"),MB_OK|MB_ICONEXCLAMATION);
					g_MyStat.szQueryTypeMsg = _T("��ȡ�ͺ�ʧ�ܣ�");
				}
			}

			testHwc.close();
		}
		//------#########-----------��ȡ·������½��ʽ.END

		if(g_MyStat.nLinkStat == CRouterXml::OK)
		{
			if(!g_MyStat.bXmlLoaded)
			{
				return FALSE;//�˺�Ĺ���������Xml�Ѽ���Ϊǰ��
			}
			if(s_MyInfo.authMethod == CRouterXml::Dlg)
			{
				return TRUE;//-----Dlg��½������������
			}

			//-----Page��½���ȡ��½״̬
			CHttpWebConnection hwc(s_MyInfo.szURL+g_RouterXml.m_Router.RequestPathList[g_RouterXml.m_Router.Auth.nPathIndex].szPath,
				g_RouterXml.m_Router.RequestPathList[g_RouterXml.m_Router.Auth.nPathIndex].requestMethod);
			if(g_RouterXml.m_Router.Auth.mRequestMethod == CHttpWebConnection::GET)
			{
				if(s_MyInfo.authMethod == Page)
				{
					hwc.setCookie(_T("Cookie"),_T("Authorization=Basic%20")+s_MyInfo.szBasicAuthorize);
				}
			}
			else//POST
			{
				hwc.writeForm(g_RouterXml.m_Router.Auth.szUsername,s_MyInfo.szUsername);
				hwc.writeForm(g_RouterXml.m_Router.Auth.szPassword,s_MyInfo.szPassword);
				hwc.setCookie(_T(""),g_RouterXml.m_Router.Auth.szCookie);
			}

			if(hwc.sendRequest())
			{
				//------#########-----------��ȡ·��������״̬��,&������ҳԴ��
				CRouterXml::RouterLinkStatus statCodeTemp=(CRouterXml::RouterLinkStatus)hwc.getConnectionStatusCode();//Ϊ���߳�ͬ�������Բ�ֱ��ʹ��g_MyStat.nLinkStat�����⴫�ݸ�״̬��ʾ�̴߳�����Ϣ
				if(statCodeTemp==CRouterXml::OK)
				{
					hwc.readResponseString(szHtml);//������ҳԴ��
					regexHtml = CRegex(szHtml);
					if(statCodeTemp==CRouterXml::OK)
					{
						CString sCode;
						regexHtml.match(g_RouterXml.m_Router.Auth.szStatRegex);
						if(regexHtml.getMatchString(sCode,2))
						{
							g_RouterXml.ConvertRouterLinkStatStrToCode(sCode,statCodeTemp);
						}
					}
					else
					{
						statCodeTemp = CRouterXml::TimeOut;
					}
					g_MyStat.nLinkStat=statCodeTemp;
					//------#########-----------��ȡ·��������״̬��.END
				}
			}
		}

		if(g_MyStat.nLinkStat == CRouterXml::Denied)
		{
			g_MyStat.szQueryTypeMsg=_T("������˺�����");
		}

		return g_MyStat.nLinkStat == CRouterXml::OK && g_MyStat.bXmlLoaded;
	}
	return FALSE;
}

BOOL Router::setCurrentForm( INT nFormIndex )
{
	if(nFormIndex<g_RouterXml.m_Router.szFormContainerList.GetLength())
	{
		s_pszCurrentForm = &g_RouterXml.m_Router.szFormContainerList[nFormIndex];
		*s_pszCurrentForm = _T("");
		return TRUE;
	}

	return FALSE;
}

void Router::writeCurrentForm( CString szExpression )
{
	if(*s_pszCurrentForm != _T(""))
	{
		*s_pszCurrentForm += _T("&")+szExpression ;
	}
	else
	{
		*s_pszCurrentForm += szExpression;
	}
}

void Router::writeCurrentForm( CString szName,CString szValue )
{
	writeCurrentForm(szName+_T("=")+szValue);
}

Router::DHCPClientList Router::g_DHCPClientList;

CString* Router::s_pszCurrentForm;

CRouterXml Router::g_RouterXml;

Router::RouterTaskDef Router::g_TrafficTask;

Router::RouterTaskDef Router::g_WanTask;

CString Router::g_szDefaultUsername=_T("");

Router::ConfigDef Router::g_MyConfig;

CNetworkAdapter::AdapterInfoDef Router::g_MyNetAdpInf;

CThread* Router::pTh_QueryStat = NULL;

Router::TrafficStatusList Router::g_TrafficStatList;

Router::RouterStatDef Router::g_MyStat;

Router::RouterInfoDef Router::s_MyInfo;

