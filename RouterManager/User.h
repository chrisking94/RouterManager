#pragma once
#include "StringEx.h"
#include "Thread.h"

class __declspec(dllexport) User
{
public:
	typedef struct
	{
		CString strLinkUrl;
		DWORD dwFlag;
		DWORD dwInterval;
		CStringEx::CStringArray  strArrFiles;
	}PicAdDef;

	typedef CArrayEx<PicAdDef> PicAdArray;

	typedef struct UserInfoDef
	{
		CString szID;
		CString szName;
		BOOL bAdvertiseDownloaded;
		BOOL bUpdateChecked;
		BOOL bSignedIn;
		UserInfoDef():bAdvertiseDownloaded(FALSE),bUpdateChecked(FALSE){}
	}UserInfoDef;


	static UserInfoDef g_Info;
	static PicAdArray g_Ads;
private:
	static CString m_strWebFolder;

	static CThread *s_pThread_UserClient;


	static DWORD WINAPI Thread_UserClient( LPVOID lpParam );//用户客户端管理线程
public:
	User(void);
	~User(void);
	static void Initialize();//路径后必须有 /
	static void Destroy();

	static bool ReadAdvertisePictureList(PicAdArray& PicList);
	static void DownloadAdvertisePictures(PicAdArray& PicList);
	static void SignIn();
};

