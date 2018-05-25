#include "stdafx.h"
#include "ProxySetting.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <Wininet.h>
#include<iostream> 

#pragma comment(lib, "Wininet.lib")

namespace Proxy
{

	bool QueryProxy()
	{
		BOOL ret=FALSE;

		INTERNET_PER_CONN_OPTION_LIST    List;
		INTERNET_PER_CONN_OPTION         Option[5];
		unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

		Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
		Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
		Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
		Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
		Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

		List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
		List.pszConnection = NULL;
		List.dwOptionCount = 5;
		List.dwOptionError = 0;
		List.pOptions = Option;

		if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
			_tprintf(_T("InternetQueryOption failed! (%d)\n"), GetLastError());
		if(Option[0].Value.pszValue != NULL)
			_tprintf(_T("%s\n"), Option[0].Value.pszValue);

		if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL)
			_tprintf(_T("PROXY_TYPE_AUTO_PROXY_URL\n"));

		if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT)
			_tprintf(_T("PROXY_TYPE_AUTO_DETECT\n"));
		if((Option[2].Value.dwValue &  PROXY_TYPE_PROXY) == PROXY_TYPE_PROXY)
		{
			_tprintf(_T("Proxy ENABLED!\n"));
			ret=TRUE;
		}
		else _tprintf(_T("Proxy DISABLED!\n"));

		_tprintf(_T("Current proxy:%s\n"),Option[4].Value.pszValue);

		INTERNET_VERSION_INFO      Version;
		nSize = sizeof(INTERNET_VERSION_INFO);

		InternetQueryOption(NULL, INTERNET_OPTION_VERSION, &Version, &nSize);

		if(Option[0].Value.pszValue != NULL)
			GlobalFree(Option[0].Value.pszValue);

		if(Option[3].Value.pszValue != NULL)
			GlobalFree(Option[3].Value.pszValue);

		if(Option[4].Value.pszValue != NULL)
			GlobalFree(Option[4].Value.pszValue);
		return ret;
	}

	void SetProxy(bool enableproxy,TCHAR *pProxyServer)
	{
		TCHAR buff[256] = _T("127.0.0.1:8087"); 
		_tcscpy(buff,pProxyServer);
		// To include server for FTP, HTTPS, and so on, use the string
		// (ftp=http://<ProxyServerName>:80; https=https://<ProxyServerName>:80) 
		INTERNET_PER_CONN_OPTION_LIST    List; 
		INTERNET_PER_CONN_OPTION         Option[3]; 
		unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 


		Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER; 
		Option[0].Value.pszValue = buff; 

		Option[1].dwOption = INTERNET_PER_CONN_FLAGS; 
		Option[1].Value.dwValue = PROXY_TYPE_DIRECT ; 
		if(enableproxy) Option[1].Value.dwValue |= PROXY_TYPE_PROXY; 
		// This option sets all the possible connection types for the client. 
		// This case specifies that the proxy can be used or direct connection is possible.

		Option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS; 
		Option[2].Value.pszValue = _T("<local>"); 



		List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 
		List.pszConnection = NULL; 
		List.dwOptionCount = 3; 
		List.dwOptionError = 0; 
		List.pOptions = Option; 

		if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize)) 
			_tprintf(_T("InternetSetOption failed! (%d)\n"), GetLastError()); 

		InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL,NULL); 
		//The connection settings for other instances of Internet Explorer. 

		_tprintf(_T("Proxy was %s!\n"),enableproxy?_T("ENABLED"):_T("DISABLED"));
	}

}  // namespace Proxy


//int _tmain(int argc, _TCHAR* argv[])


extern int ProxySettingDemo();
int ProxySettingDemo()
{
	::CoInitialize(NULL);
	bool bProxy = Proxy::QueryProxy();
	Proxy::SetProxy(!bProxy,_T("127.0.0.1:8087"));
	return 0;
}