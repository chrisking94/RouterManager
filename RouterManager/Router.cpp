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
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szWanType);//设置WanType为PPPoE
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szLinkType);//设置手动连接
			//s_pTaskHwc->writeForm(s_RouterXml.m_Router.Wan.PPPoE.szSave);//保存命令
			writeCurrentForm(g_RouterXml.m_Router.Wan.PPPoE.Ctrl.szConnect);
			g_WanTask.szTaskName=_T("拨号");

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
			g_WanTask.szTaskName=_T("断开连接");

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
	/* ---查找文件--- */
	int j=0;
	BOOL bXmlMatched=FALSE;
	CString strFileName,strMatch=_T(""),szRouterType=s_MyInfo.szType;
	szRouterType.MakeLower();

	if(szRouterType==_T("")) return false;
	//查找
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
				if(strFileName[j]=='x')//x为通配符
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
				if(szRouterType[j]==0)//匹配成功
				{
					//进入文件内部匹配
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
								if(strSupportedType[j]==_T('x')||strSupportedType[j]==strMatch[j])//'x'为通配符
								{
									j++;
									if(strMatch[j]==0)//完全匹配，加载XML
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
							if(strMatch[0]==0)//完全匹配，加载XML
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
	/* ---查找结束--- */


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
				//重置超时计数器
				nTimeOutCounter=0;

				//筛选出当前需要用的链接
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
				
				//下载Html
				CRouterXml::RouterLinkStatus nLSTmp;
				INT nCurPathIndex ;
				CString szTask;
				CStringEx::CStringArray szFormContainer ;
				//调试收获：
				//如果使用CStringEx::CStringArray szFormContainer = ...;
				//则szFormContainer的空实例不会创建，CStringEx::CStringArray szFormContainer直接为...
				//而使用
				//CStringEx::CStringArray szFormContainer;
				//szFormContainer = s_RouterXml.m_Router.szFormContainerList;
				//才会先创建一个空实例
				//这两种写法的是不一样的
				szFormContainer = g_RouterXml.m_Router.szFormContainerList;//创建表单容器副本
				
				//s_RouterXml.ResetContainer();//重置表单容器
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
								//------#########-----------获取路由器连接状态码,&下载网页源码
								g_MyStat.nLinkStat = nLSTmp;
								//转换为文本形式
								g_RouterXml.ConvertRouterLinkStatCodeToStr(g_MyStat.nLinkStat ,g_MyStat.szLinkStat);
								//------#########-----------获取路由器连接状态码.END
							}

							if(nLSTmp == CRouterXml::OK)
							{
								//下载源码
								pHwc->readResponseString(szFormContainer[nCurPathIndex]);
								szTask=_T("完成.");
							}
							else
							{
								szTask=_T("失败.");
							}

							//任务状态更新
							if(nCurPathIndex == g_RouterXml.m_Router.Wan.PPPoE.Ctrl.nPathIndex)
							{
								if(!g_WanTask.bTaskCompleted)
								{
									g_WanTask.bTaskCompleted=TRUE;//不管任务是否成功
									szTask = g_WanTask.szTaskName + szTask;
									Application::ShowMsg(szTask);//显示提示信息
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


				pszHtml = &szFormContainer[*pnWanStatPathIndexs[g_MyConfig.wanMode]];//网页Html的String形式
				pszStatRegex = pszWanStatRegexIndexs[g_MyConfig.wanMode];

				regexHtml = CRegex(*pszHtml);
				//------#########-----------获取Wan连接状态码
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
						if(g_WanTask.szTaskName==_T("断开连接"))
						{
							g_WanTask.bTaskCompleted=TRUE;
							Application::ShowMsg(g_WanTask.szTaskName+_T("完成."));
						}
					}
					if(g_MyConfig.bAutoReconnect)//断线重连
					{
						if(g_MyStat.nAutoReconnectTriedTimes<3)//尝试3次自动重连
						{
							Application::AddThreadTask(Application::Task_BroadBandConnect,_T("Wan连接"));
							g_MyStat.nAutoReconnectTriedTimes++;
							continue;
						}
					}
				}
				else
				{
					if(!g_WanTask.bTaskCompleted)//拨号任务完成
					{
						g_WanTask.bTaskCompleted=TRUE;
						Application::ShowMsg(g_WanTask.szTaskName+_T("完成."));
					}
				}



				//---------------------##############----流量统计----##############---------------------
				if(g_MyStat.workMode==QueryTrafStat)
				{
					pszHtml = &szFormContainer[g_RouterXml.m_Router.Traf.Stat.nPathIndex];//网页Html的String形式
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
						if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.nItemPerRow)//序号
						{
							nArraySubItemIndex=0;
						}

						if(nArraySubItemIndex==0)//序号，新的Item
						{
							//TRACE("%d---%d---\n",s_RouterXml.m_Router.TrafStatList.GetLength()-1);
							if(nItemIndex>g_TrafficStatList.GetLength()-1)
							{
								pCurTrafStat=g_TrafficStatList.AppendNew();//添加一个空白元素
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
									pCurTrafStat->szOnline=_T("在线");
								}
								else
								{
									pCurTrafStat->szOnline=_T("离线");
								}

								if(pCurTrafStat->szMac==g_MyNetAdpInf.szMac)//本机
								{
									pCurTrafStat->szUsername=_T("我【点击修改昵称】");
								}
								else
								{
									if(curContact.szRemark==_T(""))
									{
										pCurTrafStat->szUsername=_T("【添加备注】");
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
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[2])//总流量,下载
							{
								pCurTrafStat->szUpDownload=CCodeString::ConvertUnits(_ttol(szArrTraf[i]));
								pCurTrafStat->szUpDownload.AppendChar('/');
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[3])
							{
								pCurTrafStat->szUpDownload+=CCodeString::ConvertUnits(_ttol(szArrTraf[i]));
							}
							else if(nArraySubItemIndex==g_RouterXml.m_Router.Traf.Stat.pnItemIndexs[4])//流率
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
						//删除多余Item
						g_TrafficStatList.RemoveAt(i);
					}

					//DHCP
					pszHtml = &szFormContainer[g_RouterXml.m_Router.DHCP.Stat.nPathIndex];//网页Html的String形式
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
				//---------------------##############----流量统计----##############---------------------END



			}
			else//超时
			{
				if(nTimeOutCounter>4)//允许超时的次数
				{
					g_MyStat.nLinkStat=CRouterXml::TimeOut;
					g_MyStat.szLinkStat=_T("连接超时");
					g_MyStat.szQueryTypeMsg=_T("请先检查故障");
					AfxMessageBox(_T("无法连接到路由器 [")+s_MyInfo.szIP+_T("] ！该错误可能是由于以下原因导致：\n①未连接路由器\n②路由器过热（请将路由器断电冷却2分钟）"),MB_OK|MB_ICONWARNING);
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
	//-CString 析构时会自动调用ReleaseBuffer()，导致指针指向的地址内容被情况
	发现m_szRouterIP.GetBuffer()所申请的内存只在SetIP()范围内有效
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
	//获取IP
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
	else//自动查找IP
	{
		for(int i=0;i<adpInfoArr.GetLength();i++)
		{
			for(int j=0;j<adpInfoArr[i].nGateWayCount;j++)
			{
				strRouterIP=adpInfoArr[i].szGateWays[j];//获取网关（路由器IP）

				USES_CONVERSION;//这里必须将wchar转换为char，如果没转换虽然不会报错但结果不正确  ！注意类似问题
				if(routerPing.Ping((const char *)T2A(strRouterIP.GetBuffer()),1,200)==1)
				{
					//AfxMessageBox(strRouterIP);
					SetIP(strRouterIP);
					//-------------*这里添加路由器连接后的初始化操作*-----------------------

					//

					g_MyNetAdpInf=adpInfoArr[i];

					bCheck = TRUE;
				}
			}
		}
	}


	if(bCheck)
	{
		if(Login())//尝试登陆路由器
		{
			ConfigRouter();//配置路由器
			Traffic_TurnOn();

			//实例化后需要重新构造连接，如：现在pHwc.url="http://192.168.1.1",是不正确的
			//需要其为"http://192.168.1.1/userRpm/..."

			if(pTh_QueryStat==NULL)
			{
				pTh_QueryStat = new CThread(AFX_THREADPROC(Thread_QueryRouterStatus));
				pTh_QueryStat->start();
			}
			Router::g_MyStat.workMode=Router::QueryStat;//启动查询
			g_MyStat.bInitialized=TRUE;//标记为已初始化
		}
		else
		{
			if(g_MyStat.nLinkStat == CRouterXml::Denied)
			{
				g_RouterXml.ConvertRouterLinkStatCodeToStr(g_MyStat.nLinkStat,g_MyStat.szLinkStat);

				if(g_RouterXml.m_Router.Auth.szDefalutUsername==_T(""))
				{
					g_MyStat.szQueryTypeMsg=_T("请更正账号密码");
				}
				else
				{
					g_MyStat.szQueryTypeMsg=_T("请更正密码");
				}

				g_MyStat.workMode=Pause;//暂停读取，等待用户修改账户密码
			}
		}
	}
	else
	{
		g_MyStat.szQueryTypeMsg=_T("请设置IP");
		g_MyStat.workMode=Pause;//暂停读取，等待用户修改账户密码
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
		//------#########-----------获取路由器登陆方式&读取型号
		if(s_MyInfo.authMethod==Null)
		{
			CRouterXml::RouterLinkStatus dwRouterStatCode=CRouterXml::TimeOut;
			CHttpWebConnection testHwc(s_MyInfo.szURL,CHttpWebConnection::GET);//构建一个不带用户名和密码的请求
			//m_pHwc->addRequestHeaderString(_T("Referer: ")+m_szRouterURL);
			testHwc.sendRequest();
			dwRouterStatCode=(CRouterXml::RouterLinkStatus)testHwc.getConnectionStatusCode();
			if(dwRouterStatCode==CRouterXml::Denied)
			{
				s_MyInfo.authMethod=Dlg;//如果Denied，则说明登陆方式为对话框式
				//---读取Dlg版的html
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
				s_MyInfo.authMethod=Page;//否则登陆方式为网页式，可以先读取路由器型号
				//读取源码
				g_MyStat.nLinkStat = (CRouterXml::RouterLinkStatus)testHwc.getConnectionStatusCode();
				testHwc.readResponseString(szHtml);
				testHwc.close();
			}
			g_RouterXml.m_Router.Auth.amAuth=(CRouterXml::AuthMethod)s_MyInfo.authMethod;

			if(g_MyStat.nLinkStat == CRouterXml::OK)
			{
				//------#########-----------获取路由器型号
				g_MyStat.szQueryTypeMsg=_T("正在读取型号...");
				s_MyInfo.szType=CStringEx(szHtml).mid(_T("<title>"),_T("</title>"));
				CStringEx(s_MyInfo.szType).RemoveChineseChars(s_MyInfo.szType);//去掉中文;

				//------#########-----------加载Xml
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
							//本地无支持该路由器的文件，联网查找
							CHttpWebConnection hwc(_T("http://www.crystsoft.xyz"));//检查是否能连通

							if(hwc.sendRequest())
							{
								g_MyStat.szQueryTypeMsg=_T("正在联网查找型号");
								static BOOL bDownloaded=FALSE;
								if(!bDownloaded)
								{
									bDownloaded=TRUE;
									Application::Task_DownloadRouterXmls();
									g_MyStat.szQueryTypeMsg=s_MyInfo.szType;
									g_MyStat.nLinkStat=CRouterXml::Initilizing;//重置状态，直到联网下载路由器xml文档
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
									AfxMessageBox(_T("未找到支持 ")+s_MyInfo.szType+_T(" 的配置文件！请先联网，程序将会从网络下载相关的配置文件。"),MB_ICONINFORMATION);
								}
								g_MyStat.nLinkStat=CRouterXml::Initilizing;//重置状态，直到联网下载路由器xml文档
								s_MyInfo.szType=_T("");
							}
							bXmlDownloaded = TRUE;
						}
					}

					g_MyStat.szQueryTypeMsg = s_MyInfo.szType;
					if(g_MyStat.bXmlLoaded)
					{
						/*-----------------这里添加路由器实例化后的操作-------------------------*/

						g_szDefaultUsername=g_RouterXml.m_Router.Auth.szDefalutUsername;
						SetAuthorize(g_szDefaultUsername);//一些路由器只能使用默认用户名
					}
				}
				else
				{
					//AfxMessageBox(_T("无法读取路由器[")+s_MyInfo.szIP+_T("]型号！\n请先将正确的路由器账号密码填入路由器栏中对应文本框\n然后点击【应用】按钮。"),MB_OK|MB_ICONEXCLAMATION);
					g_MyStat.szQueryTypeMsg = _T("读取型号失败！");
				}
			}

			testHwc.close();
		}
		//------#########-----------获取路由器登陆方式.END

		if(g_MyStat.nLinkStat == CRouterXml::OK)
		{
			if(!g_MyStat.bXmlLoaded)
			{
				return FALSE;//此后的工作必须以Xml已加载为前提
			}
			if(s_MyInfo.authMethod == CRouterXml::Dlg)
			{
				return TRUE;//-----Dlg登陆版的任务已完成
			}

			//-----Page登陆版获取登陆状态
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
				//------#########-----------获取路由器连接状态码,&下载网页源码
				CRouterXml::RouterLinkStatus statCodeTemp=(CRouterXml::RouterLinkStatus)hwc.getConnectionStatusCode();//为了线程同步，所以不直接使用g_MyStat.nLinkStat，以免传递给状态显示线程错误信息
				if(statCodeTemp==CRouterXml::OK)
				{
					hwc.readResponseString(szHtml);//下载网页源码
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
					//------#########-----------获取路由器连接状态码.END
				}
			}
		}

		if(g_MyStat.nLinkStat == CRouterXml::Denied)
		{
			g_MyStat.szQueryTypeMsg=_T("请更正账号密码");
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

