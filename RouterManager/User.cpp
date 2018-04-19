#include "StdAfx.h"
#include "User.h"
#include "HttpWebConnection.h"
#include "Xml.h"
#include "Common.h"
#include "StringEx.h"
#include "Router.h"
#include "Application.h"

User::User(void)
{
	
}

void User::Initialize( )
{
	m_strWebFolder=Common::GetWebFolder();
	g_Info.szID=CCodeString(Router::g_MyNetAdpInf.szMac).Md5Encrypt();

	if(s_pThread_UserClient==NULL)
	{
		s_pThread_UserClient=new CThread(AFX_THREADPROC(Thread_UserClient));
		s_pThread_UserClient->start();
	}
}


User::~User(void)
{
}

bool User::ReadAdvertisePictureList( PicAdArray& PicList )
{
	//读取xml
	CHttpWebConnection hwc(m_strWebFolder+_T("ads.xml"));
	hwc.sendRequest();
	CString strTmp;
	if(hwc.readResponseString(strTmp))
	{
		CXml xml("R");
		
		if(xml.LoadString(strTmp))
		{
			while(xml.EnumerateChildElement())//进入广告列表
			{
				PicAdDef* pPa=PicList.AppendNew();
				pPa->strLinkUrl=xml.Read("l");
				pPa->dwFlag=xml.ReadNumber("f");
				pPa->dwInterval=xml.ReadNumber("i");
				//下载文件
				while(xml.EnumerateChildElement(1))//进入图片文件列表
				{
					pPa->strArrFiles.Append(xml.Read());
				}
			}
		}
	}
	hwc.close();
	
	return true;
}

void User::DownloadAdvertisePictures( PicAdArray& PicList )
{
	CString sPicDlPath;
	Common::GetSpecialFolder(sPicDlPath,CSIDL_TEMPLATES);
	if(!Common::PathFileExiest(sPicDlPath))
	{
		Common::GetStartUpPath(sPicDlPath);
		sPicDlPath+=_T("Doc\\");
		if(!Common::CreateDirectoryW(sPicDlPath))
		{
			return;
		}
	}
	for(int i=0;i<PicList.GetLength();i++)
	{
		CStringEx::CStringArray* pPicNameList=&(PicList[i].strArrFiles);
		for(int j=0;j<pPicNameList->GetLength();j++)
		{
			CFile picFile(sPicDlPath+pPicNameList->At(j),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
			BYTE buffer[64];
			UINT nRead;
			CHttpWebConnection picDl(m_strWebFolder+pPicNameList->At(j));
			picDl.sendRequest();
			while(nRead=picDl.readBytes(buffer,64))
			{
				picFile.Write(buffer,nRead);
			}
			picFile.Close();
			picDl.close();
		}
	}
	//AfxMessageBox(_T("ad dl end"));
}

void User::SignIn()
{
	CHttpWebConnection hwc(Common::GetWebFolder()+_T("user.php"),CHttpWebConnection::POST);
	hwc.writeForm(_T("macmd5"),g_Info.szID);
	hwc.sendRequest();
	hwc.close();
}

DWORD WINAPI User::Thread_UserClient( LPVOID lpParam )
{
	AfxOleInit();

	CPing ping;
	while(true)
	{
		if(ping.Ping("www.crystsoft.xyz",2,200)>1)
		{
			/* 服务器已连接 */

			//下载广告图片
			if(!g_Info.bAdvertiseDownloaded)
			{
				if(ReadAdvertisePictureList(g_Ads))
				{
					if(g_Ads.GetLength()>1)
					{
						DownloadAdvertisePictures(g_Ads);
						Application::g_pMainAdvertise=&g_Ads[0];
						Application::g_pTrafAdvertise=&g_Ads[1];
					}
					g_Info.bAdvertiseDownloaded=TRUE;
				}
			}

			//检查更新
			if(!g_Info.bUpdateChecked)
			{
				g_Info.bUpdateChecked = TRUE;

				Application::CheckForUpdate();
			}

			//登陆
			if(!g_Info.bSignedIn)
			{
				g_Info.bSignedIn=TRUE;
			}
		}

		if(g_Info.bAdvertiseDownloaded&&g_Info.bSignedIn&&g_Info.bUpdateChecked)
		{
			//所有任务完成后退出线程
			break;
		}

		Sleep(2000);
	}

	return 0;
}

void User::Destroy()
{

}

User::PicAdArray User::g_Ads;

CThread * User::s_pThread_UserClient(NULL);

CString User::m_strWebFolder;

User::UserInfoDef User::g_Info;

