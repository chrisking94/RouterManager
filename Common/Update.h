#pragma once
#include "ArrayEx.h"
#include "StringEx.h"

class __declspec(dllexport) CUpdate
{
public:
	typedef struct
	{
		CString strFilePath;
		BYTE btVer_Major;//主版本
		BYTE btVer_Minor;//子版本
		BYTE btVer_Revision;//修订
		ULONG ulFileSize;
		bool bExeOrDll;
	}FileVersionDef;

	typedef struct UpdateCmdString
	{
	private:
		int i;
		BOOL m_bRunbat;
	public:
		CString strCmdFilePath;
		CString strTaskListFind;
		CString strTaskKill;
		CString strDelete;
		CString strRename;
		CString strEndExecute;
		CString strCmds;
		UpdateCmdString(CString cmdFilePath):i(0),strCmdFilePath(cmdFilePath),m_bRunbat(FALSE)
		{
			
		}
		void addExcutionFile(CString strFileName)
		{
			CString strFlag;
			strFlag.Format(_T("f%d"),i++);
			strTaskListFind+=_T("set ")+strFlag+_T("=0\r\ntasklist|findstr -i ")+strFileName+_T("&&set "+strFlag+_T("=1\r\n"));
			strTaskKill+=_T("if %")+strFlag+_T("%==1 taskkill -f -im ")+strFileName+_T("\r\n");//关闭进程
			strEndExecute+=_T("if %")+strFlag+_T("%==1 start ")+strFileName+_T("\r\n");//如果程序在运行，升级完成后启动程序
		}
		void addFile(CString strFileName)
		{
			CString szName=strFileName;
			strDelete+=_T("del -f -q ")+strFileName+_T("\r\n");//删除原来文件
			if(szName.Find(_T("\\"))>=0)
			{
				szName=CStringEx(strFileName).glors(_T("\\"),CStringEx::RIGHT);
			}
			strRename+=_T("ren ")+strFileName+_T(".upd ")+szName+_T("\r\n");//重命名下载的文件
		}
		void addCmds(CString szCmds)
		{
			strCmds=szCmds+_T("\r\n");
		}
		void writeOut()
		{
			CFile updBat(strCmdFilePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
			CString strWriteOut=_T("@echo off\r\n");
			strWriteOut+=strTaskListFind;
			strWriteOut+=strTaskKill;
			strWriteOut+=strDelete;
			strWriteOut+=strRename;
			strWriteOut+=strEndExecute;
			strWriteOut+=strCmds;
			strWriteOut+=_T("del -f -q update.bat");
			USES_CONVERSION;
			updBat.Write(T2A(strWriteOut),strWriteOut.GetLength());
			updBat.Close();
			m_bRunbat=TRUE;
		}
		void runBat()
		{
			if(!m_bRunbat) return;
			ShellExecute(NULL,_T("open"),strCmdFilePath,NULL,NULL,SW_SHOW);
		}
	}UpdateCmdString;
typedef CArrayEx<FileVersionDef> FileVersionArray;

public:
	CUpdate(void);
	CUpdate(LPCTSTR strURLFolder,LPCTSTR strLocalFolder=_T(""));//strURLFolder最后必须加上 [/] 
	~CUpdate(void);

	UINT ReadWebFileVersions(FileVersionArray &fvaVersionsReturned,CString &strVersionComment);
	void ReadCmds(CString &strCmd);
	UINT GetAvailableNewVersions(FileVersionArray &fvaNeedCompare);//输入一个需要对比的网络版本记录表，返回需要更新的文件数目，fvaNeedCompare中返回需要更新的文件信息列表
	bool ReadLocalFileVersion(FileVersionDef& fvReturned);//fvReturned.strFilePath需要预先填入文件相对m_strLocalFolder路径
	bool NewVersionAvailable(CString& newVersionComment);
	//文件对比的版本信息表，处理完成后，不需要更新的文件信息会从表中移除
private:
	CString m_strURLFolder;
	CString m_strLocalFolder;
};

