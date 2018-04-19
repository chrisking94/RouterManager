#pragma once

class BroadBand
{
public:
	BroadBand(void);
	~BroadBand(void);
private:
	static CString s_szAccount;
	static CString s_szPassword;

public:
	static CString g_szAccount;
	static CString g_szPassword;

	static void Initialize();
	static void Destroy();

	static void SetAccount(CString szAcc){s_szAccount=szAcc;g_szAccount=szAcc;};
	static void SetPassword(CString szPsw){s_szPassword=szPsw;g_szPassword=szPsw;};
	static void Reset(){g_szAccount=s_szAccount;g_szPassword=s_szPassword;};//将s_的值赋给g_

	static void ConfigRouter();
	static BOOL Connect();
	static BOOL Disconnect();
};

