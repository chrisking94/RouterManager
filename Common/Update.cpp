#include "StdAfx.h"
#include "Update.h"
#include "Common.h"

#pragma comment(lib,"Version.lib")

CUpdate::CUpdate(void)
{
}

CUpdate::CUpdate( LPCTSTR strURLFolder ,LPCTSTR strLocalFolder):
m_strURLFolder(strURLFolder)
	,m_strLocalFolder(strLocalFolder)
{
	if(m_strLocalFolder[0]!=0) return;
	Common::GetStartUpPath(m_strLocalFolder);
}

CUpdate::~CUpdate(void)
{
}

UINT CUpdate::ReadWebFileVersions( FileVersionArray &fvaVersionsReturned ,CString &strVersionComment)
{
	CHttpWebConnection hwcComment(m_strURLFolder+_T("comment.txt"));
	hwcComment.sendRequest();
	hwcComment.readResponseString(strVersionComment);
	hwcComment.close();

	CHttpWebConnection hwc(m_strURLFolder+_T("versions.xml"));
	hwc.sendRequest();
	CString strTmp;
	hwc.readResponseString(strTmp);
	CXml xm("V");
	if(xm.LoadString(strTmp))
	{
		FileVersionDef fv;
		VS_FIXEDFILEINFO *pFileInfo(NULL);
		
		while(xm.EnumerateChildElement())
		{
			fv.bExeOrDll=(xm.ReadNumber("ed")==1);
			fv.strFilePath=xm.Read("fp");
			if(fv.bExeOrDll)
			{
				fv.btVer_Major=xm.ReadNumber("va");
				fv.btVer_Minor=xm.ReadNumber("vi");
				fv.btVer_Revision=xm.ReadNumber("vr");
				fv.bExeOrDll=true;
			}
			fv.ulFileSize=xm.ReadNumber("fs");
			fvaVersionsReturned.Append(fv);
		}
		return fvaVersionsReturned.GetLength();
	}
	return 0;
}

UINT CUpdate::GetAvailableNewVersions(FileVersionArray &fvaNeedCompare)
{
	UINT fvaLen=fvaNeedCompare.GetLength();
	
	for(UINT i =0 ;i<fvaLen;i++)
	{
		FileVersionDef* pFvWeb=&fvaNeedCompare[i];
		FileVersionDef fvLocal=*pFvWeb;//调试发现：for里面声明的变量生存期为for运行期间，而不是每次for都会重新声明一次
		
		if(ReadLocalFileVersion(fvLocal))
		{
			if(fvLocal.bExeOrDll)
			{
				bool bNewVer=false;
				if(fvLocal.btVer_Major!=pFvWeb->btVer_Major) bNewVer=true;
				if(fvLocal.btVer_Minor!=pFvWeb->btVer_Minor) bNewVer=true;
				if(fvLocal.btVer_Revision!=pFvWeb->btVer_Revision) bNewVer=true;
				if(!bNewVer)
				{
					TRACE("%d----\n",fvaNeedCompare.GetLength());
					fvaNeedCompare.RemoveAt(i);
					TRACE("%d----\n",fvaNeedCompare.GetLength());
					//!这里导致了严重的错误，删除元素后fvaNeedCompare长度减一（调试了几个小时才找到错误）
					//假设其长度为2，则Remove后长度为1,在下一次for时i=1，即指向fvaNeedCompare的第二个元素（虚节点），则会出现错误
					//解决方案：
					fvaLen--;//长度减一
					i--;//指针减一
					//总结：
					//在使用一个数组变量时，删除它的元素后一定要修改记录其长度的相关变量，特别是在循环中使用时
				}
			}
			else
			{
				if(fvLocal.ulFileSize==pFvWeb->ulFileSize)
				{
					fvaNeedCompare.RemoveAt(i);
					fvaLen--;
					i--;
				}
			}
		}
	}
	return fvaNeedCompare.GetLength();
}

bool CUpdate::ReadLocalFileVersion( FileVersionDef& fvReturned )
{
	CString strFilePath=m_strLocalFolder+fvReturned.strFilePath;
	
	if(fvReturned.bExeOrDll)
	{
		if(!PathFileExists(strFilePath))
		{
			fvReturned.btVer_Major=0;
			return true;
		}
		//获取当前文件的版本信息
		DWORD dwLen = GetFileVersionInfoSize(strFilePath,NULL); 
		char *pszAppVersion = new char[dwLen+1];
		if(pszAppVersion)
		{
			memset(pszAppVersion,0,sizeof(char)*(dwLen+1));
			GetFileVersionInfo(strFilePath,NULL,dwLen,pszAppVersion);
			UINT nLen(0);
			VS_FIXEDFILEINFO *pFileInfo(NULL);
			VerQueryValue(pszAppVersion,_T("\\"),(LPVOID*)&pFileInfo,&nLen);
			if(pFileInfo)
			{
				//获取版本号
				fvReturned.btVer_Major=HIWORD(pFileInfo->dwFileVersionMS);
				fvReturned.btVer_Minor=LOWORD(pFileInfo->dwFileVersionMS);
				fvReturned.btVer_Revision=HIWORD(pFileInfo->dwFileVersionLS);
				//v_sub=LOWORD(pFileInfo->dwFileVersionLS)
			}else return false;
		}else return false;
		delete pszAppVersion;
	}
	else//文档文件以文件大小来标示版本
	{
		CFileStatus fileStatus;
		if (CFile::GetStatus(strFilePath, fileStatus))
		{
			fvReturned.ulFileSize=fileStatus.m_size;
		}
		else
		{
			fvReturned.ulFileSize=0;
			return false;
		}
	}
	return true;
}

bool CUpdate::NewVersionAvailable(CString& newVersionComment)
{
	FileVersionArray fva;
	UINT fvaLen=ReadWebFileVersions(fva,newVersionComment);

	for(UINT i =0 ;i<fvaLen;i++)
	{
		FileVersionDef* pFvWeb=&fva[i];
		FileVersionDef fvLocal=*pFvWeb;

		if(ReadLocalFileVersion(fvLocal))
		{
			if(fvLocal.bExeOrDll)
			{
				if(pFvWeb->btVer_Major!=fvLocal.btVer_Major) return true;
				if(pFvWeb->btVer_Minor!=fvLocal.btVer_Minor) return true;
				if(pFvWeb->btVer_Revision!=fvLocal.btVer_Revision) return true;
			}
			else
			{
				if(pFvWeb->ulFileSize!=fvLocal.ulFileSize) return true;
			}
		}
		else
		{
			newVersionComment=_T("err");
			return false;
		}
	}
	return false;
}

void CUpdate::ReadCmds( CString &strCmd )
{
	CHttpWebConnection hwcCmd(m_strURLFolder+_T("cmds.txt"));
	hwcCmd.sendRequest();
	hwcCmd.readResponseString(strCmd);
	hwcCmd.close();
}

