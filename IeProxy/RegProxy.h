#pragma once
#include <string>

#ifdef UNICODE
#define   xstring std::wstring 
#else
#define   xstring std::string
#endif
class RegProxy
{
public:
	RegProxy(void);
	~RegProxy(void);
	bool IsProxy();
	bool SetProxy(bool bEnabled,xstring strProxyServer);
};


