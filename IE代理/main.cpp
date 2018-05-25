#include <stdio.h>  
#include <tchar.h>
#include <ObjBase.h>
#include <iostream>

#include "IEProxy.h";
#include "RegProxy.h"

using namespace ie_proxy;

int IEProxy_Test(){

	ProxyConfig pc;

	// pc.auto_config_url = L"http://www.proxynova.com/proxy.pac";
	// pc.auto_detect_settings = true;
	// pc.bypass_domain_list = L"google.com; yahoo.com; bing.com;";
	// pc.proxy_server = L"124.88.67.40:8080";

	// change proxy settings
	//setProxyConfig(&pc);

	ProxyConfig pc2;
	// retrieve current proxy settings
	getProxyConfig(&pc2);

	std::wcout << pc2.proxy_server << std::endl;
	return 0;
}


extern int ProxySettingDemo();
extern int ieProxy_Demo();

void RegProxy_Demo()
{
	RegProxy regProxy;
	bool bProxy = regProxy.IsProxy();
	if (bProxy)
	{
		printf("Proxy Enable\n");
	}
	else
	{
		printf("Proxy Disable\n");
	}
}

int main(int argc, char *argv[])  
{  
	::CoInitialize(NULL);
	ProxySettingDemo();
	IEProxy_Test();
	return 0;  
}  