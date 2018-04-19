#pragma once
#include "ArrayEx.h"
#include "StringEx.h"

class __declspec(dllexport) CUpdate
{
public:
	typedef struct
	{
		CString strFilePath;
		BYTE btVer_Major;//���汾
		BYTE btVer_Minor;//�Ӱ汾
		BYTE btVer_Revision;//�޶�
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
			strTaskKill+=_T("if %")+strFlag+_T("%==1 taskkill -f -im ")+strFileName+_T("\r\n");//�رս���
			strEndExecute+=_T("if %")+strFlag+_T("%==1 start ")+strFileName+_T("\r\n");//������������У�������ɺ���������
		}
		void addFile(CString strFileName)
		{
			CString szName=strFileName;
			strDelete+=_T("del -f -q ")+strFileName+_T("\r\n");//ɾ��ԭ���ļ�
			if(szName.Find(_T("\\"))>=0)
			{
				szName=CStringEx(strFileName).glors(_T("\\"),CStringEx::RIGHT);
			}
			strRename+=_T("ren ")+strFileName+_T(".upd ")+szName+_T("\r\n");//���������ص��ļ�
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
	CUpdate(LPCTSTR strURLFolder,LPCTSTR strLocalFolder=_T(""));//strURLFolder��������� [/] 
	~CUpdate(void);

	UINT ReadWebFileVersions(FileVersionArray &fvaVersionsReturned,CString &strVersionComment);
	void ReadCmds(CString &strCmd);
	UINT GetAvailableNewVersions(FileVersionArray &fvaNeedCompare);//����һ����Ҫ�Աȵ�����汾��¼��������Ҫ���µ��ļ���Ŀ��fvaNeedCompare�з�����Ҫ���µ��ļ���Ϣ�б�
	bool ReadLocalFileVersion(FileVersionDef& fvReturned);//fvReturned.strFilePath��ҪԤ�������ļ����m_strLocalFolder·��
	bool NewVersionAvailable(CString& newVersionComment);
	//�ļ��Աȵİ汾��Ϣ��������ɺ󣬲���Ҫ���µ��ļ���Ϣ��ӱ����Ƴ�
private:
	CString m_strURLFolder;
	CString m_strLocalFolder;
};

