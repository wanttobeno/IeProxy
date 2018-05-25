#include "stdafx.h"
#include "RegProxy.h"
#include <Windows.h>
#include <tchar.h>
#include <WinInet.h>

RegProxy::RegProxy(void)
{
}

RegProxy::~RegProxy(void)
{
}

#define PROXY_REG_ITEM _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings")

bool RegProxy::IsProxy()
{
	BOOL bReturn;
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[1];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_FLAGS;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 1;
	List.dwOptionError = 0;
	List.pOptions = Option;


	HINTERNET Inte = InternetOpen(_T("Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1)"), 1, _T("") ,_T("") ,0);

	InternetQueryOption(Inte, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize);
	if ((Option[0].Value.dwValue &  PROXY_TYPE_PROXY) == PROXY_TYPE_PROXY)
	{
		return TRUE;
	}
	InternetCloseHandle(Inte);
	return FALSE;
}

//  
// Set IE Proxy  
//  
// HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Internet Settings  
// ProxyEnable:  
//            0            proxy is disable  
//            1            proxy enable.  

bool RegProxy::SetProxy(bool bEnabled,xstring strProxyServer)  
{  
	HKEY hKey = NULL;  

	LONG lret = RegOpenKeyEx(HKEY_CURRENT_USER,  
		PROXY_REG_ITEM,  
		NULL,  
		KEY_WRITE |  
		KEY_SET_VALUE,  
		&hKey);  
	if(hKey == NULL || lret != ERROR_SUCCESS)  
	{  
		return FALSE;  
	}  

	// enable or disable  
	if(bEnabled)  
	{  
		lret = RegSetValueEx(hKey,  
			_T("ProxyServer"),  
			NULL,  
			REG_SZ,  
			(BYTE*)strProxyServer.c_str(),  
#ifdef UNICODE
			strProxyServer.length()*2);  
#else
			strProxyServer.length());  
#endif // 

		DWORD dwenable = 1;  
		lret = RegSetValueEx(hKey,  
			_T("ProxyEnable"),  
			NULL,  
			REG_DWORD,  
			(LPBYTE) & dwenable,  
			sizeof(dwenable));  
	}  
	else  
	{  
		DWORD dwenable = 0;  
		lret = RegSetValueEx(hKey,  
			_T("ProxyEnable"),  
			NULL,  
			REG_DWORD,  
			(LPBYTE) & dwenable,  
			sizeof(dwenable));  
	}  
	RegCloseKey(hKey);  

	return TRUE;  
}  